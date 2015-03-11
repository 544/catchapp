//
//  MainScene.cpp
//  Catch
//
//  Created by Masato Tsukagoshi on 2015/03/10.
//
//

#include "MainScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;


// フルーツの画面上端からのマージン(px)
const int FRUIT_TOP_MARGIN = 40;
// 出現率
const int FRUIT_SPAWN_RATE = 20;
// 制限時間
const float TIME_LIMIT_SECOND = 10;
// あたりをとった時の点数
const int GOLDEN_FLUIT_SCORE = 5;
// はずれをとった時の減点
const int BOMB_PENALTY_SCORE = 4;

MainScene::MainScene()
// このクラスをインスタンス化した際に変数もNULLで初期化
: _score(0)
,_second(TIME_LIMIT_SECOND)
,_state(GameState::PLAYING)
,_player(NULL)
,_scoreLabel(NULL)
,_secondLabel(NULL)
,_isCrash(false)
{
    
}

MainScene::~MainScene()
{
    // デストラクタ
    CC_SAFE_RELEASE_NULL(_player);
    CC_SAFE_RELEASE_NULL(_scoreLabel);
    CC_SAFE_RELEASE_NULL(_secondLabel);
}

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainScene::create();
    scene->addChild(layer);
    return scene;
}

/*
 * クラスの初期化処理
 */
bool MainScene::init()
{
    if (!Layer::init() ) {
        return false;
    }
    
    // 初期化処理
    
    //Director を取り出す
    auto director = Director::getInstance();
    // 画面サイズを取り出す
    auto size = director->getWinSize();
    // 背景のスプライトを生成する。
    auto background = Sprite::create("background.png");
    // スプライトの表示位置を設定
    background->setPosition(Vec2(size.width/2.0, size.height/2.0));
    // 親ノードにスプライトを追加
    this->addChild(background);
    
    // for player
    // スプライトを生成して変数に格納
    this->setPlayer(Sprite::create("player.png"));
    // スプライトの表示位置を設定
    _player->setPosition(Vec2(size.width/2.0, size.height-445));
    // 親ノードにスプライトを追加
    this->addChild(_player);
    
    // キャラクタスプライトのタッチイベント
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [](Touch* touch, Event* event) {
        // タッチされた時の処理
        // 本メソッドは必ず定義する。trueを返した時のみ、他のイベントメソッドが処理される。
        log("Touch at (%f, %f)", touch->getLocation().x, touch->getLocation().y);
        return true;
    };
    listener->onTouchMoved = [this](Touch* touch, Event* event) {
        if (this->getIsCrash()) {
            return;
        }
        // タッチ中に動いた時の処理
        // 前回のタッチ位置との差を取得
        Vec2 delta = touch->getDelta();
        // 現在の位置を取得する
        Vec2 position = _player->getPosition();
        // 現在位置に移動量を追加する。
        Vec2 newPosition = position + delta;
        // 縦方向は画面サイズを超えないように。横方向は変わらないようにする。
        auto winSize = Director::getInstance()->getWinSize();
        newPosition = newPosition.getClampPoint(Vec2(0, position.y), Vec2(winSize.width, position.y));
        
        _player->setPosition(newPosition);
    };
    director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    // updateを毎フレーム実行する。
    this->scheduleUpdate();
    
    // スコアラベルの追加
    auto scoreLabel = Label::createWithSystemFont(StringUtils::toString(_score), "Marker Flet", 16);
    scoreLabel->setPosition(Vec2(size.width/2.0 * 1.5, size.height-40));
    scoreLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
    scoreLabel->enableOutline(Color4B::BLACK, 1.5);
    this->setScoreLabel(scoreLabel);
    this->addChild(_scoreLabel);
    
    // スコアヘッダーの追加
    auto scoreHeader = Label::createWithSystemFont("SCORE", "Marker Flet", 16);
    scoreHeader->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
    scoreHeader->enableOutline(Color4B::BLACK, 1.5);
    scoreHeader->setPosition(size.width/2.0*1.5, size.height-20);
    this->addChild(scoreHeader);
    
    // タイマーラベルの追加
    int second = static_cast<int>(_second);
    auto secondLabel = Label::createWithSystemFont(StringUtils::toString(second), "Marker Flet", 16);
    this->setSecondLabel(secondLabel);
    secondLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
    secondLabel->enableOutline(Color4B::BLACK, 1.5);
    secondLabel->setPosition(Vec2(size.width/2.0, size.height-40));
    this->addChild(secondLabel);
    
    // タイマーヘッダの追加
    auto secondHeader = Label::createWithSystemFont("TIME", "Marker Flet", 16);
    secondHeader->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
    secondHeader->enableOutline(Color4B::BLACK, 1.5);
    secondHeader->setPosition(Vec2(size.width/2.0, size.height-20));
    this->addChild(secondHeader);
    
    return true;
}

void MainScene::update(float dt)
{
    if (_state == GameState::RESULT) {
        return;
    }
    
    // 毎フレーム実行される。
    int random = rand() % FRUIT_SPAWN_RATE;
    if (random == 0) {
        this->addFruit();
    }
    
    // あたり判定
    for (auto& fruit:_fruits) {
        Vec2 busketPosition = _player->getPosition() - Vec2(0,10);
        Rect boundingBox = fruit->getBoundingBox(); // フルーツの矩形を取り出す
        bool isHit = boundingBox.containsPoint(busketPosition);
        if (isHit) {
            this->catchFruit(fruit);
        }
    }
    
    // 残り時間を減らす
    _second -= dt;
    int second = static_cast<int>(_second);
    _secondLabel->setString(StringUtils::toString(second));
    
    if (_second < 0) {
        _state = GameState::RESULT;
        this->onResult();
    }
}

Sprite* MainScene::addFruit()
{
    // 画面サイズを取り出す
    auto winSize = Director::getInstance()->getWinSize();
    // フルーツの種類を選択する。
    int fruitType = rand() % static_cast<int>(FruitsType::COUNT);
    
    // フルーツを作成する。
    std::string filename = StringUtils::format("fruit%d.png",fruitType);
    auto fruit = Sprite::create(filename);
    fruit->setTag(fruitType); // フルーツの種類をタグとして指定
    
    auto fruitSize = fruit->getContentSize();
    float fruitXPos = rand() % static_cast<int>(winSize.width); // x軸のランダム位置
    fruit->setPosition(Vec2(fruitXPos, winSize.height - FRUIT_TOP_MARGIN - fruitSize.height/2.0));
    this->addChild(fruit);
    _fruits.pushBack(fruit);
    
    // フルーツに動きをつける

    // 地面の座標
    auto ground = Vec2(fruitXPos, 0);
    // 3秒かけて地面に落とす。
    auto fall = MoveTo::create(3, ground);
    // removeFruitを即座に呼び出すアクション
    auto remove = CallFuncN::create([this](Node *node){
        // NodeをSpriteにダウンキャスト
        auto sprite = dynamic_cast<Sprite *>(node);
        this->removeFruit(sprite);
    });
    
    // fallとremoveを連続して実行するアクション
    auto sequence = Sequence::create(fall, remove, NULL);
    fruit->runAction(sequence);
    
    return fruit;
    
}

bool MainScene::removeFruit(cocos2d::Sprite *fruit){
    if (_fruits.contains(fruit)) {
        // 親ノードから削除
        fruit->removeFromParent();
        // _fruits配列から削除
        _fruits.eraseObject(fruit);
        
        return true;
    }
    return false;
}

void MainScene::catchFruit(cocos2d::Sprite *fruit){
    
    if (this->getIsCrash()) {
        return;
    }
    
    log("Catch Fruit!");
//    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("catch_fruit.mp3");
    auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
    
    FruitsType fruitType = static_cast<FruitsType>(fruit->getTag());
    switch (fruitType) {
        case MainScene::FruitsType::GOLDEN:
            // あたりの場合
            _score += GOLDEN_FLUIT_SCORE;
            audioEngine->playEffect("catch_golden.mp3");
            break;
        case MainScene::FruitsType::BOMB:
            // はずれ
            this->onCatchBomb();
            audioEngine->playEffect("catch_bomb.mp3");
            break;
        default:
            _score += 1;
            audioEngine->playEffect("catch_fruit.mp3");
            break;
    }
    
    this->removeFruit(fruit);
    
    _scoreLabel->setString(StringUtils::toString(_score));
}

void MainScene::onResult()
{
    _state = GameState::RESULT;
    auto winSize = Director::getInstance()->getWinSize();
    
    // もう一度遊ぶボタン
    auto replayButton = MenuItemImage::create("replay_button.png",
                                              "replay_button_pressed.png",
                                              [](Ref* ref) {
                                                  // もう一度遊ぶをおした時の処理
                                                  CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("decide.mp3");
                                                  auto scene = MainScene::createScene();
                                                  auto transition = TransitionFade::create(0.5, scene);
                                                  Director::getInstance()->replaceScene(transition);
                                              }
                                             );
    // タイトルに戻るボタン
    auto titleButton = MenuItemImage::create("title_button.png",
                                             "title_button_pressed.png",
                                             [](Ref* ref) {
                                                 // タイトルに戻る処理
                                             });
    // ボタンからメニューを作成する。
    auto menu = Menu::create(replayButton, titleButton, NULL);
    menu->alignItemsVerticallyWithPadding(15);
    menu->setPosition(Vec2(winSize.width/2.0, winSize.height/2.0));
    this->addChild(menu);
}

void MainScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    // BGMを再生する
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("main.mp3", true);
}

void MainScene::onCatchBomb()
{
    // クラッシュ状態にする
    _isCrash = true;
    
    // アニメーションの作成
    Vector<SpriteFrame *> frames;
    auto playerSize = _player->getContentSize();
    const int animationFrameCount = 3;
    for (int i = 0; i < animationFrameCount; ++i) {
        auto rect = Rect(playerSize.width*i, 0, playerSize.width, playerSize.height);
        auto frame = SpriteFrame::create("player_crash.png", rect);
        frames.pushBack(frame);
    }
    
    // アニメーションを再生する。
    auto animation = Animation::createWithSpriteFrames(frames, 10.0/60.0);
    animation->setLoops(3);
    animation->setRestoreOriginalFrame(true);
    _player->runAction(Sequence::create(Animate::create(animation)
                                        , CallFunc::create([this]{
                                            _isCrash = false;
                                            })
                                        , NULL));
    _score = MAX(0, _score - BOMB_PENALTY_SCORE);
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("crash.mp3");
    
}
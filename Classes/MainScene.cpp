//
//  MainScene.cpp
//  Catch
//
//  Created by Masato Tsukagoshi on 2015/03/10.
//
//

#include "MainScene.h"

USING_NS_CC;

MainScene::MainScene()
// このクラスをインスタンス化した際に変数もNULLで初期化
: _player(NULL)
{
    
}

MainScene::~MainScene()
{
    // デストラクタ
    CC_SAFE_RELEASE_NULL(_player);
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
    
    
    return true;
}
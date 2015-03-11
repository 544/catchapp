//
//  TitleScene.cpp
//  Catch
//
//  Created by Masato Tsukagoshi on 2015/03/11.
//
//

#include "TitleScene.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* TitleScene::createScene()
{
    auto scene = Scene::create();
    auto layer = TitleScene::create();
    scene->addChild(layer);
    return scene;
}

TitleScene::TitleScene()
{
    
}

TitleScene::~TitleScene()
{
    
}

bool TitleScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto director = Director::getInstance();
    auto winSize = director->getWinSize();
    
    // 背景
    auto background = Sprite::create("title_background.png");
    background->setPosition(Vec2(winSize.width/2.0, winSize.height/2.0));
    this->addChild(background);
    
    // ロゴ
    auto logo = Sprite::create("title_logo.png");
    logo->setPosition(Vec2(winSize.width/2.0, winSize.height-150));
    this->addChild(logo);
    
    // 開始用スプライト
    auto touchToStart = Sprite::create("title_start.png");
    touchToStart->setPosition(Vec2(winSize.width/2.0, 90));
    // 点滅アクション
    auto blink = Sequence::create(FadeTo::create(0.5, 127), FadeTo::create(0.5, 255), NULL);
    touchToStart->runAction(RepeatForever::create(blink));
    this->addChild(touchToStart);
    
    // 画面タッチでメインに移動
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("decide.mp3");
        // 二度押し防止のためにアクション無効化
        this->getEventDispatcher()->removeAllEventListeners();
        // ゲームを始めるアクションを呼ぶ
        auto delay = DelayTime::create(0.5);
        // ゲームを始めるアクション
        auto startGame = CallFunc::create([]{
            auto scene = MainScene::createScene();
            auto transition = TransitionPageTurn::create(0.5, scene, true);
            Director::getInstance()->replaceScene(transition);
        });
        
        this->runAction(Sequence::create(delay, startGame, NULL));
        return true;
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void TitleScene::onEnterTransitionDidFinish()
{
    // BGMの再生
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("title.mp3", true);
}
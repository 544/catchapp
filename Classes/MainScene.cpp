//
//  MainScene.cpp
//  Catch
//
//  Created by Masato Tsukagoshi on 2015/03/10.
//
//

#include "MainScene.h"

USING_NS_CC;

MainScene::MainScene(){
    
}

MainScene::~MainScene(){
    
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
    
    return true;
}
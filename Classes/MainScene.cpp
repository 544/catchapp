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
    
    return true;
}
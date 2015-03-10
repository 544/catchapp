//
//  MainScene.h
//  Catch
//
//  Created by Masato Tsukagoshi on 2015/03/10.
//
//

#ifndef __Catch__MainScene__
#define __Catch__MainScene__

#include <stdio.h>

class MainScene :public cocos2d::Layer
{
protected:
    MainScene();
    virtual ~MainScene();
    bool init() override;

public:
    static cocos2d::Scene* createScene();
    // コンビニエンスコンストラクタ（createを作るマクロ。init()を宣言する必要がある。)
    CREATE_FUNC(MainScene);
    
    // _player変数とそのアクセサが自動的に実装される。
    CC_SYNTHESIZE_RETAIN(cocos2d::Sprite* , _player, Player);
};

#endif /* defined(__Catch__MainScene__) */

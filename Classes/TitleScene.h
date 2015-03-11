//
//  TitleScene.h
//  Catch
//
//  Created by Masato Tsukagoshi on 2015/03/11.
//
//

#ifndef __Catch__TitleScene__
#define __Catch__TitleScene__

#include <stdio.h>

class TitleScene :public cocos2d::Layer
{
protected:
    TitleScene();
    virtual ~TitleScene();
    bool init() override;

public:
    static cocos2d::Scene* createScene();
    void onEnterTransitionDidFinish() override;
    CREATE_FUNC(TitleScene);

};

#endif /* defined(__Catch__TitleScene__) */

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
    // ゲームの状態を定義
    enum class GameState
    {
        READY, // 開始演出
        PLAYING,
        ENDING, // 終了演出
        RESULT
    };
    
    // 落下物の種類を定義
    enum class FruitsType
    {
        APPLE,
        GRAPE,
        ORANGE,
        BANANA,
        CHERRY,
        GOLDEN, // あたり
        BOMB, // はずれ
        COUNT
    };

    static cocos2d::Scene* createScene();
    void update(float dt);
    void onEnterTransitionDidFinish() override;
    // コンビニエンスコンストラクタ（createを作るマクロ。init()を宣言する必要がある。)
    CREATE_FUNC(MainScene);
    
    // _player変数とそのアクセサが自動的に実装される。
    CC_SYNTHESIZE_RETAIN(cocos2d::Sprite* , _player, Player);
    // 落下物とアクセサの自動定義、ただし、Vectorなのでリファレンスカウンタは操作しない。
    CC_SYNTHESIZE(cocos2d::Vector<cocos2d::Sprite *>, _fruits, Fruits);
    // スコアと表示用のラベル
    CC_SYNTHESIZE(int, _score, Score);
    CC_SYNTHESIZE_RETAIN(cocos2d::Label*, _scoreLabel, ScoreLabel);
    // 残り時間と表示用のラベル
    CC_SYNTHESIZE(float, _second, Second);
    CC_SYNTHESIZE_RETAIN(cocos2d::Label*, _secondLabel, SecondLabel);
    // ゲームの状態
    CC_SYNTHESIZE(GameState, _state, State);
    // 硬直判定
    CC_SYNTHESIZE(bool, _isCrash, IsCrash);
    // フルーツスプライトバッチ
    CC_SYNTHESIZE_RETAIN(cocos2d::SpriteBatchNode*, _fruitsBatchNode, FruitsBatchNode);

private:
    /**
     * 画面にフルーツを配置し、それを返す。
     * @return 新たに作成されたフルーツ
     */
    cocos2d::Sprite* addFruit();
    
    /**
     * 画面からフルーツを取り除く
     * @param fruit 削除するフルーツ
     * @return 正しく削除されたか
     */
    bool removeFruit(cocos2d::Sprite* fruit);
    
    /**
     * フルーツを取得する。
     * @param Sprite* 取得するフルーツ
     */
    void catchFruit(cocos2d::Sprite* fruit);
    
    /**
     * ゲームが終了した時に呼び出される。
     */
    void onResult();
    /**
     * 爆弾をとってしまった時の処理
     */
    void onCatchBomb();
    /**
     * ゲーム開始の文字を追加
     */
    void addReadyLabel();
};

#endif /* defined(__Catch__MainScene__) */

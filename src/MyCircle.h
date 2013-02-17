#ifndef _MY_CIRCLE
#define _MY_CIRCLE

#include "ofMain.h"

class MyCircle {
    
public: 
    //コンストラクタ
    MyCircle(ofPoint pos, float radius, float maxSpeed = 8.0, float phaseSpeed = 0.06, float lifeLength = 10.0);
    //デストラクタ
    ~MyCircle();
    //半径の更新
    void update();
    
    void update2(ofPoint prevPos);
    
    //円を描く
    void draw();
    //アクセサ
    void setPos(ofPoint pos);
    void setRadius(float radius);
    
    //円の移動スピード
    ofPoint speed;
    //移動スピードの最大値
    float maxSpeed;
    //収縮を制御する正弦波の位相
    float phase;
    //収縮スピード
    float phaseSpeed;
    //生死の判定
    bool dead;
    //寿命
    float lifeLength;

    //円の位置
    ofPoint pos;

private:
    //プロパティはprivateで宣言
    //円の半径
    float radius;
};

#endif
#include "MyCircle.h"

//コンストラクタ
MyCircle::MyCircle(ofPoint _pos, float _radius, float _maxSpeed, float _phaseSpeed, float _lifeLength) 
{
    //位置と半径を設定
  //  pos = _pos+ofRandom(-1,1);
    pos = _pos;

    //   pos.x = _pos.x+ofRandom(-20,20);
 //   pos.y = _pos.y+ofRandom(-20,20);

    radius = _radius;
    phaseSpeed = _phaseSpeed;
    maxSpeed = _maxSpeed;
    maxSpeed = 2.0;//_maxSpeed;

    lifeLength = _lifeLength;
    
    //スピードを設定
    speed.x = ofRandom(-maxSpeed, maxSpeed);
    speed.y = ofRandom(-maxSpeed, maxSpeed);
    //初期位相
    phase = ofRandom(0, PI);
    //生死
    dead = false;
}

//デストラクタ
MyCircle::~MyCircle()
{

}

void MyCircle::update()
{
    //円の半径の伸び縮みの位相を更新

    phase += phaseSpeed;
    if (phase > lifeLength) {
        dead = true;
    }
 
    //円の半径をだんだん小さくする
    radius=radius*(1-0.05);
    if (radius < 0.02) {
        dead = true;
    }

    /*
    if (phase > PI) {
        phase -= PI;
    }
     */
    
    //位置を更新
    pos += speed;
    
/*    
    if (pos.x < 0 || pos.x > ofGetWidth()) {
        speed.x *= -1;
    }
    if (pos.y < 0 || pos.y > ofGetHeight()) {
        speed.y *= -1;
    }
  */   
}

void MyCircle::update2(ofPoint prevPos)
{
    //円の半径の伸び縮みの位相を更新
    
    phase += phaseSpeed;
    if (phase > lifeLength) {
        dead = true;
    }
    //円の半径をだんだん小さくする
    radius=radius*(1-0.01);
    if (radius < 0.02) {
        dead = true;
    }
        
    //位置を更新
    pos += speed;
    
    //pos.x=pos.x*0.99+speed.x+prevPos.x*0.01;
    //pos.y=pos.y*0.99+speed.y+prevPos.y*0.01;
   // pos.x=pos.x+0.01*(pos.x-prevPos.x)+speed.x;
   // pos.y=pos.y+0.01*(pos.y-prevPos.y)+speed.y;
    
}

void MyCircle::draw()
{
    //半径の収縮を計算
    float r = sin(phase)*radius;
    //色を指定
    
    /*
    ofEnableSmoothing();
    ofNoFill();
    ofSetColor(127, 255, 255, 31); 
    ofCircle(pos.x, pos.y, radius*2.0);
    
    ofDisableSmoothing();
    ofFill();
    
    */
    ofSetColor(127, 255, 255, 15); 
    ofCircle(pos.x, pos.y, radius*2.0);
    
    ofSetColor(31, 127, 255, 127); 
    ofCircle(pos.x, pos.y, r);

    ofSetColor(255, 255, 255); 
    ofCircle(pos.x, pos.y, r*0.25);
}

void MyCircle::setPos(ofPoint _pos)
{
    pos = _pos;
}

void MyCircle::setRadius(float _radius)
{
    radius = _radius;
}
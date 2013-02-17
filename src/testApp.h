#pragma once

#include "ofMain.h"

//GUI
//#include "ofxNativeGui.h"

//particle
#include "MyCircle.h"


#ifndef _TEST_APP
#define _TEST_APP

//カメラのウインドウサイズ
#define CAM_WIDTH 800
#define CAM_HEIGHT 600
//640 480, 800 600
#include "ofMain.h"
//#include "ofxVectorMath.h"
#include "ofxOpenCv.h"

//カラートラッキングのマージン
static float colorChkWidth;
//色を見つけた近さのパラメタ
static float nextClorParam;

//パーティクル表示モードかどうか
static bool isParticle;

//色の基本的な情報を持ったクラスを作ります。
class Color {
public:
	float hue, sat, bri;
    ofVec2f pos;
	//ofxVec2f pos;
    //ofVec4f rgba;
    ofVec3f color;
    float lineWidth;
    
    Color()
    {
    }
    
    Color(int r0, int s0, int b0)
    {
        hue = r0;
        sat = s0;
        bri = b0;
    }
    
};


class testApp : public ofBaseApp{
    
public:
    //この辺はいつも通り
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    void createColorContourFinder(Color col, unsigned char * huePixels,unsigned char * satPixels,unsigned char * briPixels,unsigned char * colorTrackedPixelsRed);
	
    void updateColorPos(unsigned char * huePixels,unsigned char * satPixels,unsigned char * briPixels);
    
    //軌跡を表示するメソッド
    void updateFlowPoint(ofVec2f to);
    
    
	//カメラの映像を取得するためのオブジェクト
	ofVideoGrabber vidGrabber;
	
	//カメラの幅と高さ
	int camWidth;
	int camHeight;
	
	//もともとの映像情報
	ofxCvColorImage colorImg;
	
	//HSV系に変換した映像情報
	ofxCvColorImage colorImgHSV;
	
    //grayscallの映像情報
    ofxCvGrayscaleImage grayImage; //グレースケールに変換後
    
	//HSV系の色相、彩度、明度のマップ
	ofxCvGrayscaleImage hueImg;
	ofxCvGrayscaleImage satImg;
	ofxCvGrayscaleImage briImg;
    
	//色を追跡して輪郭を出すための映像情報
	ofxCvGrayscaleImage reds;
    //ofxCvGrayscaleImage reds1;
	//ofxCvGrayscaleImage reds2;
	//ofxCvGrayscaleImage reds3;
    
    
	
	//追跡する色です。
	Color one;
	Color two;
	Color three;
	Color four;
    ofVec3f selectedColor;
    
	
	//もとの映像情報のピクセルの彩度と明度が
	//指定した色に近ければ255を代入、遠ければ0を代入
	unsigned char * colorTrackedPixelsRed0;
	unsigned char * colorTrackedPixelsRed1;
	unsigned char * colorTrackedPixelsRed2;
	unsigned char * colorTrackedPixelsRed3;
    
    
	//二値画像
	ofTexture trackedTextureRed;
    
	//輪郭を判別してくれるメチャクチャ便利なやつです。
	ofxCvContourFinder finderOne;
    ofxCvContourFinder finderTwo;
    ofxCvContourFinder finderThree;
    ofxCvContourFinder finderFour;
    
    
    
    //ピクセルの数
	int nPixels;
    
    
    //GUI 用
    // stuff we pass in when we are making the gui.
    vector < string > comboVals1;
    vector < string > comboVals2;
    vector < string > comboVals3;
    vector < string > comboVals4;
    
    vector < string > comboSetColors;
    
    
    // these variables are passed into the gui, and get altered as the gui is altered:
    int comboVal1;
    int comboVal2;
    int comboVal3;
    int comboVal4;
    
    int comboSetColor;
    
    int sliderVal;
    string enteredText;
    
    string circleSizeText;
    string colorChkWidthText;
    
    float oneH;
};

#endif

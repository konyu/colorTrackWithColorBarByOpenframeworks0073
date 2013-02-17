#include "testApp.h"
#include "ofColor.h"
#include <math.h>
#include <deque>

//軌跡を描画するためのクラス
class FlowLine{
    
public:
    ofVec3f pos;//軌跡の先頭位置
    ofVec3f color;//軌跡の先頭の色
    deque<Color> points;//軌跡を構成する点列
    
    deque<MyCircle *>circles;//particle
    
    float alpha;//アルファ値
    float rise_speed;//上昇スピード
    
    float lineWidth_default;
    bool fkeisu;
    
    
    
    FlowLine(){
        //軌跡が登っていくスピードをランダムで散らす
        rise_speed=ofRandom(0.3,2);
        alpha=1.0;
        fkeisu=false;
        lineWidth_default=20.0;
    }
    
    void update(){
        if(fkeisu){
            for (int i = 0; i < 20; i++) {
                circles.push_front(new MyCircle(ofPoint(pos[0], pos[1]), ofRandom(0.9, 4), 0.4));
            }
        }
        
        deque <MyCircle *>::iterator iter;
        int i=0;
        for( iter = circles.begin(); iter != circles.end();i++){
            if(points.size()>1 && i>0 && (points.size()-1)>=i-1){
                // cout << i << "  aaaaaaaa \n\n";
                ofPoint * prePoint =new ofPoint(points[i-1].pos.x,points[i-1].pos.y);
                (* iter)->update2(*prePoint);
                
            }else{
                (* iter)->update();
            }
            if ((* iter)->dead) {
                // if (true) {
                
                delete * iter;
                //++iter;
                
                iter = circles.erase(iter);
            } else {
                ++iter;
            }
            
        }
        
        //軌跡の先頭に現在のポイントを追加
        Color p;
        //平滑化してる　2割ぐらい一つ前の値をいれるかな？
        if(fkeisu){
            p.pos=(points.front().pos*0.3+pos*0.7);
            p.lineWidth=lineWidth_default;
            points.push_front(p);
        }
        //いろをだんだんたいしょくさせる？？
        // p.color=(points.front().color*0.2+color*0.8);
        
        if(points.size()==1)
        {
            points[0].lineWidth=lineWidth_default;//10.0;//lineWidth_default;
            //cout << points[0].lineWidth <<"  0 line Width  \n\n";
            points[0].color=ofVec3f(color[0], color[1],color[2]);
            
            
        }
        else if (points.size()>2){
            //  float tmpline=lineWidth_default;
            for(int i=0;i<points.size();i++){
                //軌跡がだんだん登って行くように
                if(i>=1){
                    points[i].pos.y -= rise_speed;
                    
                }
                //線をだんだん細くする
                points[i].lineWidth=points[i].lineWidth*0.95;
                // 線をなめらかに
                //この平滑化をコメントアウトすると、線分が細切れになる
                //points[i-i].pos = points[i].pos*0.6+points[i-1].pos*0.4;
                //  tmpline*=0.9;
                //   points[i].lineWidth=tmpline;
                //cout << points[i].lineWidth <<"  0 line Width  \n\n";
                
                //黒い色のは削除したほうがいいかな？
                points[i].color=ofVec3f(color[0]-i*0.03, color[1]-i*0.03,color[2]-i*0.03);
                if(points[i].color[3]<0.2|| points[i].lineWidth<1.0){
                    points.pop_back();
                    i--;
                }
                
            }
        }
        //軌跡の長さを制限する
        if(points.size()>200){
            points.pop_back();
            circles.pop_back();
        }
        //色が真っ黒になったら削除する うまく行ってない
        /*for (int i=0; i<points.size(); i++) {
         Color &p =points[i];
         if(0.0>=(p.color[0]+p.color[1]+p.color[2])){
         for(int j; j<(points.size()-i+1);j++){
         points.pop_back();
         }
         break;
         }
         }
         */
        //アルファ値をだんだん小さくする
        alpha += -alpha *0.3;
    }
    
    void found(){
        fkeisu=true;
    }
    
    void notFound(){
        
        if(points.size()>0){
            int rep=(int)(points.size()*0.1)+1;
            for(int j=0;j<rep;j++){
                points.pop_back();
            }
        }
        fkeisu=false;
        
    }
    void drow(){
        
        //circle のdraw
        if(isParticle){
            for(deque <MyCircle *>::iterator it = circles.begin(); it != circles.end(); ++it){
                (* it)->draw();
            }
        }
        //軌跡の描画　//OpenGLの関数で描画
        
        if(isParticle==false){
            
            //アンチエイリアス
            glEnable(GL_LINE_SMOOTH);
            
            //lineWidth=lineWidth*fkeisu;
            
            for (int i=0; i<points.size(); i++) {
                if((i+1)<points.size()){
                    Color &p =points[i];
                    //ofVec4f col=p.getRgbColor();
                    //色指定
                    glColor3f(p.color[0], p.color[1],p.color[2]);
                    //                p.color=ofVec3f(color[0]-i*0.02, color[1]-i*0.02,color[2]-i*0.02);
                    //glColor4f(1-i*0.02, 0-i*0.02, 0-i*0.02,1.0f);// a*alpha);
                    
                    float lineW=points[i].lineWidth;
                    if(lineW<0){ lineW=0;};
                    
                    //glBegin(GL_LINE_STRIP);
                    glBegin(GL_TRIANGLE_STRIP);
                    
                    //float p_x=p.pos.x-lineW;
                    //if(p_x<0){p_x=0;}
                    float p_y=p.pos.y-lineW;
                    // cout << lineW <<" line Width  \n\n";
                    if(p_y<0){p_y=0;}
                    
                    float p_plus_y=p.pos.y+lineW;
                    if(p_plus_y>CAM_HEIGHT){p_plus_y=CAM_HEIGHT;}
                    
                    //TODO　画面の加減を超えた場合の処理も入れないと、上にゴミが出るかな？
                    //TODO　2点目が飛びすぎた時に無視するし仕組みがいるかも？
                    // glVertex2f(p_x,p_y);
                    
                    glVertex2f(p.pos.x,p_y);
                    // glVertex2f(p.pos.x,p.pos.y+lineW);
                    
                    glVertex2f(p.pos.x,p_plus_y);
                    
                    
                    Color &p2 =points[i+1];
                    
                    float p2_y=p2.pos.y-lineW;
                    if(p2_y<0){p2_y=0;}
                    
                    float p2_plus_y=p2.pos.y+lineW;
                    if(p2_plus_y>CAM_HEIGHT){p2_plus_y=CAM_HEIGHT;}
                    
                    // glVertex2f(p2_x,p2_y);
                    
                    glVertex2f(p2.pos.x,p2_y);
                    // glVertex2f(p2.pos.x,p2.pos.y+lineW);
                    
                    glVertex2f(p2.pos.x,p2_plus_y);
                    
                    
                    glEnd();
                }
            }}
        
        
    }
    
    //アルファ値が十分に小さいときに消去するためのフラグ用の関数
    bool alive(){
        return true;// return alpha > 0.05;
    }
    
    void clear(){
        
        points.clear();
    }
    
    bool isNearPos(float nowX,float nowY){
        if(points.size()<1){
            return true;
        }
        Color &p =points.front();
        float diffX=p.pos.x-nowX;
        float diffY=p.pos.y-nowY;
        
        if(fabs(diffX)<nextClorParam && fabs(diffY)<nextClorParam){
            return true;
        }
        else{
            return false;
        }
    }
    
};

//軌跡オブジェクトの配列
vector<FlowLine*> flow_lines;

//一番近い点を持ったオブジェクトを探すためにソートをかける関数オブジェクト
struct sort_by_distance {
    sort_by_distance(ofVec2f pos){
        this->pos =pos;
    }
    bool operator()(const FlowLine* a, const FlowLine* b){
        float len_a = (a->pos - pos).squareLength();
        float len_b = (b->pos - pos).squareLength();
        return len_a < len_b;
    }
    
    ofVec2f pos;
};


//--------------------------------------------------------------
void testApp::setup(){
	
    isParticle=false;
    //フレームレートの設定
    ofSetFrameRate(60);
    //最初は色が原点にあることにします
	one.pos = ofVec2f(0,0);
	
	//カメラの大きさを指定
	camWidth = CAM_WIDTH;
	camHeight = CAM_HEIGHT;
    
	//それぞれの映像情報の大きさを指定してあげます。
	colorImg.allocate(camWidth, camHeight);
	colorImgHSV.allocate(camWidth, camHeight);
	
    grayImage.allocate(camWidth, camHeight);
    
	hueImg.allocate(camWidth, camHeight);
	satImg.allocate(camWidth, camHeight);
	briImg.allocate(camWidth, camHeight);
	
	reds.allocate(camWidth, camHeight);
	
	//二値画像を作るための配列の大きさを指定
    //TODOこれ一つだけでいい　たぶん
	colorTrackedPixelsRed0 =new unsigned char [camWidth*camHeight];
    colorTrackedPixelsRed1 =new unsigned char [camWidth*camHeight];
	colorTrackedPixelsRed2 =new unsigned char [camWidth*camHeight];
	colorTrackedPixelsRed3 =new unsigned char [camWidth*camHeight];
	
    
	//colorTrackedPixelsblue =new unsigned char [camWidth*camHeight];
    
	//二値画像の大きさ
	trackedTextureRed.allocate(camWidth, camHeight, GL_LUMINANCE);
    
	//Grabberの「何か」と大きさ設定
	//setVerboseってなんだろ？ Verbose:冗長な、言葉数の多い
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(camWidth, camHeight);
    
    colorChkWidth=12;
    
    //ピクセルの数
	nPixels = camWidth*camHeight;
    
    //デフォルトでチェックする色　これもちゃんと決めたい
	//ゴム手袋黄色
    one = *new Color(20,109,28);
	
    //ゴム手袋青
    two = *new Color(18, 170, 30);
    
    //ビニールテープ赤
    three = *new Color(0,255,28);
    
    //ビニールテープ青
	four = *new Color(120,255,11);
    
    //4つぶん
    flow_lines.push_back(new FlowLine());
    flow_lines.push_back(new FlowLine());
    flow_lines.push_back(new FlowLine());
    flow_lines.push_back(new FlowLine());
    
    
    //###############################################
    //###############################################
    // 軌跡の色指定
    //
    //###############################################
    //###############################################
    FlowLine *line = flow_lines[0];
    line->color=ofVec3f(255/255,0/255,0/255);
    line = flow_lines[1];
    line->color=ofVec3f(0/255,255/255,0/255);
    //line->color=ofVec3f(255/255,100/255,0/255);
    
    line = flow_lines[2];
    line->color=ofVec3f(0/255,0/255,255/255);
    line = flow_lines[3];
    line->color=ofVec3f(255/255,255/255,0/255);
    //  line->color=ofVec3f(0/255,100/255,255/255);
    
    
    //gui
    comboVals1.push_back("off");
    comboVals1.push_back("ON");
    comboVals2.push_back("off");
    
    comboVals2.push_back("ON");
    comboVals3.push_back("off");
    comboVals3.push_back("ON");
    comboVals4.push_back("off");
    comboVals4.push_back("ON");
    
    
    colorChkWidth = 12;
    nextClorParam=300;
    
    comboVal1 = 0;
    comboVal2 = 0;
    comboVal3 = 0; //"no combo selected yet";
    comboVal4 = 0; //"no combo selected yet";
    
    enteredText = "no entered text yet";
    // circleSizeText= "CircleSize";
    // colorChkWidthText="color check range";
    // --------------- setup the gui window
    
    
    comboSetColors.push_back("0");
    comboSetColors.push_back("1");
    comboSetColors.push_back("2");
    comboSetColors.push_back("3");
    
    comboSetColor = 0; //"no combo selected yet";
    
    
}

//--------------------------------------------------------------
void testApp::update(){
	//映像を取得！
	vidGrabber.grabFrame();
    
	//colorImgの中身をピクセルごとに指定
	colorImg.setFromPixels(vidGrabber.getPixels(), camWidth, camHeight);
    //左右反転
    colorImg.mirror(false, true);
    
    
    //カラーのイメージをグレースケールに変換
    grayImage = colorImg;
	//HSV系に変換
	colorImgHSV = colorImg;
	colorImgHSV.convertRgbToHsv();
    
	//色相、彩度、明度にマッピング
	colorImgHSV.convertToGrayscalePlanarImages(hueImg, satImg, briImg);
    
	//ここが何やってんのか分からん。
	hueImg.flagImageChanged();
	satImg.flagImageChanged();
	briImg.flagImageChanged();
    
	//ピクセルの配列をそれぞれに作成
	unsigned char * huePixels = hueImg.getPixels();
	unsigned char * satPixels = satImg.getPixels();
	unsigned char * briPixels = briImg.getPixels();
    
    //データの更新処理
    updateColorPos(huePixels,satPixels,briPixels);
    
    
    //FlowLineの更新　削除
    vector<FlowLine*>::iterator it= flow_lines.begin();
    while (it != flow_lines.end()) {
        FlowLine *line = *it;
        //更新
        line->update();
        it++;
    }
    
}

//--------------------------------------------------------------
//ここは計算した結果をただ表示するだけ
void testApp::draw(){
	//背景色を指定
	ofBackground(100, 100, 100);
    
    //	ofSetColor(0xffffff);
  	ofSetHexColor(0xffffff);
    
	//元映像を表示
    //	vidGrabber.draw(0, 0);
    grayImage.draw(0, 0);
	//HSV系に変換したものを表示
    //	colorImgHSV.draw(340, 0);
    
	//二値画像を表示
    //	trackedTextureRed.draw(20, 300);
    //	ofDrawBitmapString("red", 20, 280);
    
	//元映像に輪郭線を表示
	//finderOne.draw();
    
    //FlowLineの描画
    /*
     vector<FlowLine*>::iterator it= flow_lines.begin();
     while (it != flow_lines.end()) {
     FlowLine *line = *it;
     //更新
     line->drow();
     it++;
     
     }
     */
    if(comboVal1>0){
        flow_lines[0]->drow();
    }
    if(comboVal2>0){
        flow_lines[1]->drow();
    }
    if(comboVal3>0){
        flow_lines[2]->drow();
    }
    if(comboVal4>0){
        flow_lines[3]->drow();
    }
    
	//二値画像の方に輪郭線表示
    //	glPushMatrix();
    //  glTranslatef(20, 300, 0);
    //   finderOne.draw();
    //	glPopMatrix();
    
    
    //HSV系に変換したものにも輪郭線表示
    //	glPushMatrix();
    //   glTranslatef(340, 0, 0);
    //  finderOne.draw();
    //	glPopMatrix();
    
	//追跡する色の位置を表示
	if (finderOne.blobs.size()>0) {
		char tempStr1[255];
		sprintf(tempStr1, "x:%f\ny:%f",
                finderOne.blobs[0].centroid.x,
                finderOne.blobs[0].centroid.y);
		ofDrawBitmapString(tempStr1, 20, 650);
        sprintf(tempStr1, "H:%f  S:%f  B:%f",
                selectedColor[0],
                selectedColor[1],
                selectedColor[2]);
        
        ofDrawBitmapString(tempStr1, 20, 670);
        
        //comboVal1
		
        sprintf(tempStr1,"color num:%d",
                comboSetColor);
        ofDrawBitmapString(tempStr1, 20, 690);
        
        
	}
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    cout << "key press "<<key << "\n";
    switch (key) {
        case 48:
            //0
            if(isParticle==false){
                isParticle=true;
            }else {
                isParticle=false;
            }
            break;
        case 49:
            //1
            if(comboVal1==1){
                comboVal1=0;
            }else {
                comboVal1=1;
            }
            break;
        case 50:
            //2
            if(comboVal2==1){
                comboVal2=0;
            }else {
                comboVal2=1;
            }
            break;
        case 51:
            //3
            if(comboVal3==1){
                comboVal3=0;
            }else {
                comboVal3=1;
            }
            break;
        case 52:
            //4
            if(comboVal4==1){
                comboVal4=0;
            }else {
                comboVal4=1;
            }
            break;
        case 33:
            //shift+1
            comboSetColor=0;
            break;
        case 34:
            //shift+2
            comboSetColor=1;
            break;
        case 35:
            //shift+3
            comboSetColor=2;
            break;
        case 36:
            //shift+4
            comboSetColor=3;
            break;
            
        default:
            break;
    }
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	unsigned char * huePixels = hueImg.getPixels();
	unsigned char * satPixels = satImg.getPixels();
	unsigned char * briPixels = briImg.getPixels();
    
	//クリックした場所の色を追跡する色に設定。
	x=MIN(x,hueImg.width-1);
	y=MIN(y,hueImg.height-1);
	
    
	if (button==0) {
		selectedColor[0] = huePixels[x+(y*hueImg.width)];
		selectedColor[1] = satPixels[x+(y*satImg.width)];
		selectedColor[2] = briPixels[x+(y*briImg.width)];
        
        switch (comboSetColor) {
            case 0:
                one.hue=selectedColor[0];
                one.sat=selectedColor[1];
                one.bri=selectedColor[2];
                break;
            case 1:
                two.hue=selectedColor[0];
                two.sat=selectedColor[1];
                two.bri=selectedColor[2];
                break;
            case 2:
                three.hue=selectedColor[0];
                three.sat=selectedColor[1];
                three.bri=selectedColor[2];
                break;
            case 3:
                four.hue=selectedColor[0];
                four.sat=selectedColor[1];
                four.bri=selectedColor[2];
                break;
                
            default:
                break;
        }
        
        
        cout << "(hue,sat,bri)=" << one.hue<< "," <<one.sat<< "," <<one.bri<< "," ;
        
	}
    
    cout << "comboVals1=" << comboVal1  ;
}
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
/*
 void testApp::gotMessage(ofMessage msg){
 
 }
 
 //--------------------------------------------------------------
 void testApp::dragEvent(ofDragInfo dragInfo){
 
 }
 */

void testApp::createColorContourFinder(Color col, unsigned char * huePixels,unsigned char * satPixels,unsigned char * briPixels,unsigned char * colorTrackedPixelsRed){
    
    //ピクセルの色が指定した色と色相と彩度が近ければ、
	//colorTrackedPixelsRedに255を、遠ければ0を代入。
    for (int i=0; i<nPixels; i++) {
		if ( (huePixels[i]>=col.hue-colorChkWidth && huePixels[i] <= col.hue + colorChkWidth) &&
            (satPixels[i]>=col.sat-colorChkWidth && satPixels[i]<=col.sat+colorChkWidth)){
			colorTrackedPixelsRed[i] = 255;
		}else {
			colorTrackedPixelsRed[i]=0;
		}
	}
}

void testApp::updateColorPos(unsigned char * huePixels,unsigned char * satPixels,unsigned char * briPixels){
    
    ///////////////////////////////////　ここは関数化したい
    createColorContourFinder(one,huePixels,satPixels,briPixels,colorTrackedPixelsRed0);
    
    //colorTrackedPixelsRedをもとにredsを作成
	//redsは輪郭線を求めるためだけにあるのかな？
	reds.setFromPixels(colorTrackedPixelsRed0, camWidth, camHeight);
    
	//輪郭線を見つける
	finderOne.findContours(reds, 10, nPixels/3, 1, false, true);
    
	//colorTrackedPixelsRedをもとにtrackedTextureRedを作成
	//これが二値画像になってるっぽい
	trackedTextureRed.loadData(colorTrackedPixelsRed0,
                               camWidth, camHeight, GL_LUMINANCE);
    
	//追跡する色の位置を中心にあわせる
	if (finderOne.blobs.size()>0) {
		one.pos = ofVec2f(finderOne.blobs[0].centroid.x,
                          finderOne.blobs[0].centroid.y);
        FlowLine *line = flow_lines[0];
        line->found();//色が存在した
        
        if(line->isNearPos(finderOne.blobs[0].centroid.x,finderOne.blobs[0].centroid.y)){
            line->pos = ofVec3f(finderOne.blobs[0].centroid.x,finderOne.blobs[0].centroid.y);
            line->update();
        }
        
	}else{
        FlowLine *line = flow_lines[0];
        line->notFound();        //色を見つけられなかった時
        //トラッキングする線をだんだん細くする
    }
    ///////////////////////////////////
    createColorContourFinder(two,huePixels,satPixels,briPixels,colorTrackedPixelsRed1);
    
	//colorTrackedPixelsRedをもとにredsを作成
	//redsは輪郭線を求めるためだけにあるのかな？
	reds.setFromPixels(colorTrackedPixelsRed1, camWidth, camHeight);
    
	//輪郭線を見つける
	finderTwo.findContours(reds, 10, nPixels/3, 1, false, true);
    
	//colorTrackedPixelsRedをもとにtrackedTextureRedを作成
	//これが二値画像になってるっぽい
	trackedTextureRed.loadData(colorTrackedPixelsRed1,
                               camWidth, camHeight, GL_LUMINANCE);
    
	//追跡する色の位置を中心にあわせる
	if (finderTwo.blobs.size()>0) {
		two.pos = ofVec2f(finderTwo.blobs[0].centroid.x,
                          finderTwo.blobs[0].centroid.y);
        FlowLine *line = flow_lines[1];
        line->found();//色が存在した
        
        if(line->isNearPos(finderTwo.blobs[0].centroid.x,finderTwo.blobs[0].centroid.y)){
            line->pos = ofVec3f(finderTwo.blobs[0].centroid.x,finderTwo.blobs[0].centroid.y);
            line->update();
        }
    }else{
        FlowLine *line = flow_lines[1];
        line->notFound();        //色を見つけられなかった時
        //トラッキングする線をだんだん細くする
    }
    
    
    ///////////////////////////////////
    createColorContourFinder(three,huePixels,satPixels,briPixels,colorTrackedPixelsRed2);
    
	//colorTrackedPixelsRedをもとにredsを作成
	//redsは輪郭線を求めるためだけにあるのかな？
	reds.setFromPixels(colorTrackedPixelsRed2, camWidth, camHeight);
    
	//輪郭線を見つける
	finderThree.findContours(reds, 10, nPixels/3, 1, false, true);
    
	//colorTrackedPixelsRedをもとにtrackedTextureRedを作成
	//これが二値画像になってるっぽい
	trackedTextureRed.loadData(colorTrackedPixelsRed2,
                               camWidth, camHeight, GL_LUMINANCE);
    
	//追跡する色の位置を中心にあわせる
	if (finderThree.blobs.size()>0) {
		three.pos = ofVec2f(finderThree.blobs[0].centroid.x,
                            finderThree.blobs[0].centroid.y);
        FlowLine *line = flow_lines[2];
        line->found();//色が存在した
        
        if(line->isNearPos(finderThree.blobs[0].centroid.x,finderThree.blobs[0].centroid.y)){
            line->pos = ofVec3f(finderThree.blobs[0].centroid.x,finderThree.blobs[0].centroid.y);
            line->update();
        }
    }else{
        FlowLine *line = flow_lines[2];
        line->notFound();        //色を見つけられなかった時
        //トラッキングする線をだんだん細くする
    }
    
    
    ///////////////////////////////////
    createColorContourFinder(four,huePixels,satPixels,briPixels,colorTrackedPixelsRed3);
    
	//colorTrackedPixelsRedをもとにredsを作成
	//redsは輪郭線を求めるためだけにあるのかな？
	reds.setFromPixels(colorTrackedPixelsRed3, camWidth, camHeight);
    
	//輪郭線を見つける
	finderFour.findContours(reds, 10, nPixels/3, 1, false, true);
    
	//colorTrackedPixelsRedをもとにtrackedTextureRedを作成
	//これが二値画像になってるっぽい
	trackedTextureRed.loadData(colorTrackedPixelsRed3,
                               camWidth, camHeight, GL_LUMINANCE);
    
	//追跡する色の位置を中心にあわせる
	if (finderFour.blobs.size()>0) {
		four.pos = ofVec2f(finderFour.blobs[0].centroid.x,
                           finderFour.blobs[0].centroid.y);
        FlowLine *line = flow_lines[3];
        line->found();//色が存在した
        
        if(line->isNearPos(finderFour.blobs[0].centroid.x,finderFour.blobs[0].centroid.y)){
            line->pos = ofVec3f(finderFour.blobs[0].centroid.x,finderFour.blobs[0].centroid.y);
            line->update();
        }
    }else{
        FlowLine *line = flow_lines[3];
        line->notFound();        //色を見つけられなかった時
        //トラッキングする線をだんだん細くする
    }
    
    
}

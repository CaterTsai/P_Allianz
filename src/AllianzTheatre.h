#ifndef ALLIANZ_THEATRE
#define ALLIANZ_THEATRE

#include "constParameter.h"
#include "ofxTheatreDirector.h"
#include "ofxHapPlayer.h"

static const ofRectangle cOPEN_YSE_BTN_RECT = ofRectangle(246, 882, 242, 242);
static const ofRectangle cOPEN_NO_BTN_RECT = ofRectangle(637, 882, 242, 242);
static const ofRectangle cINTRO_NEXT_BTN_RECT = ofRectangle(419, 877, 244, 244);
static const ofRectangle cINTRO_START_BTN_RECT = ofRectangle(419, 877, 244, 244);
static const ofRectangle cPHOTO_HOME_BTN_RECT = ofRectangle(419, 877, 244, 244);

static const ofRectangle cRESET_BTN_RECT = ofRectangle(815, 1672, 85, 85);
static const float cRESET_TIMER = 3.0;
enum ePhotoMode : int
{
	ePhotoLove = 0
	,ePhotoYourself
	,ePhotoChange
	,ePhotoFly
	,ePhotoModeNum
};

typedef struct _stDisplayElement
{
	ofImage	photoTitle;
	ofImage	intro, intro2, msg;
	ofImage	phtotframe;
}stDisplayElement;

class AllianzTheatre
{
public:
	void setup();
	void update(float fDelta);
	void draw();
	void reset();

	void AnimInit(string name);
	void onTheatreEvent(ofxTheatreEventArgs& e);

private:
	void initialStage();
	void nextScnece();

public:
	ePhotoMode	_eMode;
	ofxDirector		_Director;
	bool					_bCanCtrl;

//-------------------------------
// @Scenes : S_OPEN
//-------------------------------
private:
	void initialOpen();
	void updateOpen();
	void animInitOpen(string name);

private:
	bool _bWaiting;

//-------------------------------
// @Scenes : S_INTRO
//-------------------------------
private:
	void initialIntro();
	void updateIntro();
	void animInitIntro(string name);

	void setIntro();

private:
	bool _bNext;
//-------------------------------
// @Scenes : S_PHOTO
//-------------------------------
public:
	int getShootNum();
	ofImage& getPhotoframe();
	void processingFinish();

private:
	void initialPhoto();
	void updatePhoto();
	void animInitPhoto(string name);

	void setPhoto();
	void updateShoot();

private:
	map<int, ofImage>	_shootImg;
	bool								_bIsTakePicture;
	int	_iShootNum;
	
//-------------------------------
// Display Element
//-------------------------------
private:
	void initialDisplayElement();

private:
	map<ePhotoMode, stDisplayElement>	_DisplayMap;

//-------------------------------
//Event
//-------------------------------
public:
	ofEvent<string>	_AllianzTheaterEvent;

//------------------------------
//Mouse Event
//------------------------------
public:
	virtual void mouseMoved(ofMouseEventArgs & args){};
	virtual void mousePressed(ofMouseEventArgs & args);
	virtual void mouseReleased(ofMouseEventArgs & args);
	virtual void mouseDragged(ofMouseEventArgs & args){};

private:
	bool						_bClickReset;
	float						_fResetTimer;
	ofSoundPlayer	_click;
};
#endif // !ALLIANZ_THEATRE

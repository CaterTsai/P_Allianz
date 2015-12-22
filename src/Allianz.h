#pragma once

#include "ofMain.h"
#include "AllianzTheatre.h"
#include "ofxCTSystemCaller.h"
#include "ofxHttpUtils.h"
#include "ofxXmlSettings.h"
#include "QRCodeConnector.h"
#include "SlackReporter.h"

class Allianz : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	void stop();

	void keyPressed(int key);
	
private:
	bool	_bShowCursor;
	float _fMainTimer;
	ofSoundPlayer	_bgm;

#pragma region Theatre
//---------------------------------------
//Theatre
//---------------------------------------
private:
	void drawBeforeTheatre();
	void drawAfterTheatre();
	void onTheaterEvent(string& e);

private:
	AllianzTheatre	_theatre;
#pragma endregion

#pragma region Webcam
//---------------------------------------
//Webcam
//---------------------------------------
private:
	void setupWebcam();
	void updateWebcam();
	void drawWebcam();
	
	void setWebcamDisplay(bool value = true);
	
private:
	bool						_bWebcamSetup, _bDisplayWebcam, _bForDebug;
	ofVideoGrabber	_webcam;
	
#pragma endregion

#pragma region Photo
//-------------------------------------------------
//Photo
//-------------------------------------------------
private:
	void setupPhoto();
	void updatePhoto(float fDelta);	
	void drawPhoto();
	void drawCameraLight();

	void resetPhoto();

	void playPhoto();

	void mixPhoto();
	void savePhoto();

private:
	bool									_bDisplayPhoto;
	bool									_bSaveFinish;
	float									_PhotoTimer, _PhotoFps;
	string									_strPhotoName;
	list<ofImage>					_photoList;
	list<ofImage>::iterator	_photoIter;

	ofSoundPlayer				_camSound;
	float									_lightAlpha;
#pragma endregion

#pragma region Gif Create
//-------------------------------------------------
//Gif Create
//-------------------------------------------------
public:
	void createGif(string strFileName);
	void onGifCreate(string& e);
private:
	ofxCTSystemCaller		_gifCreater;
#pragma endregion

#pragma region Uploader
//-------------------------------------------------
//Gif Uploader
//-------------------------------------------------
public:
	void upload(string name);
	void httpRespone(ofxHttpResponse& Response);
private:
	ofxHttpUtils			_httpConn;
#pragma endregion

#pragma region QRCode Printer
//-------------------------------------------------
//QRCode Printer
//-------------------------------------------------
private:
	QRCodeConnector		_qrPrinter;
#pragma endregion

#pragma region Slack
//-------------------------------------------------
//Slack
//-------------------------------------------------
public:
	SlackReporter		_reporter;
#pragma endregion

#pragma region Xml Config
//-------------------------------------------------
//Config file 
//-------------------------------------------------
public:
	void loadConfig();

private:
	string _exOrderPath;
	string _exUploadURL;
	string _exQRIP;
	int	_exQRPort;
#pragma endregion

};

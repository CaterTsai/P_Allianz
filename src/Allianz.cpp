#include "Allianz.h"

#pragma region Base method
//--------------------------------------------------------------
void Allianz::setup()
{
	ofBackground(231);
	
	loadConfig();

	//Slack
	_reporter.setup();

	//Theatre setup
	_theatre.setup();
	ofAddListener(_theatre._AllianzTheaterEvent, this, &Allianz::onTheaterEvent);

	//Webcam setup
	setupWebcam();
	
	//Photo setup
	setupPhoto();
	resetPhoto();

	//Gif creater
	_gifCreater.start();
	ofAddListener(_gifCreater.SystemCallerFinishEvent, this, &Allianz::onGifCreate);

	//Uploader
	ofAddListener(_httpConn.newResponseEvent, this, &Allianz::httpRespone);
	_httpConn.start();

	//QR printer
	_qrPrinter.initQRConnector(_exQRIP, _exQRPort);

	//Bgm
	_bgm.loadSound("audios/bgm.mp3");
	_bgm.setLoop(true);
	_bgm.play();
	_bgm.setVolume(0.5);

	_reporter.Report("[NOTICE]Program Start");

	_bShowCursor = false;
	ofHideCursor();

	ofToggleFullscreen();

	_fMainTimer = ofGetElapsedTimef();
	ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void Allianz::update()
{
	float fDelta_ = ofGetElapsedTimef() - _fMainTimer;
	_fMainTimer += fDelta_;
	
	_theatre.update(fDelta_);
	updateWebcam();
	updatePhoto(fDelta_);

	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void Allianz::draw()
{
	drawBeforeTheatre();
	_theatre.draw();
	drawAfterTheatre();

}

//--------------------------------------------------------------
void Allianz::stop()
{
	_gifCreater.stop();
	_qrPrinter.closeQRConnector();
	_reporter.exit();
}

//--------------------------------------------------------------
void Allianz::keyPressed(int key)
{
	switch(key)
	{
	case 'f':
		{
			ofToggleFullscreen();
		}
		break;
	case 'c':
		{
			_bShowCursor ^= true;
			_bShowCursor?ofShowCursor():ofHideCursor();
		}
		break;
	case 'd':
		{
			_bForDebug ^= true;
			setWebcamDisplay(_bForDebug);
		}
		break;
	case 's':
		{
			_webcam.videoSettings();
		}
		break;
	}
}
#pragma endregion

#pragma region Theatre
//--------------------------------------------------------------
void Allianz::drawBeforeTheatre()
{
	drawPhoto();
	drawWebcam();
}

//--------------------------------------------------------------
void Allianz::drawAfterTheatre()
{
	drawCameraLight();

	//For Debug
	if(_bForDebug)
	{
		drawWebcam();
	}
}

//--------------------------------------------------------------
void Allianz::onTheaterEvent(string& e)
{
	if(e == NAME_MGR::T_DISPLAY_WEBCAM)
	{
		setWebcamDisplay();
	}
	else if(e == NAME_MGR::T_TAKE_PICTURE)
	{
		mixPhoto();
		_camSound.play();
		_lightAlpha = 255.0;
	}
	else if(e == NAME_MGR::T_START_PROCESS)
	{
		setWebcamDisplay(false);
		savePhoto();
	}
	else if(e == NAME_MGR::T_RESTART)
	{
		resetPhoto();
		setWebcamDisplay(false);
	}
}
#pragma endregion

#pragma region Webcam
//--------------------------------------------------------------
void Allianz::setupWebcam()
{
	_webcam.setDeviceID(0);
	_bWebcamSetup = _webcam.initGrabber(cWEBCAM_WIDTH, cWEBCAM_HEIGHT);
	_bDisplayWebcam = false;
	_bForDebug = false;

	if(!_bWebcamSetup)
	{
		ofLog(OF_LOG_ERROR, "[Webcam]setup webcam failed");
		_reporter.Report("[ERROR]setup webcam failed");
	}
}

//--------------------------------------------------------------
void Allianz::updateWebcam()
{
	if(!_bWebcamSetup || !_bDisplayWebcam)
	{
		return;
	}

	_webcam.update();
}

//--------------------------------------------------------------
void Allianz::drawWebcam()
{
	if(!_bWebcamSetup || !_bDisplayWebcam)
	{
		return;
	}

	ofPushStyle();
	ofSetColor(255);
	{
		ofPushMatrix();
		{
			ofTranslate(cWINDOW_WIDTH, 0);
			ofScale(-1, 1);
			ofTranslate(cWINDOW_WIDTH/2, cWINDOW_HEIGHT/2);
			ofRotateZ(90);
			_webcam.draw(-_webcam.getWidth()/2, -_webcam.getHeight()/2);
		}
		ofPopMatrix();
		
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void Allianz::setWebcamDisplay(bool value)
{
	_bDisplayWebcam = value;
}

#pragma endregion

#pragma region Photo
//--------------------------------------------------------------
void Allianz::setupPhoto()
{
	_bDisplayPhoto = false;
	_bSaveFinish = false;
	_PhotoFps = cPHOTO_FPS;
	_PhotoTimer = 0;

	_lightAlpha = 0;
	_camSound.loadSound("audios/Camera.wav");
	_camSound.setLoop(false);
	_camSound.setMultiPlay(false);
}

//--------------------------------------------------------------
void Allianz::updatePhoto(float fDelta)
{
	if(_bDisplayPhoto)
	{
		if(_photoList.size() != cPHOTO_NUM)
		{
			ofLog(OF_LOG_ERROR, "[PHOTO]_photoList is empty");
			_reporter.Report("[PHOTO]_photoList is empty");
			_bDisplayPhoto = false;
			return;
		}

		_PhotoTimer -= fDelta;
		if(_PhotoTimer <= 0)
		{
			_photoIter++;
			if(_photoIter == _photoList.end())
			{
				_photoIter = _photoList.begin();
			}
			_PhotoTimer = _PhotoFps;
		}
	}
	else
	{
		if(_bSaveFinish)
		{
			_bSaveFinish = false;
			createGif(_strPhotoName);
		}
	}

	if(_lightAlpha > 5.0)
	{
		_lightAlpha *= 0.85;
	}
	else
	{
		_lightAlpha = 0.0;
	}
}

//--------------------------------------------------------------
void Allianz::drawPhoto()
{
	if(!_bDisplayPhoto)
	{
		return;
	}

	ofPushStyle();
	ofSetColor(255);
	{
		_photoIter->draw(0, 0);
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void Allianz::drawCameraLight()
{
	if(_lightAlpha > 0.0)
	{
		ofPushStyle();
		ofSetColor(255, _lightAlpha);
		{
			ofRect(0, 0, cWINDOW_WIDTH, cWINDOW_HEIGHT);
		}
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void Allianz::resetPhoto()
{
	_bDisplayPhoto = _bSaveFinish = false;
	_PhotoTimer = 0;
	_photoList.clear();
}

//--------------------------------------------------------------
void Allianz::playPhoto()
{
	if(_photoList.size() == cPHOTO_NUM)
	{
		_bDisplayPhoto = true;
		_photoIter = _photoList.begin();
		_PhotoTimer = _PhotoFps;
	}
}


//--------------------------------------------------------------
void Allianz::mixPhoto()
{
	ofFbo canvas_;
	canvas_.allocate(cPHOTO_WIDTH, cPHOTO_HEIGHT, GL_RGBA);
	
	ofImage result_;

	ofPushStyle();
	canvas_.begin();
	{
		//Webcam
		ofPushMatrix();
		{
			ofSetColor(255);
			ofTranslate(ofGetWidth(), 0);
			ofScale(-1, 1);
			ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
			ofRotateZ(90);
			_webcam.draw(-_webcam.getWidth()/2, -_webcam.getHeight()/2);
		}
		ofPopMatrix();
		
		//Draw photoframe
		_theatre.getPhotoframe().draw(0, 0);
	}
	canvas_.end();
	ofPopStyle();

	//save to list
	ofPixels	pixel_;
	canvas_.readToPixels(pixel_);
	result_.setFromPixels(pixel_);

	_photoList.push_back(result_);	


}

//--------------------------------------------------------------
void Allianz::savePhoto()
{
	if(!_bSaveFinish)
	{
		_strPhotoName =  ofGetTimestampString("%Y%m%d%H%M%S");
		thread t(
			[&]()
			{
				int idx_ = 0;
				for(auto& Iter_ : _photoList)
				{
					ofImage Img_;
					Img_.clone(Iter_);
					
					Img_.resize(Img_.width * cPHOTO_SAVE_SCALE, Img_.height * cPHOTO_SAVE_SCALE);
					Img_.saveImage(cPHOTO_FOLDER_PATH + _strPhotoName + "_" + ofToString(idx_)+".png");
					idx_++;
				}

				//Create Order file
				string strPath_ = _exOrderPath + _strPhotoName + ".order";
				fstream	_order(strPath_, ios::out);
				_order.close();

				_bSaveFinish = true;
				ofLog(OF_LOG_NOTICE, "[savePhoto]Save photo success :" + _strPhotoName);
			}
		);
		t.detach();		
	}
	
}
#pragma endregion

#pragma region Gif Create
//--------------------------------------------------------------
void Allianz::createGif(string strFileName)
{
	string result_ = cBASE_CMD_1 + strFileName + cBASE_CMD_2 + strFileName + ".gif";
	_gifCreater.addCMD(result_);

	_gifCreater.signal();
}

//--------------------------------------------------------------
void Allianz::onGifCreate(string& e)
{
	if(e == "SUCCESS")
	{
		ofLog(OF_LOG_NOTICE, "[onGifCreate] Create Gif success :" + _strPhotoName);

		//Upload
		upload(_strPhotoName);
	}
}

#pragma endregion

#pragma region Uploader
//--------------------------------------------------------------
void Allianz::upload(string name)
{
	string gifPath_ = ofFilePath::getCurrentExeDir() + "data/gif/" + name + ".gif";
	ofxHttpForm	httpForm_;
	httpForm_.action = _exUploadURL;
	httpForm_.method = OFX_HTTP_POST;

	httpForm_.addFile("fileToUpload", gifPath_);
	httpForm_.addFormField("id", name);	
	httpForm_.addFormField("active", "uploadGIF");	

	_httpConn.addForm(httpForm_);
}

//--------------------------------------------------------------
void Allianz::httpRespone(ofxHttpResponse& Response)
{
	if(Response.status == 200)
	{
		string strResult_ = Response.responseBody.getText();

		if(strResult_.find("true") != string::npos)
		{
			ofLog(OF_LOG_NOTICE, "[Uploader] uploader gif success : " + _strPhotoName);
			_reporter.Report("[Uploader]uploader gif success :" + _strPhotoName);

			//Print QR Code
			string strFBShare_ = cFB_SHARE_URL + _strPhotoName;
			_qrPrinter.printQR(strFBShare_);
		}
		else
		{
			ofLog(OF_LOG_ERROR, "[Uploader] uploader gif php error : " + _strPhotoName);
			_reporter.Report("[ERROR][Uploader] uploader gif php error :" + _strPhotoName);
		}				
	}
	else
	{
		ofLog(OF_LOG_ERROR, "[Uploader] uploader gif internet error : " + Response.status);
		_reporter.Report("[ERROR][Uploader] uploader gif internet error :" + Response.status);
	}
	//next setp
	playPhoto();
	_theatre.processingFinish();
}
#pragma endregion

#pragma region Xml Config
void Allianz::loadConfig()
{
	ofxXmlSettings	xml_;

	if(!xml_.loadFile("_config.xml"))
	{
		_exOrderPath = "orders/";
		_exUploadURL = "http://127.0.0.1/allianz/s/allianz.php";
		_exQRIP = "192.168.2.199";
		_exQRPort = 11999;
		ofLog(OF_LOG_ERROR, "[loadConfig]load config failed, used default value");
		_reporter.Report("[ERROR][loadConfig]load config failed, used default value");
	}
	else
	{
		_exOrderPath = xml_.getValue("ORDER_PATH", "orders/");
		_exUploadURL = xml_.getValue("UPLOAD_URL", "http://127.0.0.1/allianz/s/allianz.php", 0);
		_exQRIP = xml_.getValue("QR_IP", "192.168.2.199");
		_exQRPort = xml_.getValue("QR_PORT", 11999);
		ofLog(OF_LOG_NOTICE, "[loadConfig]load config success");
	}
}
#pragma endregion

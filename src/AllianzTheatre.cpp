#include "AllianzTheatre.h"

#pragma region Base Method
//--------------------------------------------------------------
void AllianzTheatre::setup()
{
	initialStage();

	_eMode = ePhotoLove;
	_bClickReset = false;
	_fResetTimer = 0.0;
	setIntro();
	setPhoto();

	_click.loadSound("audios/click.wav");
	_click.setLoop(false);

	ofRegisterMouseEvents(this, OF_EVENT_ORDER_BEFORE_APP);
}

//--------------------------------------------------------------
void AllianzTheatre::update(float fDelta)
{
	string strScenesName_ = _Director.GetNowScenes()->GetScenesName();
	
	if(strScenesName_ == NAME_MGR::S_OPEN)
	{
		updateOpen();
	}
	else if(strScenesName_ == NAME_MGR::S_INTRO)
	{
		updateIntro();
	}
	else if(strScenesName_ == NAME_MGR::S_PHOTO)
	{
		updatePhoto();
	}

	//Rest check
	if(_bClickReset)
	{
		_fResetTimer -= fDelta;
		if(_fResetTimer <= 0.0)
		{
			int iMode_ = (int)_eMode + 1;
			if(iMode_ >= ePhotoModeNum)
			{
				_eMode = ePhotoLove;
			}
			else
			{
				_eMode = (ePhotoMode)iMode_;
			}
			reset();
		}
	}

	_Director.update();
}

//--------------------------------------------------------------
void AllianzTheatre::draw()
{
	ofPushStyle();
	ofSetColor(255);
	{
		_Director.draw();
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void AllianzTheatre::reset()
{
	_Director.TransitTo(NAME_MGR::S_OPEN, TRANSITION_TYPE::eTRANSITION_FADE);
	
	_bCanCtrl = false;
	_bWaiting = true;
	_bNext = true;
	_iShootNum = 0;
	_bIsTakePicture = true;
	_bClickReset = false;
	_fResetTimer = 0.0;
	setIntro();
	setPhoto();

}

//--------------------------------------------------------------
void AllianzTheatre::AnimInit(string name)
{
	string strScenesName_ = _Director.GetNowScenes()->GetScenesName();
	
	if(strScenesName_ == NAME_MGR::S_OPEN)
	{
		animInitOpen(name);
	}
	else if(strScenesName_ == NAME_MGR::S_INTRO)
	{
		animInitIntro(name);
	}
	else if(strScenesName_ == NAME_MGR::S_PHOTO)
	{
		animInitPhoto(name);
	}
}

//--------------------------------------------------------------
void AllianzTheatre::onTheatreEvent(ofxTheatreEventArgs& e)
{
	//Scence event
	if(e.strMessage == NAME_MGR::S_OPEN)
	{
		AnimInit(NAME_MGR::S_OPEN);

		string strMsg_ = NAME_MGR::T_RESTART;
		ofNotifyEvent(this->_AllianzTheaterEvent, strMsg_);
	}
	else if(e.strMessage == NAME_MGR::S_INTRO)
	{
		AnimInit(NAME_MGR::S_INTRO);
	}
	else if(e.strMessage == NAME_MGR::S_PHOTO)
	{
		AnimInit(NAME_MGR::S_PHOTO);
	}

	//Animate event
	//Open
	if(e.strMessage == NAME_MGR::ANIM_OPEN_BTN_IN)
	{
		_bWaiting = true;
		_bCanCtrl = true;
	}
	else if(e.strMessage == NAME_MGR::ANIM_OPEN_DISCLAIMER_IN)
	{
		_bWaiting = false;
		_bCanCtrl = true;
	}
	//Intro
	else if(e.strMessage == NAME_MGR::ANIM_INTRO_NEXT_IN)
	{
		_bNext = true;
		_bCanCtrl = true;
	}
	else if(e.strMessage == NAME_MGR::ANIM_INTRO_INTRO2_OUT)
	{
		ofxVideoElement* ptr_;
		_Director.GetElementPtr(NAME_MGR::E_INTRO_TEACH, ptr_);
		ptr_->PlayVideo();		
	}
	else if(e.strMessage == NAME_MGR::ANIM_INTRO_TEACH_IN)
	{
		ofxVideoElement* ptr_;
		_Director.GetElementPtr(NAME_MGR::E_INTRO_LOOP, ptr_);
		ptr_->StopVideo();
		ptr_->SetVisible(false);
	}
	else if(e.strMessage == NAME_MGR::ANIM_INTRO_START_IN)
	{
		_bNext = false;
		_bCanCtrl = true;
	}
	//Photo
	else if(e.strMessage == NAME_MGR::ANIM_PHOTO_SHOOT_IN)
	{
		ofxAnimationImageElement* ptr_;

		if(_iShootNum == 0)
		{
			_Director.GetElementPtr(NAME_MGR::E_PHOTO_COUNTDOWN5, ptr_);
		}
		else if(_iShootNum > 0 && _iShootNum < 4)
		{
			_Director.GetElementPtr(NAME_MGR::E_PHOTO_COUNTDOWN3, ptr_);
		}		
		ptr_->SetVisible(true);
		ptr_->PlayAnimation();
	}
	else if(e.strMessage == NAME_MGR::ANIM_PHOTO_SHOOT_OUT)
	{
		_iShootNum++;
		if(_iShootNum < 4)
		{
			//Next shoot
			updateShoot();
			AnimInit(NAME_MGR::INIT_PHOTO_SHOT_IN);
		}
		else
		{
			_bIsTakePicture = false;
			AnimInit(NAME_MGR::INIT_PHOTO_WAIT_IN);
		}	
	}
	else if(e.strMessage == NAME_MGR::ANIM_PHOTO_WAIT_ENTER)
	{
		//Disable Logo & titile
		ofxBaseElement* pImg_;
		_Director.GetElementPtr(NAME_MGR::E_PHOTO_LOGO, pImg_);
		pImg_->SetVisible(false);

		_Director.GetElementPtr(NAME_MGR::E_PHOTO_TITLE, pImg_);
		pImg_->SetVisible(false);

		//Start Processing
		string strMsg_ = NAME_MGR::T_START_PROCESS;
		ofNotifyEvent(_AllianzTheaterEvent, strMsg_);
	}
	else if(e.strMessage ==NAME_MGR::ANIM_PHOTO_HOME_BTN_IN)
	{
		_bCanCtrl = true;
	}

	//Animate element event
	if(e.strMessage == NAME_MGR::E_PHOTO_COUNTDOWN3 || e.strMessage == NAME_MGR::E_PHOTO_COUNTDOWN5)
	{
		ofxAnimationImageElement* ptr_;
		_Director.GetElementPtr(e.strMessage, ptr_);
		ptr_->SetVisible(false);

		string strMsg_ = NAME_MGR::T_TAKE_PICTURE;
		AnimInit(NAME_MGR::INIT_PHOTO_SHOT_OUT);
		ofNotifyEvent(_AllianzTheaterEvent, strMsg_);
	}
}

//--------------------------------------------------------------
void AllianzTheatre::initialStage()
{
	_Director.SetStageSize(ofGetWindowWidth(), ofGetWindowHeight());
	
	///////////////////////
	//Scenes
	_Director.AddScenes(NAME_MGR::S_OPEN);
	_Director.AddScenes(NAME_MGR::S_INTRO);
	_Director.AddScenes(NAME_MGR::S_PHOTO);

#pragma region Actor
	//////////////////////
	//Actor
	////Open
	_Director.AddActor(new ofxImageActor(NAME_MGR::A_LOGO_1, "images/logo_1.png", eBLEND_ALPHA));
	_Director.AddActor(new ofxImageActor(NAME_MGR::A_LOGO_2, "images/logo_2.png", eBLEND_ALPHA));
	_Director.AddActor(new ofxVideoActor(NAME_MGR::A_LOOP_BG, "videos/loop.mov", ofPtr<ofxHapPlayer>(new ofxHapPlayer)));
	_Director.AddActor(new ofxImageActor(NAME_MGR::A_TEXT_BACKPLANE, "images/backplane.png", eBLEND_ALPHA));
	
	_Director.AddActor(new ofxImageActor(NAME_MGR::A_START_TEXT, "images/text/tOpen.png", eBLEND_ALPHA));
	_Director.AddActor(new ofxImageActor(NAME_MGR::A_DISCLAIMER, "images/text/tDisclaimer.png", eBLEND_ALPHA));
	_Director.AddActor(new ofxImageActor(NAME_MGR::A_YES_NO_BTN, "images/yes_no.png", eBLEND_ALPHA));
	_Director.AddActor(new ofxVideoActor(NAME_MGR::A_OPEN_BTN, "videos/startBtn.mov", ofPtr<ofxHapPlayer>(new ofxHapPlayer), eBLEND_ALPHA));

	////Intro
	_Director.AddActor(new ofxImageActor(NAME_MGR::A_NEXT_BTN, "images/next.png", eBLEND_ALPHA));
	_Director.AddActor(new ofxImageActor(NAME_MGR::A_START_BTN, "images/start.png", eBLEND_ALPHA));

	_Director.AddActor(new ofxVideoActor(NAME_MGR::A_LOVE_LOOP, "videos/love_loop.mov", ofPtr<ofxHapPlayer>(new ofxHapPlayer)));
	_Director.AddActor(new ofxVideoActor(NAME_MGR::A_YOURSELF_LOOP, "videos/yourself_loop.mov", ofPtr<ofxHapPlayer>(new ofxHapPlayer)));
	_Director.AddActor(new ofxVideoActor(NAME_MGR::A_CHANGE_LOOP, "videos/change_loop.mov", ofPtr<ofxHapPlayer>(new ofxHapPlayer)));
	_Director.AddActor(new ofxVideoActor(NAME_MGR::A_FLY_LOOP, "videos/fly_loop.mov", ofPtr<ofxHapPlayer>(new ofxHapPlayer)));

	_Director.AddActor(new ofxVideoActor(NAME_MGR::A_LOVE_EX, "videos/love_ex.mov", ofPtr<ofxHapPlayer>(new ofxHapPlayer)));
	_Director.AddActor(new ofxVideoActor(NAME_MGR::A_YOURSELF_EX, "videos/yourself_ex.mov", ofPtr<ofxHapPlayer>(new ofxHapPlayer)));
	_Director.AddActor(new ofxVideoActor(NAME_MGR::A_CHANGE_EX, "videos/change_ex.mov", ofPtr<ofxHapPlayer>(new ofxHapPlayer)));
	_Director.AddActor(new ofxVideoActor(NAME_MGR::A_FLY_EX, "videos/fly_ex.mov", ofPtr<ofxHapPlayer>(new ofxHapPlayer)));

	_Director.AddActor(new ofxDynamicImageActor(NAME_MGR::A_INTRO_1, eBLEND_ALPHA));
	_Director.AddActor(new ofxDynamicImageActor(NAME_MGR::A_INTRO_2, eBLEND_ALPHA));
	_Director.AddActor(new ofxDynamicImageActor(NAME_MGR::A_MSG, eBLEND_ALPHA));

	////Photo
	_Director.AddActor(new ofxDynamicImageActor(NAME_MGR::A_PHOTO_TITLE, eBLEND_ALPHA));
	_Director.AddActor(new ofxDynamicImageActor(NAME_MGR::A_SHOOT,  eBLEND_ALPHA));
	_Director.AddActor(new ofxVideoActor(NAME_MGR::A_WAITING_PAGE, "videos/wating.mov", ofPtr<ofxHapPlayer>(new ofxHapPlayer)));
	_Director.AddActor(new ofxImageActor(NAME_MGR::A_FINISH_TEXT, "images/text/tFinish.png", eBLEND_ALPHA));
	_Director.AddActor(new ofxImageActor(NAME_MGR::A_HOME_BTN, "images/home.png", eBLEND_ALPHA));
		
	_Director.AddActor(new ofxDynamicImageActor(NAME_MGR::A_RESULT, eBLEND_ALPHA));	
	_Director.AddActor(new ofxAnimationImageActor(NAME_MGR::A_COUNTDOWN_3, "images/countdown3/", eBLEND_ALPHA));
	_Director.AddActor(new ofxAnimationImageActor(NAME_MGR::A_COUNTDOWN_5, "images/countdown5/", eBLEND_ALPHA));
#pragma endregion

#pragma region initial each scence
	initialOpen();
	initialIntro();
	initialPhoto();
#pragma endregion

	ofAddListener(ofxTheatreEventArgs::TheatreEvent, this, &AllianzTheatre::onTheatreEvent);
	initialDisplayElement();
	_Director.Play();

	AnimInit(NAME_MGR::S_OPEN);
}

//--------------------------------------------------------------
void AllianzTheatre::nextScnece()
{
	string strScenesName_ = _Director.GetNowScenes()->GetScenesName();
	
	if(strScenesName_ == NAME_MGR::S_OPEN)
	{
		_Director.TransitTo(NAME_MGR::S_INTRO);
		
	}
	else if(strScenesName_ == NAME_MGR::S_INTRO)
	{
		_Director.TransitTo(NAME_MGR::S_PHOTO);
		string strMsg_ = NAME_MGR::T_DISPLAY_WEBCAM;
		ofNotifyEvent(_AllianzTheaterEvent, strMsg_);
	}
	else if(strScenesName_ == NAME_MGR::S_PHOTO)
	{
		int iMode_ = (int)_eMode + 1;
		if(iMode_ >= ePhotoModeNum)
		{
			_eMode = ePhotoLove;
		}
		else
		{
			_eMode = (ePhotoMode)iMode_;
		}
		reset();
	}
}

#pragma endregion

#pragma region S_OPEN
//--------------------------------------------------------------
void AllianzTheatre::initialOpen()
{
	//Plane
	_Director.AddPlane(NAME_MGR::S_OPEN, NAME_MGR::P_OPEN_BG, 0);
	_Director.AddPlane(NAME_MGR::S_OPEN, NAME_MGR::P_OPEN_UI, 1);

	//Elements
	_Director.AddElement(NAME_MGR::E_OPEN_BACKPLANE, NAME_MGR::P_OPEN_UI, NAME_MGR::A_TEXT_BACKPLANE, 5, ofVec2f(92, 92), false);
	_Director.AddElement(NAME_MGR::E_OPEN_START_TEXT, NAME_MGR::P_OPEN_UI, NAME_MGR::A_START_TEXT, 6, ofVec2f(0), false);
	_Director.AddElement(NAME_MGR::E_OPEN_BTN, NAME_MGR::P_OPEN_UI, NAME_MGR::A_OPEN_BTN, 3, ofVec2f(0), false);
	_Director.AddElement(NAME_MGR::E_OPEN_DISCLAIMER_TEXT, NAME_MGR::P_OPEN_UI, NAME_MGR::A_DISCLAIMER, 7, ofVec2f(0), false);
	_Director.AddElement(NAME_MGR::E_OPEN_YES_NO_BTN, NAME_MGR::P_OPEN_UI, NAME_MGR::A_YES_NO_BTN, 4, ofVec2f(0), false);
	_Director.AddElement(NAME_MGR::E_OPEN_LOGO, NAME_MGR::P_OPEN_UI, NAME_MGR::A_LOGO_1, 10);

	_Director.AddElement(NAME_MGR::E_OPEN_LOOP, NAME_MGR::P_OPEN_BG, NAME_MGR::A_LOOP_BG);

	ofxVideoElement* pVideo_;
	_Director.GetElementPtr(NAME_MGR::E_OPEN_LOOP, pVideo_);
	pVideo_->SetVideoLoop(true);
	pVideo_->SetVideoAutoPlay(true);

	//-------------------
	_bWaiting = true;
	_bCanCtrl = false;
}

//--------------------------------------------------------------
void AllianzTheatre::updateOpen()
{
}

//--------------------------------------------------------------
void AllianzTheatre::animInitOpen(string name)
{
	ofxBaseElement* pElementPtr_;

	if(name == NAME_MGR::S_OPEN)
	{
		_Director.GetElementPtr(NAME_MGR::E_OPEN_BACKPLANE, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_OPEN, 
			0, 
			new ofxEnterAnimation(NAME_MGR::ANIM_OPEN_BACKPLANE_ENTER, pElementPtr_, ENTER_TYPE::eENTER_FROM_LEFT, ofGetWidth(), ofGetHeight(), 0.5f, 2.0)
		);

		_Director.GetElementPtr(NAME_MGR::E_OPEN_START_TEXT, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_OPEN,
			0,
			new ofxFadeInAnimation(NAME_MGR::ANIM_OPEN_TEXT_IN, pElementPtr_)
		);

		_Director.GetElementPtr(NAME_MGR::E_OPEN_BTN, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_OPEN,
			0,
			new ofxFadeInAnimation(NAME_MGR::ANIM_OPEN_BTN_IN, pElementPtr_)
		);
	}
	else if(name == NAME_MGR::INIT_OPEN_TO_DISCLAIMER)
	{
		_Director.GetElementPtr(NAME_MGR::E_OPEN_START_TEXT, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_OPEN, 
			0, 
			new ofxFadeOutAnimation(NAME_MGR::ANIM_OPEN_TEXT_OUT, pElementPtr_)
		);

		_Director.GetElementPtr(NAME_MGR::E_OPEN_DISCLAIMER_TEXT, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_OPEN,
			0,
			new ofxFadeInAnimation(NAME_MGR::ANIM_OPEN_DISCLAIMER_IN, pElementPtr_)
		);

		_Director.GetElementPtr(NAME_MGR::E_OPEN_BTN, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_OPEN,
			1,
			new ofxFadeOutAnimation(NAME_MGR::ANIM_OPEN_BTN_OUT, pElementPtr_)
		);

		_Director.GetElementPtr(NAME_MGR::E_OPEN_YES_NO_BTN, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_OPEN,
			1,
			new ofxFadeInAnimation(NAME_MGR::ANIM_OPEN_YES_NO_IN, pElementPtr_)
		);
	}
	else if(name == NAME_MGR::INIT_OPEN_DISCLAIMER_BACK)
	{
		_Director.GetElementPtr(NAME_MGR::E_OPEN_DISCLAIMER_TEXT, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_OPEN, 
			0, 
			new ofxFadeOutAnimation(NAME_MGR::ANIM_OPEN_DISCLAIMER_OUT, pElementPtr_)
		);

		_Director.GetElementPtr(NAME_MGR::E_OPEN_START_TEXT, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_OPEN,
			0,
			new ofxFadeInAnimation(NAME_MGR::ANIM_OPEN_TEXT_IN, pElementPtr_)
		);

		_Director.GetElementPtr(NAME_MGR::E_OPEN_YES_NO_BTN, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_OPEN,
			1,
			new ofxFadeOutAnimation(NAME_MGR::ANIM_OPEN_YES_NO_OUT, pElementPtr_)
		);

		_Director.GetElementPtr(NAME_MGR::E_OPEN_BTN, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_OPEN,
			1,
			new ofxFadeInAnimation(NAME_MGR::ANIM_OPEN_BTN_IN, pElementPtr_)
		);
	}
}
#pragma endregion

#pragma region S_INTRO
//--------------------------------------------------------------
void AllianzTheatre::initialIntro()
{
	//Plane
	_Director.AddPlane(NAME_MGR::S_INTRO, NAME_MGR::P_INTRO_BG, 0);
	_Director.AddPlane(NAME_MGR::S_INTRO, NAME_MGR::P_INTRO_UI, 1);

	//Elements
	_Director.AddElement(NAME_MGR::E_INTRO_BACKPLANE, NAME_MGR::P_INTRO_UI, NAME_MGR::A_TEXT_BACKPLANE, 10, ofVec2f(436, 92), false);
	_Director.AddElement(NAME_MGR::E_INTRO_INTRO_1_TEXT, NAME_MGR::P_INTRO_UI, NAME_MGR::A_INTRO_1, 11, ofVec2f(0), false);
	_Director.AddElement(NAME_MGR::E_INTRO_INTRO_2_TEXT, NAME_MGR::P_INTRO_UI, NAME_MGR::A_INTRO_2, 12, ofVec2f(0), false);
	_Director.AddElement(NAME_MGR::E_INTRO_INTRO_MSG_TEXT, NAME_MGR::P_INTRO_UI, NAME_MGR::A_MSG, 13, ofVec2f(0), false);

	_Director.AddElement(NAME_MGR::E_INTRO_NEXT_BTN, NAME_MGR::P_INTRO_UI, NAME_MGR::A_NEXT_BTN, 5, ofVec2f(0), false);
	_Director.AddElement(NAME_MGR::E_INTRO_START_BTN, NAME_MGR::P_INTRO_UI, NAME_MGR::A_START_BTN, 6, ofVec2f(0), false);

	_Director.AddElement(NAME_MGR::E_INTRO_LOGO, NAME_MGR::P_INTRO_UI, NAME_MGR::A_LOGO_2, 15);

	_Director.AddElement(NAME_MGR::E_INTRO_LOOP, NAME_MGR::P_INTRO_BG, NAME_MGR::A_LOVE_LOOP, 0);
	_Director.AddElement(NAME_MGR::E_INTRO_TEACH, NAME_MGR::P_INTRO_BG, NAME_MGR::A_LOVE_EX, 1, ofVec2f(0), false);
			
	ofxVideoElement* pVideo_;
	_Director.GetElementPtr(NAME_MGR::E_INTRO_LOOP, pVideo_);
	pVideo_->SetVideoLoop(true);
	pVideo_->SetVideoAutoPlay(true);

	_Director.GetElementPtr(NAME_MGR::E_INTRO_TEACH, pVideo_);
	pVideo_->SetVideoLoop(true);
	pVideo_->SetVideoAutoPlay(false);
}

//--------------------------------------------------------------
void AllianzTheatre::updateIntro()
{
}

//--------------------------------------------------------------
void AllianzTheatre::animInitIntro(string name)
{
	ofxBaseElement* pElementPtr_;
	if(name == NAME_MGR::S_INTRO)
	{
		_Director.GetElementPtr(NAME_MGR::E_INTRO_BACKPLANE, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_INTRO, 
			0, 
			new ofxEnterAnimation(NAME_MGR::ANIM_INTRO_BACKPLANE_ENTER, pElementPtr_, ENTER_TYPE::eENTER_FROM_LEFT, ofGetWidth(), ofGetHeight(), 0.5f, 2.0)
		);

		_Director.GetElementPtr(NAME_MGR::E_INTRO_INTRO_1_TEXT, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_INTRO,
			0,
			new ofxFadeInAnimation(NAME_MGR::ANIM_INTRO_INTRO_IN, pElementPtr_)
		);

		_Director.GetElementPtr(NAME_MGR::E_INTRO_NEXT_BTN, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_INTRO,
			0,
			new ofxFadeInAnimation(NAME_MGR::ANIM_INTRO_NEXT_IN, pElementPtr_)
		);
	}
	else if(name == NAME_MGR::INIT_INTRO_NEXT)
	{
		_Director.GetElementPtr(NAME_MGR::E_INTRO_INTRO_1_TEXT, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_INTRO,
			0,
			new ofxFadeOutAnimation(NAME_MGR::ANIM_INTRO_INTRO_OUT, pElementPtr_)
		);

		_Director.GetElementPtr(NAME_MGR::E_INTRO_NEXT_BTN, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_INTRO,
			1,
			new ofxFadeOutAnimation(NAME_MGR::ANIM_INTRO_NEXT_OUT, pElementPtr_)
		);

		_Director.GetElementPtr(NAME_MGR::E_INTRO_INTRO_2_TEXT, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_INTRO,
			0,
			new ofxFadeInAnimation(NAME_MGR::ANIM_INTRO_INTRO2_IN, pElementPtr_)
		);

		_Director.GetElementPtr(NAME_MGR::E_INTRO_INTRO_2_TEXT, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_INTRO,
			0,
			new ofxFadeOutAnimation(NAME_MGR::ANIM_INTRO_INTRO2_OUT, pElementPtr_, 0.5f, 3.0f)
		);

		_Director.GetElementPtr(NAME_MGR::E_INTRO_TEACH, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_INTRO,
			0,
			new ofxFadeInAnimation(NAME_MGR::ANIM_INTRO_TEACH_IN, pElementPtr_)
		);

		_Director.GetElementPtr(NAME_MGR::E_INTRO_INTRO_MSG_TEXT, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_INTRO,
			0,
			new ofxFadeInAnimation(NAME_MGR::ANIM_INTRO_MSG_IN, pElementPtr_)
		);

		_Director.GetElementPtr(NAME_MGR::E_INTRO_START_BTN, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_INTRO,
			0,
			new ofxFadeInAnimation(NAME_MGR::ANIM_INTRO_START_IN, pElementPtr_)
		);
	}
}

//--------------------------------------------------------------
void AllianzTheatre::setIntro()
{
	ofxDynamicImageElement* ptr_;
	_Director.GetElementPtr(NAME_MGR::E_INTRO_INTRO_1_TEXT, ptr_);
	ptr_->updateImg(_DisplayMap[_eMode].intro);

	_Director.GetElementPtr(NAME_MGR::E_INTRO_INTRO_2_TEXT, ptr_);
	ptr_->updateImg(_DisplayMap[_eMode].intro2);

	_Director.GetElementPtr(NAME_MGR::E_INTRO_INTRO_MSG_TEXT, ptr_);
	ptr_->updateImg(_DisplayMap[_eMode].msg);

	ofxVideoElement* VideoPtr_;
	ofxVideoActor *exActorPtr_, *loopActorPtr_;
	
	switch(_eMode)
	{
	case ePhotoLove:
		{
			exActorPtr_ = dynamic_cast<ofxVideoActor*>(_Director.GetActor(NAME_MGR::A_LOVE_EX));
			loopActorPtr_ = dynamic_cast<ofxVideoActor*>(_Director.GetActor(NAME_MGR::A_LOVE_LOOP));
		}
		break;
	case ePhotoYourself:
		{
			exActorPtr_ = dynamic_cast<ofxVideoActor*>(_Director.GetActor(NAME_MGR::A_YOURSELF_EX));
			loopActorPtr_ = dynamic_cast<ofxVideoActor*>(_Director.GetActor(NAME_MGR::A_YOURSELF_LOOP));
		}
		break;
	case ePhotoChange:
		{
			exActorPtr_ = dynamic_cast<ofxVideoActor*>(_Director.GetActor(NAME_MGR::A_CHANGE_EX));
			loopActorPtr_ = dynamic_cast<ofxVideoActor*>(_Director.GetActor(NAME_MGR::A_CHANGE_LOOP));
		}
		break;
	case ePhotoFly:
		{
			exActorPtr_ = dynamic_cast<ofxVideoActor*>(_Director.GetActor(NAME_MGR::A_FLY_EX));
			loopActorPtr_ = dynamic_cast<ofxVideoActor*>(_Director.GetActor(NAME_MGR::A_FLY_LOOP));
		}
		break;
	}

	//Change loop
	_Director.GetElementPtr(NAME_MGR::E_INTRO_LOOP, VideoPtr_);	
	VideoPtr_->ChangeVideoActor(loopActorPtr_);

	//Change example
	_Director.GetElementPtr(NAME_MGR::E_INTRO_TEACH, VideoPtr_);	
	VideoPtr_->ChangeVideoActor(exActorPtr_);

}
#pragma endregion

#pragma region S_PHOTO
//--------------------------------------------------------------
int AllianzTheatre::getShootNum()
{
	return _iShootNum;
}

//--------------------------------------------------------------
ofImage& AllianzTheatre::getPhotoframe()
{
	return _DisplayMap[_eMode].phtotframe;
}

//--------------------------------------------------------------
void AllianzTheatre::initialPhoto()
{
	//Plane
	_Director.AddPlane(NAME_MGR::S_PHOTO, NAME_MGR::P_PHOTO_BG, 0);
	_Director.AddPlane(NAME_MGR::S_PHOTO, NAME_MGR::P_PHOTO_UI, 1);

	//Elements
	_Director.AddElement(NAME_MGR::E_PHOTO_SHOOT, NAME_MGR::P_PHOTO_UI, NAME_MGR::A_SHOOT, 1, ofVec2f(0), false);

	_Director.AddElement(NAME_MGR::E_PHOTO_COUNTDOWN3, NAME_MGR::P_PHOTO_UI, NAME_MGR::A_COUNTDOWN_3, 2, ofVec2f(423, 741), false);
	_Director.AddElement(NAME_MGR::E_PHOTO_COUNTDOWN5, NAME_MGR::P_PHOTO_UI, NAME_MGR::A_COUNTDOWN_5, 3, ofVec2f(423, 741), false);	

	_Director.AddElement(NAME_MGR::E_PHOTO_BACKPLANE, NAME_MGR::P_PHOTO_UI, NAME_MGR::A_TEXT_BACKPLANE, 5, ofVec2f(436, 92), false);
	_Director.AddElement(NAME_MGR::E_PHOTO_RESULT_TEXT, NAME_MGR::P_PHOTO_UI, NAME_MGR::A_FINISH_TEXT, 6, ofVec2f(0), false);

	_Director.AddElement(NAME_MGR::E_PHOTO_HOME, NAME_MGR::P_PHOTO_UI, NAME_MGR::A_HOME_BTN, 7, ofVec2f(0), false);

	_Director.AddElement(NAME_MGR::E_PHOTO_TITLE, NAME_MGR::P_PHOTO_UI, NAME_MGR::A_PHOTO_TITLE, 10, ofVec2f(0), false);
	_Director.AddElement(NAME_MGR::E_PHOTO_LOGO, NAME_MGR::P_PHOTO_UI, NAME_MGR::A_LOGO_2, 10);
	_Director.AddElement(NAME_MGR::E_PHOTO_WAITING, NAME_MGR::P_PHOTO_UI, NAME_MGR::A_WAITING_PAGE, 15, ofVec2f(0), false);

	//Setting
	ofxVideoElement* VideoPtr_;
	_Director.GetElementPtr(NAME_MGR::E_PHOTO_WAITING, VideoPtr_);
	VideoPtr_->SetVideoLoop(true);
	VideoPtr_->SetVideoAutoPlay(true);

	ofxAnimationImageElement* AnimPtr_;
	_Director.GetElementPtr(NAME_MGR::E_PHOTO_COUNTDOWN5, AnimPtr_);
	AnimPtr_->SetSPF(1);
	AnimPtr_->SetEvent(true);
	
	_Director.GetElementPtr(NAME_MGR::E_PHOTO_COUNTDOWN3, AnimPtr_);
	AnimPtr_->SetSPF(1);
	AnimPtr_->SetEvent(true);

	//Initial shoot image
	ofImage shoot1_, shoot2_, shoot3_, shoot4_;
	shoot1_.loadImage("images/shoot_1.png");
	_shootImg[0] = shoot1_;
	shoot2_.loadImage("images/shoot_2.png");
	_shootImg[1] = shoot2_;
	shoot3_.loadImage("images/shoot_3.png");
	_shootImg[2] = shoot3_;
	shoot4_.loadImage("images/shoot_4.png");
	_shootImg[3] = shoot4_;

	_iShootNum = 0;
	_bIsTakePicture = true;
}

//--------------------------------------------------------------
void AllianzTheatre::updatePhoto()
{
}

//--------------------------------------------------------------
void AllianzTheatre::animInitPhoto(string name)
{
	ofxBaseElement* pElementPtr_;
	if(name == NAME_MGR::S_PHOTO)
	{
		_Director.GetElementPtr(NAME_MGR::E_PHOTO_TITLE, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_PHOTO,
			0,
			new ofxFadeInAnimation(NAME_MGR::ANIM_PHOTO_TITLE_IN, pElementPtr_, 0.5f, 1.0f)
		);

		_Director.GetElementPtr(NAME_MGR::E_PHOTO_SHOOT, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_PHOTO,
			0,
			new ofxFadeInAnimation(NAME_MGR::ANIM_PHOTO_SHOOT_IN, pElementPtr_, 0.2f)
		);
	}
	else if(name == NAME_MGR::INIT_PHOTO_SHOT_IN)
	{
		_Director.GetElementPtr(NAME_MGR::E_PHOTO_SHOOT, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_PHOTO,
			0,
			new ofxFadeInAnimation(NAME_MGR::ANIM_PHOTO_SHOOT_IN, pElementPtr_, 0.2f)
		);
	}
		else if(name == NAME_MGR::INIT_PHOTO_SHOT_OUT)
	{
		_Director.GetElementPtr(NAME_MGR::E_PHOTO_SHOOT, pElementPtr_);

		_Director.AddAnimation(
			NAME_MGR::S_PHOTO,
			0,
			new ofxFadeOutAnimation(NAME_MGR::ANIM_PHOTO_SHOOT_OUT, pElementPtr_, 0.2f)
		);
	}
	else if(name == NAME_MGR::INIT_PHOTO_WAIT_IN)
	{
		_Director.GetElementPtr(NAME_MGR::E_PHOTO_WAITING, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_PHOTO,
			0,
			new ofxEnterAnimation(NAME_MGR::ANIM_PHOTO_WAIT_ENTER, pElementPtr_, ENTER_TYPE::eENTER_FROM_LEFT, ofGetWidth(), ofGetHeight(), 0.5f, 1.0)
		);
	}
	else if(name == NAME_MGR::INIT_PHOTO_SHOW_RESULT)
	{
		_Director.GetElementPtr(NAME_MGR::E_PHOTO_WAITING, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_PHOTO,
			0,
			new ofxExitAnimation(NAME_MGR::ANIM_PHOTO_WAIT_LEAVE, pElementPtr_, EXIT_TYPE::eEXIT_TO_RIGHT, ofGetWidth(), ofGetHeight(), 0.5f, 1.0)
		);

		_Director.GetElementPtr(NAME_MGR::E_PHOTO_BACKPLANE, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_PHOTO,
			0,
			new ofxEnterAnimation(NAME_MGR::ANIM_PHOTO_BACKPLANE_ENTER, pElementPtr_, ENTER_TYPE::eENTER_FROM_LEFT, ofGetWidth(), ofGetHeight(), 0.5f, 3.0)
		);

		_Director.GetElementPtr(NAME_MGR::E_PHOTO_RESULT_TEXT, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_PHOTO,
			0,
			new ofxFadeInAnimation(NAME_MGR::ANIM_PHOTO_RESULT_TEXT_IN, pElementPtr_)
		);

		_Director.GetElementPtr(NAME_MGR::E_PHOTO_HOME, pElementPtr_);
		_Director.AddAnimation(
			NAME_MGR::S_PHOTO,
			0,
			new ofxFadeInAnimation(NAME_MGR::ANIM_PHOTO_HOME_BTN_IN, pElementPtr_)
		);
	}
}

//--------------------------------------------------------------
void AllianzTheatre::setPhoto()
{
	ofxDynamicImageElement* ptr_;
	_Director.GetElementPtr(NAME_MGR::E_PHOTO_TITLE, ptr_);
	ptr_->updateImg(_DisplayMap[_eMode].photoTitle);

	_iShootNum = 0;
	updateShoot();
}

//--------------------------------------------------------------
void AllianzTheatre::updateShoot()
{
	ofxDynamicImageElement* ptr_;

	_Director.GetElementPtr(NAME_MGR::E_PHOTO_SHOOT, ptr_);
	ptr_->updateImg(_shootImg[_iShootNum]);
}

//--------------------------------------------------------------
void AllianzTheatre::processingFinish()
{
	AnimInit(NAME_MGR::INIT_PHOTO_SHOW_RESULT);
}

#pragma endregion

#pragma region Display Element
//--------------------------------------------------------------
void AllianzTheatre::initialDisplayElement()
{
	stDisplayElement Love_, Yourself_, Change_, Fly_;
	Love_.photoTitle.loadImage("images/title_love.png");
	Love_.intro.loadImage("images/text/tLoveIntro.png");
	Love_.intro2.loadImage("images/text/tLoveIntro2.png");
	Love_.msg.loadImage("images/text/tLoveMsg.png");
	Love_.phtotframe.loadImage("images/photoframe/pf_love.png");

	_DisplayMap[ePhotoLove] = Love_;

	Yourself_.photoTitle.loadImage("images/title_yourself.png");
	Yourself_.intro.loadImage("images/text/tYourselfIntro.png");
	Yourself_.intro2.loadImage("images/text/tYourselfIntro2.png");
	Yourself_.msg.loadImage("images/text/tYourselfMsg.png");
	Yourself_.phtotframe.loadImage("images/photoframe/pf_yourself.png");
	_DisplayMap[ePhotoYourself] = Yourself_;

	Change_.photoTitle.loadImage("images/title_change.png");
	Change_.intro.loadImage("images/text/tChangeIntro.png");
	Change_.intro2.loadImage("images/text/tChangeIntro2.png");
	Change_.msg.loadImage("images/text/tChangeMsg.png");
	Change_.phtotframe.loadImage("images/photoframe/pf_change.png");
	_DisplayMap[ePhotoChange] = Change_;

	Fly_.photoTitle.loadImage("images/title_fly.png");
	Fly_.intro.loadImage("images/text/tFlyIntro.png");
	Fly_.intro2.loadImage("images/text/tFlyIntro2.png");
	Fly_.msg.loadImage("images/text/tFlyMsg.png");
	Fly_.phtotframe.loadImage("images/photoframe/pf_fly.png");
	_DisplayMap[ePhotoFly] = Fly_;


}
#pragma endregion

#pragma region Mouse Event
//--------------------------------------------------------------
void AllianzTheatre::mousePressed(ofMouseEventArgs & args)
{
	string strScenesName_ = _Director.GetNowScenes()->GetScenesName();
	
	//Exit check
	if(strScenesName_ == NAME_MGR::S_INTRO)
	{
		if(cRESET_BTN_RECT.inside(args))
		{
			if(!_bClickReset)
			{
				_bClickReset = true;
				_fResetTimer = cRESET_TIMER;
			}
		}
	}	
	
	if(!_bCanCtrl)
	{
		return;
	}
	
	if(strScenesName_ == NAME_MGR::S_OPEN)
	{
		if(_bWaiting)
		{
			//Open
			AnimInit(NAME_MGR::INIT_OPEN_TO_DISCLAIMER);
			_bCanCtrl = false;
			_click.play();
		}
		else
		{
			//Dirslaimer
			if(cOPEN_YSE_BTN_RECT.inside(args))
			{
				nextScnece();
				_bCanCtrl = false;
				_click.play();
			}
			else if(cOPEN_NO_BTN_RECT.inside(args))
			{
				AnimInit(NAME_MGR::INIT_OPEN_DISCLAIMER_BACK);
				_bCanCtrl = false;
				_click.play();
			}
		}
	}
	else if(strScenesName_ == NAME_MGR::S_INTRO)
	{
		if(_bNext)
		{
			//Next
			if(cINTRO_NEXT_BTN_RECT.inside(args))
			{
				AnimInit(NAME_MGR::INIT_INTRO_NEXT);
				_bCanCtrl = false;
				_click.play();
			}
		}
		else
		{
			//Teach
			if(cINTRO_START_BTN_RECT.inside(args))
			{
				nextScnece();
				_bCanCtrl = false;
				_click.play();
			}			
		}
	}
	else if(strScenesName_ == NAME_MGR::S_PHOTO)
	{
		if(cPHOTO_HOME_BTN_RECT.inside(args))
		{
			nextScnece();
			_bCanCtrl = false;
		}
	}	
}

//--------------------------------------------------------------
void AllianzTheatre::mouseReleased(ofMouseEventArgs & args)
{
	if(_bClickReset)
	{
		_bClickReset = false;
		_fResetTimer = 0.0;
	}
}
#pragma endregion

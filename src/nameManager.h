#ifndef _NAME_MANAGER_
#define _NAME_MANAGER_

#include <string>
using namespace std;

#define CREATE_VALUE(NAME) static const string NAME = #NAME;
#define CREATE_VALUE_WITH_VALUE(NAME, VALUE) static const string NAME = VALUE;

namespace NAME_MGR
{
//-------------------------------------------------
//Theatre
//-------------------------------------------------
///////////////////////////////////////
// Scenes
//////////////////////////////////////
	CREATE_VALUE(S_OPEN);
	CREATE_VALUE(S_INTRO);
	CREATE_VALUE(S_PHOTO);

///////////////////////////////////////
// Actor
//////////////////////////////////////
	//Open
	CREATE_VALUE(A_LOGO_1);
	CREATE_VALUE(A_LOGO_2);
	CREATE_VALUE(A_LOOP_BG);
	CREATE_VALUE(A_TEXT_BACKPLANE);
	CREATE_VALUE(A_START_TEXT);
	CREATE_VALUE(A_OPEN_BTN);
	CREATE_VALUE(A_DISCLAIMER);
	CREATE_VALUE(A_YES_NO_BTN);
	
	//Intro
	CREATE_VALUE(A_NEXT_BTN);
	CREATE_VALUE(A_START_BTN);

	CREATE_VALUE(A_LOVE_LOOP);
	CREATE_VALUE(A_YOURSELF_LOOP);
	CREATE_VALUE(A_CHANGE_LOOP);
	CREATE_VALUE(A_FLY_LOOP);
	
	CREATE_VALUE(A_YOURSELF_EX);
	CREATE_VALUE(A_CHANGE_EX);
	CREATE_VALUE(A_FLY_EX);
	CREATE_VALUE(A_LOVE_EX);

	CREATE_VALUE(A_INTRO_1);
	CREATE_VALUE(A_INTRO_2);
	CREATE_VALUE(A_MSG);

	//Photo
	CREATE_VALUE(A_PHOTO_TITLE);
	CREATE_VALUE(A_SHOOT);
	CREATE_VALUE(A_WAITING_PAGE);
	CREATE_VALUE(A_RESULT);
	CREATE_VALUE(A_FINISH_TEXT);
	CREATE_VALUE(A_HOME_BTN);
	
	CREATE_VALUE(A_COUNTDOWN_5);
	CREATE_VALUE(A_COUNTDOWN_3);

///////////////////////////////////////
// Plane
///////////////////////////////////////
	CREATE_VALUE(P_OPEN_BG);
	CREATE_VALUE(P_OPEN_UI);

	CREATE_VALUE(P_INTRO_BG);
	CREATE_VALUE(P_INTRO_UI);

	CREATE_VALUE(P_PHOTO_BG);
	CREATE_VALUE(P_PHOTO_UI);

///////////////////////////////////////
// Element
//////////////////////////////////////
	//Open
	CREATE_VALUE(E_OPEN_BACKPLANE);
	CREATE_VALUE(E_OPEN_START_TEXT);
	CREATE_VALUE(E_OPEN_BTN)
	CREATE_VALUE(E_OPEN_LOOP);
	CREATE_VALUE(E_OPEN_LOGO);
	CREATE_VALUE(E_OPEN_DISCLAIMER_TEXT);
	CREATE_VALUE(E_OPEN_YES_NO_BTN);

	//Intro
	CREATE_VALUE(E_INTRO_LOGO);
	CREATE_VALUE(E_INTRO_BACKPLANE);
	CREATE_VALUE(E_INTRO_INTRO_1_TEXT);
	CREATE_VALUE(E_INTRO_INTRO_2_TEXT);
	CREATE_VALUE(E_INTRO_INTRO_MSG_TEXT);
	CREATE_VALUE(E_INTRO_NEXT_BTN);
	CREATE_VALUE(E_INTRO_START_BTN);
	CREATE_VALUE(E_INTRO_LOOP);
	CREATE_VALUE(E_INTRO_TEACH);
	
	//Photo
	CREATE_VALUE(E_PHOTO_LOGO);
	CREATE_VALUE(E_PHOTO_TITLE);
	CREATE_VALUE(E_PHOTO_COUNTDOWN3);
	CREATE_VALUE(E_PHOTO_COUNTDOWN5);
	CREATE_VALUE(E_PHOTO_SHOOT);
	CREATE_VALUE(E_PHOTO_WAITING);
	CREATE_VALUE(E_PHOTO_BACKPLANE);
	CREATE_VALUE(E_PHOTO_RESULT_TEXT);
	CREATE_VALUE(E_PHOTO_HOME);
	
///////////////////////////////////////
// Animation Init
//////////////////////////////////////
	//Open
	CREATE_VALUE(INIT_OPEN_TO_DISCLAIMER);
	CREATE_VALUE(INIT_OPEN_DISCLAIMER_BACK);

	//Intro;
	CREATE_VALUE(INIT_INTRO_NEXT);
	
	//Photo
	CREATE_VALUE(INIT_PHOTO_SHOT_IN);
	CREATE_VALUE(INIT_PHOTO_SHOT_OUT);
	CREATE_VALUE(INIT_PHOTO_WAIT_IN);
	CREATE_VALUE(INIT_PHOTO_SHOW_RESULT);

///////////////////////////////////////
// Animation
//////////////////////////////////////
	//Open
	CREATE_VALUE(ANIM_OPEN_BACKPLANE_ENTER);
	CREATE_VALUE(ANIM_OPEN_TEXT_IN);
	CREATE_VALUE(ANIM_OPEN_BTN_IN);

	CREATE_VALUE(ANIM_OPEN_TEXT_OUT);
	CREATE_VALUE(ANIM_OPEN_DISCLAIMER_IN);
	CREATE_VALUE(ANIM_OPEN_BTN_OUT);
	CREATE_VALUE(ANIM_OPEN_YES_NO_IN);

	CREATE_VALUE(ANIM_OPEN_DISCLAIMER_OUT);
	CREATE_VALUE(ANIM_OPEN_YES_NO_OUT);

	//Intro
	CREATE_VALUE(ANIM_INTRO_BACKPLANE_ENTER);
	CREATE_VALUE(ANIM_INTRO_INTRO_IN);
	CREATE_VALUE(ANIM_INTRO_NEXT_IN);

	CREATE_VALUE(ANIM_INTRO_INTRO_OUT);
	CREATE_VALUE(ANIM_INTRO_NEXT_OUT);
	CREATE_VALUE(ANIM_INTRO_INTRO2_IN);
	CREATE_VALUE(ANIM_INTRO_INTRO2_OUT);
	CREATE_VALUE(ANIM_INTRO_TEACH_IN);
	CREATE_VALUE(ANIM_INTRO_MSG_IN);	
	CREATE_VALUE(ANIM_INTRO_START_IN);

	//Photo
	CREATE_VALUE(ANIM_PHOTO_TITLE_IN);

	CREATE_VALUE(ANIM_PHOTO_SHOOT_IN);
	CREATE_VALUE(ANIM_PHOTO_SHOOT_OUT);

	CREATE_VALUE(ANIM_PHOTO_WAIT_ENTER);

	CREATE_VALUE(ANIM_PHOTO_WAIT_LEAVE);
	CREATE_VALUE(ANIM_PHOTO_BACKPLANE_ENTER);
	CREATE_VALUE(ANIM_PHOTO_RESULT_TEXT_IN);
	CREATE_VALUE(ANIM_PHOTO_HOME_BTN_IN);

///////////////////////////////////////
// Outside event
//////////////////////////////////////
	CREATE_VALUE(T_DISPLAY_WEBCAM);
	CREATE_VALUE(T_TAKE_PICTURE);
	CREATE_VALUE(T_START_PROCESS);
	CREATE_VALUE(T_RESTART);
};



#endif //_NAME_MANAGER_


#ifndef CONST_PARAMETER
#define CONST_PARAMETER

#include <thread>

#include "ofMain.h"
#include "nameManager.h"

const static int cWINDOW_WIDTH = 1080;
const static int cWINDOW_HEIGHT = 1920;
const static int cPHOTO_WIDTH = 1080;
const static int cPHOTO_HEIGHT = 1920;

const static int cWEBCAM_WIDTH = 1920;
const static int cWEBCAM_HEIGHT = 1080;

const static int cDISPLAY_HEIGHT = 1737;
const static int cDISPLAY_WIDTH = 1080;

//Photo
const static string cPHOTO_FOLDER_PATH = "photos/";
const static int cPHOTO_NUM = 4;
const static float cPHOTO_SAVE_SCALE = 0.4;
const static float cPHOTO_FPS = 1.0;

//Gif
const static int cCMD_MAX_SIZE = 100;
const static string cBASE_CMD_1 = "convert -loop 0 -delay 80x100 data/" + cPHOTO_FOLDER_PATH;
const static string cBASE_CMD_2	= "_*.png -set dispose background data/gif/";

//FB
const static string cFB_SHARE_URL = "http://events.artgital.com/allianz/s/share.php?id=";

#endif // !CONST_PARAMETER

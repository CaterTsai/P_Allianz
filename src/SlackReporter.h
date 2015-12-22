#ifndef SLACK_REPORTER
#define SLACK_REPORTER

#include "ofMain.h"
#include "ofxHttpUtils.h"

#include "../json/json.h"


#ifdef _DEBUG
	#ifndef _JSON
		#define _JSON
		#pragma comment(lib,"json/lib/lib_json_debug.lib")
	#endif //_JSON
#else //_DEBUG
	#ifndef _JSON
		#define _JSON
		#pragma comment(lib,"json/lib/lib_json.lib")
	#endif //_JSON
#endif //_DEBUG

const static string cSLACK_URL = "https://hooks.slack.com/services/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";

class SlackReporter
{
public:
	SlackReporter();
	~SlackReporter();

	void setup();
	void exit();
	void onHttpResponse(ofxHttpResponse& response);
	void Report(string strMsg);
	

public:
	bool			_bSetup;
	ofxHttpUtils	_SlackReporter;
};

#endif // !SLACK_REPORTER

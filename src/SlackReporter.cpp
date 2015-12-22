#include "SlackReporter.h"

//--------------------------------------------------------------
SlackReporter::SlackReporter()
	:_bSetup(false)
{
	//ofAddListener(_SlackReporter.newResponseEvent, this, &SlackReporter::onHttpResponse);
}

//--------------------------------------------------------------
SlackReporter::~SlackReporter()
{	

}

//--------------------------------------------------------------
void SlackReporter::setup()
{
	_bSetup = true;
	_SlackReporter.start();
}

//--------------------------------------------------------------
void SlackReporter::exit()
{

	_SlackReporter.stop();
}

//--------------------------------------------------------------
void SlackReporter::onHttpResponse(ofxHttpResponse& response)
{
	//To-DO
}

//--------------------------------------------------------------
void SlackReporter::Report(string strMsg)
{
	if(!_bSetup)
	{
		return;
	}
	ofxHttpForm	HttpForm_;
	
	HttpForm_.action = cSLACK_URL;
	HttpForm_.method = OFX_HTTP_POST;

	Json::Value Root_;
	Json::FastWriter Writer_;
	Root_["text"] = strMsg;

	string strJson_ = Writer_.write(Root_);

	HttpForm_.addFormField("payload", strJson_);

	_SlackReporter.addForm(HttpForm_);
}

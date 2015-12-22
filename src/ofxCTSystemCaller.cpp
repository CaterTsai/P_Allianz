#include "ofxCTSystemCaller.h"

//--------------------------------------------------------------
//@CLASS ofxCTSystemCaller
//--------------------------------------------------------------
void ofxCTSystemCaller::addCMD(string strCmd)
{
	stSYSTEM_CMD	newCMD_;
	newCMD_.strCMD = strCmd;
	_CmdQueue.push(newCMD_);
}

//--------------------------------------------------------------
void ofxCTSystemCaller::signal()
{
	_Condition.signal();
}

//--------------------------------------------------------------
void ofxCTSystemCaller::threadedFunction()
{
	while(isThreadRunning())
	{
		if(!_CmdQueue.empty())
		{
			stSYSTEM_CMD Cmd_ = _CmdQueue.front();
			system(Cmd_.strCMD.c_str());
			_CmdQueue.pop();

			if(_CmdQueue.empty())
			{
				string strMsg_ = "SUCCESS";
				ofNotifyEvent(SystemCallerFinishEvent, strMsg_, this);
			}
		}
		else
		{
			_Condition.wait(_Mutex);
		}
	}
}
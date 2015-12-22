#ifndef OFX_CT_SYSTEM_CALLER
#define OFX_CT_SYSTEM_CALLER

#include "ofMain.h"

typedef struct _stSYSTEM_CMD
{
	string strCMD;
}stSYSTEM_CMD;

class ofxCTSystemCaller : ofThread
{
public:
	
	void addCMD(string strCmd);
	void signal();
	void threadedFunction();
	

///////////////////////
//Inline method
///////////////////////
	inline void start()
	{
		startThread(true, false);
	}

	inline void stop()
	{
		stopThread();
	}
	
private:

	queue<stSYSTEM_CMD>		_CmdQueue;
	ofMutex					_Mutex;
	Poco::Condition			_Condition;

////////////////////////////
//Event
////////////////////////////
public:
	ofEvent<string>	SystemCallerFinishEvent;
};


#endif // !OFX_CT_SYSTEM_CALLER

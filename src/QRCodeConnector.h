#ifndef QR_CONNECTOR
#define QR_CONNECTOR

#include "ofMain.h"
#include "ofxNetwork.h"

#define cCMD_LENGTH 1000

const string cQR_START_TEX = "\x01";
const string cQR_END_TEX = "\x02";

class QRCodeConnector
{
public:
	void initQRConnector(string strIP, int iPort);
	void closeQRConnector();
	void printQR(string strValue = "");

private:
	ofxUDPManager	_qrSender;

};

#endif // !QR_CONNECTOR

#include "QRCodeConnector.h"

void QRCodeConnector::initQRConnector(string strIP, int iPort)
{
	//Sender
	_qrSender.Create();
	_qrSender.Connect(strIP.c_str(), iPort);
	_qrSender.SetNonBlocking(true);
}

//--------------------------------------------------------------
void QRCodeConnector::closeQRConnector()
{
	_qrSender.Close();
}

//--------------------------------------------------------------
void QRCodeConnector::printQR(string strValue)
{
	string CMD_ = cQR_START_TEX + strValue + cQR_END_TEX;
	
	_qrSender.Send(CMD_.c_str(), CMD_.length());

	ofLog(OF_LOG_NOTICE,"[QRCodeConnector]Print QR Code");
}
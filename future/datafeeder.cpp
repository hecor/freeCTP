#include <sstream>
#include <iostream>
#include <cstring>
#include "datafeeder.h"

void DataFeeder::init()
{
    m_pMdApi->RegisterSpi(this);
    m_pMdApi->RegisterFront(const_cast<char*>(this->front_address.c_str()));
    // 使客户端开始与后台服务建立连接
    m_pMdApi->Init();
}

void DataFeeder::OnFrontConnected()
{
	CThostFtdcReqUserLoginField reqUserLogin;
    strcpy(reqUserLogin.BrokerID, this->brokerID.c_str());
    strcpy(reqUserLogin.UserID, this->userID.c_str());
    strcpy(reqUserLogin.Password, this->passwd.c_str());
    m_pMdApi->ReqUserLogin(&reqUserLogin, 0);
}

void DataFeeder::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cout << "OnRspUserLogin:\t" << "ErrorCode=[" << pRspInfo->ErrorID << "], ErrorMsg=[" << pRspInfo->ErrorMsg << "]" << endl;
    if (pRspInfo->ErrorID != 0)
        cout << "Error: Login Error!" << endl;
}

void DataFeeder::subscrib_market_data(string InstrumentID)
{
	char* id = const_cast<char*>(InstrumentID.c_str());
	int result = m_pMdApi->SubscribeMarketData( &id, 1 );
	if( result != 0)
		cerr << "------->>>发送行情订阅请求失败!" << endl;
}

void DataFeeder::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	ostringstream oss;
	CThostFtdcDepthMarketDataField *p = pDepthMarketData;

	oss << p->InstrumentID << ',' << p->TradingDay << ',' << p->UpdateTime << ','
		<< p->LastPrice << ',' << p->PreClosePrice << ',' << p->OpenPrice << ',' << p->ClosePrice << ',' << p->HighestPrice << ',' << p->LowestPrice << ',' 
		<< p->Volume << ',' << p->Turnover << ',' 
		<< p->BidPrice1 << ',' << p->BidVolume1 << ',' << p->BidPrice2 << ',' << p->BidVolume2 << ',' << p->BidPrice3 << ',' << p->BidVolume3 << ',' 
		<< p->AskPrice1 << ',' << p->AskVolume1 << ',' << p->AskPrice2 << ',' << p->AskVolume2 << ',' << p->AskPrice3 << ',' << p->AskVolume3 << ',';
		

	stock_datas[p->InstrumentID] = oss.str();
//	cout << stock_datas[p->InstrumentID] << endl;
}

string DataFeeder::get_market_data(string InstrumentID)
{
	return this->stock_datas[InstrumentID];
}


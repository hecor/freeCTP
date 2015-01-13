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
	CSecurityFtdcReqUserLoginField reqUserLogin;
	memset(&reqUserLogin, 0, sizeof(reqUserLogin));
    strcpy(reqUserLogin.BrokerID, this->brokerID.c_str());
    strcpy(reqUserLogin.UserID, this->userID.c_str());
    strcpy(reqUserLogin.Password, this->passwd.c_str());
    m_pMdApi->ReqUserLogin(&reqUserLogin, 0);
}

void DataFeeder::OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cout << "OnRspUserLogin:\t" << "ErrorCode=[" << pRspInfo->ErrorID << "], ErrorMsg=[" << pRspInfo->ErrorMsg << "]" << endl;
    if (pRspInfo->ErrorID != 0)
        cout << "Error: Login Error!" << endl;
}

void DataFeeder::subscrib_market_data(string stockid)
{
	std::string InstrumentID = stockid.substr(2);
	std::string ExchangeID = this->ExchangeIDDict[ stockid.substr(0,2) ];
	char* id = const_cast<char*>(InstrumentID.c_str());
	int result = m_pMdApi->SubscribeMarketData( &id, 1, const_cast<char*>(ExchangeID.c_str()) );
	if( result != 0)
		cerr << "------->>>Failed to subscribe " << ExchangeID << ":" << InstrumentID<< endl;
	else
		cerr << "------->>>Success to subscribe " << ExchangeID << ":" << InstrumentID<< endl;
}

void DataFeeder::OnRtnDepthMarketData(CSecurityFtdcDepthMarketDataField *pDepthMarketData)
{
	CSecurityFtdcDepthMarketDataField *p = pDepthMarketData;
	string stockid = this->ExchangeIDDict_Reverse[p->ExchangeID] + p->InstrumentID;

	rapidjson::StringBuffer s;
	rapidjson::Writer<rapidjson::StringBuffer> writer(s);
	writer.StartObject();

	writer.String("id");
	writer.String(stockid.c_str());
	writer.String("date");
	writer.String(p->TradingDay);
	writer.String("time");
	writer.String(p->UpdateTime);
	writer.String("current_price");
	writer.Double(p->LastPrice);
	writer.String("yesterday_close_price");
	writer.Double(p->PreClosePrice);
	writer.String("today_open_price");
	writer.Double(p->OpenPrice);
	writer.String("today_close_price");
	writer.Double(p->ClosePrice);
	writer.String("today_highest_price");
	writer.Double(p->HighestPrice);
	writer.String("today_lowest_price");
	writer.Double(p->LowestPrice);
	writer.String("volume");
	writer.Int(p->Volume);
	writer.String("turnover");
	writer.Double(p->Turnover);
	writer.String("buy_1_price");
	writer.Double(p->BidPrice1);
	writer.String("buy_1_amount");
	writer.Int(p->BidVolume1);
	writer.String("buy_2_price");
	writer.Double(p->BidPrice2);
	writer.String("buy_2_amount");
	writer.Int(p->BidVolume2);
	writer.String("buy_3_price");
	writer.Double(p->BidPrice3);
	writer.String("buy_3_amount");
	writer.Int(p->BidVolume3);
	writer.String("sell_1_price");
	writer.Double(p->AskPrice1);
	writer.String("sell_1_amount");
	writer.Int(p->AskVolume1);
	writer.String("sell_2_price");
	writer.Double(p->AskPrice2);
	writer.String("sell_2_amount");
	writer.Int(p->AskVolume2);
	writer.String("sell_3_price");
	writer.Double(p->AskPrice3);
	writer.String("sell_3_amount");
	writer.Int(p->AskVolume3);

	writer.EndObject();
	stock_datas[stockid] = s.GetString();
}

string DataFeeder::get_market_data(string stockid)
{
	return this->stock_datas[stockid];
}


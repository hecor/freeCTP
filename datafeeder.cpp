#include <sstream>
#include <iostream>
#include <cstring>
#include "datafeeder.h"

void DataFeeder::init()
{
    m_pLevel2Api->RegisterSpi(this);
    m_pLevel2Api->RegisterFront(const_cast<char*>(this->front_address.c_str()));
    // 使客户端开始与后台服务建立连接
    m_pLevel2Api->Init();
}

void DataFeeder::OnFrontConnected()
{
	CThostFtdcReqUserLoginField reqUserLogin;
    strcpy(reqUserLogin.BrokerID, this->brokerID.c_str());
    strcpy(reqUserLogin.UserID, this->userID.c_str());
    strcpy(reqUserLogin.Password, this->passwd.c_str());
    m_pLevel2Api->ReqUserLogin(&reqUserLogin, 0);
}

void DataFeeder::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    std::cout << "OnRspUserLogin:\t" << "ErrorCode=[" << pRspInfo->ErrorID << "], ErrorMsg=[" << pRspInfo->ErrorMsg << "]" << std::endl;
    if (pRspInfo->ErrorID != 0)
        std::cout << "Error: Login Error!" << std::endl;
}

void DataFeeder::subscrib_L2_market_data(std::string stockid)
{
	CThostFtdcSpecificSecurityField query;
	memset(&query, 0, sizeof(query));
	std::string security_id = stockid.substr(2);
	std::string exchange_id = this->ExchangeIDDict[ stockid.substr(0,2) ];
	strcpy(query.SecurityID, security_id.c_str());
	strcpy(query.ExchangeID, exchange_id.c_str());

	m_pLevel2Api->SubscribeLevel2MarketData(&query, 1);
}

void DataFeeder::OnRtnLevel2MarketData(CThostFtdcLevel2MarketDataField *pLevel2MarketData)
{
	std::ostringstream oss;
	CThostFtdcLevel2MarketDataField *p = pLevel2MarketData;
	std::string stockid = this->ExchangeIDDict_Reverse[p->ExchangeID] + p->SecurityID;

	oss << stockid << ',' << p->OpenPx << ',' << p->PreClosePx << ',' << p->LastPx << ',' << p->HighPx << ',' << p->LowPx << ',' 
		<< p->TotalVolumeTrade << ',' << p->TotalValueTrade << ',' 
		<< p->BidPx1 << ',' << p->BidOrderQty1 << ',' << p->BidPx2 << ',' << p->BidOrderQty2 << ',' << p->BidPx3 << ',' << p->BidOrderQty3 << ',' 
		<< p->OfferPx1 << ',' << p->OfferOrderQty1 << ',' << p->OfferPx2 << ',' << p->OfferOrderQty2 << ',' << p->OfferPx3 << ',' << p->OfferOrderQty3 << ',' 
		<< p->TradingDay << ',' << p->DataTimeStamp;

	stock_datas[stockid] = oss.str();
//	std::cout << stock_datas[stockid] << std::endl;
}

std::string DataFeeder::get_L2_market_data(std::string stockid)
{
	return this->stock_datas[stockid];
}


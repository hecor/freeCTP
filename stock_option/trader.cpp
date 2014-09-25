#include <iostream>
#include <cstring>
#include <cstdio>
#include <sstream>
#include "trader.h"

int Trader::m_sOrderRef = 0;
int Trader::m_sRequestID = 0;

void Trader::init()
{
	m_pTradeApi->RegisterSpi(this);
	m_pTradeApi->RegisterFront(const_cast<char*>(this->front_address.c_str()));
	// 使客户端开始与后台服务建立连接
	m_pTradeApi->Init();
}

void Trader::OnFrontConnected()
{
	CSecurityFtdcReqUserLoginField reqUserLogin;
	strcpy(reqUserLogin.BrokerID, this->brokerID.c_str());
	strcpy(reqUserLogin.UserID, this->userID.c_str());
	strcpy(reqUserLogin.Password, this->passwd.c_str());
	m_pTradeApi->ReqUserLogin(&reqUserLogin, ++m_sRequestID);
}

void Trader::OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> OnRspUserLogin" << endl;
	IsErrorRspInfo(pRspInfo);
}

bool Trader::IsErrorRspInfo(CSecurityFtdcRspInfoField *pRspInfo)
{
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult){
		cerr << "------>>>>>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
		this->error_msg = pRspInfo->ErrorMsg;
	}
	return bResult;
}

void Trader::buy(string stockid, string limit_price, int amount)
{
	this->trade(stockid.substr(2), this->ExchangeIDDict[stockid.substr(0,2)], limit_price, amount, SECURITY_FTDC_D_Buy);
}

void Trader::sell(string stockid, string limit_price, int amount)
{
	this->trade(stockid.substr(2), this->ExchangeIDDict[stockid.substr(0,2)], limit_price, amount, SECURITY_FTDC_D_Sell);
}

void Trader::trade(string stockID, string exchangeID, string limit_price, int amount, TSecurityFtdcDirectionType direction)
{
	CSecurityFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	
	strcpy(req.BrokerID, this->brokerID.c_str());
	strcpy(req.InvestorID, this->userID.c_str());
	req.OrderPriceType = SECURITY_FTDC_OPT_LimitPrice;

	strcpy(req.InstrumentID, stockID.c_str());
	strcpy(req.ExchangeID, exchangeID.c_str());
	strcpy(req.LimitPrice, limit_price.c_str());
	req.VolumeTotalOriginal = amount;
	req.Direction = direction;
	
	char buffer[10];
	sprintf(buffer, "%d", m_sOrderRef++);
	strcpy(req.OrderRef, buffer);
	
	req.TimeCondition = SECURITY_FTDC_TC_GFD;
	req.VolumeCondition = SECURITY_FTDC_VC_AV;
	req.ContingentCondition = SECURITY_FTDC_CC_Immediately;
	req.ForceCloseReason = SECURITY_FTDC_FCC_NotForceClose;
	
//	req.CombOffsetFlag[0]=SECURITY_FTDC_OF_Open;
//	req.CombHedgeFlag[0]=SECURITY_FTDC_HF_Speculation;
//	req.MinVolume=1;
//	req.IsAutoSuspend = 0;
	req.UserForceClose = 0;
	req.RequestID = ++m_sRequestID;
	
	m_pTradeApi->ReqOrderInsert(&req, m_sRequestID);
}

void Trader::OnRspOrderInsert(CSecurityFtdcInputOrderField *pInputOrder,CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
	// 输出报单录入结果
	cout << "--->>> OnRspOrderInsert" << endl;
	IsErrorRspInfo(pRspInfo);
}

void Trader::OnRtnTrade(CSecurityFtdcTradeField *pTrade)
{
	cerr << "--->>> Trade notification: \n" << pTrade->InstrumentID << '\t' << pTrade->Direction << '\t' << pTrade->Price << '\t' << pTrade->Volume << '\t' << pTrade->TradeDate << ' ' << pTrade->TradeTime << endl;
}

void Trader::OnRtnOrder(CSecurityFtdcOrderField *pOrder)
{
	cerr << "--->>> Order Return notification: " << pOrder->InstrumentID << '\t' << pOrder->LimitPrice << '\t' << pOrder->VolumeTraded << endl;
}

// 针对用户请求的出错通知
void Trader::OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> OnRspError" << endl;
	IsErrorRspInfo(pRspInfo);
}

void Trader::update_stock_info()
{
	stock_info.clear();

	CSecurityFtdcQryInvestorPositionField query;
	memset(&query, 0, sizeof(query));
	strcpy(query.BrokerID, this->brokerID.c_str());
	strcpy(query.InvestorID, this->userID.c_str());
	m_pTradeApi->ReqQryInvestorPosition(&query, ++m_sRequestID);
}

map< string, int > Trader::get_stock_info()
{
	return this->stock_info;
}

void Trader::OnRspQryInvestorPosition(CSecurityFtdcInvestorPositionField *pInvestorPosition, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if( pInvestorPosition )
	{
		string stockid = ExchangeIDDict_Reverse[pInvestorPosition->ExchangeID] + pInvestorPosition->InstrumentID;
		this->stock_info[ stockid ] = pInvestorPosition->YdPosition;
	}
}

void Trader::update_trade_records()
{
	trade_records.clear();

	CSecurityFtdcQryTradeField query;
	memset(&query, 0, sizeof(query));

	strcpy(query.BrokerID, this->brokerID.c_str());
	strcpy(query.InvestorID, this->userID.c_str());
	m_pTradeApi->ReqQryTrade(&query, ++m_sRequestID);
}

vector< string > Trader::get_trade_records()
{
	return this->trade_records;
}

void Trader::OnRspQryTrade(CSecurityFtdcTradeField *pTrade, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if( pTrade )
	{
		ostringstream oss;
		oss << pTrade->TradeDate << ' ' << pTrade->TradeTime << ' ' << pTrade->Direction << ' ' << ExchangeIDDict_Reverse[pTrade->ExchangeID] << pTrade->InstrumentID << ' ' << pTrade->Price << ' ' << pTrade->Volume;
		this->trade_records.push_back( oss.str() );
	}
}

//void Trader::takeout_fund(int amount)
//{
//	CSecurityFtdcReqFundIOCTPAccountField query;
//	memset(&query, 0, sizeof(query));
//
//	strcpy(query.BrokerID, this->brokerID.c_str());
////	strcpy(query.InvestorID, this->userID.c_str());
////	strcpy(query.AccountID, this->userID.c_str());
//	strcpy(query.UserID, this->userID.c_str());
//	strcpy(query.Password, "111203");
//	query.TradeAmount = amount;
//	m_pTradeApi->ReqFundOutCTPAccount(&query, ++m_sRequestID);
//}
//
//void Trader::OnRspFundOutCTPAccount(CSecurityFtdcRspFundIOCTPAccountField *pRspFundIOCTPAccount, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
//{
//	cerr << "--->>> OnRspFundOutCTPAccount" << endl;
//	IsErrorRspInfo(pRspInfo);
//}


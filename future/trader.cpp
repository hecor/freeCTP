#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
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
	CThostFtdcReqUserLoginField reqUserLogin;
	memset(&reqUserLogin, 0, sizeof(reqUserLogin));
	strcpy(reqUserLogin.BrokerID, this->brokerID.c_str());
	strcpy(reqUserLogin.UserID, this->userID.c_str());
	strcpy(reqUserLogin.Password, this->passwd.c_str());
	m_pTradeApi->ReqUserLogin(&reqUserLogin, ++m_sRequestID);
}

void Trader::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> OnRspUserLogin" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		m_sOrderRef = atoi(pRspUserLogin->MaxOrderRef) + 1;
		ReqSettlementInfoConfirm();
	}
}

void Trader::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, this->brokerID.c_str());
	strcpy(req.InvestorID, this->userID.c_str());
	int result = m_pTradeApi->ReqSettlementInfoConfirm(&req, ++m_sRequestID);
	cerr << "------>>> Request settlement info confirm " << ((result == 0)? "success" : "fail") << endl;
}

bool Trader::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult){
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
		this->error_msg = pRspInfo->ErrorMsg;
	}
	return bResult;
}

void Trader::buy(string InstrumentID, double limit_price, int amount)
{
	this->trade(InstrumentID, limit_price, amount, THOST_FTDC_D_Buy);
}

void Trader::sell(string InstrumentID, double limit_price, int amount)
{
	this->trade(InstrumentID, limit_price, amount, THOST_FTDC_D_Sell);
}

void Trader::trade(string InstrumentID, double limit_price, int amount, TThostFtdcDirectionType direction)
{
	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));

	strcpy(req.BrokerID, this->brokerID.c_str());
	strcpy(req.InvestorID, this->userID.c_str());
	strcpy(req.InstrumentID, InstrumentID.c_str());
	
	char buffer[10];
	sprintf(buffer, "%d", m_sOrderRef++);
	strcpy(req.OrderRef, buffer);

	req.Direction = direction;
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	req.LimitPrice = limit_price;
	req.VolumeTotalOriginal = amount;
	
	req.TimeCondition = THOST_FTDC_TC_GFD;
	req.VolumeCondition = THOST_FTDC_VC_AV;
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	req.CombOffsetFlag[0]=THOST_FTDC_OF_Open;
	req.CombHedgeFlag[0]=THOST_FTDC_HF_Speculation;
	
	req.MinVolume=1;
	req.IsAutoSuspend = 0;
	req.UserForceClose = 0;
	
	m_pTradeApi->ReqOrderInsert(&req, ++m_sRequestID);
}

void Trader::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder,CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
	// 输出报单录入结果
	cout << "--->>> OnRspOrderInsert" << endl;
	IsErrorRspInfo( pRspInfo );
}

void Trader::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	cout << "Trade notification: \n" << pTrade->InstrumentID << '\t' << pTrade->Direction << '\t' << pTrade->Price << '\t' << pTrade->Volume << '\t' << pTrade->TradeDate << ' ' << pTrade->TradeTime << endl;
}

void Trader::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	cout << "Order Return notification: " << pOrder->InstrumentID << '\t' << pOrder->LimitPrice << '\t' << pOrder->VolumeTraded << endl;
}

// 针对用户请求的出错通知
void Trader::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> OnRspError" << endl;
	IsErrorRspInfo(pRspInfo);
}

void Trader::update_position_info()
{
	position_info.clear();

	CThostFtdcQryInvestorPositionField query;
	memset(&query, 0, sizeof(query));
	strcpy(query.BrokerID, this->brokerID.c_str());
	strcpy(query.InvestorID, this->userID.c_str());
	m_pTradeApi->ReqQryInvestorPosition(&query, ++m_sRequestID);
}

map< string, int > Trader::get_position_info()
{
	return this->position_info;
}

void Trader::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> OnRspQryInvestorPosition" << endl;
	if( bIsLast && !IsErrorRspInfo(pRspInfo) && pInvestorPosition )
		this->position_info[ pInvestorPosition->InstrumentID ] = pInvestorPosition->YdPosition;
}

void Trader::update_trade_records()
{
	trade_records.clear();

	CThostFtdcQryTradeField query;
	memset(&query, 0, sizeof(query));

	strcpy(query.BrokerID, this->brokerID.c_str());
	strcpy(query.InvestorID, this->userID.c_str());
	m_pTradeApi->ReqQryTrade(&query, ++m_sRequestID);
}

vector< string > Trader::get_trade_records()
{
	return this->trade_records;
}

void Trader::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if( bIsLast && !IsErrorRspInfo(pRspInfo) && pTrade )
	{
		ostringstream oss;
		oss << pTrade->TradeDate << ' ' << pTrade->TradeTime << ' ' << pTrade->Direction << ' ' << pTrade->InstrumentID << ' ' << pTrade->Price << ' ' << pTrade->Volume;
		this->trade_records.push_back( oss.str() );
	}
}

//void Trader::takeout_fund(int amount)
//{
//	CThostFtdcReqFundIOCTPAccountField query;
//	memset(&query, 0, sizeof(query));
//
//	strcpy(query.BrokerID, this->brokerID.c_str());
//	strcpy(query.InvestorID, this->userID.c_str());
//	strcpy(query.AccountID, this->userID.c_str());
//	strcpy(query.UserID, this->userID.c_str());
//	strcpy(query.Password, "111203");
//	query.TradeAmount = amount;
//	m_pTradeApi->ReqFundOutCTPAccount(&query, ++m_sRequestID);
//}
//
//void Trader::OnRspFundOutCTPAccount(CThostFtdcRspFundIOCTPAccountField *pRspFundIOCTPAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
//{
//	cout << "Response on takeout_fund: ErrorCode=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
//}


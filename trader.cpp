#include <iostream>
#include <cstring>
#include <cstdio>
#include <sstream>
#include "trader.h"

int Trader::m_sOrderRef = 0;

void Trader::init()
{
	m_pTradeApi->RegisterSpi(this);
	m_pTradeApi->RegisterFront(const_cast<char*>(this->front_address.c_str()));
	// 使客户端开始与后台服务建立连接
	m_pTradeApi->Init();
}

void Trader::OnFrontConnected()
{
	CZQThostFtdcReqUserLoginField reqUserLogin;
	strcpy(reqUserLogin.BrokerID, this->brokerID.c_str());
	strcpy(reqUserLogin.UserID, this->userID.c_str());
	strcpy(reqUserLogin.Password, this->passwd.c_str());
	m_pTradeApi->ReqUserLogin(&reqUserLogin, 0);
}

void Trader::OnRspUserLogin(CZQThostFtdcRspUserLoginField *pRspUserLogin, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "OnRspUserLogin:\t" << "ErrorCode=[" << pRspInfo->ErrorID << "], ErrorMsg=[" << pRspInfo->ErrorMsg << "]" << std::endl;
	if (pRspInfo->ErrorID != 0)
		std::cout << "Error: Login Error!" << std::endl;
}

void Trader::buy(std::string stockid, std::string limit_price, int amount)
{
	this->trade(stockid.substr(2), this->ExchangeIDDict[stockid.substr(0,2)], limit_price, amount, THOST_FTDC_D_Buy);
}

void Trader::sell(std::string stockid, std::string limit_price, int amount)
{
	this->trade(stockid.substr(2), this->ExchangeIDDict[stockid.substr(0,2)], limit_price, amount, THOST_FTDC_D_Sell);
}

void Trader::trade(std::string stockID, std::string exchangeID, std::string limit_price, int amount, TZQThostFtdcDirectionType direction)
{
	CZQThostFtdcInputOrderField pInputOrder;
	memset(&pInputOrder, 0, sizeof(pInputOrder));
	CZQThostFtdcInputOrderField *pIptOrdFld = &pInputOrder;

	strcpy(pIptOrdFld->InstrumentID, stockID.c_str());
	strcpy(pIptOrdFld->ExchangeID, exchangeID.c_str());
	strcpy(pIptOrdFld->LimitPrice, limit_price.c_str());
	pIptOrdFld->VolumeTotalOriginal = amount;
	pIptOrdFld->Direction = direction;
	
	strcpy(pIptOrdFld->BrokerID, this->brokerID.c_str());
	strcpy(pIptOrdFld->InvestorID, this->userID.c_str());
	pIptOrdFld->OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	pIptOrdFld->TimeCondition = THOST_FTDC_TC_GFD;
	pIptOrdFld->VolumeCondition = THOST_FTDC_VC_AV;
	pIptOrdFld->ContingentCondition = THOST_FTDC_CC_Immediately;
	pIptOrdFld->ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	char buffer[10];
	sprintf(buffer, "%d", m_sOrderRef++);
	strcpy(pIptOrdFld->OrderRef, buffer);
//		strcpy(pIptOrdFld->UserID,"698048");
//		pIptOrdFld->CombOffsetFlag[0]=THOST_FTDC_OF_Open;
//		pIptOrdFld->CombHedgeFlag[0]=THOST_FTDC_HF_Speculation;
//		strcpy(pIptOrdFld->GTDDate,"20140202");
//		pIptOrdFld->MinVolume=1;
//		pIptOrdFld->IsAutoSuspend = 0;
	pIptOrdFld->UserForceClose = 0;
	pIptOrdFld->RequestID = 1;
	
	m_pTradeApi->ReqOrderInsert(pIptOrdFld, 1);
}

void Trader::OnRspOrderInsert(CZQThostFtdcInputOrderField *pInputOrder,CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
	// 输出报单录入结果
//	std::cout << "OnRspOrderInsert: inputorder.VolumeCondition: " << pInputOrder->VolumeCondition << std::endl;
//	std::cout << "OnRspOrderInsert: inputorder.TimeCondition: " << pInputOrder->TimeCondition << std::endl;
//	std::cout << "OnRspOrderInsert: inputorder.ForceCloseReason: " << pInputOrder->ForceCloseReason << std::endl;
	std::cout << "OnRspOrderInsert: ErrorCode=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg;
}

void Trader::OnRtnTrade(CZQThostFtdcTradeField *pTrade)
{
	std::cout << "Trade notification: \n" << pTrade->InstrumentID << '\t' << pTrade->Direction << '\t' << pTrade->Price << '\t' << pTrade->Volume << '\t' << pTrade->TradeDate << ' ' << pTrade->TradeTime << std::endl;
}

void Trader::OnRtnOrder(CZQThostFtdcOrderField *pOrder)
{
	std::cout << "Order Return notification: " << pOrder->InstrumentID << '\t' << pOrder->LimitPrice << '\t' << pOrder->VolumeTraded << std::endl;
}

// 针对用户请求的出错通知
void Trader::OnRspError(CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "OnRspError: ErrorCode=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
}

void Trader::update_stock_info()
{
	stock_info.clear();

	CZQThostFtdcQryInvestorPositionField query;
	memset(&query, 0, sizeof(query));
	strcpy(query.BrokerID, this->brokerID.c_str());
	strcpy(query.InvestorID, this->userID.c_str());
	m_pTradeApi->ReqQryInvestorPosition(&query, 3);
}

std::map< std::string, int > Trader::get_stock_info()
{
	return this->stock_info;
}

void Trader::OnRspQryInvestorPosition(CZQThostFtdcInvestorPositionField *pInvestorPosition, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if( pInvestorPosition )
	{
		std::string stockid = ExchangeIDDict_Reverse[pInvestorPosition->ExchangeID] + pInvestorPosition->InstrumentID;
		this->stock_info[ stockid ] = pInvestorPosition->YdPosition;
	}
}

void Trader::update_trade_records()
{
	trade_records.clear();

	CZQThostFtdcQryTradeField query;
	memset(&query, 0, sizeof(query));

	strcpy(query.BrokerID, this->brokerID.c_str());
	strcpy(query.InvestorID, this->userID.c_str());
	m_pTradeApi->ReqQryTrade(&query, 4);
}

std::vector< std::string > Trader::get_trade_records()
{
	return this->trade_records;
}

void Trader::OnRspQryTrade(CZQThostFtdcTradeField *pTrade, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if( pTrade )
	{
		std::ostringstream oss;
		oss << pTrade->TradeDate << ' ' << pTrade->TradeTime << ' ' << pTrade->Direction << ' ' << ExchangeIDDict_Reverse[pTrade->ExchangeID] << pTrade->InstrumentID << ' ' << pTrade->Price << ' ' << pTrade->Volume;
		this->trade_records.push_back( oss.str() );
	}
}

void Trader::takeout_fund(int amount)
{
	CZQThostFtdcReqFundIOCTPAccountField query;
	memset(&query, 0, sizeof(query));

	strcpy(query.BrokerID, this->brokerID.c_str());
	strcpy(query.InvestorID, this->userID.c_str());
	strcpy(query.AccountID, this->userID.c_str());
	strcpy(query.UserID, this->userID.c_str());
	strcpy(query.Password, "111203");
	query.TradeAmount = amount;
	m_pTradeApi->ReqFundOutCTPAccount(&query, 5);
}

void Trader::OnRspFundOutCTPAccount(CZQThostFtdcRspFundIOCTPAccountField *pRspFundIOCTPAccount, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "Response on takeout_fund: ErrorCode=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
}


#ifndef _TRADER_H__
#define _TRADER_H__

#include <string>
#include <vector>
#include <map>
#include "./api/ThostFtdcTraderApiSSE.h"
#include "./api/ThostFtdcUserApiDataTypeSSE.h"

class Trader : public CZQThostFtdcTraderSpi
{
public:
	Trader(std::string front_address, std::string brokerID, std::string userID, std::string passwd){
		this->front_address = front_address;
		this->brokerID = brokerID;
		this->userID = userID;
		this->passwd = passwd;
		m_pTradeApi = CZQThostFtdcTraderApi::CreateFtdcTraderApi("/tmp/CTP_tradedata/");
		this->init();
		this->ExchangeIDDict["sh"] = "SSE";
		this->ExchangeIDDict["sz"] = "SZE";
		this->ExchangeIDDict_Reverse["SSE"] = "sh";
		this->ExchangeIDDict_Reverse["SZE"] = "sz";
		m_pTradeApi->SubscribePrivateTopic(ZQTHOST_TERT_RESUME);
		m_pTradeApi->SubscribePublicTopic(ZQTHOST_TERT_RESUME);
	}
	
	~Trader(){
		m_pTradeApi->Release();
	}

	void buy(std::string stockid, std::string limit_price, int amount);
	void sell(std::string stockid, std::string limit_price, int amount);
	void update_stock_info();
	std::map< std::string, int > get_stock_info();
	void update_trade_records();
	std::vector< std::string > get_trade_records();
	void takeout_fund(int amount);

private:
	static int m_sOrderRef;
	CZQThostFtdcTraderApi *m_pTradeApi;
	std::string brokerID;
	std::string userID;
	std::string passwd;
	std::string front_address;
	std::map< std::string, std::string > ExchangeIDDict;
	std::map< std::string, std::string > ExchangeIDDict_Reverse;
	std::map< std::string, int > stock_info;
	std::vector< std::string > trade_records;

	void init();
	void OnFrontConnected();
	void trade(std::string stockID, std::string exchangeID, std::string limit_price, int amount, TZQThostFtdcDirectionType direction);

	void OnRtnOrder(CZQThostFtdcOrderField *pOrder);
	void OnRspError(CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRtnTrade(CZQThostFtdcTradeField *pTrade);
	
	void OnRspUserLogin(CZQThostFtdcRspUserLoginField *pRspUserLogin, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspOrderInsert(CZQThostFtdcInputOrderField *pInputOrder,CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspQryInvestorPosition(CZQThostFtdcInvestorPositionField *pInvestorPosition, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspQryTrade(CZQThostFtdcTradeField *pTrade, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspFundOutCTPAccount(CZQThostFtdcRspFundIOCTPAccountField *pRspFundIOCTPAccount, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
};

#endif /* end of include guard: _TRADER_H__ */


#ifndef _TRADER_H__
#define _TRADER_H__

#include <string>
#include <vector>
#include <map>
#include "./api/ThostFtdcTraderApi.h"
#include "./api/ThostFtdcUserApiDataType.h"

using namespace std;

class Trader : public CThostFtdcTraderSpi
{
public:
	Trader(string front_address, string brokerID, string userID, string passwd){
		this->front_address = front_address;
		this->brokerID = brokerID;
		this->userID = userID;
		this->passwd = passwd;
		m_pTradeApi = CThostFtdcTraderApi::CreateFtdcTraderApi("/tmp/CTP_future_data/");
		this->init();
		m_pTradeApi->SubscribePrivateTopic(THOST_TERT_RESUME);
		m_pTradeApi->SubscribePublicTopic(THOST_TERT_RESUME);
	}
	
	~Trader(){
		m_pTradeApi->Release();
	}

	void buy(string ExchangeID, double limit_price, int amount);
	void sell(string ExchangeID, double limit_price, int amount);
	void update_position_info();
	map< string, int > get_position_info();
	void update_trade_records();
	vector< string > get_trade_records();
//	void takeout_fund(int amount);
	bool IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo);
	string get_error_msg(){ return this->error_msg; }

private:
	static int m_sRequestID;
	static int m_sOrderRef;
	CThostFtdcTraderApi *m_pTradeApi;
	string brokerID;
	string userID;
	string passwd;
	string front_address;
	map< string, int > position_info;
	vector< string > trade_records;
	string error_msg;

	void init();
	void OnFrontConnected();
	void ReqSettlementInfoConfirm();

	void trade(string InstrumentID, double limit_price, int amount, TThostFtdcDirectionType direction);

	void OnRtnOrder(CThostFtdcOrderField *pOrder);
	void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRtnTrade(CThostFtdcTradeField *pTrade);
	
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder,CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
//	void OnRspFundOutCTPAccount(CThostFtdcRspFundIOCTPAccountField *pRspFundIOCTPAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
};

#endif /* end of include guard: _TRADER_H__ */


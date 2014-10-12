#ifndef _TRADER_H__
#define _TRADER_H__

#include <string>
#include <vector>
#include <map>
#include "./api/SecurityFtdcTraderApi.h"
#include "./api/SecurityFtdcUserApiDataType.h"

using namespace std;

class Trader : public CSecurityFtdcTraderSpi
{
public:
	Trader(string front_address, string brokerID, string userID, string passwd){
		this->front_address = front_address;
		this->brokerID = brokerID;
		this->userID = userID;
		this->passwd = passwd;
		m_pTradeApi = CSecurityFtdcTraderApi::CreateFtdcTraderApi("/tmp/CTP_LTS_trade/");
		this->init();
		this->ExchangeIDDict["sh"] = "SSE";
		this->ExchangeIDDict["sz"] = "SZE";
		this->ExchangeIDDict_Reverse["SSE"] = "sh";
		this->ExchangeIDDict_Reverse["SZE"] = "sz";
		m_pTradeApi->SubscribePrivateTopic(SECURITY_TERT_RESUME);
		m_pTradeApi->SubscribePublicTopic(SECURITY_TERT_RESUME);
	}
	
	~Trader(){
		m_pTradeApi->Release();
	}

	void buy(string stockid, string limit_price, int amount);
	void sell(string stockid, string limit_price, int amount);
	void update_position_info();
	map< string, int > get_position_info();
	void update_account_info();
	map< string, double > get_account_info();
	void update_trade_records();
	vector< string > get_trade_records();

	bool IsErrorRspInfo(CSecurityFtdcRspInfoField *pRspInfo);
	string get_error_msg(){ return error_msg; }

private:
	static int m_sRequestID;
	static int m_sOrderRef;
	CSecurityFtdcTraderApi *m_pTradeApi;
	string brokerID;
	string userID;
	string passwd;
	string front_address;
	map< string, string > ExchangeIDDict;
	map< string, string > ExchangeIDDict_Reverse;
	map< string, double > account_info;
	map< string, int > position_info;
	vector< string > trade_records;
	string error_msg;

	void init();
	void OnFrontConnected();
	void trade(string stockID, string exchangeID, string limit_price, int amount, TSecurityFtdcDirectionType direction);

	void OnRtnOrder(CSecurityFtdcOrderField *pOrder);
	void OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRtnTrade(CSecurityFtdcTradeField *pTrade);
	
	void OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspOrderInsert(CSecurityFtdcInputOrderField *pInputOrder,CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspQryInvestorPosition(CSecurityFtdcInvestorPositionField *pInvestorPosition, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspQryTradingAccount(CSecurityFtdcTradingAccountField *pTradingAccount, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspQryTrade(CSecurityFtdcTradeField *pTrade, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

};

#endif /* end of include guard: _TRADER_H__ */


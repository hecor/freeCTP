#ifndef _DATAFEEDER_H__
#define _DATAFEEDER_H__

#include <string>
#include <map>
#include "./api/SecurityFtdcMdApi.h"
#include "./api/SecurityFtdcUserApiStruct.h"

using namespace std;

class DataFeeder : public CSecurityFtdcMdSpi
{
public:
    DataFeeder(string front_address, string brokerID, string userID, string passwd){
        this->front_address = front_address;
        this->brokerID = brokerID;
        this->userID = userID;
        this->passwd = passwd;
        this->m_pMdApi = CSecurityFtdcMdApi::CreateFtdcMdApi("/tmp/CTP_LTS_data/");
        this->init();
        this->ExchangeIDDict["sh"] = "SSE";
        this->ExchangeIDDict["sz"] = "SZE";
        this->ExchangeIDDict_Reverse["SSE"] = "sh";
        this->ExchangeIDDict_Reverse["SZE"] = "sz";
    }

    ~DataFeeder(){
        this->m_pMdApi->Release();
    }

	void subscrib_market_data(string InstrumentID);
	string get_market_data(string InstrumentID);

private:
    CSecurityFtdcMdApi *m_pMdApi;
    string brokerID;
    string userID;
    string passwd;
    string front_address;
    map< string, string > ExchangeIDDict;
    map< string, string > ExchangeIDDict_Reverse;
    map< string, string > stock_datas;

	void init();
	void OnFrontConnected();
	void OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	void OnRtnDepthMarketData(CSecurityFtdcDepthMarketDataField *pDepthMarketData);
};

#endif /* end of include guard: _DATAFEEDER_H__ */


#ifndef _DATAFEEDER_H__
#define _DATAFEEDER_H__

#include <string>
#include <vector>
#include <map>
#include "./api/ThostFtdcMdApi.h"
#include "./api/ThostFtdcUserApiStruct.h"

using namespace std;

class DataFeeder : public CThostFtdcMdSpi
{
public:
    DataFeeder(string front_address, string brokerID, string userID, string passwd, string tmpPath){
        this->front_address = front_address;
        this->brokerID = brokerID;
        this->userID = userID;
        this->passwd = passwd;
        this->m_pMdApi = CThostFtdcMdApi::CreateFtdcMdApi(tmpPath.c_str());
        this->init();
    }

    ~DataFeeder(){
        this->m_pMdApi->Release();
    }

	void subscrib_market_data( string InstrumentID );
	string get_market_data( string InstrumentID );

private:
    CThostFtdcMdApi *m_pMdApi;
    string brokerID;
    string userID;
    string passwd;
    string front_address;
    map< string, string > stock_datas;

	void init();
	void OnFrontConnected();
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
};

#endif /* end of include guard: _DATAFEEDER_H__ */


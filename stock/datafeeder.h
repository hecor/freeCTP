#ifndef _DATAFEEDER_H__
#define _DATAFEEDER_H__

#include <string>
#include <map>
#include "./api/level2_ns.h"
#include "./api/ThostFtdcLevel2UserApi.h"
#include "./api/ThostFtdcUserApiStructSSE.h"

using namespace _LEVEL2_;

class DataFeeder : public CThostFtdcLevel2UserSpi
{
public:
    DataFeeder(std::string front_address, std::string brokerID, std::string userID, std::string passwd){
        this->front_address = front_address;
        this->brokerID = brokerID;
        this->userID = userID;
        this->passwd = passwd;
        this->m_pLevel2Api = CThostFtdcLevel2UserApi::CreateFtdcLevel2UserApi("/tmp/CTP_L2data/");
        this->init();
        this->ExchangeIDDict["sh"] = "SSE";
        this->ExchangeIDDict["sz"] = "SZE";
        this->ExchangeIDDict_Reverse["SSE"] = "sh";
        this->ExchangeIDDict_Reverse["SZE"] = "sz";
    }

    ~DataFeeder(){
        this->m_pLevel2Api->Release();
    }


	void subscrib_L2_market_data(std::string stockid);
	std::string get_L2_market_data( std::string stockid );

private:
    CThostFtdcLevel2UserApi *m_pLevel2Api;
    std::string brokerID;
    std::string userID;
    std::string passwd;
    std::string front_address;
    std::map< std::string, std::string > ExchangeIDDict;
    std::map< std::string, std::string > ExchangeIDDict_Reverse;
    std::map< std::string, std::string > stock_datas;

	void init();
	void OnFrontConnected();
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	void OnRtnLevel2MarketData(CThostFtdcLevel2MarketDataField *pLevel2MarketData);
};

#endif /* end of include guard: _DATAFEEDER_H__ */


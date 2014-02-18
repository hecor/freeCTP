#include <iostream>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include "ThostFtdcTraderApiSSE.h"

using namespace std;

class TestTrader : public CZQThostFtdcTraderSpi
{
private:
	CZQThostFtdcTraderApi* m_pTradeApi;

public:
	TestTrader(){
		m_pTradeApi = CZQThostFtdcTraderApi::CreateFtdcTraderApi();
	}
	
	~TestTrader(){
		m_pTradeApi->Release();
	}

	void init(){
		m_pTradeApi->RegisterSpi(this);
		m_pTradeApi->RegisterFront("tcp://116.228.234.67:41205");
		// 使客户端开始与后台服务建立连接
		m_pTradeApi->Init();
	}

	virtual void OnFrontConnected(){
		CZQThostFtdcReqUserLoginField reqUserLogin;
		strcpy(reqUserLogin.BrokerID, "2011");
		strcpy(reqUserLogin.UserID, "020090005951");
		strcpy(reqUserLogin.Password, "123321");
		m_pTradeApi->ReqUserLogin(&reqUserLogin, 0);
	}

	virtual void OnRspUserLogin(CZQThostFtdcRspUserLoginField *pRspUserLogin, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
		printf("OnRspUserLogin:\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
		if (pRspInfo->ErrorID != 0)
		{
			// 端登失败，客户端需进行错误处理
			printf("Failed to login, errorcode=%d errormsg=%s requestid=%d chain=%d", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
			return;
		}
		test_trade();
//		ReqQryTradingAccount();
//		get_trade_records();
	}

	void ReqQryTradingAccount()
	{
		CZQThostFtdcQryTradingAccountField qryTradingAccount;
		memset(&qryTradingAccount, 0, sizeof(qryTradingAccount));
		CZQThostFtdcQryTradingAccountField *pQryTradingAccount = &qryTradingAccount;
		strcpy(pQryTradingAccount->BrokerID,"2011");
		strcpy(pQryTradingAccount->InvestorID,"020090005951");
		m_pTradeApi->ReqQryTradingAccount( pQryTradingAccount, 2 );
	}

	void get_trade_records()
	{
		CZQThostFtdcQryTradeField pQryTrade1;
		memset(&pQryTrade1,0,sizeof(pQryTrade1));
		CZQThostFtdcQryTradeField * pQryTrade= &pQryTrade1;

		//≤È—Ø≥…Ωª Ken////////
		strcpy(pQryTrade->BrokerID,"2011");
		strcpy(pQryTrade->InvestorID,"020090005951");
		//strcpy(pQryTrade->TradeID,pTrade->TradeID);
		//strcpy(pQryTrade->InstrumentID,pTrade->InstrumentID);
		//strcpy(pQryTrade->TradeTimeStart,"00:00:00");
		//strcpy(pQryTrade->TradeTimeEnd,"23:00:00");

		int iResult1 = m_pTradeApi->ReqQryTrade(pQryTrade, 1);
	}

	void OnRspQryTrade(CZQThostFtdcTradeField *pTrade, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		cout << pTrade->InstrumentID << ',' << pTrade->Price << endl;
	}


	virtual void OnRspQryTradingAccount(CZQThostFtdcTradingAccountField *pTradingAccount, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		cout << pTradingAccount->StockValue << ',' << pTradingAccount->Available << endl;
	}

	void test_trade()
	{
		CZQThostFtdcInputOrderField pInputOrder;
		
		memset(&pInputOrder,0,sizeof(pInputOrder));
		CZQThostFtdcInputOrderField * pIptOrdFld=&pInputOrder;

		strcpy(pIptOrdFld->BrokerID,"2011");
		strcpy(pIptOrdFld->InvestorID,"020090005951");
		strcpy(pIptOrdFld->InstrumentID,"600000");
		strcpy(pIptOrdFld->ExchangeID,"SSE");
		pIptOrdFld->OrderPriceType=THOST_FTDC_OPT_LimitPrice;
		pIptOrdFld->Direction=THOST_FTDC_D_Sell;
		pIptOrdFld->VolumeTotalOriginal=2500;
		pIptOrdFld->TimeCondition = THOST_FTDC_TC_GFD;
		pIptOrdFld->VolumeCondition=THOST_FTDC_VC_AV;
		pIptOrdFld->ContingentCondition = THOST_FTDC_CC_Immediately;
		pIptOrdFld->ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
		strcpy(pIptOrdFld->LimitPrice,"7.77");
//		strcpy(pIptOrdFld->UserID,"698048");
		strcpy(pIptOrdFld->OrderRef,"1");
//		pIptOrdFld->CombOffsetFlag[0]=THOST_FTDC_OF_Open;
//		pIptOrdFld->CombHedgeFlag[0]=THOST_FTDC_HF_Speculation;
//		strcpy(pIptOrdFld->GTDDate,"20140202");
//		pIptOrdFld->MinVolume=1;
//		pIptOrdFld->IsAutoSuspend = 0;
		pIptOrdFld->UserForceClose = 0;
		pIptOrdFld->RequestID=1;
		
		int iResult= m_pTradeApi->ReqOrderInsert(pIptOrdFld,  1);
		cout << iResult << endl;
	}

	virtual void OnRspOrderInsert(CZQThostFtdcInputOrderField *pInputOrder,CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
		// 输出报单录入结果
		cout << "OnRspOrderInsert: inputorder.VolumeCondition: " << pInputOrder->VolumeCondition << endl;
		cout << "OnRspOrderInsert: inputorder.TimeCondition: " << pInputOrder->TimeCondition << endl;
		cout << "OnRspOrderInsert: inputorder.ForceCloseReason: " << pInputOrder->ForceCloseReason << endl;
		printf("OnRspOrderInsert: ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}

	virtual void OnRtnOrder(CZQThostFtdcOrderField *pOrder)
	{
		cout << "OnRtnOrder: " << pOrder->InstrumentID << '\t' << pOrder->LimitPrice << '\t' << pOrder->VolumeTraded << endl;
	}

	// 针对用户请求的出错通知
	virtual void OnRspError(CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspError:\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	}

	virtual void OnRtnTrade(CZQThostFtdcTradeField *pTrade)
	{
		std::cout << "成交通知: \n" << pTrade->InstrumentID << '\t' << pTrade->Direction << '\t' << pTrade->Price << '\t' << pTrade->Volume << '\t' << pTrade->TradeDate << ':' << pTrade->TradeTime << std::endl;
	}
};

int main()
{
	TestTrader sh;
	sh.init();
	sleep(3);

	return 0;
}



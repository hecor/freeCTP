pzyctp
======

my python wrapper for CTP

Install:
========
* run make in each directory then ./setup.sh
* export LD_LIBRARY_PATH="$HOME/programs/lib/python2.7/site-packages/pzyctp/stock:$HOME/programs/lib/python2.7/site-packages/pzyctp/future:$LD_LIBRARY_PATH"

**Remember to change to PYTHON library path in scripts and export command**

**Remember to create two dirs in /tmp before you run pzyctp.**
* if you use ctp for stock: ("/tmp/CTP_L2data/" and "/tmp/CTP_tradedata/")
* if you use ctp for future: ("/tmp/CTP_future_data/" and "/tmp/CTP_future_trade")
* if you use ctp for stock_option: ("/tmp/CTP_stock_option_data/" and "/tmp/CTP_stock_option_trade/")


Format of market data:
=============================
* Stock:  
	
	stockid, OpenPx, PreClosePx, LastPx, HighPx, LowPx, TotalVolumeTrade, TotalValueTrade, BidPx1, BidOrderQty1, BidPx2, BidOrderQty2, BidPx3, BidOrderQty3, OfferPx1, OfferOrderQty1, OfferPx2, OfferOrderQty2, OfferPx3, OfferOrderQty3, TradingDay, DataTimeStamp

* Future:

	InstrumentID, TradingDay, UpdateTime, LastPrice, PreClosePrice, OpenPrice, ClosePrice, HighestPrice, LowestPrice, Volume, Turnover, BidPrice1, BidVolume1, BidPrice2, BidVolume2, BidPrice3, BidVolume3, AskPrice1, AskVolume1, AskPrice2, AskVolume2, AskPrice3, AskVolume3

* stock_option:

	stockid, TradingDay, UpdateTime, LastPrice, PreClosePrice, OpenPrice, ClosePrice, HighestPrice, LowestPrice, Volume, Turnover, BidPrice1, BidVolume1, BidPrice2, BidVolume2, BidPrice3, BidVolume3, AskPrice1, AskVolume1, AskPrice2, AskVolume2, AskPrice3, AskVolume3


Error Code for ctp
==================
* OnRspOrderInsert: ErrorCode=15, 报单字段有误

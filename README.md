pzyctp
======

my python wrapper for CTP

Install:
========
* run ./wrap.sh and then ./setup.sh
* export LD_LIBRARY_PATH="$HOME/programs/lib/python2.7/site-packages/pzyctp:$LD_LIBRARY_PATH"

**Remember to change to PYTHON library path in scripts and export command**

**Remember to create two dirs in /tmp before you run pzyctp. (/tmp/CTP_L2data/ and /tmp/CTP_tradedata/)**


The format of L2 market data:
=============================
cout << stockid << ',' << p->OpenPx << ',' << p->PreClosePx << ',' << p->LastPx << ',' << p->HighPx << ',' << p->LowPx << ',' 
<< p->TotalVolumeTrade << ',' << p->TotalValueTrade << ',' 
<< p->BidPx1 << ',' << p->BidOrderQty1 << ',' << p->BidPx2 << ',' << p->BidOrderQty2 << ',' << p->BidPx3 << ',' << p->BidOrderQty3 << ',' 
<< p->OfferPx1 << ',' << p->OfferOrderQty1 << ',' << p->OfferPx2 << ',' << p->OfferOrderQty2 << ',' << p->OfferPx3 << ',' << p->OfferOrderQty3 << ',' 
<< p->TradingDay << ',' << p->DataTimeStamp;


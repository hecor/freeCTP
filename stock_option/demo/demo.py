#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
from pzyctp.stock_option import datafeeder

def test_datafeeder():
    t = datafeeder.DataFeeder('tcp://211.144.195.163:34513', '2011', '020090031176', '123321')
    time.sleep(5)
    t.subscrib_market_data('sh10000033')
    while 1:
        print t.get_L2_market_data('sz002029')
        time.sleep(2)

def test_trade():
    t = trader.Trader('tcp://116.228.234.67:41205', '2011', '020090005951', '123321')
    time.sleep(5)
    t.buy('sz002029', '8.90', 100)
    t.buy('sh601566', '8.90', 100)
#    t.sell('sh204007', '11.90', 100)

#    t.update_stock_info()
#    time.sleep(3)
#    print t.get_stock_info()
#
#    t.update_trade_records()
#    time.sleep(3)
#    print t.get_trade_records()
#
#    t.takeout_fund(10000)
#    time.sleep(5)

if __name__ == '__main__':
    test_datafeeder()
#    test_trade()

    time.sleep(5)

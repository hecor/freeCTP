#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
from pzyctp.stock_option import datafeeder, trader

def test_datafeeder():
    t = datafeeder.DataFeeder('tcp://211.144.195.163:34513', '2011', '020090031176', '123321')
    time.sleep(5)
    t.subscrib_market_data('sh10000033')
    while 1:
        print t.get_L2_market_data('sh10000033')
        time.sleep(2)

def test_trade():
    t = trader.Trader('tcp://211.144.195.163:34505', '2011', '020090031176', '123321')
    time.sleep(5)
    t.buy('sh10000033', '3.09', 1)

#    t.update_stock_info()
#    time.sleep(3)
#    print t.get_stock_info()
#
#    t.update_trade_records()
#    time.sleep(3)
#    print t.get_trade_records()


if __name__ == '__main__':
#    test_datafeeder()
    test_trade()

    time.sleep(5)

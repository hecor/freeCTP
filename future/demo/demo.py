#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
from pzyctp.future import datafeeder, trader

def test_datafeeder():
    t = datafeeder.DataFeeder('tcp://221.12.30.9:51213', '1017', '00000111', '123456')
    time.sleep(5)
    t.subscrib_market_data('IF1403')
    while 1:
        print t.get_market_data('IF1403')
        time.sleep(2)

def test_trade():
    t = trader.Trader('tcp://221.12.30.9:51205', '1017', '00000111', '123456')
    time.sleep(5)
    t.sell('IF1403', 2144, 100)
    t.buy('IF1404', 2143, 100)

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

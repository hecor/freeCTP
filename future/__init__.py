#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os

dirs = ['/tmp/CTP_future_data/', '/tmp/CTP_future_trade/']
for d in dirs:
    if not os.path.isdir(d):
        os.makedirs(d)


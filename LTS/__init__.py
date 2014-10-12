#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os

dirs = ['/tmp/CTP_L2data/', '/tmp/CTP_tradedata/']
for d in dirs:
    if not os.path.isdir(d):
        os.makedirs(d)


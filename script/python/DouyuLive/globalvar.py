#!/usr/bin/python
# -*- coding: utf-8 -*-

def _init():
    global CONF,logger
    CONF = {}

def set_logger(value):
    logger = value
def set_conf(value):
    CONF = value

def get_logger(defValue=None):
    try:
        return logger
    except KeyError:
        return defValue
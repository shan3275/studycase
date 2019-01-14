#!/usr/bin/env python
# -*- coding:utf-8 -*-
#Filename:app.py

import sys
import re
import os
import time
import datetime
import logging
import json
import socket

from flask import Flask, request, jsonify, render_template, redirect,url_for, send_file, send_from_directory
import urllib
import random
import chardet

from werkzeug.utils import secure_filename
import inits
import globalvar as gl
from strapi   import  strapi_bp
from stradmin import  stradmin_bp
import libdb as libdb
global logger
global CONF

DEF_PORT = 8888
CUR_PORT = DEF_PORT

app = Flask(__name__, template_folder="templates/html",static_folder="templates/html",static_url_path="")
app.register_blueprint(strapi_bp,   url_prefix='/strapi')
app.register_blueprint(stradmin_bp, url_prefix='/admin')

@app.route('/')
def index():
    return 'Hello'

if __name__ == '__main__':
    reload(sys)
    sys.setdefaultencoding('utf-8')
    logger = gl.get_logger()
    CONF = gl.get_conf()
    CUR_PORT = CONF['port']
    if len(sys.argv) > 1:
        CUR_PORT = sys.argv[1]

    app.run(debug=True,host='0.0.0.0', port=CUR_PORT)
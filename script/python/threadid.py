#!/usr/bin/env python

import os
import ctypes
import platform
import threading


thread = threading.current_thread()
print thread.getName()
print threading.currentThread().ident

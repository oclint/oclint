#! /usr/bin/env python

import os

def is_mingw32():
    return os.uname()[0].startswith('MINGW32')

def is_darwin():
    return os.uname()[0] == "Darwin"

def is_linux():
    return os.uname()[0] == "Linux"

def is_unix():
    return is_darwin or is_linux

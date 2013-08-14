#! /usr/bin/env python

import platform

uname_system = platform.uname()[0]

def is_mingw32():
    return uname_system == "Windows"
    # Cygwin should say CYGWIN_NT-6.1-WOW64

def is_darwin():
    return uname_system == "Darwin"

def is_linux():
    return uname_system == "Linux"

def is_unix():
    return is_darwin() or is_linux()

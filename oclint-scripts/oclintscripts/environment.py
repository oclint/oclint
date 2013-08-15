#! /usr/bin/env python

import platform

def kernel():
    return platform.uname()[0]

def is_mingw32():
    return kernel() == "Windows"
    # Cygwin should say CYGWIN_NT-6.1-WOW64

def is_darwin():
    return kernel() == "Darwin"

def is_linux():
    return kernel() == "Linux"

def is_unix():
    return is_darwin() or is_linux()

def kernel_version():
    return platform.uname()[2]

def arch():
    return platform.uname()[4]

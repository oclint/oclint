#! /usr/bin/env python

import platform
import multiprocessing

def cpu_count():
    return multiprocessing.cpu_count()

def kernel():
    return platform.uname()[0].lower()

def kernel_version():
    return platform.uname()[2]

def arch():
    return platform.uname()[4].lower()

def is_mingw32():
    return kernel().startswith("windows")
    # Cygwin should say CYGWIN_NT-6.1-WOW64

def is_darwin():
    return kernel().startswith("darwin")

def is_darwin_13():
    if is_darwin():
        kernel_full_version = kernel_version()
        first_dot_index = kernel_full_version.find('.')
        kernel_main_version_str = kernel_full_version[:first_dot_index]
        return int(kernel_main_version_str) is 13
    return False

def is_linux():
    return kernel().startswith("linux")

def is_unix():
    return is_darwin() or is_linux()

#! /usr/bin/env python

import platform
import multiprocessing
import subprocess

def cpu_count():
    return multiprocessing.cpu_count()

def kernel():
    return platform.uname()[0].lower()

def kernel_version():
    return platform.uname()[2]

def arch():
    return platform.uname()[4].lower()

def linux_distribution():
    return platform.linux_distribution()

def dist_env():
    return arch() + '-' + kernel() + '-' + kernel_version()

def is_mingw32():
    return kernel().startswith("windows")
    # Cygwin should say CYGWIN_NT-6.1-WOW64

def is_darwin():
    return kernel().startswith("darwin")

def is_xcode8():
    if not is_darwin():
        return 0
    xcodebuild_version = subprocess.check_output(['xcodebuild', '-version'])
    return xcodebuild_version.find("Xcode 8.") != -1

def is_linux():
    return kernel().startswith("linux")

def is_unix():
    return is_darwin() or is_linux()

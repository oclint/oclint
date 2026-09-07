#! /usr/bin/env python3

import sysconfig
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

def linux_distribution():
    return platform.linux_distribution()

def dist_env():
    return arch() + '-' + kernel() + '-' + kernel_version()

def is_aarch64_or_arm64():
    return arch().startswith("aarch64") or arch().startswith("arm64")

def is_mingw32():
    return  sysconfig.get_platform().startswith('mingw')

def is_windows():
    return sysconfig.get_platform().startswith('win')

def is_darwin():
    return kernel().startswith("darwin")

def is_linux():
    return kernel().startswith("linux")

def is_unix():
    return is_darwin() or is_linux()

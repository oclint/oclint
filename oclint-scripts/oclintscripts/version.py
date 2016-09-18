#! /usr/bin/env python

import subprocess
import os

from oclintscripts import path
from oclintscripts import environment

def dev_version():
    current_working_folder = os.getcwd()
    try:
        os.chdir(path.root_dir)
        git_hash = subprocess.check_output(['git', 'log', '-n', '1', '--pretty=%h']).split()[0]
        path.cd(current_working_folder)
        return git_hash
    except:
        print("Ignore the git error above if you are building from a source code download instead of a git clone.")
        path.cd(current_working_folder)
        return "src"

def oclint_version():
    return "0.11.1"

def oclint_dev_version():
    return oclint_version() + '.dev.' + dev_version()

def llvm_branches():
    return [llvm_master_branch(), llvm_latest_release_branch()]

def llvm_default_branch():
    return llvm_latest_release_branch()

def llvm_master_branch():
    return 'trunk'

def llvm_latest_release_branch():
    return 'tags/RELEASE_390/final'

def countly_cpp_tag():
    return '16.9.1'

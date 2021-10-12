#! /usr/bin/env python3

import subprocess
import os

from oclintscripts import path

def dev_version():
    current_working_folder = os.getcwd()
    try:
        os.chdir(path.root_dir)
        git_hash = subprocess.check_output(['git', 'log', '-n', '1', '--pretty=%h'], encoding='utf8').split()[0]
        path.cd(current_working_folder)
        return git_hash
    except:
        print("Ignore the git error above if you are building from a source code download instead of a git clone.")
        path.cd(current_working_folder)
        return "src"

def oclint_version():
    return "21.10"

def oclint_dev_version():
    return oclint_version() + '.dev.' + dev_version()

def llvm_version():
    return "13.0.0"

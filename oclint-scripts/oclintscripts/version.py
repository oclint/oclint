#! /usr/bin/env python

import subprocess
import os

from oclintscripts import path

def git_hash():
    current_working_folder = os.getcwd()
    os.chdir(path.root_dir)
    git_hash = subprocess.check_output(['git', 'log', '-n', '1', '--pretty=%h']).split()[0]
    path.cd(current_working_folder)
    return git_hash

def oclint_version():
    return "0.10.4"

def oclint_dev_version():
    return oclint_version() + '.dev.' + git_hash()

def llvm_branches():
    return ['trunk', llvm_default_branch()]

def llvm_default_branch():
    return 'tags/RELEASE_381/rc1'

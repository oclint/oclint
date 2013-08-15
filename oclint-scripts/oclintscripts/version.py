#! /usr/bin/env python

import subprocess
import os
import path

def oclint_version():
    return "0.8"

def git_hash():
    os.chdir(path.root_dir)
    git_hash = subprocess.check_output(['git', 'log', '-n', '1', '--pretty=%h']).split()[0]
    return git_hash

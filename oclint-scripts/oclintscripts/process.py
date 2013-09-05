#! /usr/bin/env python

import subprocess
import sys

def call(command):
    command_exit_code = subprocess.call(command, shell=True)
    if command_exit_code:
        sys.exit(command_exit_code)

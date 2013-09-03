#! /usr/bin/env python

import os
import re
import argparse

SCRIPT_WORKING_FOLDER = os.path.dirname(os.path.abspath(__file__))
REPORTERS_PROJECT_DIRECTORY = os.path.join(SCRIPT_WORKING_FOLDER, "../oclint-reporters")
REPORTERS_SOURCE_ROOT_DIRECTORY = os.path.join(REPORTERS_PROJECT_DIRECTORY, "reporters")
REPORTERS_SOURCE_ROOT_CMAKELISTS = os.path.join(REPORTERS_SOURCE_ROOT_DIRECTORY, "CMakeLists.txt")
REPORTERS_TEMPLATE_DIRECTORY = os.path.join(REPORTERS_PROJECT_DIRECTORY, "template")
REPORTERS_TEMPLATE_FILE_PATH = os.path.join(REPORTERS_TEMPLATE_DIRECTORY, "Reporter.tmpl")

def pre_validate(class_name):
    new_class_path = os.path.join(REPORTERS_SOURCE_ROOT_DIRECTORY, class_name + "Reporter.cpp")
    file_exits = os.path.isfile(new_class_path)
    if file_exits:
        print "Error: " + reporter_class_name + "Reporter.cpp already exists."
    return file_exits

def replace_wildcard(each_line, class_name, reporter_name):
    class_name_wildcard = '{{REPORTER_CLASS_NAME}}'
    reporter_name_wildcard = '{{REPORTER_NAME}}'
    return each_line.replace(class_name_wildcard, class_name).replace(reporter_name_wildcard, reporter_name)

def copy_reporter(class_name, reporter_name):
    new_class_path = os.path.join(REPORTERS_SOURCE_ROOT_DIRECTORY, class_name + "Reporter.cpp")
    new_class_file = open(new_class_path, 'w')
    with open(REPORTERS_TEMPLATE_FILE_PATH, 'r') as reporter_template_file:
        for each_line in reporter_template_file:
            new_class_file.write(replace_wildcard(each_line, class_name, reporter_name))
    new_class_file.close()

def write_cmake_lists(reporter_class_name):
    build_dynamic_reporter = 'BUILD_DYNAMIC_REPORTER(' + reporter_class_name + ')'
    with open(REPORTERS_SOURCE_ROOT_CMAKELISTS, 'a') as cmake_lists_file:
        cmake_lists_file.write('\n' + build_dynamic_reporter + '\n')

def scaffold(class_name, reporter_name):
    copy_reporter(class_name, reporter_name)
    write_cmake_lists(class_name)

def lower_add_space(name_re):
    return ' ' + name_re.group(0).lower()

arg_parser = argparse.ArgumentParser()
arg_parser.add_argument("class_name", help="class name of the reporter")
arg_parser.add_argument("-n", "--name", dest='reporter_name', default="")
args = arg_parser.parse_args()

reporter_class_name = args.class_name
reporter_name = args.reporter_name
if not reporter_name:
    reporter_name = re.sub('[A-Z]', lower_add_space, reporter_class_name)[1:]
if pre_validate(reporter_class_name):
    scaffold(reporter_class_name, reporter_name)

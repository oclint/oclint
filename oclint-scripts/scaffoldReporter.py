#! /usr/bin/env python

import os
import re
import argparse

SCRIPT_WORKING_FOLDER = os.path.dirname(os.path.abspath(__file__))
REPORTERS_PROJECT_DIRECTORY = os.path.join(SCRIPT_WORKING_FOLDER, "../oclint-reporters")
REPORTERS_SOURCE_ROOT_DIRECTORY = os.path.join(REPORTERS_PROJECT_DIRECTORY, "reporters")
REPORTERS_SOURCE_ROOT_CMAKELISTS = os.path.join(REPORTERS_SOURCE_ROOT_DIRECTORY, "CMakeLists.txt")
REPORTERS_TEST_ROOT_DIRECTORY = os.path.join(REPORTERS_PROJECT_DIRECTORY, "test")
REPORTERS_TEST_ROOT_CMAKELISTS = os.path.join(REPORTERS_TEST_ROOT_DIRECTORY, "CMakeLists.txt")
REPORTERS_TEMPLATE_DIRECTORY = os.path.join(REPORTERS_PROJECT_DIRECTORY, "template")
REPORTERS_TEMPLATE_FILE_PATH = os.path.join(REPORTERS_TEMPLATE_DIRECTORY, "Reporter.tmpl")
REPORTERS_TEST_TEMPLATE_FILE_PATH = os.path.join(REPORTERS_TEMPLATE_DIRECTORY, "ReporterTest.tmpl")

def pre_validate(class_name, generate_tests):
    new_class_path = os.path.join(REPORTERS_SOURCE_ROOT_DIRECTORY, class_name + "Reporter.cpp")
    class_file_exits = os.path.isfile(new_class_path)
    if class_file_exits:
        print "Error: " + reporter_class_name + "Reporter.cpp already exists."
    new_test_path = os.path.join(REPORTERS_TEST_ROOT_DIRECTORY, class_name + "ReporterTest.cpp")
    test_file_exits = generate_tests and os.path.isfile(new_test_path)
    if test_file_exits:
        print "Error: " + reporter_class_name + "ReporterTest.cpp already exists."
    return not (class_file_exits or test_file_exits)

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

def copy_test(class_name, reporter_name):
    new_test_path = os.path.join(REPORTERS_TEST_ROOT_DIRECTORY, class_name + "ReporterTest.cpp")
    new_test_file = open(new_test_path, 'w')
    with open(REPORTERS_TEST_TEMPLATE_FILE_PATH, 'r') as reporter_template_file:
        for each_line in reporter_template_file:
            new_test_file.write(replace_wildcard(each_line, class_name, reporter_name))
    new_test_file.close()

def write_cmake_lists(reporter_class_name):
    build_dynamic_reporter = 'BUILD_DYNAMIC_REPORTER(' + reporter_class_name + ')'
    with open(REPORTERS_SOURCE_ROOT_CMAKELISTS, 'a') as cmake_lists_file:
        cmake_lists_file.write('\n' + build_dynamic_reporter + '\n')


def write_test_cmake_lists(reporter_class_name):
    build_test = 'BUILD_TEST(' + reporter_class_name + 'ReporterTest)'
    with open(REPORTERS_TEST_ROOT_CMAKELISTS, 'a') as cmake_lists_file:
        cmake_lists_file.write('\n' + build_test + '\n')

def scaffold(class_name, reporter_name):
    copy_reporter(class_name, reporter_name)
    write_cmake_lists(class_name)

def scaffold_test(class_name, reporter_name):
    copy_test(class_name, reporter_name)
    write_test_cmake_lists(class_name)

def lower_add_space(name_re):
    return ' ' + name_re.group(0).lower()

arg_parser = argparse.ArgumentParser()
arg_parser.add_argument("class_name", help="class name of the reporter")
arg_parser.add_argument("-n", "--name", dest='reporter_name', default="")
arg_parser.add_argument(      "--tests",    dest='generate_tests', action='store_true',  help="Generate a test for the new reporter")
arg_parser.add_argument(      "--no-tests", dest='generate_tests', action='store_false', help="Do not generate a test for the new reporter (default)")
args = arg_parser.parse_args()

reporter_class_name = args.class_name
reporter_name = args.reporter_name
if not reporter_name:
    reporter_name = re.sub('[A-Z]', lower_add_space, reporter_class_name)[1:]
if pre_validate(reporter_class_name, args.generate_tests):
    scaffold(reporter_class_name, reporter_name)
    if args.generate_tests:
        scaffold_test(reporter_class_name, reporter_name)

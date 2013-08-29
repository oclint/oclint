#! /usr/bin/env python

import os
import re
import argparse

SCRIPT_WORKING_FOLDER = os.path.dirname(os.path.abspath(__file__))
RULES_PROJECT_DIRECTORY = os.path.join(SCRIPT_WORKING_FOLDER, "../oclint-rules")
RULES_SOURCE_ROOT_DIRECTORY = os.path.join(RULES_PROJECT_DIRECTORY, "rules")
RULES_SOURCE_ROOT_CMAKELISTS = os.path.join(RULES_SOURCE_ROOT_DIRECTORY, "CMakeLists.txt")
RULES_TEST_ROOT_DIRECTORY = os.path.join(RULES_PROJECT_DIRECTORY, "test")
RULES_TEST_ROOT_CMAKELISTS = os.path.join(RULES_TEST_ROOT_DIRECTORY, "CMakeLists.txt")
RULES_TEMPLATE_DIRECTORY = os.path.join(RULES_PROJECT_DIRECTORY, "template")
CLANG_BASIC_DIRECTORY = os.path.join(SCRIPT_WORKING_FOLDER, "../llvm/tools/clang/include/clang/Basic")

def custom_folder_path(folder_name):
    return os.path.join(RULES_SOURCE_ROOT_DIRECTORY, folder_name)

def custom_cmake_lists_path(folder_name):
    return os.path.join(custom_folder_path(folder_name), "CMakeLists.txt")

def custom_test_folder_path(folder_name):
    return os.path.join(RULES_TEST_ROOT_DIRECTORY, folder_name)

def custom_test_cmake_lists_path(folder_name):
    return os.path.join(custom_test_folder_path(folder_name), "CMakeLists.txt")

def pre_check(folder_name):
    folder_path = custom_folder_path(folder_name)
    if not os.path.isdir(folder_path):
        os.mkdir(folder_path)
    test_folder_path = custom_test_folder_path(folder_name)
    if not os.path.isdir(test_folder_path):
        os.mkdir(test_folder_path)
    cmake_lists_path = custom_cmake_lists_path(folder_name)
    if not os.path.isfile(cmake_lists_path):
        open(cmake_lists_path, 'a').close()
        with open(RULES_SOURCE_ROOT_CMAKELISTS, 'a') as rules_cmake_lists_file:
            rules_cmake_lists_file.write('\nADD_RULE_CATEGORY_DIRECTORY(' + folder_name + ')\n')
    test_cmake_lists_path = custom_test_cmake_lists_path(folder_name)
    if not os.path.isfile(test_cmake_lists_path):
        open(test_cmake_lists_path, 'a').close()
        with open(RULES_TEST_ROOT_CMAKELISTS, 'a') as test_cmake_lists_file:
            test_cmake_lists_file.write('\nADD_SUBDIRECTORY(' + folder_name + ')\n')

def generate_code_block(line, node_type = ""):
    line_re = re.search('def (.+) :', line)
    if (line_re):
        node_name = line_re.group(1) + node_type
        code_block = "\n    /* Visit " + node_name + "\n"
        code_block += "    bool Visit" + node_name + "(" + node_name + " *node)\n"
        code_block += "    {\n        return true;\n    }\n"
        code_block += "     */\n"
        return code_block
    else:
        return ""

def visit_ast_nodes():
    visit_nodes = ""
    with open(os.path.join(CLANG_BASIC_DIRECTORY, "StmtNodes.td"), 'r') as stmt_nodes:
        for each in stmt_nodes:
            visit_nodes += generate_code_block(each)
    with open(os.path.join(CLANG_BASIC_DIRECTORY, "DeclNodes.td"), 'r') as stmt_nodes:
        for each in stmt_nodes:
            visit_nodes += generate_code_block(each, "Decl")
    return visit_nodes

def replace_wildcard(each_line, rule_dict):
    wildcard_attributes = ['RULE_CLASS_NAME', 'RULE_NAME', 'RULE_PRIORITY', "RULE_CATEGORY", "RULE_NAME"]
    for each in wildcard_attributes:
        wildcard = '{{' + each + '}}'
        each_line = each_line.replace(wildcard, rule_dict[each])
    return each_line.replace('{{VISIT_AST_NODE_BLOCK}}', visit_ast_nodes())

def copy_template(template_path, class_path, rule_dict):
    class_file = open(class_path, 'w')
    with open(template_path, 'r') as template_file:
        for each_line in template_file:
            class_file.write(replace_wildcard(each_line, rule_dict))
    class_file.close()

def copy_rule(folder_path, rule_dict):
    rule_type = rule_dict['RULE_TYPE']
    rule_template_path = os.path.join(RULES_TEMPLATE_DIRECTORY, rule_type + 'Rule.tmpl')
    rule_class_name = rule_dict['RULE_CLASS_NAME']
    rule_class_path = os.path.join(folder_path, rule_class_name + 'Rule.cpp')
    copy_template(rule_template_path, rule_class_path, rule_dict)

def copy_test(folder_path, rule_dict):
    test_template_path = os.path.join(RULES_TEMPLATE_DIRECTORY, 'RuleTest.tmpl')
    rule_class_name = rule_dict['RULE_CLASS_NAME']
    test_class_path = os.path.join(folder_path, rule_class_name + 'RuleTest.cpp')
    copy_template(test_template_path, test_class_path, rule_dict)

def write_cmake_lists(cmake_lists_path, rule_dict):
    rule_class_name = rule_dict['RULE_CLASS_NAME']
    set_build_rule = 'SET(Build' + rule_class_name + 'Rule ' + rule_class_name + ')'
    build_dynamic_rules = 'BUILD_DYNAMIC_RULES("${Build' + rule_class_name + 'Rule}")'
    with open(cmake_lists_path, 'a') as cmake_lists_file:
        cmake_lists_file.write('\n' + set_build_rule + '\n' + build_dynamic_rules + '\n')

def write_test_cmake_lists(cmake_lists_path, rule_dict):
    rule_class_name = rule_dict['RULE_CLASS_NAME']
    build_test_rule = 'BUILD_TEST(' + rule_class_name + 'RuleTest)'
    with open(cmake_lists_path, 'a') as cmake_lists_file:
        cmake_lists_file.write('\n' + build_test_rule + '\n')


def scaffold(folder_name, rule_dict):
    copy_rule(custom_folder_path(folder_name), rule_dict)
    write_cmake_lists(custom_cmake_lists_path(folder_name), rule_dict)
    if (rule_dict['RULE_TEST']):
        copy_test(custom_test_folder_path(folder_name), rule_dict)
        write_test_cmake_lists(custom_test_cmake_lists_path(folder_name), rule_dict)

def lower_add_space(name_re):
    return ' ' + name_re.group(0).lower()

arg_parser = argparse.ArgumentParser()
arg_parser.add_argument("class_name", help="class name of the rule")
arg_parser.add_argument("-t", "--type", dest='rule_type', choices=['Generic', 'SourceCodeReader', 'ASTVisitor', 'ASTMatcher'], default="Generic")
arg_parser.add_argument("-c", "--category", dest='rule_category', default="custom")
arg_parser.add_argument("-n", "--name", dest='rule_name', default="")
arg_parser.add_argument("-p", "--priority", type=int, dest='rule_priority', choices=[1, 2, 3], default=3)
arg_parser.add_argument(      "--test",    dest='generate_tests', action='store_true',  help="Generate a test for the new rule")
arg_parser.add_argument(      "--no-test", dest='generate_tests', action='store_false', help="Do not generate a test for the new rule (default)")
arg_parser.set_defaults(generate_tests=True)
args = arg_parser.parse_args()

rule_info_dict = {"RULE_CLASS_NAME" : args.class_name, 
                  "RULE_TYPE"       : args.rule_type,
                  "RULE_PRIORITY"   : str(args.rule_priority),
                  "RULE_NAME"       : args.rule_name,
                  "RULE_TEST"       : args.generate_tests,
                  "RULE_CATEGORY"   : args.rule_category}

if not args.rule_name:
    rule_info_dict["RULE_NAME"] = re.sub('[A-Z]', lower_add_space, args.class_name)[1:]
pre_check(args.rule_category)
scaffold(args.rule_category, rule_info_dict)

#! /usr/bin/env python

import os
import re
import argparse

SCRIPT_WORKING_FOLDER = os.path.dirname(os.path.abspath(__file__))
RULES_PROJECT_DIRECTORY = os.path.join(SCRIPT_WORKING_FOLDER, "../oclint-rules")
RULES_SOURCE_ROOT_DIRECTORY = os.path.join(RULES_PROJECT_DIRECTORY, "rules")
RULES_TEMPLATE_DIRECTORY = os.path.join(RULES_PROJECT_DIRECTORY, "template")
CLANG_BASIC_DIRECTORY = os.path.join(SCRIPT_WORKING_FOLDER, "../llvm/tools/clang/include/clang/Basic")

def custom_folder_path(folder_name):
    return os.path.join(RULES_SOURCE_ROOT_DIRECTORY, folder_name)

def custom_cmake_lists_path(folder_name):
    return os.path.join(custom_folder_path(folder_name), "CMakeLists.txt")

def pre_check(folder_name):
    folder_path = custom_folder_path(folder_name)
    if not os.path.isdir(folder_path):
        os.mkdir(folder_path)
    cmake_lists_path = custom_cmake_lists_path(folder_name)
    if not os.path.isfile(cmake_lists_path):
        open(cmake_lists_path, 'a').close()

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
    wildcard_attributes = ['RULE_CLASS_NAME', 'RULE_NAME', 'RULE_PRIORITY']
    for each in wildcard_attributes:
        wildcard = '{{' + each + '}}'
        each_line = each_line.replace(wildcard, rule_dict[each])
    return each_line.replace('{{VISIT_AST_NODE_BLOCK}}', visit_ast_nodes())

def copy_rule(folder_path, rule_dict):
    rule_type = rule_dict['RULE_TYPE']
    rule_template_path = os.path.join(RULES_TEMPLATE_DIRECTORY, rule_type + 'Rule.tmpl')
    rule_class_name = rule_dict['RULE_CLASS_NAME']
    rule_class_path = os.path.join(folder_path, rule_class_name + 'Rule.cpp')
    rule_class_file = open(rule_class_path, 'w')
    with open(rule_template_path, 'r') as rule_template_file:
        for each_line in rule_template_file:
            rule_class_file.write(replace_wildcard(each_line, rule_dict))
    rule_class_file.close()

def write_cmake_lists(cmake_lists_path, rule_dict):
    rule_class_name = rule_dict['RULE_CLASS_NAME']
    set_build_rule = 'SET(Build' + rule_class_name + 'Rule ' + rule_class_name + ')'
    build_dynamic_rules = 'BUILD_DYNAMIC_RULES("${Build' + rule_class_name + 'Rule}")'
    with open(cmake_lists_path, 'a') as cmake_lists_file:
        cmake_lists_file.write('\n' + set_build_rule + '\n' + build_dynamic_rules + '\n')

def scaffold(folder_name, rule_dict):
    copy_rule(custom_folder_path(folder_name), rule_dict)
    write_cmake_lists(custom_cmake_lists_path(folder_name), rule_dict)

def lower_add_space(name_re):
    return ' ' + name_re.group(0).lower()

arg_parser = argparse.ArgumentParser()
arg_parser.add_argument("class_name", help="class name of the rule")
arg_parser.add_argument("-t", "--type", dest='rule_type', choices=['Generic', 'SourceCodeReader', 'ASTVisitor', 'ASTMatcher'], default="Generic")
arg_parser.add_argument("-c", "--category", dest='rule_category', default="custom")
arg_parser.add_argument("-n", "--name", dest='rule_name', default="")
arg_parser.add_argument("-p", "--priority", type=int, dest='rule_priority', default=3)
args = arg_parser.parse_args()

rule_info_dict = {"RULE_CLASS_NAME" : args.class_name, "RULE_TYPE" : args.rule_type, "RULE_PRIORITY" : str(args.rule_priority), "RULE_NAME" : args.rule_name}
if not args.rule_name:
    rule_info_dict["RULE_NAME"] = re.sub('[A-Z]', lower_add_space, args.class_name)[1:]
pre_check(args.rule_category)
scaffold(args.rule_category, rule_info_dict)

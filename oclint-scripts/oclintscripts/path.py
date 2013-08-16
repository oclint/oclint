#! /usr/bin/env python

import os

__python_package_dir = os.path.dirname(os.path.abspath(__file__))
scripts_dir = os.path.dirname(__python_package_dir)
root_dir = os.path.dirname(scripts_dir)
build_root = os.path.join(root_dir, "build")

def module_source_dir(module_name):
    return os.path.join(root_dir, module_name)

def oclint_module_source_dir(module_name):
    return module_source_dir("oclint-" + module_name)

def module_build_dir(module_name):
    return os.path.join(build_root, module_name)

def oclint_module_build_dir(module_name):
    return module_build_dir("oclint-" + module_name)

def oclint_module_test_dir(module_name):
    return module_build_dir("oclint-" + module_name + "-test")

def oclint_module_champagne_dir(module_name):
    return module_build_dir("oclint-" + module_name + "-champagne")

class source:
    clang_dir = module_source_dir("llvm")
    googletest_dir = module_source_dir("googletest")

    core_dir = oclint_module_source_dir("core")
    metrics_dir = oclint_module_source_dir("metrics")
    rules_dir = oclint_module_source_dir("rules")
    reporters_dir = oclint_module_source_dir("reporters")
    driver_dir = oclint_module_source_dir("driver")

    json_compilation_database_dir = oclint_module_source_dir("json-compilation-database")
    xcodebuild_dir = oclint_module_source_dir("xcodebuild")

class build:
    clang_build_dir = module_build_dir('llvm')
    clang_install_dir = module_build_dir("llvm-install")
    googletest_build_dir = module_build_dir("googletest")

    core_build_dir = oclint_module_build_dir("core")
    metrics_build_dir = oclint_module_build_dir("metrics")
    rules_build_dir = oclint_module_build_dir("rules")
    reporters_build_dir = oclint_module_build_dir("reporters")
    driver_build_dir = oclint_module_build_dir("driver")

    bundle_dir = oclint_module_build_dir("release")

    core_test_dir = oclint_module_test_dir("core")
    metrics_test_dir = oclint_module_test_dir("metrics")
    rules_test_dir = oclint_module_test_dir("rules")
    reporters_test_dir = oclint_module_test_dir("reporters")

    core_champagne_dir = oclint_module_champagne_dir("core")
    metrics_champagne_dir = oclint_module_champagne_dir("metrics")
    rules_champagne_dir = oclint_module_champagne_dir("rules")
    reporters_champagne_dir = oclint_module_champagne_dir("reporters")
    driver_champagne_dir = oclint_module_champagne_dir("driver")

class url:
    llvm = 'http://llvm.org/svn/llvm-project/llvm/'
    clang = 'http://llvm.org/svn/llvm-project/cfe/'
    clang_rt = 'http://llvm.org/svn/llvm-project/compiler-rt/'
    googletest = 'http://googlemock.googlecode.com/svn/trunk/'

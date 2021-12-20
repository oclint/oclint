#! /usr/bin/env python3

import os
import shutil

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

def oclint_module_dogfooding_dir(module_name):
    return module_build_dir("oclint-" + module_name + "-dogfooding")

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
    googletest_build_dir = os.path.join(module_build_dir("googletest"), 'lib')

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

    core_dogfooding_dir = oclint_module_dogfooding_dir("core")
    metrics_dogfooding_dir = oclint_module_dogfooding_dir("metrics")
    rules_dogfooding_dir = oclint_module_dogfooding_dir("rules")
    reporters_dogfooding_dir = oclint_module_dogfooding_dir("reporters")
    driver_dogfooding_dir = oclint_module_dogfooding_dir("driver")

class url:
    googletest = 'https://github.com/google/googletest.git'

    json_compilation_database = 'https://github.com/oclint/oclint-json-compilation-database.git'
    xcodebuild = 'https://github.com/oclint/oclint-xcodebuild.git'

    clang_prebuilt_binary_for_macos = 'https://github.com/llvm/llvm-project/releases/download/llvmorg-13.0.0/clang+llvm-13.0.0-x86_64-apple-darwin.tar.xz'
    clang_prebuilt_binary_for_macos_arm64 = 'https://github.com/ryuichis/llvm-builder/releases/download/llvm-13.0.0-m1/llvm-13.0.0-arm64-apple-darwin.tar.xz'
    clang_prebuilt_binary_for_ubuntu_lts_20 = 'https://github.com/llvm/llvm-project/releases/download/llvmorg-13.0.0/clang+llvm-13.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz'
    clang_prebuilt_binary_for_ubuntu_lts_20_aarch64 = 'https://github.com/llvm/llvm-project/releases/download/llvmorg-13.0.0/clang+llvm-13.0.0-aarch64-linux-gnu.tar.xz'

def cd(dir_path):
    os.chdir(dir_path)

def mv(src_path, dst_path):
    os.rename(src_path, dst_path)

def rm_f(dir_path):
    if is_dir(dir_path):
        shutil.rmtree(dir_path)

def mkdir_p(dir_path):
    if not is_dir(dir_path):
        os.makedirs(dir_path)

def cp(src_path, dst_path):
    shutil.copy2(src_path, dst_path)

def cp_r(src_path, dst_path):
    shutil.copytree(src_path, dst_path)

def is_dir(dir_path):
    return os.path.isdir(dir_path)

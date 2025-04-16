# Copyright 2025 The Lynx Authors. All rights reserved.
# Licensed under the Apache License Version 2.0 that can be found in the
# LICENSE file in the root directory of this source tree.

# /usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import yaml

LYNX_ROOT_PATH = os.getcwd()
TOOLS_PATH = os.path.join(LYNX_ROOT_PATH, 'tools')
ANDROID_API_PATH = os.path.join(LYNX_ROOT_PATH, 'platform', 'android', 'api')
IOS_API_PATH = os.path.join(LYNX_ROOT_PATH, 'platform', 'darwin', 'ios', 'api')
ERROR_CODE_PACKAGE_DIR = os.path.join(LYNX_ROOT_PATH, 'tools', 'error_code')
FEATURE_COUNT_PACKAGE_DIR = os.path.join(LYNX_ROOT_PATH, 'tools',
                                         'feature_count')

API_CONFIG_PATH = os.path.abspath(
    os.path.join(LYNX_ROOT_PATH, 'tools', 'api', 'config.yml'))
with open(API_CONFIG_PATH, 'r') as f:
    api_config = yaml.safe_load(f)
    DOXYGEN_PATH = os.path.normpath(
        os.path.join(LYNX_ROOT_PATH, api_config['path']['doxygen']))
    HANDLE_FAILED_INSTRUCTION = api_config['path']['instruction_doc']


def guarantee_generated_files():
    """
    Generate error code and feature count files.
    """
    sys.path.append(TOOLS_PATH)
    sys.path.append(ERROR_CODE_PACKAGE_DIR)
    from error_code.gen_error_code import main as gen_error_code
    sys.path.remove(ERROR_CODE_PACKAGE_DIR)
    gen_error_code()

    sys.path.append(FEATURE_COUNT_PACKAGE_DIR)
    from feature_count.generate_feature_count import main as generate_feature_count
    sys.path.remove(FEATURE_COUNT_PACKAGE_DIR)
    generate_feature_count()


if __name__ == '__main__':
    guarantee_generated_files()

# Copyright (c) 2021-2023 HPMicro
# SPDX-License-Identifier: BSD-3-Clause
cmake_minimum_required(VERSION 3.13)
# set(CONFIG_HPM_MATH 1)
set(CONFIG_MOTORCTRL 1)
set(CONFIG_HPM_ADC 1)
# set(HPM_MATH_DSP_SES_LIB "libdspf")

set(RV_ABI "ilp32f")
set(RV_ARCH "rv32imafc")

find_package(hpm-sdk REQUIRED HINTS $ENV{HPM_SDK_BASE})
# sdk_compile_options("-Os")
project(bldc_foc)
sdk_app_inc(src)
sdk_app_src(src/bldc_foc_callback.c)
sdk_app_src(src/bldc_foc.c)
sdk_ld_options("-lm")
generate_ses_project()
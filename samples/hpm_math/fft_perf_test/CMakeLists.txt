# Copyright (c) 2022 HPMicro
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.13)
set(SES_TOOLCHAIN_VARIANT "Andes")

set(CONFIG_HPM_MATH 1)
set(CONFIG_HPM_MATH_DSP 1)
set(HPM_MATH_DSP_SES_LIB "libdspf")
find_package(hpm-sdk REQUIRED HINTS $ENV{HPM_SDK_BASE})

project(fft_perf_test)
sdk_ses_compile_options(-mabi=ilp32f)
sdk_ses_compile_options(-march=rv32imafc)

sdk_app_src(src/fft_perf_test.c)
sdk_compile_options("-O3")

generate_ses_project()

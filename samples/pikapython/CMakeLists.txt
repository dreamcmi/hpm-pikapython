# Copyright (c) 2023 Dreamcmi
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13)

set(CONFIG_FREERTOS 1)
set(CONFIG_MBEDTLS 1)
set(CONFIG_MBEDTLS_USE_HPM_SDP 1)

find_package(hpm-sdk REQUIRED HINTS $ENV{HPM_SDK_BASE})
project(pikapython)

sdk_compile_definitions(-D__freertos_irq_stack_top=_stack)
sdk_compile_definitions(-DCONFIG_FREERTOS=1)
sdk_compile_definitions(-DUSE_NONVECTOR_MODE=1)
sdk_compile_definitions(-DDISABLE_IRQ_PREEMPTIVE=1)

sdk_compile_definitions(-DMBEDTLS_CONFIG_FILE="hpm_sdk_mbedtls_config.h")
sdk_compile_definitions(-DCONFIG_MBEDTLS_USE_HPM_SDP=1)

sdk_compile_definitions(-DPIKA_FREERTOS_ENABLE=1)

# sdk_compile_options(-mabi=ilp32d)
# sdk_compile_options(-march=rv32gc)
# sdk_compile_options("-Os")

sdk_ses_compile_options(-mabi=ilp32d)
sdk_ses_compile_options(-march=rv32gc)
sdk_ses_compile_options("-Os")

sdk_inc(src)
sdk_app_inc(pikascript-api)
sdk_app_inc(pikascript-core)
sdk_app_inc(pikascript-lib/PikaStdLib)
sdk_app_inc(pikascript-lib/PikaStdDevice)
sdk_app_inc(pikascript-lib/re)
sdk_app_inc(pikascript-lib/json)
sdk_app_inc(pikascript-lib/pika_cjson)

sdk_app_src(src/app_main.c)
sdk_app_src(src/pika_config.c)

# PikaStdLib src
sdk_app_src(pikascript-lib/PikaStdLib/PikaDebuger_Debuger.c)
sdk_app_src(pikascript-lib/PikaStdLib/PikaStdData_ByteArray.c)
sdk_app_src(pikascript-lib/PikaStdLib/PikaStdData_Dict.c)
sdk_app_src(pikascript-lib/PikaStdLib/PikaStdData_FILEIO.c)
sdk_app_src(pikascript-lib/PikaStdLib/PikaStdData_List.c)
sdk_app_src(pikascript-lib/PikaStdLib/PikaStdData_String.c)
sdk_app_src(pikascript-lib/PikaStdLib/PikaStdData_Tuple.c)
sdk_app_src(pikascript-lib/PikaStdLib/PikaStdData_Utils.c)
sdk_app_src(pikascript-lib/PikaStdLib/PikaStdLib_MemChecker.c)
sdk_app_src(pikascript-lib/PikaStdLib/PikaStdLib_RangeObj.c)
sdk_app_src(pikascript-lib/PikaStdLib/PikaStdLib_StringObj.c)
sdk_app_src(pikascript-lib/PikaStdLib/PikaStdLib_SysObj.c)
sdk_app_src(pikascript-lib/PikaStdLib/PikaStdTask_Task.c)

# time src
sdk_app_src(pikascript-lib/time/_time.c)

# binascii src
sdk_app_src(pikascript-lib/binascii/binascii.c)

# PikaStdDevice src
sdk_app_src(pikascript-lib/PikaStdDevice/pika_hal_SOFT_IIC.c)
sdk_app_src(pikascript-lib/PikaStdDevice/pika_hal_SOFT_SPI.c)
sdk_app_src(pikascript-lib/PikaStdDevice/pika_hal.c)
sdk_app_src(pikascript-lib/PikaStdDevice/PikaStdDevice_ADC.c)
sdk_app_src(pikascript-lib/PikaStdDevice/PikaStdDevice_BaseDev.c)
sdk_app_src(pikascript-lib/PikaStdDevice/PikaStdDevice_CAN.c)
sdk_app_src(pikascript-lib/PikaStdDevice/PikaStdDevice_common.c)
sdk_app_src(pikascript-lib/PikaStdDevice/PikaStdDevice_DAC.c)
sdk_app_src(pikascript-lib/PikaStdDevice/PikaStdDevice_GPIO.c)
sdk_app_src(pikascript-lib/PikaStdDevice/PikaStdDevice_IIC.c)
sdk_app_src(pikascript-lib/PikaStdDevice/PikaStdDevice_PWM.c)
sdk_app_src(pikascript-lib/PikaStdDevice/PikaStdDevice_SPI.c)
sdk_app_src(pikascript-lib/PikaStdDevice/PikaStdDevice_UART.c)

# random src
sdk_app_src(pikascript-lib/random/_random.c)

# re src
sdk_app_src(pikascript-lib/re/cre.c)
sdk_app_src(pikascript-lib/re/pcre_chartables.c)
sdk_app_src(pikascript-lib/re/pcre_compile.c)
sdk_app_src(pikascript-lib/re/pcre_exec.c)
sdk_app_src(pikascript-lib/re/pcre_fullinfo.c)
sdk_app_src(pikascript-lib/re/pcre_globals.c)
sdk_app_src(pikascript-lib/re/pcre_newline.c)
sdk_app_src(pikascript-lib/re/pcre_ord2utf8.c)
sdk_app_src(pikascript-lib/re/pcre_tables.c)
sdk_app_src(pikascript-lib/re/pcre_try_flipped.c)
sdk_app_src(pikascript-lib/re/pcre_valid_utf8.c)
sdk_app_src(pikascript-lib/re/pcre_xclass.c)
sdk_app_src(pikascript-lib/re/re-api-adapter.c)

# aes src
sdk_app_src(pikascript-lib/aes/_aes_AES.c)

# base64 src
sdk_app_src(pikascript-lib/base64/_base64.c)

# hashlib src
sdk_app_src(pikascript-lib/hashlib/_hashlib_Hash.c)

# hmac src
sdk_app_src(pikascript-lib/hmac/_hmac_HMAC.c)

# pika math src
sdk_app_src(pikascript-lib/PikaMath/PikaMath_Math.c)
sdk_app_src(pikascript-lib/PikaMath/PikaMath_Operator.c)
sdk_app_src(pikascript-lib/PikaMath/PikaMath_Quaternion.c)

sdk_app_src(pikascript-lib/math/_math.c)

# pika json src
sdk_app_src(pikascript-lib/json/_json.c)
sdk_app_src(pikascript-lib/json/_pika_cJSON.c)

sdk_app_src(pikascript-lib/pika_cjson/cJSON.c)
sdk_app_src(pikascript-lib/pika_cjson/pika_cjson_cJSON.c)
sdk_app_src(pikascript-lib/pika_cjson/pika_cjson_item.c)
sdk_app_src(pikascript-lib/pika_cjson/pika_cjson.c)

# _thread src
sdk_app_src(pikascript-lib/_thread/_thread.c)

# pika-core src
sdk_app_src(pikascript-core/BaseObj.c)
sdk_app_src(pikascript-core/dataArg.c)
sdk_app_src(pikascript-core/dataArgs.c)
sdk_app_src(pikascript-core/dataLink.c)
sdk_app_src(pikascript-core/dataLinkNode.c)
sdk_app_src(pikascript-core/dataMemory.c)
sdk_app_src(pikascript-core/dataQueue.c)
sdk_app_src(pikascript-core/dataQueueObj.c)
sdk_app_src(pikascript-core/dataStack.c)
sdk_app_src(pikascript-core/datastring.c)
sdk_app_src(pikascript-core/dataStrs.c)
sdk_app_src(pikascript-core/PikaCompiler.c)
sdk_app_src(pikascript-core/PikaObj.c)
sdk_app_src(pikascript-core/PikaParser.c)
sdk_app_src(pikascript-core/PikaPlatform.c)
sdk_app_src(pikascript-core/PikaVM.c)
sdk_app_src(pikascript-core/TinyObj.c)

# pika-api src
sdk_app_src(pikascript-api/__asset_pikaModules_py_a.c)
sdk_app_src(pikascript-api/__pikaBinding.c)
sdk_app_src(pikascript-api/pikaScript.c)

generate_ses_project()

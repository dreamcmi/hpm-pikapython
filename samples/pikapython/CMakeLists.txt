# Copyright (c) 2023 Dreamcmi
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13)

set(CONFIG_FREERTOS 1)
# set(CONFIG_PIKAPYTHON 1)
# set(SES_TOOLCHAIN_VARIANT "Andes")

find_package(hpm-sdk REQUIRED HINTS $ENV{HPM_SDK_BASE})
project(pikapython)

sdk_compile_definitions(-D__freertos_irq_stack_top=_stack)
sdk_compile_definitions(-DCONFIG_FREERTOS=1)
sdk_compile_definitions(-DUSE_NONVECTOR_MODE=1)
sdk_compile_definitions(-DDISABLE_IRQ_PREEMPTIVE=1)

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

sdk_app_src(src/app_main.c)
sdk_app_src(src/pika_config.c)

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

sdk_app_src(pikascript-api/__asset_pikaModules_py_a.c)
sdk_app_src(pikascript-api/__pikaBinding.c)
sdk_app_src(pikascript-api/pikaScript.c)

generate_ses_project()

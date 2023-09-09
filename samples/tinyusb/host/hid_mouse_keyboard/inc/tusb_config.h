/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

/*
 * Copyright (c) 2022 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef TUSB_CONFIG_H
#define TUSB_CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif

/*---------------------------------------------------------------------*
 * COMMON CONFIGURATION
 *---------------------------------------------------------------------*/

/* defined by CMakeLists.txt */
#ifndef CFG_TUSB_MCU
	#error CFG_TUSB_MCU must be defined
#endif

/* RHPort number used for host can be defined by CMakeLists.txt, default to port 0 */
#ifndef BOARD_HOST_RHPORT_NUM
    #define BOARD_HOST_RHPORT_NUM     0
#endif

#if   BOARD_HOST_RHPORT_NUM == 0
	#define CFG_TUSB_RHPORT0_MODE       (OPT_MODE_HOST | OPT_MODE_HIGH_SPEED)
#elif BOARD_HOST_RHPORT_NUM == 1
	#define CFG_TUSB_RHPORT1_MODE       (OPT_MODE_HOST | OPT_MODE_HIGH_SPEED)
#endif

#ifndef CFG_TUSB_OS
	#define CFG_TUSB_OS                 OPT_OS_NONE
#endif

 #define CFG_TUSB_DEBUG					0

/* USB DMA on some MCUs can only access a specific SRAM region with restriction on alignment.
 * Tinyusb use follows macros to declare transferring memory so that they can be put
 * into those specific section.
 * e.g
 * - CFG_TUSB_MEM SECTION : __attribute__ (( section(".usb_ram") ))
 * - CFG_TUSB_MEM_ALIGN   : __attribute__ ((aligned(4)))
 */
#ifndef CFG_TUSB_MEM_SECTION
	#define CFG_TUSB_MEM_SECTION		__attribute__ ((section(".noncacheable")))
#endif

#ifndef CFG_TUSB_MEM_ALIGN
	#define CFG_TUSB_MEM_ALIGN      __attribute__ ((aligned(4)))
#endif

#ifndef CFG_TUH_EP_MAX
	#define CFG_TUH_EP_MAX             8
#endif

/*---------------------------------------------------------------------*
 * CONFIGURATION
 *---------------------------------------------------------------------*/

/* Size of buffer to hold descriptors and other data used for enumeration */
#define CFG_TUH_ENUMERATION_BUFSIZE 256

#define CFG_TUH_HUB                 0
#define CFG_TUH_CDC                 0
#define CFG_TUH_HID                 4 /* typical keyboard + mouse device can have 3-4 HID interfaces */
#define CFG_TUH_MSC                 0
#define CFG_TUH_VENDOR              0

/* max device support (excluding hub device) */
#define CFG_TUSB_HOST_DEVICE_MAX          (CFG_TUH_HUB ? 4 : 1) /* hub typically has 4 ports */

/*------------- HID -------------*/
#define CFG_TUH_HID_EPIN_BUFSIZE    64
#define CFG_TUH_HID_EPOUT_BUFSIZE   64

#ifdef __cplusplus
 }
#endif

#endif /* TUSB_CONFIG_H */

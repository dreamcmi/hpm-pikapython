/*
 * Copyright (c) 2014-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * Copyright 2021 ACRIOS Systems s.r.o.
 * Copyright (c) 2022 HPMicro
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "erpc_manually_constructed.hpp"
#include "erpc_rpmsg_lite_transport.hpp"
#include "erpc_transport_setup.h"
#include "hpm_common.h"

using namespace erpc;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

#if !defined(SH_MEM_TOTAL_SIZE)
#define SH_MEM_TOTAL_SIZE (6144U)
#endif

#if defined(__ICCARM__) /* IAR Workbench */
#pragma location = "rpmsg_sh_mem_section"
char rpmsg_lite_base[SH_MEM_TOTAL_SIZE];
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION) /* Keil MDK */
char rpmsg_lite_base[SH_MEM_TOTAL_SIZE] __attribute__((section("rpmsg_sh_mem_section")));
#elif defined(__GNUC__)
ATTR_SHARE_MEM char rpmsg_lite_base[SH_MEM_TOTAL_SIZE];
#else
#error "RPMsg: Please provide your definition of rpmsg_lite_base[]!"
#endif

ERPC_MANUALLY_CONSTRUCTED(RPMsgTransport, s_transport);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

erpc_transport_t erpc_transport_rpmsg_lite_master_init(uint32_t src_addr, uint32_t dst_addr, uint32_t rpmsg_link_id)
{
    erpc_transport_t transport;

    s_transport.construct();
    if (s_transport->init(src_addr, dst_addr, rpmsg_lite_base, SH_MEM_TOTAL_SIZE, rpmsg_link_id) == kErpcStatus_Success)
    {
        transport = reinterpret_cast<erpc_transport_t>(s_transport.get());
    }
    else
    {
        transport = NULL;
    }

    return transport;
}

/*
 * Copyright (c) 2023 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef ETHERNETIF_H
#define ETHERNETIF_H


#include "lwip/err.h"
#include "lwip/netif.h"
#ifdef __cplusplus
extern "C" {
#endif
/* Exported functions---------------------------------------------------------*/
err_t ethernetif_init(struct netif *netif);
#if !NO_SYS
void ethernetif_input(void *pvParameters);
#else
err_t ethernetif_input(struct netif *netif);
#endif
void User_notification(struct netif *netif);
#ifdef __cplusplus /* __cplusplus */
}
#endif

#endif /* ETHERNETIF_H */

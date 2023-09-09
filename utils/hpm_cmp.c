/*
 * SPDX-FileCopyrightText: 2023 Dreamcmi
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include "hpm_common.h"

int __ucmpdi2 (unsigned long a, unsigned long b){
    if(a < b) return 0;
    if(a > b) return 2;
    if(a == b) return 1;
}
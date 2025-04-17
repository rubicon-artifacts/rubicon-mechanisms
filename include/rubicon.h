/*
 * Copyright (C) 2025 Matej Bölcskei, ETH Zurich
 * Licensed under the GNU General Public License as published by the Free Software Foundation, version 3.
 * See LICENSE or <https://www.gnu.org/licenses/gpl-3.0.html> for details.
 * 
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int pcp_evict();

int block_merge(void *target, unsigned order);

int migratetype_escalation(void *bait, unsigned bait_order,
                           int (*bait_allocator)());

#ifdef __cplusplus
}
#endif
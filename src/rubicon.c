/*
 * Copyright (C) 2025 Matej Bölcskei, ETH Zurich
 * Licensed under the GNU General Public License as published by the Free Software Foundation, version 3.
 * See LICENSE or <https://www.gnu.org/licenses/gpl-3.0.html> for details.
 * 
 * SPDX-License-Identifier: GPL-3.0-only
 */

#define _GNU_SOURCE

#include "rubicon.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_SIZE 0x1000UL
#define PCP_FLUSH_SIZE 0x200000UL
#define NUM_FLUSH_FILES 1000
#define PCP_PUSH_SIZE 0x2000000UL

int pcp_evict() {
  const char *buf = "ffffffffffffffff";
  int fds[NUM_FLUSH_FILES];

  void *flush_ptr = mmap(NULL, PCP_FLUSH_SIZE, PROT_READ | PROT_WRITE,
                         MAP_ANONYMOUS | MAP_PRIVATE | MAP_POPULATE, -1, 0);
  if (flush_ptr == MAP_FAILED) {
    return -1;
  }

  for (int i = 0; i < NUM_FLUSH_FILES; ++i) {
    fds[i] = open("/tmp", O_TMPFILE | O_RDWR, S_IRUSR | S_IWUSR);
    if (fds[i] == -1) {
      return -1;
    }
    write(fds[i], buf, 8);
  }

  munmap(flush_ptr, PCP_FLUSH_SIZE);

  for (int i = 0; i < NUM_FLUSH_FILES; ++i) {
    if (close(fds[i]) == -1) {
      return -1;
    }
  }

  return 0;
}

int block_merge(void *target, unsigned order) {
  if (munmap(target, PAGE_SIZE << order)) {
    return -1;
  }

  if (order == 0) {
    return 0;
  }

  void *flush_ptr = mmap(NULL, PCP_PUSH_SIZE, PROT_READ | PROT_WRITE,
                         MAP_ANONYMOUS | MAP_PRIVATE | MAP_POPULATE, -1, 0);
  if (flush_ptr == MAP_FAILED) {
    return -1;
  }

  return munmap(flush_ptr, PCP_PUSH_SIZE);
}

void *exhaust_blocks(unsigned long *exhaust_size) {
  *exhaust_size =
      sysconf(_SC_AVPHYS_PAGES) * sysconf(_SC_PAGESIZE) - 0x10000000UL;

  return mmap(NULL, *exhaust_size, PROT_READ | PROT_WRITE,
              MAP_PRIVATE | MAP_ANONYMOUS | MAP_POPULATE, -1, 0);
}

int migratetype_escalation(void *bait, unsigned bait_order,
                           int (*bait_allocator)()) {
  unsigned long exhaust_size;

  void *exhaust_ptr = exhaust_blocks(&exhaust_size);
  if (exhaust_ptr == MAP_FAILED) {
    return -1;
  }

  if (block_merge(bait, bait_order)) {
    return -1;
  }

  if (bait_allocator()) {
    return -1;
  }

  return munmap(exhaust_ptr, exhaust_size);
}
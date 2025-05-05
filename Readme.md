# Rubicon Mechanisms

This repository provides an implementation of the three core mechanisms that underpin Rubicon, the first methodology for fully deterministic memory massaging. These mechanisms are detailed in section 6 of our paper ["Rubicon: Precise Microarchitectural Attacks with Page-Granular Massaging"](https://comsec.ethz.ch/wp-content/files/rubicon_eurosp25.pdf).

## Requirements

To use the mechanisms, ensure you meet the following requirements:

- **Linux Kernel**: Tested with versions 5.4, 5.8, 5.15, and 6.8. Other versions should work as long as they are not unreasonably old.
- **Clang**: Tested with version 10.0.0, but other versions or compilers should also work.

## Overview

Rubicon consists of the following mechanisms, each designed to manipulate kernel memory allocation behavior in specific ways:

- **PCP Evict**: This mechanism clears the per-CPU page (PCP) lists of a specified CPU core by repeatedly allocating and releasing large amounts of memory. By depleting the PCP lists, the kernel is forced to allocate memory directly from the free lists.

- **Block Merge**: This mechanism merges user pages into a contiguous block and positions it at the head of a desired free list within the same migratetype. By doing so, it ensures that the next allocation from this free list will use the prepared block. This allows precise placement of arbitrary data structures at arbitrary locations in physical memory.

- **Migratetype Escalation**: This mechanism exploits the kernel's memory fragmentation reduction heuristics to move memory blocks across different migratetypes. This capability is particularly useful for manipulating kernel data structures such as page tables or kernel stacks.

## Integrating the Mechanisms in Your Project

For a practical example of how to integrate these mechanisms into your project, please refer to the [rubicon-microbenchmarks](https://github.com/rubicon-artifacts/rubicon-microbenchmarks) repository.

## Citing our Work
To cite Rubicon in academic papers, please use the following BibTeX entry:

```
@inproceedings{boelcskei_rubicon_2025,
	title = {{Rubicon: Precise Microarchitectural Attacks with Page-Granular Massaging}}, 
	url = {Paper=https://comsec.ethz.ch/wp-content/files/rubicon_eurosp25.pdf},
	booktitle = {{EuroS\&P}},
	author = {Bölcskei, Matej and Jattke, Patrick and Wikner, Johannes and Razavi, Kaveh},
	month = jun,
	year = {2025},
	keywords = {dir\_os, type\_conf}
}
```
/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_UTIME_H
#define VIAL_UTIME_H

#include <stdint.h>

uint64_t vial_utime(void);

uint64_t vial_mtime(void);

void vial_msleep(unsigned long ms);

#endif

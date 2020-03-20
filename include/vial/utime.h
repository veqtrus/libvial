/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_UTIME_H
#define VIAL_UTIME_H

#include <stdint.h>

uint64_t utime(void);

uint64_t mtime(void);

void msleep(unsigned long ms);

#endif

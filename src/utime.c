/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/utime.h>

#if defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#endif

uint64_t utime(void)
{
#if defined(_WIN32) || defined(_WIN64)
	union {
		uint64_t tm_100ns;
		FILETIME tm_file;
	} tm;
	tm.tm_100ns = 0;
	GetSystemTimeAsFileTime(&tm.tm_file);
	return tm.tm_100ns / 10;
#else
	struct timespec tm;
	clock_gettime(CLOCK_MONOTONIC, &tm);
	return tm.tv_sec * 1000000 + tm.tv_nsec / 1000;
#endif
}

uint64_t mtime(void)
{
	return utime() / 1000;
}

void msleep(unsigned long ms)
{
#if defined(_WIN32) || defined(_WIN64)
	Sleep(ms);
#else
	struct timespec req, rem;
	rem.tv_sec = ms / 1000;
	rem.tv_nsec = ms * 1000000 - rem.tv_sec * 1000000000;
	int status;
	do {
		req = rem;
		status = nanosleep(&req, &rem);
	} while (status < 0 && errno == EINTR);
#endif
}

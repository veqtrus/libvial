/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_IO_NETSTREAM_H
#define VIAL_IO_NETSTREAM_H

#include <stdint.h>

#include <vial/io/stream/stream.h>
#include "socket.h"

struct vNetStream {
	struct vStream stream;
	uint64_t read_timeout, write_timeout;
	struct vSocket socket;
};

error_t vNetStream_init(struct vNetStream *self, struct vSocket socket);

#endif

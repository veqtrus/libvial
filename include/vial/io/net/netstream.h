/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_IO_NETSTREAM_H
#define VIAL_IO_NETSTREAM_H

#include <stdint.h>

#include <vial/io/stream/stream.h>
#include "socket.h"

struct vial_netstream {
	struct vial_stream stream;
	uint64_t read_timeout, write_timeout;
	struct vial_socket socket;
};

vial_error vial_netstream_init(struct vial_netstream *self, struct vial_socket socket);

#endif

/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_IO_HASHSTREAM_H
#define VIAL_IO_HASHSTREAM_H

#include <vial/io/stream/stream.h>
#include <vial/crypto/sha256.h>

/* SHA-256 */
VIAL_BEGIN_CLASS_DECL(vial_hashstream, vial_object)
	VIAL_IMPLEMENTS(vial_stream);
	struct vial_sha256 hasher;
VIAL_END_CLASS_DECL;
VIAL_BEGIN_VIRTUALS(vial_hashstream) VIAL_END_VIRTUALS;

vial_error vial_hashstream_init(struct vial_hashstream *self);

vial_error vial_hashstream_sha256(struct vial_hashstream *self, void *buf);

vial_error vial_hashstream_sha256d(struct vial_hashstream *self, void *buf);

#endif

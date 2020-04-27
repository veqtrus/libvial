/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_COLLECTIONS_VECTOR_OF_STDINT_H
#define VIAL_COLLECTIONS_VECTOR_OF_STDINT_H

#include <stdint.h>

#include <vial/collections/vector.h>

typedef vial_vector(int8_t) vial_vector_of_int8;
typedef vial_vector(int16_t) vial_vector_of_int16;
typedef vial_vector(int32_t) vial_vector_of_int32;
typedef vial_vector(int64_t) vial_vector_of_int64;

typedef vial_vector(uint8_t) vial_vector_of_uint8;
typedef vial_vector(uint16_t) vial_vector_of_uint16;
typedef vial_vector(uint32_t) vial_vector_of_uint32;
typedef vial_vector(uint64_t) vial_vector_of_uint64;

#endif

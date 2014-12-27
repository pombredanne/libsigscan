/*
 * Signature table functions
 *
 * Copyright (c) 2014, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBSIGSCAN_SIGNATURE_TABLE_H )
#define _LIBSIGSCAN_SIGNATURE_TABLE_H

#include <common.h>
#include <types.h>

#include "libsigscan_byte_value_group.h"
#include "libsigscan_libcdata.h"
#include "libsigscan_libcerror.h"
#include "libsigscan_signature.h"
#include "libsigscan_signature_group.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libsigscan_signature_table libsigscan_signature_table_t;

struct libsigscan_signature_table
{
	/* The byte value groups list sorted by offset and byte value
	 */
	libcdata_array_t *byte_value_groups_list;

	/* The signatures array
	 */
	libcdata_array_t *signatures_array;

	/* The smallest offset
	 */
	off64_t smallest_offset;

	/* The size of the pattern of the signature with the smallest offset
	 */
	size_t smallest_size;

	/* The largest offset
	 */
	off64_t largest_offset;

	/* The size of the pattern of the signature with the largest offset
	 */
	size_t largest_size;
};

int libsigscan_signature_table_initialize(
     libsigscan_signature_table_t **signature_table,
     libcerror_error_t **error );

int libsigscan_signature_table_free(
     libsigscan_signature_table_t **signature_table,
     libcerror_error_t **error );

int libsigscan_signature_table_fill(
     libsigscan_signature_table_t *signature_table,
     libcdata_array_t *signatures_array,
     libcerror_error_t **error );

int libsigscan_signature_table_get_number_of_byte_value_groups(
     libsigscan_signature_table_t *signature_table,
     int *number_of_byte_value_groups,
     libcerror_error_t **error );

int libsigscan_signature_table_get_byte_value_group_by_index(
     libsigscan_signature_table_t *signature_table,
     int byte_value_group_index,
     libsigscan_byte_value_group_t **byte_value_group,
     libcerror_error_t **error );

int libsigscan_signature_table_get_byte_value_group_by_offset(
     libsigscan_signature_table_t *signature_table,
     off64_t pattern_offset,
     libsigscan_byte_value_group_t **byte_value_group,
     libcerror_error_t **error );

int libsigscan_signature_table_insert_signature(
     libsigscan_signature_table_t *signature_table,
     off64_t pattern_offset,
     uint8_t byte_value,
     libsigscan_signature_t *signature,
     libcerror_error_t **error );

int libsigscan_signature_table_get_number_of_signatures(
     libsigscan_signature_table_t *signature_table,
     int *number_of_signatures,
     libcerror_error_t **error );

int libsigscan_signature_table_get_signature_by_index(
     libsigscan_signature_table_t *signature_table,
     int signature_index,
     libsigscan_signature_t **signature,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

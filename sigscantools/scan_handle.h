/*
 * Scan handle
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

#if !defined( _SCAN_HANDLE_H )
#define _SCAN_HANDLE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "sigscantools_libcerror.h"
#include "sigscantools_libcstring.h"
#include "sigscantools_libsigscan.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct scan_handle scan_handle_t;

struct scan_handle
{
	/* The libsigscan scanner
	 */
	libsigscan_scanner_t *scanner;

	/* The nofication output stream
	 */
	FILE *notify_stream;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int scan_handle_initialize(
     scan_handle_t **scan_handle,
     libcerror_error_t **error );

int scan_handle_free(
     scan_handle_t **scan_handle,
     libcerror_error_t **error );

int scan_handle_signal_abort(
     scan_handle_t *scan_handle,
     libcerror_error_t **error );

int scan_handle_scan_input(
     scan_handle_t *scan_handle,
     const libcstring_system_character_t *filename,
     libcerror_error_t **error );

int scan_handle_scan_results_fprint(
     scan_handle_t *scan_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

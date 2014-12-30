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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libsigscan_byte_value_group.h"
#include "libsigscan_libcdata.h"
#include "libsigscan_libcerror.h"
#include "libsigscan_libcnotify.h"
#include "libsigscan_signature.h"
#include "libsigscan_signature_table.h"

/* Creates a signature table
 * Make sure the value signature_table is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libsigscan_signature_table_initialize(
     libsigscan_signature_table_t **signature_table,
     libcerror_error_t **error )
{
	static char *function = "libsigscan_signature_table_initialize";

	if( signature_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid signature table.",
		 function );

		return( -1 );
	}
	if( *signature_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid signature table value already set.",
		 function );

		return( -1 );
	}
	*signature_table = memory_allocate_structure(
	                    libsigscan_signature_table_t );

	if( *signature_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create signature table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *signature_table,
	     0,
	     sizeof( libsigscan_signature_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear signature table.",
		 function );

		memory_free(
		 *signature_table );

		*signature_table = NULL;

		return( -1 );
	}
	if( libcdata_list_initialize(
	     &( ( *signature_table )->byte_value_groups_list ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create byte values groups list.",
		 function );

		goto on_error;
	}
	( *signature_table )->smallest_offset = -1;
	( *signature_table )->largest_offset  = -1;

	return( 1 );

on_error:
	if( *signature_table != NULL )
	{
		memory_free(
		 *signature_table );

		*signature_table = NULL;
	}
	return( -1 );
}

/* Frees a signature table
 * Returns 1 if successful or -1 on error
 */
int libsigscan_signature_table_free(
     libsigscan_signature_table_t **signature_table,
     libcerror_error_t **error )
{
	static char *function = "libsigscan_signature_table_free";
	int result            = 1;

	if( signature_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid signature table.",
		 function );

		return( -1 );
	}
	if( *signature_table != NULL )
	{
		/* The signatures_array is references and freed somewhere else
		 */
		if( libcdata_list_free(
		     &( ( *signature_table )->byte_value_groups_list ),
		     (int (*)(intptr_t **,libcerror_error_t **)) &libsigscan_byte_value_group_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free byte value groups list.",
			 function );

			result = -1;
		}
		memory_free(
		 *signature_table );

		*signature_table = NULL;
	}
	return( result );
}

/* Fill the signature table
 * Returns 1 if successful or -1 on error
 */
int libsigscan_signature_table_fill(
     libsigscan_signature_table_t *signature_table,
     libcdata_array_t *signatures_array,
     libcerror_error_t **error )
{
	libsigscan_signature_t *signature = NULL;
	static char *function             = "libsigscan_signature_table_fill";
	off64_t pattern_offset            = 0;
	size_t pattern_index              = 0;
	int number_of_signatures          = 0;
	int signature_index               = 0;

	if( signature_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid signature table.",
		 function );

		return( -1 );
	}
	if( signature_table->signatures_array != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid signature table - signatures array already set.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     signatures_array,
	     &number_of_signatures,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of signatures.",
		 function );

		return( -1 );
	}
	for( signature_index = 0;
	     signature_index < number_of_signatures;
	     signature_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     signatures_array,
		     signature_index,
		     (intptr_t **) &signature,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve signature: %d.",
			 function,
			 signature_index );

			return( -1 );
		}
		if( signature == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing signature: %d.",
			 function,
			 signature_index );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: signature: %s, pattern offset: %" PRIi64 ", pattern:\n",
			 function,
			 signature->identifier,
			 signature->pattern_offset );
			libcnotify_print_data(
			 signature->pattern,
			 signature->pattern_size,
			 0 );
		}
#endif
		pattern_offset = signature->pattern_offset;

		if( ( signature_table->smallest_offset == -1 )
		 || ( signature_table->smallest_offset > pattern_offset ) )
		{
			signature_table->smallest_offset = pattern_offset;
			signature_table->smallest_size   = signature->pattern_size;
		}
		if( ( signature_table->largest_offset == -1 )
		 || ( signature_table->largest_offset > pattern_offset ) )
		{
			signature_table->largest_offset = pattern_offset;
			signature_table->largest_size   = signature->pattern_size;
		}
		for( pattern_index = 0;
		     pattern_index < signature->pattern_size;
		     pattern_index++ )
		{
/* TODO check ignore list for pattern offset and skip to next index */
			if( libsigscan_signature_table_insert_signature(
			     signature_table,
			     pattern_offset,
			     signature->pattern[ pattern_index ],
			     signature,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to insert signature into signature table.",
				 function );

				return( -1 );
			}
			pattern_offset++;
		}
	}
	signature_table->signatures_array = signatures_array;

	return( 1 );
}

/* Retrieves the number of byte value groups
 * Returns 1 if successful or -1 on error
 */
int libsigscan_signature_table_get_number_of_byte_value_groups(
     libsigscan_signature_table_t *signature_table,
     int *number_of_byte_value_groups,
     libcerror_error_t **error )
{
	static char *function = "libsigscan_signature_table_get_number_of_byte_value_groups";

	if( signature_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid signature table.",
		 function );

		return( -1 );
	}
	if( libcdata_list_get_number_of_elements(
	     signature_table->byte_value_groups_list,
	     number_of_byte_value_groups,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of byte value groups.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific byte value group
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libsigscan_signature_table_get_byte_value_group_by_index(
     libsigscan_signature_table_t *signature_table,
     int byte_value_group_index,
     libsigscan_byte_value_group_t **byte_value_group,
     libcerror_error_t **error )
{
	static char *function = "libsigscan_signature_table_get_byte_value_group_by_index";

	if( signature_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid signature table.",
		 function );

		return( -1 );
	}
	if( libcdata_list_get_value_by_index(
	     signature_table->byte_value_groups_list,
	     byte_value_group_index,
	     (intptr_t **) byte_value_group,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve byte value group: %d.",
		 function,
		 byte_value_group_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific byte value group by offset
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libsigscan_signature_table_get_byte_value_group_by_offset(
     libsigscan_signature_table_t *signature_table,
     off64_t pattern_offset,
     libsigscan_byte_value_group_t **byte_value_group,
     libcerror_error_t **error )
{
	libcdata_list_element_t *list_element = NULL;
	static char *function                 = "libsigscan_signature_table_get_byte_value_group_by_offset";
	int result                            = 0;

	if( signature_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid signature table.",
		 function );

		return( -1 );
	}
	if( byte_value_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte value group.",
		 function );

		return( -1 );
	}
	if( libcdata_list_get_first_element(
	     signature_table->byte_value_groups_list,
	     &list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve first list element.",
		 function );

		return( -1 );
	}
	while( list_element != NULL )
	{
		if( libcdata_list_element_get_value(
		     list_element,
		     (intptr_t **) byte_value_group,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve byte value group.",
			 function );

			return( -1 );
		}
		if( *byte_value_group == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing byte value group.",
			 function );

			return( -1 );
		}
		if( ( *byte_value_group )->pattern_offset == pattern_offset )
		{
			result = 1;

			break;
		}
		if( libcdata_list_element_get_next_element(
		     list_element,
		     &list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next list element.",
			 function );

			return( -1 );
		}
	}
	if( result == 0 )
	{
		*byte_value_group = NULL;
	}
	return( result );
}

/* Inserts a signature for a specific pattern offset and byte value
 * Returns 1 if successful or -1 on error
 */
int libsigscan_signature_table_insert_signature(
     libsigscan_signature_table_t *signature_table,
     off64_t pattern_offset,
     uint8_t byte_value,
     libsigscan_signature_t *signature,
     libcerror_error_t **error )
{
	libsigscan_byte_value_group_t *byte_value_group = NULL;
	static char *function                           = "libsigscan_signature_table_insert_signature";
	int result                                      = 0;

	if( signature_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid signature table.",
		 function );

		return( -1 );
	}
	result = libsigscan_signature_table_get_byte_value_group_by_offset(
	          signature_table,
	          pattern_offset,
	          &byte_value_group,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve byte value group for pattern offset: %" PRIi64 ".",
		 function,
		 pattern_offset );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( libsigscan_byte_value_group_initialize(
		     &byte_value_group,
		     pattern_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create byte value group for pattern offset: %" PRIi64 ".",
			 function,
			 pattern_offset );

			return( -1 );
		}
		if( libcdata_list_insert_value(
		     signature_table->byte_value_groups_list,
		     (intptr_t *) byte_value_group,
		     (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libsigscan_byte_value_group_compare,
		     LIBCDATA_INSERT_FLAG_UNIQUE_ENTRIES,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to insert byte value group for pattern offset: %" PRIi64 " into byte value groups list.",
			 function,
			 pattern_offset );

			libsigscan_byte_value_group_free(
			 &byte_value_group,
			 NULL );

			return( -1 );
		}
	}
	if( libsigscan_byte_value_group_insert_signature(
	     byte_value_group,
	     byte_value,
	     signature,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert signature into byte value group for pattern offset: %" PRIi64 ".",
		 function,
		 pattern_offset );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of signatures
 * Returns 1 if successful or -1 on error
 */
int libsigscan_signature_table_get_number_of_signatures(
     libsigscan_signature_table_t *signature_table,
     int *number_of_signatures,
     libcerror_error_t **error )
{
	static char *function = "libsigscan_signature_table_get_number_of_signatures";

	if( signature_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid signature table.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     signature_table->signatures_array,
	     number_of_signatures,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of signatures.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific signature
 * Returns 1 if successful or -1 on error
 */
int libsigscan_signature_table_get_signature_by_index(
     libsigscan_signature_table_t *signature_table,
     int signature_index,
     libsigscan_signature_t **signature,
     libcerror_error_t **error )
{
	static char *function = "libsigscan_signature_table_get_signature_by_index";

	if( signature_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid signature table.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     signature_table->signatures_array,
	     signature_index,
	     (intptr_t **) signature,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve signature: %d.",
		 function,
		 signature_index );

		return( -1 );
	}
	return( 1 );
}


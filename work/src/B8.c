/*
 * Copyright (c) 1985, J Howard Johnson, University of Waterloo.
 * Copyright (c) 2022, J Howard Johnson
 *
 * This software was developed while I was a student and, later, professor
 * at the University of Waterloo.  It has only minimal enhancements and bug
 * fixes from later than August 1988.  It was released under the GPLv3
 * licence on July 26, 2010.
 *                 -- J Howard Johnson ( j.howard.johnson *at* gmail.com )
 *
 * This file is part of INR.
 *
 *   INR is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   INR is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with INR.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "O.h"

/*
 * B8 translates from token form to octets
 * B8i translates from octet form to tokens (far harder)
 */

B8_OBJECT B8_create()
{
    B8_OBJECT B8;

    B8 = (B8_OBJECT) Salloc( sizeof(struct B8_desc) );
    B8-> Type    = B8_Object;
    B8-> B8_from = -1;
    B8-> B8_input = -1;
    B8-> B8_output = s_alloc( 2 );
    B8-> B8_output[ 0 ] = MAXSHORT;
    B8-> B8_output[ 1 ] = MAXSHORT;
    B8-> B8_to = -1;

    return( B8 );
}

void B8_destroy( B8_OBJECT B8 )
{
    if ( B8 != NULL ) {
        Sfree( (char *) B8-> B8_output );
    }
    Sfree( (char *) B8 );
}

B8_OBJECT B8_set_trans( B8_OBJECT B8,
    SHORT from, SHORT symb, T2_OBJECT T2_Sigma )
{
    char *in_str;
    int in_str_len, out_str_len, k, l, c;
    SHORT *out_str;

    B8-> B8_from = from;
    B8-> B8_input = symb;

    if ( from == START && symb == 1 ) {
        B8-> B8_output[ 0 ] = symb;
        B8-> B8_output[ 1 ] = MAXSHORT;
        B8-> B8_to = FINAL;
    } else {
        in_str = T2_name( T2_Sigma, symb );
        in_str_len = T2_length( T2_Sigma, symb );

        out_str_len = Ssize( (char *) B8-> B8_output ) / sizeof( SHORT );
        if ( out_str_len < in_str_len + 2 ) {
            B8-> B8_output = (SHORT *)
                Srealloc( (char *) B8-> B8_output,
                    2 * ( in_str_len + 2 ) * sizeof( SHORT ) );
        }
        out_str = B8-> B8_output;

        l = 0;
        for ( k = 0; k < in_str_len; ++k ) {
            c = in_str[ k ];
            out_str[ l++ ] = c + 2;
        }
        out_str[ l++ ] = 258;
        out_str[ l++ ] = MAXSHORT;
    }
    return( B8 );
}

void B8_print_trans( B8_OBJECT B8, T2_OBJECT T2_Sigma )
{
    int i;

    printf( "%d\t", B8-> B8_from );
    printf( "%s\t[ ", T2_name_pr( T2_Sigma, B8-> B8_input ) );
    for ( i = 0; B8-> B8_output[ i ] < MAXSHORT; ++i ) {
      printf( "%s ", T2_name_pr( T2_Sigma, B8-> B8_output[ i ] ) );
    }
    printf( "]\t%d\n", B8-> B8_to );
}

/* =============================================================== */

B8i_OBJECT B8i_create()
{
    B8i_OBJECT B8i;
    Tn_OBJECT Tn;
    int result;

    B8i = (B8i_OBJECT) Salloc( sizeof(struct B8i_desc) );
    B8i-> Type    = B8i_Object;
    B8i-> B8i_from = -1;
    B8i-> B8i_input = -1;
    B8i-> B8i_output = s_alloc( 2 );
    B8i-> B8i_output[ 0 ] = MAXSHORT;
    B8i-> B8i_output[ 1 ] = MAXSHORT;
    B8i-> B8i_to = -1;
    B8i-> B8i_ptok = Tn = Tn_create();
    B8i-> B8i_ts = Salloc( 100 );

    result = Tn_insert( Tn, "", 0 );
    assert( result == 0 );

    return( B8i );
}

void B8i_destroy( B8i_OBJECT B8i )
{
    if ( B8i != NULL ) {
        Sfree( (char *) B8i-> B8i_output );
        Tn_destroy( B8i-> B8i_ptok );
        Sfree( B8i-> B8i_ts );
    }
    Sfree( (char *) B8i );
}

B8i_OBJECT B8i_set_trans( B8i_OBJECT B8i,
    SHORT from, SHORT symb, T2_OBJECT T2_Sigma )
{
    Tn_OBJECT Tn;
    int octet = 257;
    char *cstr_from, *ts;
    int leng_from, i;

    B8i-> B8i_from = from;
    B8i-> B8i_input = symb;

    if ( from == START && symb == 1 ) {
        B8i-> B8i_output[ 0 ] = symb;
        B8i-> B8i_output[ 1 ] = MAXSHORT;
        B8i-> B8i_to = FINAL;
    } else {
        if ( symb >= 2 && symb <= 258 ) {
            octet = symb - 2;
        }

        if ( octet <= 255  ) {
            Tn = B8i-> B8i_ptok;
            cstr_from = Tn_name( Tn, from );
            leng_from = Tn_length( Tn, from );
            if ( Ssize( B8i-> B8i_ts ) < leng_from + 1 ) {
                B8i-> B8i_ts = Srealloc( B8i-> B8i_ts, leng_from + 1 );
            }
            ts = B8i-> B8i_ts;
            for ( i = 0; i < leng_from; ++i ) {
                ts[ i ] = cstr_from[ i ];
            }
            i = leng_from;
            ts[ i ] = '\0';
            B8i-> B8i_output[ 0 ] = T2_insert( T2_Sigma, ts, i );
            B8i-> B8i_output[ 1 ] = MAXSHORT;
            B8i-> B8i_to = 0;
        } else {
            B8i-> B8i_to = MAXSHORT;
            B8i-> B8i_output[ 0 ] = MAXSHORT;
        }
    }
    return( B8i );
}

void B8i_print_trans( B8i_OBJECT B8i, T2_OBJECT T2_Sigma )
{
    int i;

    printf( "%d\t", B8i-> B8i_from );
    printf( "%s\t[ ", T2_name_pr( T2_Sigma, B8i-> B8i_input ) );
    for ( i = 0; B8i-> B8i_output[ i ] < MAXSHORT; ++i ) {
      printf( "%s ", T2_name_pr( T2_Sigma, B8i-> B8i_output[ i ] ) );
    }
    printf( "]\t%d\n", B8i-> B8i_to );
}

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

B4_OBJECT B4_create()
{
    B4_OBJECT B4;
    Tn_OBJECT Tn;
    int result;

    B4 = (B4_OBJECT) Salloc( sizeof(struct B4_desc) );
    B4-> Type    = B4_Object;
    B4-> B4_from = -1;
    B4-> B4_input = -1;
    B4-> B4_output = s_alloc( 2 );
    B4-> B4_output[ 0 ] = MAXSHORT;
    B4-> B4_output[ 1 ] = MAXSHORT;
    B4-> B4_to = -1;
    B4-> B4_ptok = Tn = Tn_create();
    B4-> B4_ts = Salloc( 100 );

    result = Tn_insert( Tn, "", 0 );
    assert( result == 0 );

    return( B4 );
}

void B4_destroy( B4_OBJECT B4 )
{
    if ( B4 != NULL ) {
        Sfree( (char *) B4-> B4_output );
        Tn_destroy( B4-> B4_ptok );
        Sfree( B4-> B4_ts );
    }
    Sfree( (char *) B4 );
}

B4_OBJECT B4_set_trans( B4_OBJECT B4,
    SHORT from, SHORT symb, T2_OBJECT T2_Sigma )
{
    Tn_OBJECT Tn;
    int nibble = 18;
    char *cstr_from, *ts;
    int leng_from, i, k;

    B4-> B4_from = from;
    B4-> B4_input = symb;

    if ( from == START && symb == 1 ) {
        B4-> B4_output[ 0 ] = symb;
        B4-> B4_output[ 1 ] = MAXSHORT;
        B4-> B4_to = FINAL;
    } else {
        if ( symb >= 50 && symb <= 59 ) {
            nibble = symb - 50;
        } else if ( symb >= 67 && symb <= 72 ) {
            nibble = symb - 57;
        } else if ( symb == 97 ) {
            nibble = 17;
        }

        if ( nibble <= 16 ) {
            Tn = B4-> B4_ptok;
            cstr_from = Tn_name( Tn, from );
            leng_from = Tn_length( Tn, from );
            if ( Ssize( B4-> B4_ts ) < leng_from + 2 ) {
                B4-> B4_ts = Srealloc( B4-> B4_ts, leng_from + 2 );
            }
            ts = B4-> B4_ts;

            for ( i = 0; i < leng_from; ++i ) {
                ts[ i ] = cstr_from[ i ];
            }
            ts[ leng_from ] = nibble;
            ts[ leng_from + 1 ] = '\0';
            B4-> B4_to = Tn_insert( Tn, ts, leng_from + 1 );
            B4-> B4_output[ 0 ] = MAXSHORT;
        } else if ( nibble == 17 ) {
            Tn = B4-> B4_ptok;
            cstr_from = Tn_name( Tn, from );
            leng_from = Tn_length( Tn, from );
            if ( leng_from % 2 != 0 ) { Error( "Parity error in B4" ); }
            if ( Ssize( B4-> B4_ts ) < leng_from / 2 + 1 ) {
                B4-> B4_ts = Srealloc( B4-> B4_ts, leng_from / 2 + 1 );
            }
            ts = B4-> B4_ts;
            for ( i = 0; i < leng_from; i += 2 ) {
                k = i / 2;
                ts[ k ] = ( cstr_from[ k ] << 4 ) + cstr_from[ k + 1 ];
            }
            k = leng_from / 2;
            ts[ k ] = '\0';
            B4-> B4_output[ 0 ] = T2_insert( T2_Sigma, ts, k );
            B4-> B4_output[ 1 ] = MAXSHORT;
            B4-> B4_to = 0;
        } else {
            B4-> B4_to = MAXSHORT;
            B4-> B4_output[ 0 ] = MAXSHORT;
        }
    }
    return( B4 );
}

void B4_print_trans( B4_OBJECT B4, T2_OBJECT T2_Sigma )
{
/*
    Tn_OBJECT Tn;
    int nibble = 18;
    char *cstr_from, *ts;
    int leng_from, i, k;
*/
    int i;

    printf( "%d\t", B4-> B4_from );
    printf( "%s\t[ ", T2_name_pr( T2_Sigma, B4-> B4_input ) );
    for ( i = 0; B4-> B4_output[ i ] < MAXSHORT; ++i ) {
      printf( "%s ", T2_name_pr( T2_Sigma, B4-> B4_output[ i ] ) );
    }
    printf( "]\t%d\n", B4-> B4_to );
}

/* =============================================================== */

B4i_OBJECT B4i_create()
{
    B4i_OBJECT B4i;
    Tn_OBJECT Tn;
    int result;

    B4i = (B4i_OBJECT) Salloc( sizeof(struct B4i_desc) );
    B4i-> Type    = B4i_Object;
    B4i-> B4i_from = -1;
    B4i-> B4i_input = -1;
    B4i-> B4i_output = s_alloc( 2 );
    B4i-> B4i_output[ 0 ] = MAXSHORT;
    B4i-> B4i_output[ 1 ] = MAXSHORT;
    B4i-> B4i_to = -1;
    B4i-> B4i_ptok = Tn = Tn_create();
    B4i-> B4i_ts = Salloc( 100 );

    result = Tn_insert( Tn, "", 0 );
    assert( result == 0 );

    return( B4i );
}

void B4i_destroy( B4i_OBJECT B4i )
{
    if ( B4i != NULL ) {
        Sfree( (char *) B4i-> B4i_output );
        Tn_destroy( B4i-> B4i_ptok );
        Sfree( B4i-> B4i_ts );
    }
    Sfree( (char *) B4i );
}

B4i_OBJECT B4i_set_trans( B4i_OBJECT B4i,
    SHORT from, SHORT symb, T2_OBJECT T2_Sigma )
{
    Tn_OBJECT Tn;
    int nibble = 18;
    char *cstr_from, *ts;
    int leng_from, i, k;

    B4i-> B4i_from = from;
    B4i-> B4i_input = symb;

    if ( from == START && symb == 1 ) {
        B4i-> B4i_output[ 0 ] = symb;
        B4i-> B4i_output[ 1 ] = MAXSHORT;
        B4i-> B4i_to = FINAL;
    } else {
        if ( symb >= 50 && symb <= 59 ) {
            nibble = symb - 50;
        } else if ( symb >= 67 && symb <= 72 ) {
            nibble = symb - 57;
        } else if ( symb == 97 ) {
            nibble = 17;
        }

        if ( nibble <= 16 ) {
            Tn = B4i-> B4i_ptok;
            cstr_from = Tn_name( Tn, from );
            leng_from = Tn_length( Tn, from );
            if ( Ssize( B4i-> B4i_ts ) < leng_from + 2 ) {
                B4i-> B4i_ts = Srealloc( B4i-> B4i_ts, leng_from + 2 );
            }
            ts = B4i-> B4i_ts;

            for ( i = 0; i < leng_from; ++i ) {
                ts[ i ] = cstr_from[ i ];
            }
            ts[ leng_from ] = nibble;
            ts[ leng_from + 1 ] = '\0';
            B4i-> B4i_to = Tn_insert( Tn, ts, leng_from + 1 );
            B4i-> B4i_output[ 0 ] = MAXSHORT;
        } else if ( nibble == 17 ) {
            Tn = B4i-> B4i_ptok;
            cstr_from = Tn_name( Tn, from );
            leng_from = Tn_length( Tn, from );
            if ( leng_from % 2 != 0 ) { Error( "Parity error in B4i" ); }
            if ( Ssize( B4i-> B4i_ts ) < leng_from / 2 + 1 ) {
                B4i-> B4i_ts = Srealloc( B4i-> B4i_ts, leng_from / 2 + 1 );
            }
            ts = B4i-> B4i_ts;
            for ( i = 0; i < leng_from; i += 2 ) {
                k = i / 2;
                ts[ k ] = ( cstr_from[ k ] << 4 ) + cstr_from[ k + 1 ];
            }
            k = leng_from / 2;
            ts[ k ] = '\0';
            B4i-> B4i_output[ 0 ] = T2_insert( T2_Sigma, ts, k );
            B4i-> B4i_output[ 1 ] = MAXSHORT;
            B4i-> B4i_to = 0;
        } else {
            B4i-> B4i_to = MAXSHORT;
            B4i-> B4i_output[ 0 ] = MAXSHORT;
        }
    }
    return( B4i );
}

void B4i_print_trans( B4i_OBJECT B4i, T2_OBJECT T2_Sigma )
{
/*
    Tn_OBJECT Tn;
    int nibble = 18;
    char *cstr_from, *ts;
    int leng_from, i, k;
*/
    int i;

    printf( "%d\t", B4i-> B4i_from );
    printf( "%s\t[ ", T2_name_pr( T2_Sigma, B4i-> B4i_input ) );
    for ( i = 0; B4i-> B4i_output[ i ] < MAXSHORT; ++i ) {
      printf( "%s ", T2_name_pr( T2_Sigma, B4i-> B4i_output[ i ] ) );
    }
    printf( "]\t%d\n", B4i-> B4i_to );
}

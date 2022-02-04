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

A_OBJECT A_slurp_octets( char *file, T_OBJECT T_Sigma )
{
    A_OBJECT A;
    FILE *fp;
    int c;
    SHORT state, next_state;

    if ( file != NULL ) fp = fopen( file, "r" );
    if ( fp == NULL ) {
        Warning( "File does not exist" );
        return( NULL );
    }

    assert( T_Sigma != NULL );
    assert( T_Sigma-> T_n >= 258 );

    A = A_create();
    c = getc( fp );
    state = 0;
    next_state = 2;
    while ( c != EOF ) {
        assert( next_state <= MAXSHORT );
        A = A_add( A, state, c + 2, next_state );
        state = next_state;
        ++next_state;
        c = getc( fp );
    }
    A = A_add( A, state, 1, 1 );
    A = A_close( A );
    fclose( fp );
    return( A );
}

A_OBJECT A_slurp_nibbles( char *file, T_OBJECT T_Sigma )
{
    A_OBJECT A;
    FILE *fp;
    int c, i, hi, lo;
    SHORT state, state1, state2;
    char hex_digits[] = "0123456789abcdef";
    int digit_hi[ 16 ];
    int digit_lo[ 16 ];
    char ts[ 3 ];

    if ( file != NULL ) fp = fopen( file, "r" );
    if ( fp == NULL ) {
        Warning( "File does not exist" );
        return( NULL );
    }

    assert( T_Sigma != NULL );

    for ( i = 0; i < 16; ++i ) {
        ts[ 0 ] = hex_digits[ i ];
        ts[ 1 ] = hex_digits[ 0 ];
        ts[ 2 ] = 0;
        digit_hi[ i ] = T_insert( T_Sigma, ts );
        ts[ 0 ] = hex_digits[ i ];
        ts[ 1 ] = 0;
        digit_lo[ i ] = T_insert( T_Sigma, ts );
    }

    A = A_create();
    c = getc( fp );
    state = 0;
    state1 = 2;
    state2 = 3;
    while ( c != EOF ) {
        assert( state2 <= MAXSHORT );
        hi = digit_hi[ c >> 4 ];
        lo = digit_lo[ c & 0xf ];
        A = A_add( A, state, hi, state1 );
        A = A_add( A, state1, lo, state2 );
        state = state2;
        state1 = state + 1;
        state2 = state + 2;
        c = getc( fp );
    }
    A = A_add( A, state, 1, 1 );
    A = A_close( A );
    fclose( fp );
    return( A );
}

A_OBJECT A_slurp_utf8( char *file, T_OBJECT T_Sigma )
{
    A_OBJECT A;
    FILE *fp;
    int c, c1, c2, c3;
    SHORT state, next_state;
    int errors = 0;
    char *nm;
    int nm_len;
    char ts[ 10 ];

    if ( file != NULL ) fp = fopen( file, "r" );
    if ( fp == NULL ) {
        Warning( "File does not exist" );
        return( NULL );
    }

    assert( T_Sigma != NULL );
    assert( T_Sigma-> T_n >= 258 );

    A = A_create();
    c = getc( fp );
    state = 0;
    next_state = 2;
    while ( c != EOF ) {
        assert( next_state <= MAXSHORT );
        if ( ( c & 0x80 ) == 0x00 ) {
            A = A_add( A, state, c + 2, next_state );
            c = getc( fp );
        }
        if ( ( c & 0xe0 ) == 0xc0 ) {
            c1 = getc( fp );
            if ( c1 == EOF || ( c1 & 0xc0 ) != 0x80 ) {
                A = A_add( A, state, c + 2, next_state );
                c = c1;
                ++errors;
            } else {
                nm = T_name( TT, c + 2 );
                nm_len = strlen( nm );
                assert( nm_len == 2 );
                strncpy( ts, T_name( TT, c + 2 ), 2 );
                nm = T_name( TT, c1 + 2 );
                nm_len = strlen( nm );
                assert( nm_len == 2 );
                strncpy( ts + 2, T_name( TT, c1 + 2 ), 2 );
                ts[ 4 ] = '\0';
                A = A_add( A, state, T_insert( TT, ts ), next_state );
                c = getc( fp );
            }
        }
        if ( ( c & 0xf0 ) == 0xe0 ) {
            c1 = getc( fp );
            if ( c1 == EOF || ( c1 & 0xc0 ) != 0x80 ) {
                A = A_add( A, state, c + 2, next_state );
                c = c1;
                ++errors;
            } else if ( c1 != EOF ) {
                c2 = getc( fp );
                if ( c2 == EOF || ( c2 & 0xc0 ) != 0x80 ) {
                    A = A_add( A, state, c + 2, next_state );
                    state = next_state;
                    ++next_state;
                    assert( next_state <= MAXSHORT );
                    A = A_add( A, state, c1 + 2, next_state );
                    c = c2;
                    ++errors;
                } else {
                    nm = T_name( TT, c + 2 );
                    nm_len = strlen( nm );
                    assert( nm_len == 2 );
                    strncpy( ts, T_name( TT, c + 2 ), 2 );
                    nm = T_name( TT, c1 + 2 );
                    nm_len = strlen( nm );
                    assert( nm_len == 2 );
                    strncpy( ts + 2, T_name( TT, c1 + 2 ), 2 );
                    nm = T_name( TT, c2 + 2 );
                    nm_len = strlen( nm );
                    assert( nm_len == 2 );
                    strncpy( ts + 2, T_name( TT, c2 + 2 ), 2 );
                    ts[ 6 ] = '\0';
                    A = A_add( A, state, T_insert( TT, ts ), next_state );
                    c = getc( fp );
                }
            }
        }
        if ( ( c & 0xf8 ) == 0xf0 ) {
            c1 = getc( fp );
            if ( c1 == EOF || ( c1 & 0xc0 ) != 0x80 ) {
                A = A_add( A, state, c + 2, next_state );
                c = c1;
                ++errors;
            } else if ( c1 != EOF ) {
                c2 = getc( fp );
                if ( c2 == EOF || ( c2 & 0xc0 ) != 0x80 ) {
                    A = A_add( A, state, c + 2, next_state );
                    state = next_state;
                    ++next_state;
                    A = A_add( A, state, c1 + 2, next_state );
                    c = c2;
                    ++errors;
                } else if ( c2 != EOF ) {
                    c3 = getc( fp );
                    if ( c3 == EOF || ( c3 & 0xc0 ) != 0x80 ) {
                        A = A_add( A, state, c + 2, next_state );
                        state = next_state;
                        ++next_state;
                        assert( next_state <= MAXSHORT );
                        A = A_add( A, state, c1+ 2, next_state );
                        state = next_state;
                        ++next_state;
                        assert( next_state <= MAXSHORT );
                        A = A_add( A, state, c2 + 2, next_state );
                        c = c2;
                        ++errors;
                    } else {
                        nm = T_name( TT, c + 2 );
                        nm_len = strlen( nm );
                        assert( nm_len == 2 );
                        strncpy( ts, T_name( TT, c + 2 ), 2 );
                        nm = T_name( TT, c1 + 2 );
                        nm_len = strlen( nm );
                        assert( nm_len == 2 );
                        strncpy( ts + 2, T_name( TT, c1 + 2 ), 2 );
                        nm = T_name( TT, c2 + 2 );
                        nm_len = strlen( nm );
                        assert( nm_len == 2 );
                        strncpy( ts + 4, T_name( TT, c2 + 2 ), 2 );
                        nm = T_name( TT, c3 + 2 );
                        nm_len = strlen( nm );
                        assert( nm_len == 2 );
                        strncpy( ts + 6, T_name( TT, c3 + 2 ), 2 );
                        ts[ 8 ] = '\0';
                        A = A_add( A, state, T_insert( TT, ts ), next_state );
                        c = getc( fp );
                    }
                }
            }
        }
        state = next_state;
        ++next_state;
    }
    A = A_add( A, state, 1, 1 );
    A = A_close( A );
    fclose( fp );
    return( A );
}

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
        state = next_state++;
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

A_OBJECT A_slurp_utf8_alt( char *file, T_OBJECT T_Sigma )
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
        else if ( ( c & 0xe0 ) == 0xc0 ) {
            c1 = getc( fp );
            if ( c1 == EOF || ( c1 & 0xc0 ) != 0x80 ) {
                A = A_add( A, state, c + 2, next_state );
                c = c1;
                ++errors;
            } else {
                nm = T_name( T_Sigma, c + 2 );
                nm_len = strlen( nm );
                assert( nm_len == 2 );
                strncpy( ts, T_name( T_Sigma, c + 2 ), 2 );
                nm = T_name( T_Sigma, c1 + 2 );
                nm_len = strlen( nm );
                assert( nm_len == 2 );
                strncpy( ts + 2, T_name( T_Sigma, c1 + 2 ), 2 );
                ts[ 4 ] = '\0';
                A = A_add( A, state, T_insert( T_Sigma, ts ), next_state );
                c = getc( fp );
            }
        }
        else if ( ( c & 0xf0 ) == 0xe0 ) {
            c1 = getc( fp );
            if ( c1 == EOF || ( c1 & 0xc0 ) != 0x80 ) {
                A = A_add( A, state, c + 2, next_state );
                c = c1;
                ++errors;
            } else if ( c1 != EOF ) {
                c2 = getc( fp );
                if ( c2 == EOF || ( c2 & 0xc0 ) != 0x80 ) {
                    A = A_add( A, state, c + 2, next_state );
                    state = next_state++;
                    assert( next_state <= MAXSHORT );
                    A = A_add( A, state, c1 + 2, next_state );
                    c = c2;
                    ++errors;
                } else {
                    nm = T_name( T_Sigma, c + 2 );
                    nm_len = strlen( nm );
                    assert( nm_len == 2 );
                    strncpy( ts, T_name( T_Sigma, c + 2 ), 2 );
                    nm = T_name( T_Sigma, c1 + 2 );
                    nm_len = strlen( nm );
                    assert( nm_len == 2 );
                    strncpy( ts + 2, T_name( T_Sigma, c1 + 2 ), 2 );
                    nm = T_name( T_Sigma, c2 + 2 );
                    nm_len = strlen( nm );
                    assert( nm_len == 2 );
                    strncpy( ts + 2, T_name( T_Sigma, c2 + 2 ), 2 );
                    ts[ 6 ] = '\0';
                    A = A_add( A, state, T_insert( T_Sigma, ts ), next_state );
                    c = getc( fp );
                }
            }
        }
        else if ( ( c & 0xf8 ) == 0xf0 ) {
            c1 = getc( fp );
            if ( c1 == EOF || ( c1 & 0xc0 ) != 0x80 ) {
                A = A_add( A, state, c + 2, next_state );
                c = c1;
                ++errors;
            } else if ( c1 != EOF ) {
                c2 = getc( fp );
                if ( c2 == EOF || ( c2 & 0xc0 ) != 0x80 ) {
                    A = A_add( A, state, c + 2, next_state );
                    state = next_state++;
                    A = A_add( A, state, c1 + 2, next_state );
                    c = c2;
                    ++errors;
                } else if ( c2 != EOF ) {
                    c3 = getc( fp );
                    if ( c3 == EOF || ( c3 & 0xc0 ) != 0x80 ) {
                        A = A_add( A, state, c + 2, next_state );
                        state = next_state++;
                        assert( next_state <= MAXSHORT );
                        A = A_add( A, state, c1+ 2, next_state );
                        state = next_state++;
                        assert( next_state <= MAXSHORT );
                        A = A_add( A, state, c2 + 2, next_state );
                        c = c2;
                        ++errors;
                    } else {
                        nm = T_name( T_Sigma, c + 2 );
                        nm_len = strlen( nm );
                        assert( nm_len == 2 );
                        strncpy( ts, T_name( T_Sigma, c + 2 ), 2 );
                        nm = T_name( T_Sigma, c1 + 2 );
                        nm_len = strlen( nm );
                        assert( nm_len == 2 );
                        strncpy( ts + 2, T_name( T_Sigma, c1 + 2 ), 2 );
                        nm = T_name( T_Sigma, c2 + 2 );
                        nm_len = strlen( nm );
                        assert( nm_len == 2 );
                        strncpy( ts + 4, T_name( T_Sigma, c2 + 2 ), 2 );
                        nm = T_name( T_Sigma, c3 + 2 );
                        nm_len = strlen( nm );
                        assert( nm_len == 2 );
                        strncpy( ts + 6, T_name( T_Sigma, c3 + 2 ), 2 );
                        ts[ 8 ] = '\0';
                        A = A_add( A, state,
                            T_insert( T_Sigma, ts ), next_state );
                        c = getc( fp );
                    }
                }
            }
        } else {
            A = A_add( A, state, c + 2, next_state );
            c = getc( fp );
            ++errors;
        }
        state = next_state++;
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
    int i, c1, c2, c3, c4, type;
    char ts[ 5 ];

    if ( file != NULL ) fp = fopen( file, "r" );
    if ( fp == NULL ) {
        Warning( "File does not exist" );
        return( NULL );
    }

    assert( T_Sigma != NULL );
    assert( T_Sigma-> T_n >= 258 );

    A = A_slurp_octets( file, T_Sigma );
    A = A_open( A );

    for ( i = 0; i < A-> A_nrows; ++i ) {
        c1 = A-> A_t[ i ].A_b - 2;
             if ( ( c1 & 0x80 ) == 0x00 ) { type = 0; }
        else if ( ( c1 & 0xc0 ) == 0x80 ) { type = 1; }
        else if ( ( c1 & 0xe0 ) == 0xc0 ) { type = 2; }
        else if ( ( c1 & 0xf0 ) == 0xe0 ) { type = 3; }
        else if ( ( c1 & 0xf8 ) == 0xf0 ) { type = 4; }
        else { type = 5; }

        switch ( type ) {

            case 2:
                c2 = A-> A_t[ i + 1 ].A_b - 2;
                if ( ( c2 & 0xc0 ) == 0x80 ) {
                    ts[ 0 ] = c1;
                    ts[ 1 ] = c2;
                    ts[ 2 ] = '\0';
                }
                A-> A_t[ i ].A_b = T_insert( T_Sigma, ts );
                A-> A_t[ i ].A_c += 1;
                break;

            case 3:
                c2 = A-> A_t[ i + 1 ].A_b - 2;
                c3 = A-> A_t[ i + 2 ].A_b - 2;
                if (    ( c2 & 0xc0 ) == 0x80
                     && ( c3 & 0xc0 ) == 0x80 ) {
                    ts[ 0 ] = c1;
                    ts[ 1 ] = c2;
                    ts[ 2 ] = c3;
                    ts[ 3 ] = '\0';
                }
                A-> A_t[ i ].A_b = T_insert( T_Sigma, ts );
                A-> A_t[ i ].A_c += 2;
                break;

            case 4:
                c2 = A-> A_t[ i + 1 ].A_b - 2;
                c3 = A-> A_t[ i + 2 ].A_b - 2;
                c4 = A-> A_t[ i + 3 ].A_b - 2;
                if (    ( c2 & 0xc0 ) == 0x80
                     && ( c3 & 0xc0 ) == 0x80
                     && ( c4 & 0xc0 ) == 0x80 ) {
                    ts[ 0 ] = c1;
                    ts[ 1 ] = c2;
                    ts[ 2 ] = c3;
                    ts[ 3 ] = c4;
                    ts[ 4 ] = '\0';
                }
                A-> A_t[ i ].A_b = T_insert( T_Sigma, ts );
                A-> A_t[ i ].A_c += 3;
                break;

            case 0: case 1: case 5:
                break;
        }
    }

    if ( T_Sigma-> T_n > A-> A_nS ) {
        A-> A_nS = T_Sigma-> T_n;
    }
    A = A_min( A );
    fclose( fp );
    return( A );
}

A_OBJECT A_spit_utf8( A_OBJECT A, char *file, T_OBJECT T_Sigma )
{
    FILE *fp;
    int i, s1, c1, type;

    if ( file != NULL ) fp = fopen( file, "w" );
    if ( fp == NULL ) {
        Warning( "Can't open file for write" );
        return( NULL );
    }

    assert( T_Sigma != NULL );
    assert( T_Sigma-> T_n >= 258 );

    A = A_min( A );

    for ( i = 0; i < A-> A_nrows; ++i ) {
        s1 = A-> A_t[ i ].A_b;
        if ( s1 < 2 ) {
        } else if ( s1 < 258 ) {
            c1 = s1 - 2;
                 if ( ( c1 & 0x80 ) == 0x00 ) { type = 0; }
            else if ( ( c1 & 0xc0 ) == 0x80 ) { type = 1; }
            else if ( ( c1 & 0xe0 ) == 0xc0 ) { type = 2; }
            else if ( ( c1 & 0xf0 ) == 0xe0 ) { type = 3; }
            else if ( ( c1 & 0xf8 ) == 0xf0 ) { type = 4; }
            else { type = 5; }

            switch ( type ) {
                case 0: case 1: case 2: case 3: case 4: case 5:
                    fputc( c1, fp );
                    break;
            }
        } else {
            fputs( T_name( T_Sigma, s1 ), fp );
        }
    }
    fclose( fp );
    return( A );
}

A_OBJECT A_utf8_nibble_map( char *arg, T_OBJECT T_Sigma )
{
    A_OBJECT A;
    int i, j, c, hi, lo, state, len;
    char *s;
    char hex_digits[] = "0123456789abcdef";
    int digit_hi[ 16 ];
    int digit_lo[ 16 ];
    char ts[ 3 ];

    for ( i = 0; i < 16; ++i ) {
        ts[ 0 ] = hex_digits[ i ];
        ts[ 1 ] = hex_digits[ 0 ];
        ts[ 2 ] = 0;
        digit_hi[ i ] = T_insert( T_Sigma, ts );
        ts[ 0 ] = hex_digits[ i ];
        ts[ 1 ] = 0;
        digit_lo[ i ] = T_insert( T_Sigma, ts );
    }

    assert( T_Sigma != NULL );
    assert( T_Sigma-> T_n >= 258 );

    A = A_create();
    A-> A_nT = 2;

    state = 2;
    for ( i = 258; i < T_Sigma-> T_n; ++i ) {
        s = T_name( T_Sigma, i );
        A = A_add( A, 0, i * 2, state );
        len = strlen( s );
        for ( j = 0; j < len; ++j ) {
            c = s[ j ] & 0xff;
            hi = digit_hi[ c >> 4 ]  * 2 + 1;
            lo = digit_lo[ c & 0xf ] * 2 + 1;
            A = A_add( A, state, hi, state + 1 );
            A = A_add( A, state + 1, lo, state + 2 );
            state += 2;
        }
        A = A_add( A, state, 1, 1 );
    }

    A = A_min( A );
    return( A );
}

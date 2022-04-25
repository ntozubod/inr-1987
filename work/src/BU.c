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
 * BU forms valid UTF-8 code groups separated by '__'
 */

BU_OBJECT BU_create()
{
    BU_OBJECT BU;
    Tn_OBJECT Tn;
    int result;

    BU = (BU_OBJECT) Salloc( sizeof(struct BU_desc) );
    BU-> Type    = BU_Object;
    BU-> BU_from = -1;
    BU-> BU_input = -1;
    BU-> BU_output = s_alloc( 10 );
    BU-> BU_output[ 0 ] = MAXSHORT;
    BU-> BU_output[ 1 ] = MAXSHORT;
    BU-> BU_to = -1;
    BU-> BU_ptok = Tn = Tn_create();

    result = Tn_insert( Tn, "", 0 );
    assert( result == 0 );

    return( BU );
}

void BU_destroy( BU_OBJECT BU )
{
    if ( BU != NULL ) {
        Sfree( (char *) BU-> BU_output );
        Tn_destroy( BU-> BU_ptok );
    }
    Sfree( (char *) BU );
}

BU_OBJECT BU_set_trans( BU_OBJECT BU,
    SHORT from, SHORT symb, T2_OBJECT T2_Sigma )
{
    Tn_OBJECT Tn;
    int octet = 257;
    char *cstr_from, ts[ 10 ];
    int leng_from, i, c1, type;

    BU-> BU_from = from;
    BU-> BU_input = symb;

    if ( from == START && symb == 1 ) {
        BU-> BU_output[ 0 ] = symb;
        BU-> BU_output[ 1 ] = MAXSHORT;
        BU-> BU_to = FINAL;
    } else {
        octet = MAXSHORT:
        if ( symb >= 2 && symb <= 257 ) {
            octet = symb - 2;
        }

        if ( octet <= 0x7F  ) {
            /* one-octet case (7-bit ASCII) */

            if ( from == 0 ) {
                /* easy one-octet case -- just copy */

                BU-> BU_output[ 0 ] = symb;
                BU-> BU_output[ 1 ] = MAXSHORT;
                BU-> BU_to = 0;

            } else {
                /* have a UTF-8 prefix -- dump it, copy octet */

                Tn = BU-> BU_ptok;
                cstr_from = Tn_name( Tn, from );
                leng_from = Tn_length( Tn, from );
                for ( i = 0; i < leng_from; ++i ) {
                    BU-> BU_output[ i ] = cstr_from[ i ] + 2;
                }
                BU-> BU_output[ leng_from ] = symb;
                BU-> BU_output[ leng_from + 1 ] = MAXSHORT;
                BU-> BU_to = 0;
                /* utf8_parse_errors += leng_from; */

            }
        } else if ( octet <= 0xBF ) {  /* 0xBF == 10111111 */
            /* a continuation octet starting with bits '10' */

            if ( from == 0 ) {
                /* orphan continuation octet -- just copy */

                BU-> BU_output[ 0 ] = symb;
                BU-> BU_output[ 1 ] = MAXSHORT;
                BU-> BU_to = 0;
                /* utf8_parse_errors++; */

            } else {
                /* add it to the sequence if appropriate */

                Tn = BU-> BU_ptok;
                cstr_from = Tn_name( Tn, from );
                leng_from = Tn_length( Tn, from );
                for ( i = 0; i < leng_from; ++i ) {
                    ts[ i ] = cstr_from[ i ];
                }
                ts[ leng_from ] = symb;
                ts[ leng_from + 1 ] = '\0';

                /* Check if valid prefix here */

                c1 = ts[ 0 ];
                     if ( ( c1 & 0x80 ) == 0x00 ) { type = 0; }
                else if ( ( c1 & 0xc0 ) == 0x80 ) { type = 1; }
                else if ( ( c1 & 0xe0 ) == 0xc0 ) { type = 2; }
                else if ( ( c1 & 0xf0 ) == 0xe0 ) { type = 3; }
                else if ( ( c1 & 0xf8 ) == 0xf0 ) { type = 4; }
                else { type = 5; }

                if ( leng_from == type ) {

                    BU-> BU_output[ 0 ] =
                        T2_insert( T2_Sigma, ts, leng_from + 1 );
                    BU-> BU_output[ 1 ] = MAXSHORT;
                    BU-> BU_to = 0;

                } else {

                    BU-> BU_output[ 0 ] = MAXSHORT;
                    BU-> BU_to = Tn_insert( Tn, ts, leng_from + 1 );

                }
            }
        } else if ( octet <= 0xF7 ) {  /* 0xF7 == 11110111 */
            /* starts with '110', '1110', or '11110' -- new sequence */
            /* dump prefix -- remember new octet */

            Tn = BU-> BU_ptok;
            cstr_from = Tn_name( Tn, from );
            leng_from = Tn_length( Tn, from );
            for ( i = 0; i < leng_from; ++i ) {
                BU-> BU_output[ i ] = cstr_from[ i ] + 2;
            }
            BU-> BU_output[ leng_from ] = MAXSHORT;

            ts[ 0 ] = octet;
            ts[ 1 ] = '\0';
            BU-> BU_to = Tn_insert( Tn, ts, 1 );
            /* utf8_parse_errors += leng_from; */

        } else if ( octet <= 0xFF ) {  /* 0xFF == 11111111 */
            /* all other octet values -- all errors */
            /* dump prefix and new octet */

            Tn = BU-> BU_ptok;
            cstr_from = Tn_name( Tn, from );
            leng_from = Tn_length( Tn, from );
            for ( i = 0; i < leng_from; ++i ) {
                BU-> BU_output[ i ] = cstr_from[ i ] + 2;
            }
            BU-> BU_output[ leng_from ] = octet;
            BU-> BU_output[ leng_from + 1 ] = MAXSHORT;
            BU-> BU_to = 0;
            /* utf8_parse_errors += leng_from + 1; */

        } else {
            /* input is not an octet -- domain error */
            /* dump prefix and whatever token seen */

            Tn = BU-> BU_ptok;
            cstr_from = Tn_name( Tn, from );
            leng_from = Tn_length( Tn, from );
            for ( i = 0; i < leng_from; ++i ) {
                BU-> BU_output[ i ] = cstr_from[ i ] + 2;
            }
            BU-> BU_output[ leng_from ] = octet;
            BU-> BU_output[ leng_from + 1 ] = MAXSHORT;
            BU-> BU_to = 0;
            /* utf8_parse_errors += leng_from + 1; */

            /* ALTERNATIVELY: go to dead state */
            /* BU-> BU_output[ 0 ] = MAXSHORT; */
            /* BU-> BU_to = MAXSHORT; */
            /* utf8_parse_errors++; */

        }
    }
    return( BU );
}

void BU_print_trans( BU_OBJECT BU, T2_OBJECT T2_Sigma )
{
    int i;

    printf( "%d\t", BU-> BU_from );
    printf( "%s\t[ ", T2_name_pr( T2_Sigma, BU-> BU_input ) );
    for ( i = 0; BU-> BU_output[ i ] < MAXSHORT; ++i ) {
      printf( "%s ", T2_name_pr( T2_Sigma, BU-> BU_output[ i ] ) );
    }
    printf( "]\t%d\n", BU-> BU_to );
}

/* =============================================================== */

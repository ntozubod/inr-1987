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

    BU = (BU_OBJECT) Salloc( sizeof(struct BU_desc) );
    BU-> Type    = BU_Object;
    BU-> BU_from = -1;
    BU-> BU_input = -1;
    BU-> BU_output = s_alloc( 10 );
    BU-> BU_output[ 0 ] = MAXSHORT;
    BU-> BU_output[ 1 ] = MAXSHORT;
    BU-> BU_to = -1;

    return( BU );
}

void BU_destroy( BU_OBJECT BU )
{
    if ( BU != NULL ) {
        Sfree( (char *) BU-> BU_output );
    }
    Sfree( (char *) BU );
}

BU_OBJECT BU_set_trans( BU_OBJECT BU,
    SHORT from, SHORT symb, T2_OBJECT T2_Sigma )
{
    int octet = 257;
    char ts[ 10 ];
    int next_type, octet_valid, state_memo;
    int state_type, sm_contains, i;

    BU-> BU_from = from;
    BU-> BU_input = symb;

    if ( from == START && symb == 1 ) {

        BU-> BU_output[ 0 ] = symb;
        BU-> BU_output[ 1 ] = MAXSHORT;
        BU-> BU_to = FINAL;
        return ( BU );

    } else {

        sm_contains = 0;

        /* first deal with any accumulated octets */
        if ( from > 0 ) {

            state_type = BU-> BU_from & 0xF;
            state_memo = BU-> BU_from >> 4;

            octet = MAXSHORT;
            if ( symb >= 2 && symb <= 257 ) {
                octet = symb - 2;
            }

            switch( state_type ) {

            /* ========== 2 octet cases ========== */

            case 2:
                /* state_memo contains 1 octet          */
                /* expect 1 more continuation octet     */
                /* any continuation octet is acceptable */

                sm_contains = 1;
                octet_valid = ( octet >= 0x80 && octet <= 0xBF );
                next_type   = 0;
                break;

            /* ========== 3 octet cases ========== */
            /* ========== 1 - 2 subcase ========== */

            case 3:
                /* state_memo contains 1 octet          */
                /* expect 2 more continuation octets    */
                /* continuation octets 80-9F disallowed */

                sm_contains = 1;
                octet_valid = ( octet >= 0xA0 && octet <= 0xBF );
                next_type   = 6;
                break;

            case 4:
                /* state_memo contains 1 octet          */
                /* expect 2 more continuation octets    */
                /* continuation octets A0-BF disallowed */

                sm_contains = 1;
                octet_valid = ( octet >= 0x80 && octet <= 0x9F );
                next_type   = 6;
                break;

            case 5:
                /* state_memo contains 1 octet          */
                /* expect 2 more continuation octets    */
                /* any continuation octet is acceptable */

                sm_contains = 1;
                octet_valid = ( octet >= 0x80 && octet <= 0xBF );
                next_type   = 6;
                break;

            /* ========== 2 - 1 subcase ========== */

            case 6:
                /* state_memo contains 2 octets         */
                /* expect 1 more continuation octet     */
                /* any continuation octet is acceptable */

                sm_contains = 2;
                octet_valid = ( octet >= 0x80 && octet <= 0xBF );
                next_type   = 0;
                break;

            /* ========== 4 octet cases ========== */
            /* ========== 1 - 3 subcase ========== */

            case 7:
                /* state_memo contains 1 octet          */
                /* expect 3 more continuation octets    */
                /* continuation octets 80-8F disallowed */

                sm_contains = 1;
                octet_valid = ( octet >= 0x90 && octet <= 0xBF );
                next_type   = 10;
            break;

            case 8:
                /* state_memo contains 1 octet          */
                /* expect 3 more continuation octets    */
                /* continuation octets 90-BF disallowed */

                sm_contains = 1;
                octet_valid = ( octet >= 0x80 && octet <= 0x8F );
                next_type   = 10;
                break;

            case 9:
                /* state_memo contains 1 octet          */
                /* expect 3 more continuation octets    */
                /* any continuation octet is acceptable */

                sm_contains = 1;
                octet_valid = ( octet >= 0x80 && octet <= 0xBF );
                next_type   = 10;
                break;

            /* ========== 2 - 2 subcase ========== */

            case 10:
                /* state_memo contains 2 octets         */
                /* expect 2 more continuation octets    */
                /* any continuation octet is acceptable */

                sm_contains = 2;
                octet_valid = ( octet >= 0x80 && octet <= 0xBF );
                next_type   = 11;
                break;

            /* ========== 3 - 1 subcase ========== */

            case 11:
                /* state_memo contains 3 octets         */
                /* expect 1 more continuation octet    */
                /* any continuation octet is acceptable */

                sm_contains = 3;
                octet_valid = ( octet >= 0x80 && octet <= 0xBF );
                next_type   = 0;
                break;

            default:
                Error( "shouldn't occur" );

            }
            if ( octet_valid ) {

                if ( next_type == 0 ) {
                    /* a complete valid sequence */

                    for ( i = sm_contains; --i >= 0; ) {
                        ts[ i ] = state_memo & 0xFF;
                        state_memo >>= 4;
                    }
                    ts[ sm_contains ] = octet;
                    ts[ sm_contains + 1 ] = '\0';
                    
                    BU-> BU_output[ 0 ] =
                        T2_insert( T2_Sigma, ts, sm_contains + 1 );

                } else {
                    /* a larger partial valid sequence */

                    BU-> BU_output[ 0 ] = MAXSHORT;
                    BU-> BU_to = 
                        (((( state_memo << 8 ) + octet ) << 4 )
                            + next_type );

                }
                return ( BU );

            } else {
                /* dump the content of state_memo before proceeding */

                for ( i = sm_contains; --i >= 0; ) {
                    BU-> BU_output[ i ] = ( state_memo & 0xFF ) + 2;
                    state_memo >>= 4;
                }

                /* note drop trough to from == 0 case */

            }

        /* end if from > 0 */
        }

        if ( octet <= 0x7F ) {
            /* next is single octet case */

            BU-> BU_output[ sm_contains ] = symb;
            BU-> BU_output[ sm_contains + 1 ] = MAXSHORT;
            BU-> BU_to = 0;
            return ( BU );

        } else {

            if ( octet >= 0xC2 && octet <= 0xDF ) {
                next_type = 2;
            } else if ( octet == 0xE0 ) {
                next_type = 3;
            } else if ( octet == 0xED ) {
                next_type = 4;
            } else if ( octet >= 0xE1 && octet <= 0xEF ) {
                next_type = 5;
            } else if ( octet == 0xF0 ) {
                next_type = 7;
            } else if ( octet == 0xF4 ) {
                next_type = 8;
            } else if ( octet >= 0xF1 && octet <= 0xF3 ) {
                next_type = 9;
            }

            if ( next_type > 0 ) {
                /* next is start of multiple octet case */

                BU-> BU_output[ sm_contains ] = MAXSHORT;
                BU-> BU_to = ( octet << 4 ) + next_type;
                return ( BU );

            } else {
                /* next is error */

                BU-> BU_output[ sm_contains ] = symb;
                BU-> BU_output[ sm_contains + 1 ] = MAXSHORT;
                BU-> BU_to = 0;
                return ( BU );

            }
        }
    }
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

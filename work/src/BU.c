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
    BU-> BU_output = s_alloc( 2 );
    BU-> BU_output[ 0 ] = MAXSHORT;
    BU-> BU_output[ 1 ] = MAXSHORT;
    BU-> BU_to = -1;
    BU-> BU_ptok = Tn = Tn_create();
    BU-> BU_ts = Salloc( 100 );

    result = Tn_insert( Tn, "", 0 );
    assert( result == 0 );

    return( BU );
}

void BU_destroy( BU_OBJECT BU )
{
    if ( BU != NULL ) {
        Sfree( (char *) BU-> BU_output );
        Tn_destroy( BU-> BU_ptok );
        Sfree( BU-> BU_ts );
    }
    Sfree( (char *) BU );
}

BU_OBJECT BU_set_trans( BU_OBJECT BU,
    SHORT from, SHORT symb, T2_OBJECT T2_Sigma )
{
    Tn_OBJECT Tn;
    int octet = 257;
    char *cstr_from, *ts;
    int leng_from, i;

    BU-> BU_from = from;
    BU-> BU_input = symb;

    if ( from == START && symb == 1 ) {
        BU-> BU_output[ 0 ] = symb;
        BU-> BU_output[ 1 ] = MAXSHORT;
        BU-> BU_to = FINAL;
    } else {
        if ( symb >= 2 && symb <= 258 ) {
            octet = symb - 2;
        }

        if ( octet <= 255  ) {
            Tn = BU-> BU_ptok;
            cstr_from = Tn_name( Tn, from );
            leng_from = Tn_length( Tn, from );
            if ( Ssize( BU-> BU_ts ) < leng_from + 1 ) {
                BU-> BU_ts = Srealloc( BU-> BU_ts, leng_from + 1 );
            }
            ts = BU-> BU_ts;
            for ( i = 0; i < leng_from; ++i ) {
                ts[ i ] = cstr_from[ i ];
            }
            i = leng_from;
            ts[ i ] = '\0';
            BU-> BU_output[ 0 ] = T2_insert( T2_Sigma, ts, i );
            BU-> BU_output[ 1 ] = MAXSHORT;
            BU-> BU_to = 0;
        } else {
            BU-> BU_to = MAXSHORT;
            BU-> BU_output[ 0 ] = MAXSHORT;
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

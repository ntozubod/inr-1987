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
 * BU translates from octet form to UTF-8 Code Points
 * BZ translates from octet form to UTF-8 Code Point Groups
 * BUi translates from octet sequences to octets
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
    int nibble = 18;
    char *cstr_from, *ts;
    int leng_from, i, k;

    BU-> BU_from = from;
    BU-> BU_input = symb;

    if ( from == START && symb == 1 ) {
        BU-> BU_output[ 0 ] = symb;
        BU-> BU_output[ 1 ] = MAXSHORT;
        BU-> BU_to = FINAL;
    } else {
        if ( symb >= 50 && symb <= 59 ) {
            nibble = symb - 50;
        } else if ( symb >= 67 && symb <= 72 ) {
            nibble = symb - 57;
        } else if ( symb == 97 ) {
            nibble = 17;
        }

        if ( nibble <= 16 ) {
            Tn = BU-> BU_ptok;
            cstr_from = Tn_name( Tn, from );
            leng_from = Tn_length( Tn, from );
            if ( Ssize( BU-> BU_ts ) < leng_from + 2 ) {
                BU-> BU_ts = Srealloc( BU-> BU_ts, leng_from + 2 );
            }
            ts = BU-> BU_ts;

            for ( i = 0; i < leng_from; ++i ) {
                ts[ i ] = cstr_from[ i ];
            }
            ts[ leng_from ] = nibble;
            ts[ leng_from + 1 ] = '\0';
            BU-> BU_to = Tn_insert( Tn, ts, leng_from + 1 );
            BU-> BU_output[ 0 ] = MAXSHORT;
        } else if ( nibble == 17 ) {
            Tn = BU-> BU_ptok;
            cstr_from = Tn_name( Tn, from );
            leng_from = Tn_length( Tn, from );
            if ( leng_from % 2 != 0 ) { Error( "Parity error in BU" ); }
            if ( Ssize( BU-> BU_ts ) < leng_from / 2 + 1 ) {
                BU-> BU_ts = Srealloc( BU-> BU_ts, leng_from / 2 + 1 );
            }
            ts = BU-> BU_ts;
            for ( i = 0; i < leng_from; i += 2 ) {
                k = i / 2;
                ts[ k ] = ( cstr_from[ k ] << 4 ) + cstr_from[ k + 1 ];
            }
            k = leng_from / 2;
            ts[ k ] = '\0';
            BU-> BU_output[ 0 ] = T2_insert( T2_Sigma, ts, k );
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
/*
    Tn_OBJECT Tn;
    int nibble = 18;
    char *cstr_from, *ts;
    int leng_from, i, k;
*/
    int i;

    printf( "%d\t", BU-> BU_from );
    printf( "%s\t[ ", T2_name_pr( T2_Sigma, BU-> BU_input ) );
    for ( i = 0; BU-> BU_output[ i ] < MAXSHORT; ++i ) {
      printf( "%s ", T2_name_pr( T2_Sigma, BU-> BU_output[ i ] ) );
    }
    printf( "]\t%d\n", BU-> BU_to );
}

/* =============================================================== */

BZ_OBJECT BZ_create()
{
    BZ_OBJECT BZ;
    Tn_OBJECT Tn;
    int result;

    BZ = (BZ_OBJECT) Salloc( sizeof(struct BZ_desc) );
    BZ-> Type    = BZ_Object;
    BZ-> BZ_from = -1;
    BZ-> BZ_input = -1;
    BZ-> BZ_output = s_alloc( 2 );
    BZ-> BZ_output[ 0 ] = MAXSHORT;
    BZ-> BZ_output[ 1 ] = MAXSHORT;
    BZ-> BZ_to = -1;
    BZ-> BZ_ptok = Tn = Tn_create();
    BZ-> BZ_ts = Salloc( 100 );

    result = Tn_insert( Tn, "", 0 );
    assert( result == 0 );

    return( BZ );
}

void BZ_destroy( BZ_OBJECT BZ )
{
    if ( BZ != NULL ) {
        Sfree( (char *) BZ-> BZ_output );
        Tn_destroy( BZ-> BZ_ptok );
        Sfree( BZ-> BZ_ts );
    }
    Sfree( (char *) BZ );
}

BZ_OBJECT BZ_set_trans( BZ_OBJECT BZ,
    SHORT from, SHORT symb, T2_OBJECT T2_Sigma )
{
    Tn_OBJECT Tn;
    int nibble = 18;
    char *cstr_from, *ts;
    int leng_from, i, k;

    BZ-> BZ_from = from;
    BZ-> BZ_input = symb;

    if ( from == START && symb == 1 ) {
        BZ-> BZ_output[ 0 ] = symb;
        BZ-> BZ_output[ 1 ] = MAXSHORT;
        BZ-> BZ_to = FINAL;
    } else {
        if ( symb >= 50 && symb <= 59 ) {
            nibble = symb - 50;
        } else if ( symb >= 67 && symb <= 72 ) {
            nibble = symb - 57;
        } else if ( symb == 97 ) {
            nibble = 17;
        }

        if ( nibble <= 16 ) {
            Tn = BZ-> BZ_ptok;
            cstr_from = Tn_name( Tn, from );
            leng_from = Tn_length( Tn, from );
            if ( Ssize( BZ-> BZ_ts ) < leng_from + 2 ) {
                BZ-> BZ_ts = Srealloc( BZ-> BZ_ts, leng_from + 2 );
            }
            ts = BZ-> BZ_ts;

            for ( i = 0; i < leng_from; ++i ) {
                ts[ i ] = cstr_from[ i ];
            }
            ts[ leng_from ] = nibble;
            ts[ leng_from + 1 ] = '\0';
            BZ-> BZ_to = Tn_insert( Tn, ts, leng_from + 1 );
            BZ-> BZ_output[ 0 ] = MAXSHORT;
        } else if ( nibble == 17 ) {
            Tn = BZ-> BZ_ptok;
            cstr_from = Tn_name( Tn, from );
            leng_from = Tn_length( Tn, from );
            if ( leng_from % 2 != 0 ) { Error( "Parity error in BZ" ); }
            if ( Ssize( BZ-> BZ_ts ) < leng_from / 2 + 1 ) {
                BZ-> BZ_ts = Srealloc( BZ-> BZ_ts, leng_from / 2 + 1 );
            }
            ts = BZ-> BZ_ts;
            for ( i = 0; i < leng_from; i += 2 ) {
                k = i / 2;
                ts[ k ] = ( cstr_from[ k ] << 4 ) + cstr_from[ k + 1 ];
            }
            k = leng_from / 2;
            ts[ k ] = '\0';
            BZ-> BZ_output[ 0 ] = T2_insert( T2_Sigma, ts, k );
            BZ-> BZ_output[ 1 ] = MAXSHORT;
            BZ-> BZ_to = 0;
        } else {
            BZ-> BZ_to = MAXSHORT;
            BZ-> BZ_output[ 0 ] = MAXSHORT;
        }
    }
    return( BZ );
}

void BZ_print_trans( BZ_OBJECT BZ, T2_OBJECT T2_Sigma )
{
/*
    Tn_OBJECT Tn;
    int nibble = 18;
    char *cstr_from, *ts;
    int leng_from, i, k;
*/
    int i;

    printf( "%d\t", BZ-> BZ_from );
    printf( "%s\t[ ", T2_name_pr( T2_Sigma, BZ-> BZ_input ) );
    for ( i = 0; BZ-> BZ_output[ i ] < MAXSHORT; ++i ) {
      printf( "%s ", T2_name_pr( T2_Sigma, BZ-> BZ_output[ i ] ) );
    }
    printf( "]\t%d\n", BZ-> BZ_to );
}

/* =============================================================== */

BUi_OBJECT BUi_create()
{
    BUi_OBJECT BUi;

    BUi = (BUi_OBJECT) Salloc( sizeof(struct BUi_desc) );
    BUi-> Type    = BUi_Object;
    BUi-> BUi_from = -1;
    BUi-> BUi_input = -1;
    BUi-> BUi_output = s_alloc( 2 );
    BUi-> BUi_output[ 0 ] = MAXSHORT;
    BUi-> BUi_output[ 1 ] = MAXSHORT;
    BUi-> BUi_to = -1;

    return( BUi );
}

void BUi_destroy( BUi_OBJECT BUi )
{
    if ( BUi != NULL ) {
        Sfree( (char *) BUi-> BUi_output );
    }
    Sfree( (char *) BUi );
}

BUi_OBJECT BUi_set_trans( BUi_OBJECT BUi,
    SHORT from, SHORT symb, T2_OBJECT T2_Sigma )
{
    char *in_str;
    int in_str_len, out_str_len, k, l, c;
    SHORT *out_str;
    int T2idx[] = { 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
                    67, 68, 69, 70, 71, 72 };
    int T2idx_blk =  97;

    BUi-> BUi_from = from;
    BUi-> BUi_input = symb;

    if ( from == START && symb == 1 ) {
        BUi-> BUi_output[ 0 ] = symb;
        BUi-> BUi_output[ 1 ] = MAXSHORT;
        BUi-> BUi_to = FINAL;
    } else {
        in_str = T2_name( T2_Sigma, symb );
        in_str_len = T2_length( T2_Sigma, symb );

        out_str_len = Ssize( (char *) BUi-> BUi_output ) / sizeof( SHORT );
        if ( out_str_len < in_str_len * 2 + 2 ) {
            BUi-> BUi_output = (SHORT *)
                Srealloc( (char *) BUi-> BUi_output,
                    2 * ( in_str_len * 2 + 2 ) * sizeof( SHORT ) );
        }
        out_str = BUi-> BUi_output;

        l = 0;
        for ( k = 0; k < in_str_len; ++k ) {
            c = in_str[ k ];
            out_str[ l++ ] = T2idx[ ( c >> 4 ) & 0xf ];
            out_str[ l++ ] = T2idx[ c & 0xf ];
        }
        out_str[ l++ ] = T2idx_blk;
        out_str[ l++ ] = MAXSHORT;
    }
    return( BUi );
}

void BUi_print_trans( BUi_OBJECT BUi, T2_OBJECT T2_Sigma )
{
    int i;

    printf( "%d\t", BUi-> BUi_from );
    printf( "%s\t[ ", T2_name_pr( T2_Sigma, BUi-> BUi_input ) );
    for ( i = 0; BUi-> BUi_output[ i ] < MAXSHORT; ++i ) {
      printf( "%s ", T2_name_pr( T2_Sigma, BUi-> BUi_output[ i ] ) );
    }
    printf( "]\t%d\n", BUi-> BUi_to );
}

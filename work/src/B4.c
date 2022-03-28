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
    int result, ti;
    char tstr[ 2 ];

    B4 = (B4_OBJECT) Salloc( sizeof(struct B4_desc) );
    B4-> Type    = B4_Object;
    B4-> B4_ptok = Tn = Tn_create();

    result = Tn_insert( Tn, "^^", 2 );
    assert( result == 0 );
    result = Tn_insert( Tn, "-|", 2 );
    assert( result == 1 );
    for( ti = 0; ti < 256; ti++ ) {
        tstr[ 0 ] = ti;
        tstr[ 1 ] = '\0';
        result = Tn_insert( Tn, tstr, 1 );
        assert( result == ti + 2 );
    }

    return( B4 );
}

void B4_destroy( B4_OBJECT B4 )
{
    if ( B4 != NULL ) {
        Tn_destroy( B4-> B4_ptok );
    }
    Sfree( (char *) B4 );
}

SHORT B4_get_trans_to( B4_OBJECT B4, SHORT from, SHORT symb,
    T2_OBJECT T2_Sigma )
{
/*
    Tn_OBJECT Tn;
    char *cstr_from, *cstr_symb;
    int leng_from, leng_symb;

    Tn = B4-> B4_ptok;
    cstr_from = Tn_name( Tn, from );
    leng_from = Tn_length( Tn, from );

    cstr_symb = T2_name( T2_Sigma, symb );
    leng_symb = T2_length( T2_Sigma, symb );
*/

    return( 1 );
}

SHORT B4_get_trans_output( B4_OBJECT B4, SHORT from, SHORT symb,
    T2_OBJECT T2_Sigma )
{
    return( 1 );
}

void B4_test()
{
    B4_OBJECT B4;

    B4 = B4_create();
    B4_destroy( B4 );
}

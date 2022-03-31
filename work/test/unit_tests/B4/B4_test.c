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

SHORT do_trans( B4_OBJECT B4, SHORT from, char input ) {
    char ts[ 2 ];

    ts[ 0 ] = input & 0xff;
    ts[ 1 ] = '\0';
    B4 = B4_set_trans( B4, from, T2_insert( TT2, ts, 1 ), TT2 );
    B4_print_trans( B4, TT2 );
    return( B4-> B4_to );
}

int main( int argc, char *argv[] )
{
    B4_OBJECT B4;
    int state = 0;

    TT2_init();
    B4 = B4_create();

    state = do_trans( B4, state, '2' );
    state = do_trans( B4, state, '3' );
    state = do_trans( B4, state, '_' );

    state = do_trans( B4, state, '2' );
    state = do_trans( B4, state, '5' );
    state = do_trans( B4, state, '2' );
    state = do_trans( B4, state, '7' );
    state = do_trans( B4, state, '_' );

    B4_destroy( B4 );
    printf( "done\n" );
    return( 0 );
}

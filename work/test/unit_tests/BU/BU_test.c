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

SHORT do_trans( BU_OBJECT BU, SHORT from, char input ) {
    char ts[ 2 ];

    ts[ 0 ] = input & 0xff;
    ts[ 1 ] = '\0';
    BU = BU_set_trans( BU, from, T2_insert( TT2, ts, 1 ), TT2 );
    BU_print_trans( BU, TT2 );
    return( BU-> BU_to );
}

int main( int argc, char *argv[] )
{
    BU_OBJECT BU;
    int state = 0;

    TT2_init();
    BU = BU_create();

    state = do_trans( BU, state, '2' );
    state = do_trans( BU, state, '3' );
    state = do_trans( BU, state, '_' );

    state = do_trans( BU, state, '2' );
    state = do_trans( BU, state, '5' );
    state = do_trans( BU, state, '2' );
    state = do_trans( BU, state, '7' );
    state = do_trans( BU, state, '_' );

    state = do_trans( BU, state, '\x61' );
    state = do_trans( BU, state, '\xCC' );
    state = do_trans( BU, state, '\x81' );
    state = do_trans( BU, state, '_' );

    state = do_trans( BU, state, '\xC3' );
    state = do_trans( BU, state, '\xA1' );
    state = do_trans( BU, state, '_' );

    state = do_trans( BU, state, '\xE1' );
    state = do_trans( BU, state, '\xBC' );
    state = do_trans( BU, state, '\x80' );
    state = do_trans( BU, state, '\xCF' );
    state = do_trans( BU, state, '\x80' );
    state = do_trans( BU, state, '\xE1' );
    state = do_trans( BU, state, '\xBD' );
    state = do_trans( BU, state, '\xB8' );
    state = do_trans( BU, state, '_' );

    BU_destroy( BU );
    printf( "done\n" );
    return( 0 );
}

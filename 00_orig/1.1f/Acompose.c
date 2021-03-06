/*
 * Copyright (c) 1985, J Howard Johnson, University of Waterloo.
 * Not for sale or distribution without written permission of the author.
 */

#include <stdio.h>

#include "O.h"

A_OBJECT A_compose( A1, A2 )
register A_OBJECT A1, A2;
{
	register A_OBJECT A;
	int current, s1, s2, t1, t2, cur_a, cur_b, flag;
	A_row *p1, *p1z, *p2, *p2z;
	U_OBJECT U;
	A_row *cur_st;

	if ( A1-> A_nT == 1 && A2-> A_nT == 1 ) {
		A_destroy( A1 );
		A_destroy( A2 );
		return( A_lambda() );
	}
	A1 = A_min( A1 );
	A2 = A_min( A2 );
	A = A_create();
	A-> A_nT = A1-> A_nT + A2-> A_nT - 2;
	U = U_create();

	if ( U_insert( U, START, START, 1 ) != START )
		Error( "A_compose: BOTCH 1" );
	if ( U_insert( U, FINAL, FINAL, 1 ) != FINAL )
		Error( "A_compose: BOTCH 2" );

	for( current = 0; current < U-> U_n; current++ ) {
		cur_st = U_rec( U, current );
		cur_a = cur_st-> A_a;
		cur_b = cur_st-> A_b;
		flag  = cur_st-> A_c;
		p1  = A1-> A_p[ cur_a ];
		p1z = A1-> A_p[ cur_a + 1 ];
		p2  = A2-> A_p[ cur_b ];
		p2z = A2-> A_p[ cur_b + 1 ];
		s1 = s2 = (-1);
		while( p1 < p1z || p2 < p2z ) {
			if ( s1 < 0 )
				if ( p1 < p1z ) {
					if ( A1-> A_nT == 1 ) {
						s1 = p1-> A_b;
						t1 = 0;
					} else if ( A1-> A_nT == 2 ) {
						s1 = p1-> A_b;
						t1 = s1 & 1;
						s1 >>= 1;
					} else {
						s1 = p1-> A_b / A1-> A_nT;
						t1 = p1-> A_b - s1 * A1-> A_nT;
					}
				} else	s1 = MAXSHORT;
			if ( s2 < 0 )
				if ( p2 < p2z ) {
					if ( A2-> A_nT == 1 ) {
						s2 = p2-> A_b;
						t2 = 0;
					} else if ( A2-> A_nT == 2 ) {
						s2 = p2-> A_b;
						t2 = s2 & 1;
						s2 >>= 1;
					} else {
						s2 = p2-> A_b / A2-> A_nT;
						t2 = p2-> A_b - s2 * A2-> A_nT;
					}
				} else	s2 = MAXSHORT;
			if ( p1-> A_b == 1 || p2-> A_b == 1 ) {
				if ( p1-> A_b == 1 && p2-> A_b == 1 )
					A = A_add( A, current, 1, FINAL );
				if ( p1-> A_b == 1 ) {
					++p1;
					s1 = (-1);
				}
				if ( p2-> A_b == 1 ) {
					++p2;
					s2 = (-1);
				}
			} else if ( s1 <= s2 && t1 < A1-> A_nT - 1 ) {
				if ( s1 > 0 )
				A = A_add( A, current,
					( A-> A_nT == 1 ? s1 :
					( A-> A_nT == 2 ? (s1 << 1) + t1 :
					s1 * A-> A_nT + t1 ) ),
					U_insert( U, (int)p1-> A_c,
						cur_b, 0 ) );
				++p1;
				s1 = (-1);
			} else if ( s2 < s1 && t2 != 0 ) {
				if ( flag && s2 > 0 )
				A = A_add( A, current,
					( A-> A_nT == 1 ? s2 :
					( A-> A_nT == 2 ? (s2 << 1) :
					s2 * A-> A_nT ) ) + t2 + A1-> A_nT - 2,
					U_insert( U, cur_a,
						(int)p2-> A_c, 1 ) );
				++p2;
				s2 = (-1);
			} else if ( s1 == s2 && t1 == A1-> A_nT-1 && t2 == 0 ) {
				A = A_add( A, current, 0,
					U_insert( U, (int)p1-> A_c,
						(int)p2-> A_c, 1 ) );
				++p1;
				++p2;
				s1 = s2 = (-1);
			} else if ( s1 <= s2 ) {
				++p1;
				s1 = (-1);
			} else {
				++p2;
				s2 = (-1);
			}
		}
	}
	A_destroy( A1 );
	A_destroy( A2 );
	U_destroy( U );
	return( A );
}

/*********************************************************************************
    LbMatrix.cc
    Implementation file for the LightBikes2001 Matrix class

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
       Henry Southgate

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*********************************************************************************/
#include "LbMath.h"
#ifdef HAVE_MATRIX_DISPLAY
#include <stdio.h>
#endif

// Useful constants
#define A m_Values[0][0]
#define B m_Values[0][1]
#define C m_Values[0][2]
#define D m_Values[1][0]
#define E m_Values[1][1]
#define F m_Values[1][2]
#define G m_Values[2][0]
#define H m_Values[2][1]
#define I m_Values[2][2]

/* Default constructor */
LbMatrix::LbMatrix() {
   int i = 0, j = 0;

  for ( ; i < 3; i++ )
    for ( ; j < 3; j++ ) 
      m_Values[i][j] = 0;
}

/* Useful constructor */
LbMatrix::LbMatrix( float Values[3][3] ) {
  int i, j;

  for ( i = 0; i < 3; i++ )
    for ( j = 0; j < 3; j++ ) 
      m_Values[i][j] = Values[i][j];

  return;
}

/* Make like Keanau Reeves and destroy the Matrix. */
LbMatrix::~LbMatrix() {
  return;
}

/**************** Access methods **********************/
/* Return Matrix's determinate */
float LbMatrix::det() {
  return ( A * ( E * I - F * H ) ) -
         ( B * ( D * I - F * G ) ) +
         ( C * ( D * H - G * E ) );
}

/* Write matrix to console */
#ifdef HAVE_MATRIX_DISPLAY
void LbMatrix::display() {
  printf( "[%5.2f %5.2f %5.2f]\n[%5.2f %5.2f %5.2f]\n[%5.2f %5.2f %5.2f]\n",
	  A, B, C, D, E, F, G, H, I );
}
#endif


/************** Scalar Arithmetic *********************/
LbMatrix LbMatrix::operator *(float f) {
  float x[3][3] = { {A*f, B*f, C*f},
                    {D*f, E*f, F*f},
                    {G*f, H*f, I*f}  };
   return LbMatrix( x );
}

LbMatrix LbMatrix::operator /(float f) {
  float x[3][3] = { {A/f, B/f, C/f},
                    {D/f, E/f, F/f},
                    {G/f, H/f, I/f}  };
   return LbMatrix( x );
}


/************** Matrix Arithmetic *********************/
LbMatrix LbMatrix::operator *( LbMatrix& M ) {
  float m[3][3] = { {0.0, 0.0, 0.0},
		    {0.0, 0.0, 0.0},
		    {0.0, 0.0, 0.0} };
  int x,y;
  
  for ( y = 0; y < 3; y++ )
    for ( x = 0; x < 3; x++ )
      m[y][x] = m_Values[y][0]*M.getXY(x,0)+
	        m_Values[y][1]*M.getXY(x,1)+
	        m_Values[y][2]*M.getXY(x,2);

  return LbMatrix( m );
}

LbVector LbMatrix::operator *( LbVector& V ) {
  return LbVector( A * V.getX() + B * V.getY() + C * V.getZ(),
		   D * V.getX() + E * V.getY() + F * V.getZ(),
		   G * V.getX() + H * V.getY() + I * V.getZ() );
  
}

/*************** Matrix Operations *******************/
LbMatrix LbMatrix::invert() {
  if ( det() == 0 ) return LbMatrix(m_Values);

  float m[3][3] = { {cofactor(1,2,1,2), cofactor(0,2,1,2), cofactor(0,1,1,2) },
		    {cofactor(1,2,0,2), cofactor(0,2,0,2), cofactor(0,1,0,2) },
		    {cofactor(1,2,0,1), cofactor(0,2,0,1), cofactor(0,1,0,1) } };
  LbMatrix M = LbMatrix(m).transpose().applyChequerboard() / det();
  return M;
}

LbMatrix LbMatrix::transpose() {
  float x[3][3] = { {A, D, G},
                    {B, E, H},
                    {C, F, I}  };
  return LbMatrix( x );
}


LbMatrix LbMatrix::applyChequerboard() {
  float m[3][3] = { { A, -B,  C},
		    {-D,  E, -F},
		    { G, -H,  I} };
  return LbMatrix( m );
}










/*********************************************************************************
    LBMatrix4.cc
    Implementation file for the LightBikes2001 Matrix4 class

    Copyright (C) 2000  University of Warwick Computing Society

    Contributors to this file:
       Henry Southgate
       Adam Bowen

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

/* Constants to Make Things Easy */
#define A m_Values[0][0]
#define B m_Values[0][1]
#define C m_Values[0][2]
#define D m_Values[0][3]
#define E m_Values[1][0]
#define F m_Values[1][1]
#define G m_Values[1][2]
#define H m_Values[1][3]
#define I m_Values[2][0]
#define J m_Values[2][1]
#define K m_Values[2][2]
#define L m_Values[2][3]
#define M m_Values[3][0]
#define N m_Values[3][1]
#define O m_Values[3][2]
#define P m_Values[3][3]

/* Default constructor */
LBMatrix4::LBMatrix4() {
   int i, j;

  for ( i = 0; i < 4; i++ )
    for ( j = 0; j < 4; j++ ) 
      m_Values[i][j] = 0;
}

/* Proper Constructor */
LBMatrix4::LBMatrix4( float Values[4][4] ) {
  int i=0, j=0;

  for ( i = 0; i < 4; i++ )
    for ( j = 0; j < 4; j++ ) 
      m_Values[i][j] = Values[i][j];

  return;
}

/* Carrie-Anne Moss */
LBMatrix4::~LBMatrix4() {
  return;
}

/**************** Access methods **********************/
/* Return Matrix's determinate */
float LBMatrix4::det() {
  return ( A * cofactor(0,0) ) -
         ( B * cofactor(1,0) ) +
         ( C * cofactor(2,0) ) -
         ( D * cofactor(3,0) );
}

/* Write matrix to console */
#ifdef HAVE_MATRIX_DISPLAY
void LBMatrix4::display() {
  printf( "[%5.2f %5.2f %5.2f %5.2f]\n[%5.2f %5.2f %5.2f %5.2f]\n[%5.2f %5.2f %5.2f %5.2f]\n[%5.2f %5.2f %5.2f %5.2f]\n",
	  A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P );
}
#endif


/*************** Scalar Operations ********************/
LBMatrix4 LBMatrix4::operator *(float f) {
  float x[4][4] = { {A*f, B*f, C*f, D*f},
                    {E*f, F*f, G*f, H*f},
                    {I*f, J*f, K*f, L*f},
                    {M*f, N*f, O*f, P*f} };
   return LBMatrix4( x );
}

LBMatrix4 LBMatrix4::operator /(float f) {
  float x[4][4] = { {A/f, B/f, C/f, D/f},
                    {E/f, F/f, G/f, H/f},
                    {I/f, J/f, K/f, L/f},
                    {M/f, N/f, O/f, P/f} };
   return LBMatrix4( x );
}

/************** Matrix Arithmetic *********************/
LBMatrix4 LBMatrix4::operator *( LBMatrix4& Mat ) {
  float m[4][4] = { {0.0, 0.0, 0.0, 0.0},
		    {0.0, 0.0, 0.0, 0.0},
		    {0.0, 0.0, 0.0, 0.0},
		    {0.0, 0.0, 0.0, 0.0} };
  int x,y;
  
  for ( y = 0; y < 4; y++ )
    for ( x = 0; x < 4; x++ )
      m[y][x] = m_Values[y][0]*Mat.getXY(x,0)+
	        m_Values[y][1]*Mat.getXY(x,1)+
	        m_Values[y][2]*Mat.getXY(x,2)+
	        m_Values[y][3]*Mat.getXY(x,3);

  return LBMatrix4( m );

}

LBVector4 LBMatrix4::operator *( LBVector4& V ) {
  return LBVector4 ( A * V.getX() + B * V.getY() + C * V.getZ() + D * V.getW(),
		     E * V.getX() + F * V.getY() + G * V.getZ() + H * V.getW(),
		     I * V.getX() + J * V.getY() + K * V.getZ() + L * V.getW(),
		     M * V.getX() + N * V.getY() + O * V.getZ() + P * V.getW() );
  
}

/*************** Matrix Operations *******************/
LBMatrix4 LBMatrix4::invert() {
  if ( det() == 0 ) return LBMatrix4(m_Values);

  float m[4][4] = { {cofactor(0,0), cofactor(1,0), cofactor(2,0), cofactor(3,0) },
                    {cofactor(0,1), cofactor(1,1), cofactor(2,1), cofactor(3,1) },
                    {cofactor(0,2), cofactor(1,2), cofactor(2,2), cofactor(3,2) },
                    {cofactor(0,3), cofactor(1,3), cofactor(2,3), cofactor(3,3) } };
  LBMatrix4 Mat = LBMatrix4(m).transpose().applyChequerboard() / det();
  return Mat;
}

LBMatrix4 LBMatrix4::transpose() {
  float x[4][4] = { {A, E, I, M},
                    {B, F, J, N},
                    {C, G, K, O},
                    {D, H, L, P}  };
  return LBMatrix4( x );
}


LBMatrix4 LBMatrix4::applyChequerboard() {
  float m[4][4] = { { A, -B,  C, -D},
		    {-E,  F, -G,  H},
		    { I, -J,  K, -L},
		    {-M,  N, -O,  P} };
  return LBMatrix4( m );
}


// cofactor code by Adam Bowen
float LBMatrix4::cofactor( int x, int y ) {
  float tmp[3][3];
  int i,j;
  int m,n;
  m = n = 0;
  for(i=0;i<4;i++) {
    if(i != x) {
      for(j=0;j<4;j++) {
	if(j != y) {
	  tmp[m][n] = m_Values[j][i];
	  n++;
	}
      }
    
      m++;
      n = 0;
    }
  }
  
  return tmp[0][0]*(tmp[1][1]*tmp[2][2]-tmp[2][1]*tmp[1][2])-
    tmp[0][1]*(tmp[1][0]*tmp[2][2]-tmp[2][0]*tmp[1][2])+
    tmp[0][2]*(tmp[1][0]*tmp[2][1]-tmp[2][0]*tmp[1][1]);
}








/*********************************************************************************
    LBVector4.cc
    Implementation file for the LightBikes2001 Vector4 class

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
#ifdef HAVE_VECTOR_TOSTRING
#include <stdio.h>
#endif
#include <math.h>

/* Default Constructor: intialise components */
LBVector4::LBVector4() {
  m_x = m_y = m_z = m_w = 0;

  return;
}

/* Constructor we want to use */
LBVector4::LBVector4( float x,
		      float y,
		      float z,
		      float w
		      ) {
  m_x = x;
  m_y = y;
  m_z = z;
  m_w = w;
  return;
}

/* Destructor */
LBVector4::~LBVector4() {
  return;
}

/**************** Access methods **********************/
/* Return Vector4's Modulus */
float LBVector4::mod() {
  return sqrt( (m_x*m_x) +
	       (m_y*m_y) +
	       (m_z*m_z) +
	       (m_w*m_w) );
}

/* Return char* representation */
#ifdef HAVE_VECTOR_TOSTRING
void LBVector4::toString(char* rv) {
  sprintf( rv, "(%5.2f, %5.2f, %5.2f, %5.2f)", m_x, m_y, m_z, m_w );
  return;
}
#endif


/**************** Scalar Arithmetic *******************/
LBVector4 LBVector4::operator *(float f) {
  return LBVector4 ( m_x * f,
		     m_y * f,
		     m_z * f,
		     m_w * f
		    );
}

LBVector4 LBVector4::operator /(float f) {
  return LBVector4 ( m_x / f,
		     m_y / f,
		     m_z / f,
		     m_w / f
		    );
}


/**************** Vector Arithmetic *******************/
LBVector4 LBVector4::operator +(LBVector4& v) {
  return LBVector4 ( m_x + v.getX(),
		     m_y + v.getY(),
		     m_z + v.getZ(),
		     m_w + v.getW() 
		     );
}

LBVector4 LBVector4::operator -(LBVector4& v) {
  return LBVector4 ( m_x - v.getX(),
		     m_y - v.getY(),
		     m_z - v.getZ(),
		     m_w - v.getW() 
		     );
}

/*************** Vector Multiplication ****************/
float LBVector4::operator ^(LBVector4& v) {
  return ( (m_x * v.getX()) +
	   (m_y * v.getY()) +
	   (m_z * v.getZ()) +
	   (m_w * v.getW()) );
}


/*************** Matrix Multiplication ****************/
LBVector4 LBVector4::operator *(LBMatrix4& M ){
  return LBVector4( m_x*M.getXY(0,0) + m_y*M.getXY(0,1) + m_z*M.getXY(0,2) + m_w*M.getXY(0,3),
		    m_x*M.getXY(1,0) + m_y*M.getXY(1,1) + m_z*M.getXY(1,2) + m_w*M.getXY(1,3),
		    m_x*M.getXY(2,0) + m_y*M.getXY(2,1) + m_z*M.getXY(2,2) + m_w*M.getXY(2,3),
		    m_x*M.getXY(3,0) + m_y*M.getXY(3,1) + m_z*M.getXY(3,2) + m_w*M.getXY(3,3) );
}












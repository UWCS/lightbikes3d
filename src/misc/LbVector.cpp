/*********************************************************************************
    LbVector.cc
    Implementation file for the LightBikes2001 Vector class

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

/* Default Constructor. Don't want to use it. */
LbVector::LbVector() { 
  m_x = m_y = m_z = 0;

  return;
}

/* Proper constructor. */
LbVector::LbVector( float _,
		    float __,
		    float ___
		    ) {
  m_x = _;
  m_y = __;
  m_z = ___;
  return;
}

/* Destructor. Kill that vector! */
LbVector::~LbVector() {
  return;
}

/**************** Access methods **********************/

/* Return the modulus of the Vector */
float LbVector::mod() {
  return (float)sqrt( (m_x*m_x) +
	       (m_y*m_y) +
	       (m_z*m_z) );
}

/* Return a char* representation. */
#ifdef HAVE_VECTOR_TOSTRING
void LbVector::toString(char* rv) {
  sprintf( rv, "(%5.2f, %5.2f, %5.2f)", m_x, m_y, m_z );
  return;
}
#endif


/**************** Scalar Arithmetic *******************/
LbVector LbVector::operator *(float f) {
  return LbVector ( m_x * f,
		    m_y * f,
		    m_z * f
		    );
}

LbVector LbVector::operator /(float f) {
  return LbVector ( m_x / f,
		    m_y / f,
		    m_z / f
		    );
}


/**************** Vector Arithmetic *******************/
LbVector LbVector::operator +(LbVector& v) {
  return LbVector ( m_x + v.getX(),
		    m_y + v.getY(),
		    m_z + v.getZ() 
		    );
}

LbVector LbVector::operator -(LbVector& v) {
  return LbVector ( m_x - v.getX(),
		    m_y - v.getY(),
		    m_z - v.getZ() 
		    );
}

/*************** Vector Multiplication ****************/
// dammit if Soustroup can use >> and << for a stream
// IO then I can use ^ for dot-product!
float LbVector::operator ^(LbVector& v) {
  return ( (m_x * v.getX()) +
	   (m_y * v.getY()) +
	   (m_z * v.getZ()) );
}


/*************** Matrix Multiplication ****************/
LbVector LbVector::operator *(LbMatrix& M ){
  return LbVector( m_x*M.getXY(0,0) + m_y*M.getXY(0,1) + m_z*M.getXY(0,2),
		   m_x*M.getXY(1,0) + m_y*M.getXY(1,1) + m_z*M.getXY(1,2),
		   m_x*M.getXY(2,0) + m_y*M.getXY(2,1) + m_z*M.getXY(2,2) );
}












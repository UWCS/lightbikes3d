/*********************************************************************************
    LbMath.h
    Header file for the LightBikes2001 Matrix and Vector classes

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

#ifndef __LBMATH__
#define __LBMATH__

/* Options */
#define HAVE_VECTOR_TOSTRING
#define HAVE_MATRIX_DISPLAY


/* Prototype Classes */
class LbVector;
class LbMatrix;
class LbVector4;
class LbMatrix4;
/*********************/


/*********************************************************
 *                                                       * 
 * LbVector                                              * 
 *                                                       *
 * Definition of the 3-dimensional Vector object         * 
 *                                                       *
 *********************************************************/
class LbVector 
{

public:

  /* {Con,De}structor methods */
  LbVector();
  LbVector( float _, float __, float ___ );
  ~LbVector();

  /* Access methods */
  inline float getX() { return m_x; };
  inline float getY() { return m_y; };
  inline float getZ() { return m_z; };
  float mod();

#ifdef HAVE_VECTOR_TOSTRING
  void toString( char* rv );
#endif


  /* Scalar Arithmetic */
  LbVector operator *(float f);
  LbVector operator /(float f);

  /* Vector Arithmetic */
  LbVector operator +(LbVector& v);
  LbVector operator -(LbVector& v);

  /* Vector Multiplication */
  float operator ^(LbVector& v) ; // . product 

  /* Matrix Multiplication */
  LbVector operator *(LbMatrix& M);

  
private:

  /* The important bits */
  float m_x;
  float m_y;
  float m_z;
};



/*********************************************************
 *                                                       * 
 * LbMatrix                                              * 
 *                                                       *
 * Definition of the 3-dimensional Matrix object         * 
 *                                                       *
 *********************************************************/
class LbMatrix 
{

 public:
  /* Constructors */
  LbMatrix();
  LbMatrix( float Values[3][3] );
  ~LbMatrix();

  /* Access */
  inline float getXY( int x, int y ) { return m_Values[y][x]; }
  float det();

#ifdef HAVE_MATRIX_DISPLAY
  void display();
#endif

  /* Scalar Operations */
  LbMatrix operator *( float f );
  LbMatrix operator /( float f );

  /* Matrix Operations */
  LbMatrix operator *( LbMatrix& M );
  LbVector operator *( LbVector& V );
  LbMatrix invert();
  LbMatrix transpose();
  LbMatrix applyChequerboard();

 private:
  float m_Values[3][3];
  inline float cofactor( int xa, int xb, int ya, int yb ) {
    return ( m_Values[ya][xa]*m_Values[yb][xb] -
	     m_Values[yb][xa]*m_Values[ya][xb] );
  }

};


/*********************************************************
 *                                                       * 
 * LbVector4                                             * 
 *                                                       *
 * Definition of the 4-dimensional Vector object         * 
 *                                                       *
 *********************************************************/
class LbVector4
{

public:

  /* {Con,De}structor methods */
  LbVector4();
  LbVector4( float x, float y, float z, float w );
  ~LbVector4();

  /* Access methods */
  inline float getW() { return m_w; };
  inline float getX() { return m_x; };
  inline float getY() { return m_y; };
  inline float getZ() { return m_z; };
  float mod();

#ifdef HAVE_VECTOR_TOSTRING
  void toString( char* rv );
#endif

  /* Scalar Arithmetic */
  LbVector4 operator *(float f);
  LbVector4 operator /(float f);

  /* Vector Arithmetic */
  LbVector4 operator +(LbVector4& v);
  LbVector4 operator -(LbVector4& v);

  /* Vector Multiplication */
  float operator ^(LbVector4& v) ; // . product 

  /* Matrix Multiplication */
  LbVector4 operator *(LbMatrix4& M);

  
private:

  /* The important bits */
  float m_w;
  float m_x;
  float m_y;
  float m_z;
};


/*********************************************************
 *                                                       * 
 * LbVector4                                             * 
 *                                                       *
 * Definition of the 4-dimensional Vector object         * 
 *                                                       *
 *********************************************************/
class LbMatrix4 {

 public:
  /* Constructors */
  LbMatrix4();
  LbMatrix4( float Values[4][4] );
  ~LbMatrix4();

  /* Access */
  inline float getXY( int x, int y ) { return m_Values[y][x]; }
  float det();
#ifdef HAVE_MATRIX_DISPLAY
  void display();
#endif

  /* Scalar Operations */
  LbMatrix4 operator *( float f );
  LbMatrix4 operator /( float f );

  /* Matrix Operations */
  LbVector4 operator *( LbVector4& V );
  LbMatrix4 operator *( LbMatrix4& M );
  LbMatrix4 invert();
  LbMatrix4 transpose();
  LbMatrix4 applyChequerboard();

 private:
  float m_Values[4][4];
  float cofactor( int x, int y );


};

#endif
/*********************************************************************************
    LBMath.h
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


/* Options */
#define HAVE_VECTOR_TOSTRING
#define HAVE_MATRIX_DISPLAY


/* Prototype Classes */
class LBVector;
class LBMatrix;
class LBVector4;
class LBMatrix4;
/*********************/


/*********************************************************
 *                                                       * 
 * LBVector                                              * 
 *                                                       *
 * Definition of the 3-dimensional Vector object         * 
 *                                                       *
 *********************************************************/
class LBVector 
{

public:

  /* {Con,De}structor methods */
  LBVector();
  LBVector( float _, float __, float ___ );
  ~LBVector();

  /* Access methods */
  inline float getX() { return m_x; };
  inline float getY() { return m_y; };
  inline float getZ() { return m_z; };
  float mod();

#ifdef HAVE_VECTOR_TOSTRING
  void toString( char* rv );
#endif


  /* Scalar Arithmetic */
  LBVector operator *(float f);
  LBVector operator /(float f);

  /* Vector Arithmetic */
  LBVector operator +(LBVector& v);
  LBVector operator -(LBVector& v);

  /* Vector Multiplication */
  float operator ^(LBVector& v) ; // . product 

  /* Matrix Multiplication */
  LBVector operator *(LBMatrix& M);

  
private:

  /* The important bits */
  float m_x;
  float m_y;
  float m_z;
};



/*********************************************************
 *                                                       * 
 * LBMatrix                                              * 
 *                                                       *
 * Definition of the 3-dimensional Matrix object         * 
 *                                                       *
 *********************************************************/
class LBMatrix 
{

 public:
  /* Constructors */
  LBMatrix();
  LBMatrix( float Values[3][3] );
  ~LBMatrix();

  /* Access */
  inline float getXY( int x, int y ) { return m_Values[y][x]; }
  float det();

#ifdef HAVE_MATRIX_DISPLAY
  void display();
#endif

  /* Scalar Operations */
  LBMatrix operator *( float f );
  LBMatrix operator /( float f );

  /* Matrix Operations */
  LBMatrix operator *( LBMatrix& M );
  LBVector operator *( LBVector& V );
  LBMatrix invert();
  LBMatrix transpose();
  LBMatrix applyChequerboard();

 private:
  float m_Values[3][3];
  inline float cofactor( int xa, int xb, int ya, int yb ) {
    return ( m_Values[ya][xa]*m_Values[yb][xb] -
	     m_Values[yb][xa]*m_Values[ya][xb] );
  }

};


/*********************************************************
 *                                                       * 
 * LBVector4                                             * 
 *                                                       *
 * Definition of the 4-dimensional Vector object         * 
 *                                                       *
 *********************************************************/
class LBVector4
{

public:

  /* {Con,De}structor methods */
  LBVector4();
  LBVector4( float x, float y, float z, float w );
  ~LBVector4();

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
  LBVector4 operator *(float f);
  LBVector4 operator /(float f);

  /* Vector Arithmetic */
  LBVector4 operator +(LBVector4& v);
  LBVector4 operator -(LBVector4& v);

  /* Vector Multiplication */
  float operator ^(LBVector4& v) ; // . product 

  /* Matrix Multiplication */
  LBVector4 operator *(LBMatrix4& M);

  
private:

  /* The important bits */
  float m_w;
  float m_x;
  float m_y;
  float m_z;
};


/*********************************************************
 *                                                       * 
 * LBVector4                                             * 
 *                                                       *
 * Definition of the 4-dimensional Vector object         * 
 *                                                       *
 *********************************************************/
class LBMatrix4 {

 public:
  /* Constructors */
  LBMatrix4();
  LBMatrix4( float Values[4][4] );
  ~LBMatrix4();

  /* Access */
  inline float getXY( int x, int y ) { return m_Values[y][x]; }
  float det();
#ifdef HAVE_MATRIX_DISPLAY
  void display();
#endif

  /* Scalar Operations */
  LBMatrix4 operator *( float f );
  LBMatrix4 operator /( float f );

  /* Matrix Operations */
  LBVector4 operator *( LBVector4& V );
  LBMatrix4 operator *( LBMatrix4& M );
  LBMatrix4 invert();
  LBMatrix4 transpose();
  LBMatrix4 applyChequerboard();

 private:
  float m_Values[4][4];
  float cofactor( int x, int y );


};

/*   Rising Tide exhibit                                                    */
/*   Copyright (C) 2010 Khairi Reda                                         */
/*   Electronic Visualization Laboratory                                    */
/*   University of Illinois at Chicago                                      */
/*   http://www.evl.uic.edu/                                                */
/*                                                                          */
/*   This is free software; you can redistribute it and/or modify it        */
/*   under the terms of the  GNU General Public License  as published by    */
/*   the  Free Software Foundation;  either version 2 of the License, or    */
/*   (at your option) any later version.                                    */
/*                                                                          */
/*   This program is distributed in the hope that it will be useful, but    */
/*   WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of    */
/*   MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU    */
/*   General Public License for more details.                               */
/* ======================================================================== */

#ifndef _VECTOR_MATH_H_
#define _VECTOR_MATH_H_

#include <map>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

using namespace std;

#define RADIANS(x) (x * (3.1415926535/180.0))
#define DEGREES(x) (x * (180.0/3.1415926535))

class Veci2D
{
public:
	int x, y;
	Veci2D(): x(0), y(0) {}
	Veci2D(int _x, int _y): x(_x), y(_y) {}
};

class Vec3D
{
public:
	double x, y, z;
	Vec3D(): x(0.0), y(0.0), z(0.0) {}
	Vec3D(double _x, double _y): x(_x), y(_y), z(0.0) {}
	Vec3D(double _x, double _y, double _z): x(_x), y(_y), z(_z) {}

	void zero() {
		x = y = z = 0.0;
	}
	
	bool operator== (const Vec3D & rhs) const {
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}
	
	Vec3D operator+(const Vec3D & rhs) const {
		return Vec3D( x+rhs.x, y+rhs.y, z+rhs.z );
	}

	Vec3D & operator+= (const Vec3D & rhs) {
		x += rhs.x; y += rhs.y; z += rhs.z;
		return *this;
	}
	
	Vec3D & operator-= (const Vec3D & rhs) {
		x -= rhs.x; y -= rhs.y; z -= rhs.z;
		return *this;
	}
	
	
	Vec3D & operator*=(float alpha) 
	{
		x *= alpha;
		y *= alpha;
		z *= alpha;
		return *this;
	}
	
	Vec3D operator-(const Vec3D & rhs) const {
		return Vec3D( x-rhs.x, y-rhs.y, z-rhs.z );
	}
	
	Vec3D operator / (double a) const {
		return Vec3D( x / a, y / a, z / a );
	}
	
	// dot product
	double operator * (const Vec3D & rhs) const
	{
		return x*rhs.x + y*rhs.y + z*rhs.z;
	}
	
	// scalar product
	Vec3D operator * (double scalar) const
	{
		return Vec3D(x*scalar, y*scalar , z*scalar);
	}
	
	// cross product
	// return the Y component only of the cross product
	// to determine sign of the angle
	double cross_y(const Vec3D & rhs) const
	{
		return -1.0 * (x*rhs.z - z*rhs.x);
	}
	
	double cross_x(const Vec3D & rhs) const
	{
		return y*rhs.z - z*rhs.y;
	}
	
	Vec3D cross(const Vec3D & rhs) const;
	
	void randomUnit()
	{
		x = (float) rand() / (float) RAND_MAX * (rand() % 2 == 0 ? 1.0 : -1.0);
		y = (float) rand() / (float) RAND_MAX * (rand() % 2 == 0 ? 1.0 : -1.0);
		z = (float) rand() / (float) RAND_MAX * (rand() % 2 == 0 ? 1.0 : -1.0);
		this->normalize();
	}
	
	float distance(const Vec3D &) const;
	float distanceSquared(const Vec3D &) const;
	
	double magnitude() const
	{
		return sqrt( x*x + y*y + z*z );
	}
	
	void add(const float v[3])
	{
		x += double(v[0]);
		y += double(v[1]);
		z += double(v[2]); 
	}

	void print() const { std::cout << x << ", " << y << ", " << z << std::endl; }	
	void set(double _x, double _y, double _z);
	void normalize();
	void clamp();
};

inline Vec3D operator*(double a, const Vec3D & rhs)
{
	return Vec3D(a * rhs.x, a * rhs.y, a * rhs.z);
}
inline double min(double a, double b)
{
	return a > b ? a : b;
}
inline float min(float a, float b)
{
	return a > b ? a : b;
}

class Interpolator;
class Interpolator
{
private:
	float start,current, target, alpha, interpolation_time, tolerance, low_limit, high_limit;
	bool interpolating, suddenDropB;
	float suddenDropVal;
	int id;
	
	// static member variables to keep track of all interpolators
	static int serial;
	static map<int, Interpolator *> allInterpolators;
public:
	Interpolator(
		float _interpolation_time = default_interp_time, 
		float _tolerance = default_interp_tolerance):
		suddenDropB(false),
		start(0.0),current(0.0), target(0.0), alpha(1.0), interpolating(false),
		interpolation_time(_interpolation_time), tolerance(_tolerance),
		high_limit(FLT_MAX), low_limit(-FLT_MAX)
		{
			id = ++serial;
			allInterpolators[id] = this;
		}
	
		
	~Interpolator()
	{
		// remove interpolators from list
		allInterpolators.erase( id );
	}
	
	void setTarget(float x);
	void tick(float dt);
	static void tickAll(float dt);
	void setCurrent(float x) { current=target=x; }
	float getCurrent() const { return current; }
	float getTarget() const { return target; }
	void setTolerance(float t) { tolerance = t; }
	void setSuddenDrop(float _v) { suddenDropVal = _v; suddenDropB = true; }
	float getTolerance() const { return tolerance; }
	bool isInterpolating() const { return interpolating; }

	void setLowLimit(float _limit) { low_limit = _limit; }
	void setHighLimit(float _limit) { high_limit = _limit; }
	
	const float & c() const { return current; }
	float & c() { return current; }
	
	// operators
	float operator+= (float x) { setTarget( target + x ); return target; }
	float operator*= (float x) { setTarget( target * x ); return target; }
	float operator/= (float x) { setTarget( target / x ); return target; }
	float operator= (float x) { setTarget(x); return target; }
	
	// constants
	static const float default_interp_tolerance;
	static const float default_interp_time;
	
};


// A 4-channel color class
class Color
{
private:
	float theColor[4];
	void translateChar(float * p, int c) { *p = (float) c / 255.0f; }
public:
	// copy constructor
	Color(const Color & rhs)
	{
		memcpy(theColor, rhs.theColor, 4*sizeof(float));
	}
	
	Color & operator=(const Color & rhs)
	{
		memcpy(theColor, rhs.theColor, 4*sizeof(float));
	}
	
	// other constructors
	Color() {
		theColor[0] = 0.0; theColor[1] =0.0; theColor[2] = 0.0; theColor[3] = 1.0; 
	}
	
	Color(float r, float g, float b) {
		theColor[0] = r; theColor[1] = g; theColor[2] = b; theColor[3] = 1.0; 
	}
	
	Color(float r, float g, float b, float a) {
		theColor[0] = r; theColor[1] = g; theColor[2] = b; theColor[3] = a; 
	}
	
	Color(int _r, int _g, int _b)
	{
		setR(_r); setG(_g); setB(_b); theColor[3] = 1.0;
	}
	
	Color(int _r, int _g, int _b, int _a)
	{
		setR(_r); setG(_g); setB(_b); setA(_a);
	}
	
	Color(unsigned char c)
	{
		setR(c);
		setG(c);
		setB(c);
		setA(255);
	}
	
	void setHTML(int c)
	{
		if (c > 0x00FFFFFF) {
			// 4 components
			setR( (0xFF000000 & c) >> 24 );
			setG( (0x00FF0000 & c) >> 16 );
			setB( (0x0000FF00 & c) >> 8  );
			setA(  0x000000FF & c);
		}
		else
		{
			setR( (0xFF0000 & c) >> 16 );
			setG( (0x00FF00 & c) >> 8  );
			setB(  0x0000FF & c);
			theColor[3] = 1.0f;
		}
	}
	
	
	Color(int c)
	{
		setHTML(c);
	}
	
	Color(int cc, int aa)
	{
		setHTML(cc);
		setA(aa);
	}
	
	
	const float * v() const { return theColor; }
	float & r() { return theColor[0]; }
	float & g() { return theColor[1]; }
	float & b() { return theColor[2]; }
	float & a() { return theColor[3]; }
	
	const float & r() const { return theColor[0]; }
	const float & g() const { return theColor[1]; }
	const float & b() const { return theColor[2]; }
	const float & a() const { return theColor[3]; }
	
	void setR(int _r) { translateChar(theColor  , _r); }
	void setG(int _g) { translateChar(theColor+1, _g); }
	void setB(int _b) { translateChar(theColor+2, _b); }
	void setA(int _a) { translateChar(theColor+3, _a); }
};


inline float clamp(float x)
{
	return x > 1.0f ? 1.0f : x < 0.0f ? 0.0f : x;
}

#ifdef WIN32
inline double LOG2( double n )   
{   
    // log(n)/log(2) is log2. 
	static const double l_2 = log( 2.0 );
    return log( n ) / l_2;   
}
#else
inline double LOG2( double n )
{
	return log2( n );
}
#endif

float pixelIntensity(const unsigned char * p);

#endif



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

#include <math.h>
#include "vector_math.h"

void Vec3D::set(double _x, double _y, double _z)
{
	x = _x;
	y = _y;
	z = _z;
}

void Vec3D::clamp()
{
	if (x < 0.0) x = 0.0; else if (x > 1.0) x = 1.0;
	if (y < 0.0) y = 0.0; else if (y > 1.0) y = 1.0;
	if (z < 0.0) z = 0.0; else if (z > 1.0) z = 1.0;
}

void Vec3D::normalize()
{
	double k = sqrt(x*x + y*y + z*z);
	if (k > 0.0) {
		k = 1.0 / k;
		x *= k;
		y *= k;
		z *= k;
	}
}

float Vec3D::distance(const Vec3D & other) const
{
	float dX = x - other.x;
	float dY = y - other.y;
	float dZ = z - other.z;
	
	return sqrt( dX*dX + dY*dY + dZ*dZ );
}

Vec3D Vec3D::cross(const Vec3D & rhs) const
{
	Vec3D n;
	n.x = this->y * rhs.z - this->z * rhs.y;
	n.y = this->z * rhs.x - this->x * rhs.z;
	n.z = this->x * rhs.y - this->y * rhs.x;
	
	return n;
}

float Vec3D::distanceSquared(const Vec3D & other) const
{
	float dX = x - other.x;
	float dY = y - other.y;
	float dZ = z - other.z;
	
	return ( dX*dX + dY*dY + dZ*dZ );
}

void Interpolator::setTarget(float x)
{
	if (x > high_limit) {
		x = high_limit;
	}
	else if (x < low_limit) {
		x = low_limit;
	}
	
	if (fabs(target - x) > tolerance) {
		target = x;
		alpha /= 4.0;
		interpolating = true;
	}
	suddenDropB = false;
	start = current;
}

void Interpolator::tick(float dt)
{
	if (! interpolating ) {
		alpha = 0.0;
	}
	else
	{
		alpha += dt / interpolation_time;
		alpha = clamp(alpha);
		
		current = (1.0f - alpha) * current + alpha * target;
		
		if (alpha >= 1.0) {
			interpolating = false;
			if (suddenDropB) {
				suddenDropB = false;
				current = suddenDropVal;
			}
		}
	}
}

void Interpolator::tickAll(float dt)
{
	map<int, Interpolator *>::iterator it = allInterpolators.begin();
	for (; it != allInterpolators.end(); it++)
	{
		it->second->tick(dt);
	}
}


float pixelIntensity(const unsigned char * p)
{
	float I = 0.0;
	I += (float) p[0] / 255.0;
	I += (float) p[1] / 255.0;
	I += (float) p[2] / 255.0;
	
	return I;
}


// initialize static variables
const float Interpolator::default_interp_tolerance = 0.001f;
const float Interpolator::default_interp_time = .7f;
int Interpolator::serial = 0;
map<int, Interpolator *> Interpolator::allInterpolators;

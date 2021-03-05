#ifndef VEC_H
#define VEC_H

#define PI 3.14159
#define DEG2RAD PI/180.0
#define RAD2DEG 180.0/PI
#define RAYLEN 30

#include <iostream>
#include <cmath>

// General normalization for 2d object
inline void normalize(float v[2], const float len)
{
	if(len == 0.0f) {
		std::cout << "Couldn't normalize. Divide by zero error\n";
		return;
	}
	v[0] = v[0] / len;
	v[1] = v[1] / len;
}

class Vec2f {
	public:
	float pos[2];
	float length;
	float angle;

	inline Vec2f()
	{
		this->pos[0] = 0.0f;
		this->pos[1] = 0.0f;
		this->length = 0.0f;
		this->angle = 0.0f;
	}
	inline Vec2f(const float p[2])
	{
		this->pos[0] = p[0];
		this->pos[1] = p[1];
		this->length = sqrt(this->pos[0]*this->pos[0] + this->pos[1]*this->pos[1]);
		this->angle = atan(this->pos[1]/this->pos[0])*RAD2DEG;
	}
	inline Vec2f(const Vec2f& v)
	{
		this->pos[0] = v.pos[0];
		this->pos[1] = v.pos[1];
		this->length = sqrt(this->pos[0]*this->pos[0] + this->pos[1]*this->pos[1]);
		this->angle = atan(this->pos[1]/this->pos[0])*RAD2DEG;
	}
	inline Vec2f(const float angle, const float len)
	{
		this->length = len;
		this->angle = angle;
		this->pos[0] = len * cos(angle*DEG2RAD);
		this->pos[1] = len * sin(angle*DEG2RAD);
	}
	
	inline Vec2f operator+(const Vec2f& v)
	{
		float xsum = this->pos[0] + v.pos[0];
		float ysum = this->pos[1] + v.pos[1];
		float res[] = {xsum, ysum};

		return Vec2f (res);
	}
	inline Vec2f operator+=(const Vec2f& v)
	{
		this->pos[0] += v.pos[0];
		this->pos[1] += v.pos[1];
		length = sqrt(this->pos[0]*this->pos[0] + this->pos[1]*this->pos[1]);
		this->angle = atan(this->pos[1]/this->pos[0])*RAD2DEG;
	}
	inline bool operator==(const Vec2f& v)
	{
		if(angle == v.angle && length == v.length)
			return true;
		else
			return false;
	}
	inline Vec2f operator-(const Vec2f& v)
	{
		float xdiff = this->pos[0] - v.pos[0];
		float ydiff = this->pos[1] - v.pos[1];
		float res[] = {xdiff, ydiff};

		return Vec2f (res);
	}
	inline Vec2f operator*(float scalar)
	{
		float scalarx = this->pos[0] * scalar;
		float scalary = this->pos[1] * scalar;
		float res[] = {scalarx, scalary};

		return Vec2f (res);
	}
	inline Vec2f operator=(const Vec2f& v)
	{
		this->pos[0] = v.pos[0];
		this->pos[1] = v.pos[1];
		this->length = sqrt(this->pos[0]*this->pos[0] + this->pos[1]*this->pos[1]);
		this->angle = atan(this->pos[1]/this->pos[0])*RAD2DEG;
	}
	inline friend std::ostream& operator<<(std::ostream& os, const Vec2f& v)
	{
		os << "x: " << v.pos[0] << ", y: " << v.pos[1] << ", length: " << v.length
		   << ", angle: " << v.angle;
		return os;
	}
	inline void setPos(float* p)
	{
		this->pos[0] = p[0];
		this->pos[1] = p[1];
		this->length = sqrt(this->pos[0]*this->pos[0] + this->pos[1]*this->pos[1]);
		this->angle = atan(this->pos[1]/this->pos[0])*RAD2DEG;
	}
	inline void setLength()
	{
		length = sqrt(this->pos[0]*this->pos[0] + this->pos[1]*this->pos[1]);
	}
	inline void setAngle()
	{
		this->angle = atan(this->pos[1]/this->pos[0])*RAD2DEG;
	}
};

inline const float dot(const float v1[2], const float v2[2])
{
	// 1 if parallel
	// 0 if perpendicular
	// -1 if opposite directions
	float res = v1[0] * v2[0] + v1[1] * v2[1];
	return res;
}
inline float cross(const float v1[2], const float v2[2])
{
	// Cross product
	float res = v1[0] * v2[1] - v1[1] * v2[0];

	return res;
}
// Set vector length to 1
inline void normalize(Vec2f& v)
{
	if(v.length == 0.0f) {
		std::cout << "Couldn't normalize. Divide by zero error\n";
		return;
	}
	v.pos[0] = v.pos[0] / v.length;
	v.pos[1] = v.pos[1] / v.length;
	v.setLength();
	v.setAngle();
}

#endif
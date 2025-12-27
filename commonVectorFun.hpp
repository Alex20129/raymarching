#ifndef COMMONVECTORFUN_HPP
#define COMMONVECTORFUN_HPP

#include <math.h>
#include <stdint.h>

//=== Vec2 double

class Vec2d
{
public:
	double X, Y;
	Vec2d();
	Vec2d(const Vec2d *other);
	Vec2d(const Vec2d &other);
	Vec2d(double x, double y);
	void Normalize(double length=1.0);
	Vec2d Abs();
	double Length();
	double LengthSquared();
	double Dot(const Vec2d &other);
	Vec2d Max(const Vec2d &other);
	Vec2d operator=(const Vec2d &other);
	Vec2d operator+(const Vec2d &other);
	void operator+=(const Vec2d &other);
	Vec2d operator-(const Vec2d &other);
	Vec2d operator-();
	Vec2d operator/(double d);
	Vec2d operator*(const Vec2d &other);
};

Vec2d operator*(const Vec2d &vec, const double m);
Vec2d operator*(const double m, const Vec2d &vec);
Vec2d operator/(const Vec2d &vec, const double d);

//=== Vec3 double

class Vec3d
{
public:
	double X, Y, Z;
	Vec3d();
	Vec3d(const Vec3d *other);
	Vec3d(const Vec3d &other);
	Vec3d(double x, double y, double z);
	void Normalize(double length=1.0);
	Vec3d Abs();
	double Length();
	double LengthSquared();
	double Dot(const Vec3d &other);
	Vec3d Max(const Vec3d &other);
	Vec3d operator=(const Vec3d &other);
	Vec3d operator+(const Vec3d &other);
	void operator+=(const Vec3d &other);
	Vec3d operator-(const Vec3d &other);
	Vec3d operator-();
	Vec3d operator/(double d);
	Vec3d operator*(const Vec3d &other);
};

Vec3d operator*(const Vec3d &vec, const double m);
Vec3d operator*(const double m, const Vec3d &vec);

//=== Vec3 float

class Vec3f
{
public:
	float X, Y, Z;
	Vec3f();
	Vec3f(const Vec3f *other);
	Vec3f(const Vec3f &other);
	Vec3f(float x, float y, float z);
	void Normalize(float length=1.0);
	Vec3f Abs();
	float Length();
	float LengthSquared();
	float Dot(const Vec3f &other);
	Vec3f operator=(const Vec3f &other);
	Vec3f operator+(const Vec3f &other);
	void operator+=(const Vec3f &other);
	Vec3f operator-(const Vec3f &other);
	Vec3f operator-();
	Vec3f operator/(float d);
	Vec3f operator*(const Vec3f &other);
};

Vec3f operator*(const Vec3f &vec, const float m);
Vec3f operator*(const float m, const Vec3f &vec);

//=== Vec3 uchar

class Vec3uc
{
public:
	uint8_t X, Y, Z;
	Vec3uc();
	Vec3uc(const Vec3uc *vec);
	Vec3uc(const Vec3uc &vec);
	Vec3uc(uint8_t x, uint8_t y, uint8_t z);
	void Normalize(uint8_t length=1);
	uint8_t Length();
	uint8_t Dot(const Vec3uc &other);
	Vec3uc Max(const Vec3uc &other);
	Vec3uc operator=(const Vec3uc &other);
	Vec3uc operator+(const Vec3uc &other);
	void operator+=(const Vec3uc &other);
	Vec3uc operator-(const Vec3uc &other);
	Vec3uc operator-();
	Vec3uc operator/(char m);
	Vec3uc operator/(float m);
	Vec3uc operator/(double m);
	Vec3uc operator*(const Vec3uc &other);
	Vec3uc operator*(char m);
	Vec3uc operator*(float m);
	Vec3uc operator*(double m);
};

#endif // COMMONVECTORFUN_HPP

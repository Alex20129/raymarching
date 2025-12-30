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
	double Dot(const Vec2d &other) const;
	double Cross(const Vec2d &other) const;
	Vec2d Max(const Vec2d &other) const;
	Vec2d operator=(const Vec2d &other);
	Vec2d operator+(const Vec2d &other) const;
	void operator+=(const Vec2d &other);
	Vec2d operator-(const Vec2d &other) const;
	Vec2d operator-() const;
	Vec2d operator/(double d) const;
	Vec2d operator*(const Vec2d &other) const;
};

Vec2d operator*(const Vec2d &vec, const double m);
Vec2d operator*(const double m, const Vec2d &vec);

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
	double Dot(const Vec3d &other) const;
	Vec3d Cross(const Vec3d &other) const;
	Vec3d Max(const Vec3d &other) const;
	Vec3d operator=(const Vec3d &other);
	Vec3d operator+(const Vec3d &other) const;
	void operator+=(const Vec3d &other);
	Vec3d operator-(const Vec3d &other) const;
	Vec3d operator-() const;
	Vec3d operator/(double d) const;
	Vec3d operator*(const Vec3d &other) const;
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
	float Dot(const Vec3f &other) const;
	Vec3f Cross(const Vec3f &other) const;
	Vec3f Max(const Vec3f &other) const;
	Vec3f operator=(const Vec3f &other);
	Vec3f operator+(const Vec3f &other) const;
	void operator+=(const Vec3f &other);
	Vec3f operator-(const Vec3f &other) const;
	Vec3f operator-() const;
	Vec3f operator/(float d) const;
	Vec3f operator*(const Vec3f &other) const;
};

Vec3f operator*(const Vec3f &vec, const float m);
Vec3f operator*(const float m, const Vec3f &vec);

#endif // COMMONVECTORFUN_HPP

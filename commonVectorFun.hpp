#ifndef COMMONVECTORFUN_HPP
#define COMMONVECTORFUN_HPP

//=== Vec2 double

class Vec2d
{
public:
	double X, Y;
	Vec2d();
	Vec2d(const Vec2d *other);
	Vec2d(const Vec2d &other);
	Vec2d(double x, double y);
	Vec2d Normal(double length=1.0);
	void Normalize();
	Vec2d Abs();
	double Length();
	Vec2d Max(const Vec2d &other);
	Vec2d operator=(const Vec2d &other);
	Vec2d operator+(const Vec2d &other);
	Vec2d operator-(const Vec2d &other);
	Vec2d operator-();
	double operator*(const Vec2d &other);
	Vec2d operator*(double m);
	Vec2d operator/(double m);
};

//=== Vec3 double

class Vec3d
{
public:
	double X, Y, Z;
	Vec3d();
	Vec3d(const Vec3d *other);
	Vec3d(const Vec3d &other);
	Vec3d(double x, double y, double z);
	Vec3d Normal(double length=1.0);
	void Normalize();
	Vec3d Abs();
	double Length();
	Vec3d Max(const Vec3d &other);
	Vec3d operator=(const Vec3d &other);
	Vec3d operator+(const Vec3d &other);
	Vec3d operator-(const Vec3d &other);
	Vec3d operator-();
	double operator*(const Vec3d &other);
	Vec3d operator*(double m);
	Vec3d operator/(double m);
};

//=== Vec3 float

class Vec3f
{
public:
	float X, Y, Z;
	Vec3f();
	Vec3f(const Vec3f *other);
	Vec3f(const Vec3f &other);
	Vec3f(float x, float y, float z);
	Vec3f Normal(float length=1.0);
	void Normalize();
	Vec3f Abs();
	float Length();
	Vec3f operator=(const Vec3f &other);
	Vec3f operator+(const Vec3f &other);
	Vec3f operator-(const Vec3f &other);
	Vec3f operator-();
	float operator*(const Vec3f &other);
	Vec3f operator*(float m);
	Vec3f operator/(float m);
};

//=== Vec3 uchar

class Vec3uc
{
public:
	unsigned char X, Y, Z;
	Vec3uc();
	Vec3uc(const Vec3uc *vec);
	Vec3uc(const Vec3uc &vec);
	Vec3uc(unsigned char x, unsigned char y, unsigned char z);
	Vec3uc Normal(unsigned char length=1);
	unsigned char Length();
	Vec3uc operator=(const Vec3uc &other);
	Vec3uc operator+(const Vec3uc &other);
	Vec3uc operator-(const Vec3uc &other);
	Vec3uc operator-();
	unsigned char operator*(const Vec3uc &other);
	Vec3uc operator*(char m);
	Vec3uc operator*(float m);
	Vec3uc operator*(double m);
	Vec3uc operator/(char m);
	Vec3uc operator/(float m);
	Vec3uc operator/(double m);
};

#endif // COMMONVECTORFUN_HPP

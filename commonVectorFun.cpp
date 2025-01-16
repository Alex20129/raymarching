#include <cmath>
#include "commonVectorFun.hpp"

//=== Vec2 double

Vec2d::Vec2d()
{
	X=Y=0.0;
}

Vec2d::Vec2d(const Vec2d *other)
{
	if(other)
	{
		X=other->X, Y=other->Y;
	}
	else
	{
		X=Y=0.0;
	}
}

Vec2d::Vec2d(const Vec2d &other)
{
	X=other.X, Y=other.Y;
}

Vec2d::Vec2d(double x, double y)
{
	X=x, Y=y;
}

Vec2d Vec2d::Normal(double length)
{
	Vec2d res(this);
	double len=this->Length();
	len/=length;
	res.X/=len;
	res.Y/=len;
	return(res);
}

void Vec2d::Normalize()
{
	double length=this->Length();
	this->X/=length;
	this->Y/=length;
}

Vec2d Vec2d::Abs()
{
	Vec2d res(std::abs(X), std::abs(Y));
	return(res);
}

double Vec2d::Length()
{
	return(std::sqrt(X*X+Y*Y));
}

Vec2d Vec2d::Max(const Vec2d &other)
{
	Vec2d res(std::fmax(this->X, other.X),
			  std::fmax(this->Y, other.Y));
	return(res);
}

Vec2d Vec2d::operator=(const Vec2d &other)
{
	X=other.X;
	Y=other.Y;
	return(*this);
}

Vec2d Vec2d::operator+(const Vec2d &other)
{
	Vec2d res(this);
	res.X+=other.X;
	res.Y+=other.Y;
	return res;
}

Vec2d Vec2d::operator-(const Vec2d &other)
{
	Vec2d res(this);
	res.X-=other.X;
	res.Y-=other.Y;
	return res;
}

Vec2d Vec2d::operator-()
{
	return Vec2d(-X, -Y);
}

Vec2d Vec2d::operator/(double m)
{
	Vec2d res(this);
	m=m?m:1;
	res.X/=m;
	res.Y/=m;
	return res;
}

double operator*(const Vec2d &vec_a, const Vec2d &vec_b)
{
	return(vec_a.X*vec_b.X+vec_a.Y*vec_b.Y);
}

Vec2d operator*(const Vec2d &vec, const double m)
{
	return(Vec2d(vec.X*m, vec.Y*m));
}

Vec2d operator*(const double m, const Vec2d &vec)
{
	return(Vec2d(vec.X*m, vec.Y*m));
}

//=== Vec3 double

Vec3d::Vec3d()
{
	X=Y=Z=0.0;
}

Vec3d::Vec3d(const Vec3d *other)
{
	if(other)
	{
		X=other->X, Y=other->Y, Z=other->Z;
	}
	else
	{
		X=Y=Z=0.0;
	}
}

Vec3d::Vec3d(const Vec3d &other)
{
	X=other.X, Y=other.Y, Z=other.Z;
}

Vec3d::Vec3d(double x, double y, double z)
{
	X=x, Y=y, Z=z;
}

Vec3d Vec3d::Normal(double length)
{
	Vec3d res(this);
	double len=this->Length();
	len/=length;
	res.X/=len;
	res.Y/=len;
	res.Z/=len;
	return(res);
}

void Vec3d::Normalize()
{
	double length=this->Length();
	this->X/=length;
	this->Y/=length;
	this->Z/=length;
}

Vec3d Vec3d::Abs()
{
	Vec3d res(std::abs(X), std::abs(Y), std::abs(Z));
	return(res);
}

double Vec3d::Length()
{
	return(std::sqrt(X*X+Y*Y+Z*Z));
}

Vec3d Vec3d::Max(const Vec3d &other)
{
	Vec3d res(std::fmax(this->X, other.X),
			  std::fmax(this->Y, other.Y),
			  std::fmax(this->Z, other.Z));
	return(res);
}

Vec3d Vec3d::operator=(const Vec3d &other)
{
	X=other.X;
	Y=other.Y;
	Z=other.Z;
	return(*this);
}

Vec3d Vec3d::operator+(const Vec3d &other)
{
	Vec3d res(this);
	res.X+=other.X;
	res.Y+=other.Y;
	res.Z+=other.Z;
	return res;
}

Vec3d Vec3d::operator-(const Vec3d &other)
{
	Vec3d res(this);
	res.X-=other.X;
	res.Y-=other.Y;
	res.Z-=other.Z;
	return res;
}

Vec3d Vec3d::operator-()
{
	return Vec3d(-X, -Y, -Z);
}

Vec3d Vec3d::operator/(double m)
{
	Vec3d res(this);
	m=m?m:1;
	res.X/=m;
	res.Y/=m;
	res.Z/=m;
	return res;
}

double operator*(const Vec3d &vec_a, const Vec3d &vec_b)
{
	return(vec_a.X*vec_b.X+vec_a.Y*vec_b.Y+vec_a.Z*vec_b.Z);
}

Vec3d operator*(const Vec3d &vec, const double m)
{
	return(Vec3d(vec.X*m, vec.Y*m, vec.Z*m));
}

Vec3d operator*(const double m, const Vec3d &vec)
{
	return(Vec3d(vec.X*m, vec.Y*m, vec.Z*m));
}

//=== Vec3 float

Vec3f::Vec3f()
{
	X=Y=Z=0;
}

Vec3f::Vec3f(const Vec3f *other)
{
	if(other)
	{
		X=other->X, Y=other->Y, Z=other->Z;
	}
	else
	{
		X=Y=Z=0.0;
	}
}

Vec3f::Vec3f(const Vec3f &other)
{
	X=other.X, Y=other.Y, Z=other.Z;
}

Vec3f::Vec3f(float x, float y, float z)
{
	X=x, Y=y, Z=z;
}

Vec3f Vec3f::Normal(float length)
{
	Vec3f res(this);
	float len=this->Length();
	len/=length;
	res.X/=len;
	res.Y/=len;
	res.Z/=len;
	return(res);
}

void Vec3f::Normalize()
{
	float length=this->Length();
	this->X/=length;
	this->Y/=length;
	this->Z/=length;
}

Vec3f Vec3f::Abs()
{
	Vec3f res(std::abs(X), std::abs(Y), std::abs(Z));
	return(res);
}

float Vec3f::Length()
{
	return(std::sqrt(X*X+Y*Y+Z*Z));
}

Vec3f Vec3f::operator=(const Vec3f &other)
{
	X=other.X;
	Y=other.Y;
	Z=other.Z;
	return(*this);
}

Vec3f Vec3f::operator+(const Vec3f &other)
{
	Vec3f res(this);
	res.X+=other.X;
	res.Y+=other.Y;
	res.Z+=other.Z;
	return res;
}

Vec3f Vec3f::operator-(const Vec3f &other)
{
	Vec3f res(this);
	res.X-=other.X;
	res.Y-=other.Y;
	res.Z-=other.Z;
	return res;
}

Vec3f Vec3f::operator-()
{
	return Vec3f(-X, -Y, -Z);
}

Vec3f Vec3f::operator/(float m)
{
	Vec3f res(this);
	m=m?m:1;
	res.X/=m;
	res.Y/=m;
	res.Z/=m;
	return(res);
}

float operator*(const Vec3f &vec_a, const Vec3f &vec_b)
{
	return(vec_a.X*vec_b.X+vec_a.Y*vec_b.Y+vec_a.Z*vec_b.Z);
}

Vec3f operator*(const Vec3f &vec, const float m)
{
	return(Vec3f(vec.X*m, vec.Y*m, vec.Z*m));
}

Vec3f operator*(const float m, const Vec3f &vec)
{
	return(Vec3f(vec.X*m, vec.Y*m, vec.Z*m));
}

//=== Vec3 uchar

Vec3uc::Vec3uc()
{
	X=Y=Z=0;
}

Vec3uc::Vec3uc(const Vec3uc *vec)
{
	X=vec->X, Y=vec->Y, Z=vec->Z;
}

Vec3uc::Vec3uc(const Vec3uc &vec)
{
	X=vec.X, Y=vec.Y, Z=vec.Z;
}

Vec3uc::Vec3uc(unsigned char x, unsigned char y, unsigned char z)
{
	X=x, Y=y, Z=z;
}

Vec3uc Vec3uc::Normal(unsigned char length)
{
	Vec3uc res(this);
	unsigned char len=this->Length();
	len/=length;
	len=len?len:1;
	res.X/=len;
	res.Y/=len;
	res.Z/=len;
	return(res);
}

unsigned char Vec3uc::Length()
{
	return(std::sqrt(X*X+Y*Y+Z*Z));
}

Vec3uc Vec3uc::operator=(const Vec3uc &other)
{
	X=other.X;
	Y=other.Y;
	Z=other.Z;
	return(*this);
}

Vec3uc Vec3uc::operator+(const Vec3uc &other)
{
	Vec3uc res(this);
	res.X+=other.X;
	res.Y+=other.Y;
	res.Z+=other.Z;
	return res;
}

Vec3uc Vec3uc::operator-(const Vec3uc &other)
{
	Vec3uc res(this);
	res.X-=other.X;
	res.Y-=other.Y;
	res.Z-=other.Z;
	return res;
}

Vec3uc Vec3uc::operator-()
{
	return Vec3uc(-X, -Y, -Z);
}

unsigned char Vec3uc::operator*(const Vec3uc &other)
{
	unsigned char res;
	res=other.X*X+other.Y*Y+other.Z*Z;
	return res;
}

Vec3uc Vec3uc::operator*(char m)
{
	Vec3uc res(this);
	res.X*=m;
	res.Y*=m;
	res.Z*=m;
	return res;
}

Vec3uc Vec3uc::operator*(float m)
{
	Vec3uc res(this);
	res.X*=m;
	res.Y*=m;
	res.Z*=m;
	return res;
}

Vec3uc Vec3uc::operator*(double m)
{
	Vec3uc res(this);
	res.X*=m;
	res.Y*=m;
	res.Z*=m;
	return res;
}

Vec3uc Vec3uc::operator/(char m)
{
	Vec3uc res(this);
	m=m?m:1;
	res.X/=m;
	res.Y/=m;
	res.Z/=m;
	return res;
}

Vec3uc Vec3uc::operator/(float m)
{
	Vec3uc res(this);
	m=m?m:1;
	res.X/=m;
	res.Y/=m;
	res.Z/=m;
	return res;
}

Vec3uc Vec3uc::operator/(double m)
{
	Vec3uc res(this);
	m=m?m:1;
	res.X/=m;
	res.Y/=m;
	res.Z/=m;
	return res;
}

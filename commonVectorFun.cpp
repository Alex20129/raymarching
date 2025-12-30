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

void Vec2d::Normalize(double length)
{
	double len=this->Length()/length;
	this->X/=len;
	this->Y/=len;
}

Vec2d Vec2d::Abs()
{
	Vec2d res(std::abs(X), std::abs(Y));
	return(res);
}

double Vec2d::Length()
{
	double res=std::sqrt(X*X+Y*Y);
	return(res);
}

double Vec2d::LengthSquared()
{
	double res=X*X+Y*Y;
	return(res);
}

double Vec2d::Dot(const Vec2d &other) const
{
	double res=this->X*other.X+this->Y*other.Y;
	return(res);
}

double Vec2d::Cross(const Vec2d &other) const
{
	double res=this->X*other.Y-this->Y*other.X;
	return(res);
}

Vec2d Vec2d::Max(const Vec2d &other) const
{
	Vec2d res(
		std::fmax(this->X, other.X),
		std::fmax(this->Y, other.Y));
	return(res);
}

Vec2d Vec2d::operator=(const Vec2d &other)
{
	this->X=other.X;
	this->Y=other.Y;
	return(*this);
}

Vec2d Vec2d::operator+(const Vec2d &other) const
{
	Vec2d res(this->X+other.X, this->Y+other.Y);
	return(res);
}

void Vec2d::operator+=(const Vec2d &other)
{
	this->X+=other.X;
	this->Y+=other.Y;
}

Vec2d Vec2d::operator-(const Vec2d &other) const
{
	Vec2d res(this);
	res.X-=other.X;
	res.Y-=other.Y;
	return(res);
}

Vec2d Vec2d::operator-() const
{
	Vec2d res(-X, -Y);
	return(res);
}

Vec2d Vec2d::operator/(double d) const
{
	Vec2d res(X/d, Y/d);
	return(res);
}

Vec2d Vec2d::operator*(const Vec2d &other) const
{
	Vec2d res(this->X*other.X, this->Y*other.Y);
	return res;
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

void Vec3d::Normalize(double length)
{
	double len=this->Length()/length;
	this->X/=len;
	this->Y/=len;
	this->Z/=len;
}

Vec3d Vec3d::Abs()
{
	Vec3d res(std::abs(X), std::abs(Y), std::abs(Z));
	return(res);
}

double Vec3d::Length()
{
	double res=std::sqrt(X*X+Y*Y+Z*Z);
	return(res);
}

double Vec3d::LengthSquared()
{
	double res=X*X+Y*Y+Z*Z;
	return(res);
}

double Vec3d::Dot(const Vec3d &other) const
{
	double res=this->X*other.X+this->Y*other.Y+this->Z*other.Z;
	return(res);
}

Vec3d Vec3d::Cross(const Vec3d &other) const
{
	Vec3d res(
		this->Y*other.Z-this->Z*other.Y,
		this->Z*other.X-this->X*other.Z,
		this->X*other.Y-this->Y*other.X);
	return(res);
}

Vec3d Vec3d::Max(const Vec3d &other) const
{
	Vec3d res(
		std::fmax(this->X, other.X),
		std::fmax(this->Y, other.Y),
		std::fmax(this->Z, other.Z));
	return(res);
}

Vec3d Vec3d::operator=(const Vec3d &other)
{
	this->X=other.X;
	this->Y=other.Y;
	this->Z=other.Z;
	return(*this);
}

Vec3d Vec3d::operator+(const Vec3d &other) const
{
	Vec3d res(this->X+other.X, this->Y+other.Y, this->Z+other.Z);
	return(res);
}

void Vec3d::operator+=(const Vec3d &other)
{
	this->X+=other.X;
	this->Y+=other.Y;
	this->Z+=other.Z;
}

Vec3d Vec3d::operator-(const Vec3d &other) const
{
	Vec3d res(this);
	res.X-=other.X;
	res.Y-=other.Y;
	res.Z-=other.Z;
	return(res);
}

Vec3d Vec3d::operator-() const
{
	Vec3d res(-X, -Y, -Z);
	return(res);
}

Vec3d Vec3d::operator/(double d) const
{
	Vec3d res(X/d, Y/d, Z/d);
	return(res);
}

Vec3d Vec3d::operator*(const Vec3d &other) const
{
	Vec3d res(this->X*other.X, this->Y*other.Y, this->Z*other.Z);
	return(res);
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

void Vec3f::Normalize(float length)
{
	float len=this->Length()/length;
	this->X/=len;
	this->Y/=len;
	this->Z/=len;
}

Vec3f Vec3f::Abs()
{
	Vec3f res(std::abs(X), std::abs(Y), std::abs(Z));
	return(res);
}

float Vec3f::Length()
{
	float res=std::sqrt(X*X+Y*Y+Z*Z);
	return(res);
}

float Vec3f::LengthSquared()
{
	float res=X*X+Y*Y+Z*Z;
	return(res);
}

float Vec3f::Dot(const Vec3f &other) const
{
	float res=this->X*other.X+this->Y*other.Y+this->Z*other.Z;
	return(res);
}

Vec3f Vec3f::Cross(const Vec3f &other) const
{
	Vec3f res(
		this->Y*other.Z-this->Z*other.Y,
		this->Z*other.X-this->X*other.Z,
		this->X*other.Y-this->Y*other.X);
	return(res);
}

Vec3f Vec3f::Max(const Vec3f &other) const
{
	Vec3f res(
		std::fmax(this->X, other.X),
		std::fmax(this->Y, other.Y),
		std::fmax(this->Z, other.Z));
	return(res);
}

Vec3f Vec3f::operator=(const Vec3f &other)
{
	this->X=other.X;
	this->Y=other.Y;
	this->Z=other.Z;
	return(*this);
}

Vec3f Vec3f::operator+(const Vec3f &other) const
{
	Vec3f res(this->X+other.X, this->Y+other.Y, this->Z+other.Z);
	return(res);
}

void Vec3f::operator+=(const Vec3f &other)
{
	this->X+=other.X;
	this->Y+=other.Y;
	this->Z+=other.Z;
}

Vec3f Vec3f::operator-(const Vec3f &other) const
{
	Vec3f res(this);
	res.X-=other.X;
	res.Y-=other.Y;
	res.Z-=other.Z;
	return res;
}

Vec3f Vec3f::operator-() const
{
	Vec3f res(-X, -Y, -Z);
	return(res);
}

Vec3f Vec3f::operator/(float d) const
{
	Vec3f res(X/d, Y/d, Z/d);
	return(res);
}

Vec3f Vec3f::operator*(const Vec3f &other) const
{
	Vec3f res(this->X*other.X, this->Y*other.Y, this->Z*other.Z);
	return(res);
}

Vec3f operator*(const Vec3f &vec, const float m)
{
	return(Vec3f(vec.X*m, vec.Y*m, vec.Z*m));
}

Vec3f operator*(const float m, const Vec3f &vec)
{
	return(Vec3f(vec.X*m, vec.Y*m, vec.Z*m));
}

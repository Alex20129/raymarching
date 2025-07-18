#ifndef BASICOBJECTS_HPP
#define BASICOBJECTS_HPP

#include <vector>
#include <string>
#include "commonVectorFun.hpp"
#include "prng.hpp"

#define RAY_STEPS_MAX			1024
#define RAY_COLLISIONS_MAX		5
#define RAY_COLLISION_THRESHOLD	1.0/8.0
#define RAY_COLLISION_STEPOUT	1.0/4.0
#define NORMAL_CALCULATION_D	1.0/16.0

using namespace std;

class Object
{
	bool pVisible;
	double pBrightness;
	double pReflectivity;
	string *pName;
protected:
	Vec3f pColor;
	Vec3d pPosition;
public:
	Object();
	bool Visible() const;
	void SetVisible(bool visible);

	double Brightness() const;
	void SetBrightness(double brightness);

	double Reflectivity();
	void SetReflectivity(double reflectivity);

	Vec3f Color() const;
	void SetColor(Vec3f color);
	void SetColor(float r, float g, float b);

	Vec3d Position() const;
	void SetPosition(Vec3d position);
	void SetPosition(double x, double y, double z);

	string Name();
	void SetName(string name);

	virtual double GetDistance(Vec3d from) const;
	Vec3d GetNormalVector(Vec3d point) const;

	vector <Object *> *SceneObjects;
};

class Difference : public Object
{
	Object *ObjectA, *ObjectB;
public:
	Difference(Object *object_a, Object *object_b);
	double GetDistance(Vec3d from) const;
};

class Union : public Object
{
	Object *ObjectA, *ObjectB;
public:
	Union(Object *object_a, Object *object_b);
	double GetDistance(Vec3d from) const;
};

class Intersection : public Object
{
	Object *ObjectA, *ObjectB;
public:
	Intersection(Object *object_a, Object *object_b);
	double GetDistance(Vec3d from) const;
};

class Ray : public Object
{
	Vec3d pDefaultDirection;
	Vec3d pDirection;
	unsigned int pStepsDone, pCollisionsHappened;
public:
	Ray();
	void SetDefaultDirection(double x, double y, double z);
	void SetDirection(Vec3d direction);
	void SetDirection(Vec3d *direction);
	void SetDirection(double x, double y, double z);
	void Reset();
	void Run();
	Object *RunOnce();
};

class Sphere : public Object
{
	double pRadius;
public:
	Sphere();
	void SetRadius(double radius);
	double GetDistance(Vec3d from) const;
};

class Cube : public Object
{
	double pLength;
public:
	Cube();
	void SetLength(double length);
	double GetDistance(Vec3d from) const;
};

class Torus : public Object
{
	double pRadius1, pRadius2;
public:
	Torus();
	void SetRadius1(double radius);
	void SetRadius2(double radius);
	double GetDistance(Vec3d from) const;
};

class Plane : public Object
{
	Vec3d pOrientation;
public:
	Plane();
	void SetOrientation(Vec3d orientation);
	double GetDistance(Vec3d from) const;
};

#endif // BASICOBJECTS_HPP

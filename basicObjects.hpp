#ifndef BASICOBJECTS_HPP
#define BASICOBJECTS_HPP

#include <vector>
#include <string>
#include "commonVectorFun.hpp"
#include "prng.hpp"

using namespace std;

class Object
{
	bool pVisible;
	uint64_t pID;
	static uint64_t sLastKnownObjectID;
	string pName;
protected:
	double pBrightness;
	double pSpecularity;
	uint64_t pDiffusionChance;
	Vec3f pColor;
	Vec3d pPosition;
	Vec3d pOrientation;
	Vec3d WorldToLocal(const Vec3d &point) const;
public:
	static constexpr double NORMAL_CALCULATION_DIST = 1.0/16.0;
	Object();
	bool Visible() const;
	void SetVisible(bool visible);

	uint64_t ID() const;
	uint64_t DiffusionChance() const;
	double Brightness() const;
	void SetBrightness(double brightness);

	double Specularity() const;
	void SetSpecularity(double specularity);

	const Vec3f &Color() const;
	void SetColor(Vec3f color);
	void SetColor(float r, float g, float b);

	const Vec3d &Position() const;
	void SetPosition(const Vec3d &position);
	void SetPosition(double x, double y, double z);

	const Vec3d &Orientation() const;
	void SetOrientation(const Vec3d &orientation);
	void SetOrientation(double x, double y, double z);

	const string &Name() const;
	void SetName(const string &name);

	virtual double GetDistance(Vec3d from) const;
	virtual Vec3d GetNormalVector(Vec3d point) const;

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
	Vec3d pDefaultOrientation;
	uint64_t pReflectionsLimit;
	uint64_t pStepsPerRunLimit;
	prng_u64 pPRNG;
	Vec3d createRandomVector3d();
public:
	static constexpr double RAY_COLLISION_DISTANCE = 1.0/16.0;
	Ray();
	void SetDefaultOrientation(double x, double y, double z);
	void SetReflectionsLimit(uint64_t limit);
	void SetStepsPerRunLimit(uint64_t limit);
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
	Vec3d GetNormalVector(Vec3d point) const;
};

class Cube : public Object
{
	double pLength;
public:
	Cube();
	void SetLength(double length);
	double GetDistance(Vec3d from) const;
};

class Cylinder : public Object
{
	double pLength;
	double pRadius;
public:
	Cylinder();
	void SetLength(double length);
	void SetRadius(double radius);
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
public:
	Plane();
	double GetDistance(Vec3d from) const;
	Vec3d GetNormalVector(Vec3d point) const;
};

#endif // BASICOBJECTS_HPP

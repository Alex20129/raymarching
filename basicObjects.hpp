#ifndef BASICOBJECTS_HPP
#define BASICOBJECTS_HPP

#include <cstdint>
#include "commonVectorFun.hpp"

class Octree;

using namespace std;

class Object
{
	bool pVisible=true;
	uint64_t pID;
	static uint64_t sLastKnownObjectID;
protected:
	double pBrightness=0.0;
	double pSpecularity=0.0;
	double pTransparency=0.0;
	uint64_t pDiffusionChance=UINT64_MAX;
	uint64_t pPassthroughChance=0;
	Vec3f pColor;
	Vec3d pPosition;
	Vec3d pVForward, pVRight, pVUp;
	Vec3d WorldToLocal(const Vec3d &point) const;
	void UpdateBasis(const Vec3d &forward);
public:
	static constexpr double NORMAL_CALCULATION_DIST = 1.0/16.0;
	Octree *SceneTree=nullptr;
	Object();
	bool Visible() const;
	void SetVisible(bool visible);

	uint64_t ID() const;
	uint64_t DiffusionChance() const;
	uint64_t PassthroughChance() const;

	double Brightness() const;
	void SetBrightness(double brightness);

	double Specularity() const;
	void SetSpecularity(double specularity);

	double Transparency() const;
	void SetTransparency(double transparency);

	const Vec3f &Color() const;
	void SetColor(Vec3f color);
	void SetColor(float r, float g, float b);

	const Vec3d &Position() const;
	void SetPosition(const Vec3d &position);
	void SetPosition(double x, double y, double z);

	const Vec3d &Orientation() const;
	void SetOrientation(const Vec3d &orientation);
	void SetOrientation(double x, double y, double z);

	virtual double GetDistance(const Vec3d &from) const;
	virtual Vec3d GetNormalVector(const Vec3d &point) const;
};

class Difference : public Object
{
	Object *ObjectA, *ObjectB;
public:
	Difference(Object *object_a, Object *object_b);
	double GetDistance(const Vec3d &from) const;
};

class Union : public Object
{
	Object *ObjectA, *ObjectB;
public:
	Union(Object *object_a, Object *object_b);
	double GetDistance(const Vec3d &from) const;
};

class Intersection : public Object
{
	Object *ObjectA, *ObjectB;
public:
	Intersection(Object *object_a, Object *object_b);
	double GetDistance(const Vec3d &from) const;
};

class Ray : public Object
{
	Vec3d pDefaultOrientation, pFirstCollisionPoint;
	uint64_t pPrngSeedValue;
	uint64_t pReflectionsLimit;
	uint64_t pStepsPerRunLimit;
public:
	static constexpr double RAY_COLLISION_DISTANCE = 1.0/16.0;
	Ray();
	void SetDefaultOrientation(double x, double y, double z);
	void SetReflectionsLimit(uint64_t limit);
	void SetStepsPerRunLimit(uint64_t limit);
	void Trace();
	const Object *RunOnce(const Vec3d &direction, const Object *skip=nullptr);
};

class Sphere : public Object
{
	double pRadius;
public:
	Sphere();
	void SetRadius(double radius);
	double GetDistance(const Vec3d &from) const;
	Vec3d GetNormalVector(const Vec3d &point) const;
};

class Cube : public Object
{
	double pHalfLength;
public:
	Cube();
	void SetLength(double length);
	double GetDistance(const Vec3d &from) const;
};

class Cylinder : public Object
{
	double pLength;
	double pRadius;
public:
	Cylinder();
	void SetLength(double length);
	void SetRadius(double radius);
	double GetDistance(const Vec3d &from) const;
};

class Torus : public Object
{
	double pRadius1, pRadius2;
public:
	Torus();
	void SetRadius1(double radius);
	void SetRadius2(double radius);
	double GetDistance(const Vec3d &from) const;
};

class Plane : public Object
{
public:
	double GetDistance(const Vec3d &from) const;
	Vec3d GetNormalVector(const Vec3d &point) const;
};

class Gyroid : public Object
{
	double pScale;
public:
	Gyroid();
	void SetScale(double scale);
	double GetDistance(const Vec3d &from) const;
};

class SchwarzPrimitive : public Object
{
	double pScale;
public:
	SchwarzPrimitive();
	void SetScale(double scale);
	double GetDistance(const Vec3d &from) const;
};

#endif // BASICOBJECTS_HPP

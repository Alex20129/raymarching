#ifndef BASICOBJECTS_HPP
#define BASICOBJECTS_HPP

#include <cstdint>
#include <vector>
#include "commonVectorFun.hpp"

using namespace std;

class Object
{
	bool pVisible=true;
	uint64_t pID;
	static uint64_t sLastKnownObjectID;
protected:
	float pBrightness=0.0;
	float pSpecularity=0.0;
	float pTransparency=0.0;
	uint64_t pDiffusionChance=UINT64_MAX;
	uint64_t pPassthroughChance=0;
	Vec3f pColor;
	Vec3f pPosition;
	Vec3f pVForward, pVRight, pVUp;
	Vec3f WorldToLocal(const Vec3f &point) const;
	void UpdateBasis(const Vec3f &forward);
public:
	static constexpr float NORMAL_CALCULATION_DIST = 1.0/16.0;
	Object();
	bool Visible() const;
	void SetVisible(bool visible);

	uint64_t ID() const;
	uint64_t DiffusionChance() const;
	uint64_t PassthroughChance() const;

	float Brightness() const;
	void SetBrightness(float brightness);

	float Specularity() const;
	void SetSpecularity(float specularity);

	float Transparency() const;
	void SetTransparency(float transparency);

	const Vec3f &Color() const;
	void SetColor(Vec3f color);
	void SetColor(float r, float g, float b);

	const Vec3f &Position() const;
	void SetPosition(const Vec3f &position);
	void SetPosition(float x, float y, float z);

	const Vec3f &Orientation() const;
	void SetOrientation(const Vec3f &orientation);
	void SetOrientation(float x, float y, float z);

	virtual float GetDistance(const Vec3f &from) const;
	virtual Vec3f GetNormalVector(const Vec3f &point) const;
};

class Difference : public Object
{
	Object *ObjectA, *ObjectB;
public:
	Difference(Object *object_a, Object *object_b);
	float GetDistance(const Vec3f &from) const;
};

class Union : public Object
{
	Object *ObjectA, *ObjectB;
public:
	Union(Object *object_a, Object *object_b);
	float GetDistance(const Vec3f &from) const;
};

class Intersection : public Object
{
	Object *ObjectA, *ObjectB;
public:
	Intersection(Object *object_a, Object *object_b);
	float GetDistance(const Vec3f &from) const;
};

class Sphere : public Object
{
	float pRadius;
public:
	Sphere();
	void SetRadius(float radius);
	float GetDistance(const Vec3f &from) const;
	Vec3f GetNormalVector(const Vec3f &point) const;
};

class Cube : public Object
{
	float pHalfLength;
public:
	Cube();
	void SetLength(float length);
	float GetDistance(const Vec3f &from) const;
};

class Cylinder : public Object
{
	float pHalfLength;
	float pRadius;
public:
	Cylinder();
	void SetLength(float length);
	void SetRadius(float radius);
	float GetDistance(const Vec3f &from) const;
};

class Torus : public Object
{
	float pRadius1, pRadius2;
public:
	Torus();
	void SetRadius1(float radius);
	void SetRadius2(float radius);
	float GetDistance(const Vec3f &from) const;
};

class Plane : public Object
{
public:
	float GetDistance(const Vec3f &from) const;
	Vec3f GetNormalVector(const Vec3f &point) const;
};

class Gyroid : public Object
{
	float pScale;
public:
	Gyroid();
	void SetScale(float scale);
	float GetDistance(const Vec3f &from) const;
};

class SchwarzPrimitive : public Object
{
	float pScale;
public:
	SchwarzPrimitive();
	void SetScale(float scale);
	float GetDistance(const Vec3f &from) const;
};

class Ray
{
	Vec3f pDefaultDirection;
	Vec3f pFirstCollisionPoint;
public:
	Vec3f Color;
	vector <Object *> *SceneObjects;
	uint64_t PRNGSeedValue=0;
	static constexpr float COLLISION_DISTANCE = 1.0f/16.0f;
	static constexpr uint32_t STEPS_PER_RUN_LIMIT = 1024u;
	static constexpr uint32_t REFLECTIONS_LIMIT = 7u;
	void SetDefaultDirection(float x, float y, float z);
	void Reset();
	void Trace();
	const Object *RunOnce(Vec3f &position, Vec3f direction, const Object *skip);
};

#endif // BASICOBJECTS_HPP

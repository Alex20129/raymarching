#ifndef BASICOBJECTS_HPP
#define BASICOBJECTS_HPP

#include <cstdint>
#include "commonVectorFun.hpp"

static constexpr float EPSILON = 1.0f/16.0f;

class Object
{
public:
	enum ObjectType
	{
		OBJECT,
		DIFFERENCE,
		UNION,
		INTERSECTION,
		SPHERE,
		CUBE,
		CYLINDER,
		TORUS,
		PLANE,
		GYROID,
		SCHWARZ_PRIMITIVE,
	};
private:
	bool pVisibility=true;
	float pBrightness=0.0;
	float pSpecularity=0.0;
	float pTransparency=0.0;
	uint64_t pDiffusionChance=UINT64_MAX;
	uint64_t pPassthroughChance=0;
protected:
	ObjectType pType;
	float pProperties[4];
	Vec3f pColor;
	Vec3f pPosition;
	Vec3f pVForward, pVRight, pVUp;
	Vec3f WorldToLocal(const Vec3f &point) const;
	void UpdateBasis(const Vec3f &forward, float roll=0.0F);
public:
	Object();

	ObjectType Type() const;
	uint64_t DiffusionChance() const;
	uint64_t PassthroughChance() const;

	bool Visibility() const;
	void SetVisibility(bool visibility);

	float Brightness() const;
	void SetBrightness(float brightness);

	float Specularity() const;
	void SetSpecularity(float specularity);

	float Transparency() const;
	void SetTransparency(float transparency);

	const Vec3f &Color() const;
	void SetColor(const Vec3f &color);
	void SetColor(float r, float g, float b);

	const Vec3f &Position() const;
	void SetPosition(const Vec3f &position);
	void SetPosition(float x, float y, float z);

	const Vec3f &Orientation() const;
	void SetOrientation(const Vec3f &orientation, float roll=0.0F);
	void SetOrientation(float x, float y, float z, float roll=0.0F);

	float Property(uint32_t property) const;
	void SetProperty(uint32_t property, float value);

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
public:
	Sphere();
	void SetRadius(float radius);
	float GetDistance(const Vec3f &from) const;
	Vec3f GetNormalVector(const Vec3f &point) const;
};

class Cube : public Object
{
public:
	Cube();
	void SetLength(float length);
	float GetDistance(const Vec3f &from) const;
};

class Cylinder : public Object
{
public:
	Cylinder();
	void SetRadius(float radius);
	void SetLength(float length);
	float GetDistance(const Vec3f &from) const;
};

class Torus : public Object
{
public:
	Torus();
	void SetRadius1(float radius);
	void SetRadius2(float radius);
	float GetDistance(const Vec3f &from) const;
};

class Plane : public Object
{
public:
	Plane();
	float GetDistance(const Vec3f &from) const;
	Vec3f GetNormalVector(const Vec3f &point) const;
};

class Gyroid : public Object
{
public:
	Gyroid();
	void SetScale(float scale);
	float GetDistance(const Vec3f &from) const;
};

class SchwarzPrimitive : public Object
{
public:
	SchwarzPrimitive();
	void SetScale(float scale);
	float GetDistance(const Vec3f &from) const;
};

#endif // BASICOBJECTS_HPP

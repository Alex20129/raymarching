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
		CUBOID,
		CYLINDER,
		INFINITE_CYLINDER,
		ELLIPTIC_CYLINDER,
		INFINITE_ELLIPTIC_CYLINDER,
		TORUS,
		PLANE,
		GYROID,
		SCHWARZ_PRIMITIVE,
	};
	struct ObjectProperty
	{
		static constexpr uint8_t DIAMETER=0;
		static constexpr uint8_t DIAMETER_1=0;
		static constexpr uint8_t DIAMETER_2=1;
		static constexpr uint8_t LENGTH=2;
		static constexpr uint8_t LENGTH_X=2;
		static constexpr uint8_t LENGTH_Y=3;
		static constexpr uint8_t LENGTH_Z=4;
		static constexpr uint8_t SCALE=5;
		static constexpr uint8_t VISIBILITY=6;
		static constexpr uint8_t BRIGHTNESS=7;
	};
private:
	float pSpecularity=0.0;
	float pTransparency=0.0;
	uint64_t pDiffusionChance=UINT64_MAX;
	uint64_t pPassthroughChance=0;
	Vec3f pColor;
protected:
	ObjectType pType;
	float pProperties[8];
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
	float Brightness() const;

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
	float GetDistance(const Vec3f &from) const;
	Vec3f GetNormalVector(const Vec3f &point) const;
};

class Cube : public Object
{
public:
	Cube();
	float GetDistance(const Vec3f &from) const;
	Vec3f GetNormalVector(const Vec3f &from) const;
};

class Cuboid : public Object
{
public:
	Cuboid();
	float GetDistance(const Vec3f &from) const;
	Vec3f GetNormalVector(const Vec3f &from) const;
};

class Cylinder : public Object
{
public:
	Cylinder();
	float GetDistance(const Vec3f &from) const;
};

class InfiniteCylinder : public Object
{
public:
	InfiniteCylinder();
	float GetDistance(const Vec3f &from) const;
};

class EllipticCylinder : public Object
{
public:
	EllipticCylinder();
	float GetDistance(const Vec3f &from) const;
};

class InfiniteEllipticCylinder : public Object
{
public:
	InfiniteEllipticCylinder();
	float GetDistance(const Vec3f &from) const;
};

class Torus : public Object
{
public:
	Torus();
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
	float GetDistance(const Vec3f &from) const;
};

class SchwarzPrimitive : public Object
{
public:
	SchwarzPrimitive();
	float GetDistance(const Vec3f &from) const;
};

#endif // BASICOBJECTS_HPP

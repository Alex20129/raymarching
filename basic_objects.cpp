#include <cstdio>
#include <cfloat>
#include <cmath>
#include "basic_objects.hpp"

static inline uint64_t fpToUint64(float v)
{
	uint64_t result=UINT64_MAX>>9;
	uint64_t multiplicationTrick=v*512.0;
	uint64_t remainder=v*511.0;
	result*=multiplicationTrick;
	result+=remainder;
	return (result);
}

Vec3f Object::WorldToLocal(const Vec3f &point) const
{
	Vec3f dir=point-pPosition;
	return Vec3f(dir.Dot(pVRight), dir.Dot(pVUp), dir.Dot(pVForward));
}

void Object::UpdateBasis(const Vec3f &forward, float roll)
{
	uint32_t WIP;
	// the roll is not taken into account.
	// need to fix it
	pVForward=forward;
	pVForward.Normalize();
	if(fabs(pVForward.X) < 1.0)
	{
		pVRight=Vec3f(1, 0, 0).Cross(pVForward);
	}
	else
	{
		pVRight=Vec3f(0, 1, 0).Cross(pVForward);
	}
	pVRight.Normalize();
	pVUp=pVForward.Cross(pVRight);
}

Object::Object()
{
	pType=OBJECT;
	pProperties[ObjectProperty::SCALE]=1.0;
	pProperties[ObjectProperty::VISIBILITY]=1.0;
	pProperties[ObjectProperty::BRIGHTNESS]=0.0;
	pProperties[ObjectProperty::SPECULARITY]=0.0;
	pProperties[ObjectProperty::TRASPARENCY]=0.0;
	UpdateBasis({0, 0, 1}, 0.0F);
}

Object::ObjectType Object::Type() const
{
	return (pType);
}

uint64_t Object::DiffusionChance() const
{
	return (pDiffusionChance);
}

uint64_t Object::PassthroughChance() const
{
	return (pPassthroughChance);
}

float Object::Scale() const
{
	return (pProperties[ObjectProperty::SCALE]);
}

float Object::Visibility() const
{
	return (pProperties[ObjectProperty::VISIBILITY]);
}

float Object::Brightness() const
{
	return (pProperties[ObjectProperty::BRIGHTNESS]);
}

float Object::Specularity() const
{
	return (pProperties[ObjectProperty::SPECULARITY]);
}

float Object::Transparency() const
{
	return (pProperties[ObjectProperty::TRASPARENCY]);
}

const Vec3f &Object::Color() const
{
	return (pColor);
}

void Object::SetColor(const Vec3f &color)
{
	pColor=Vec3f::Min(color, {255.0F, 255.0F, 255.0F});
}

void Object::SetColor(float r, float g, float b)
{
	pColor=Vec3f::Min({r, g, b}, {255.0F, 255.0F, 255.0F});
}

const Vec3f &Object::Position() const
{
	return (pPosition);
}

void Object::SetPosition(const Vec3f &position)
{
	pPosition=position;
}

void Object::SetPosition(float x, float y, float z)
{
	pPosition={x, y, z};
}

const Vec3f &Object::Orientation() const
{
	return (pVForward);
}

void Object::SetOrientation(const Vec3f &orientation, float roll)
{
	UpdateBasis(orientation, roll);
}

void Object::SetOrientation(float x, float y, float z, float roll)
{
	UpdateBasis({x, y, z}, roll);
}

float Object::Property(uint32_t property) const
{
	return (pProperties[property]*2.0);
}

void Object::SetProperty(uint32_t property, float value)
{
	if(property<ObjectProperty::SCALE)
	{
		// Dimensional properties are stored as values ​​divided by two
		// which is helpful for speeding up distance calculations
		pProperties[property]=value/2.0;
	}
	else
	{
		switch (property)
		{
			case ObjectProperty::DIAMETER_1:
			{
				if(value<EPSILON)
				{
					value=EPSILON;
				}
				break;
			}
			case ObjectProperty::DIAMETER_2:
			{
				if(value<EPSILON)
				{
					value=EPSILON;
				}
				break;
			}
			case ObjectProperty::LENGTH_X:
			{
				if(value<EPSILON)
				{
					value=EPSILON;
				}
				break;
			}
			case ObjectProperty::LENGTH_Y:
			{
				if(value<EPSILON)
				{
					value=EPSILON;
				}
				break;
			}
			case ObjectProperty::LENGTH_Z:
			{
				if(value<EPSILON)
				{
					value=EPSILON;
				}
				break;
			}
			case ObjectProperty::SCALE:
			{
				if(value<EPSILON)
				{
					value=EPSILON;
				}
				break;
			}
			case ObjectProperty::VISIBILITY:
			{
				if(value<0.0)
				{
					value=0.0;
				}
				break;
			}
			case ObjectProperty::BRIGHTNESS:
			{
				if(value<0.0)
				{
					value=0.0;
				}
				break;
			}
			case ObjectProperty::SPECULARITY:
			{
				if(value<0.0)
				{
					value=0.0;
				}
				if(value>1.0)
				{
					value=1.0;
				}
				pDiffusionChance=fpToUint64(1.0-value);
				break;
			}
			case ObjectProperty::TRASPARENCY:
			{
				if(value<0.0)
				{
					value=0.0;
				}
				if(value>1.0)
				{
					value=1.0;
				}
				pPassthroughChance=fpToUint64(value);
				break;
			}
			default:
			{
				break;
			}
		}
		pProperties[property]=value;
	}
}

float Object::GetDistance(const Vec3f &from) const
{
	return ((from-pPosition).Length());
}

Vec3f Object::GetNormalVector(const Vec3f &point) const
{
	Vec3f normalVec(
		GetDistance(point + Vec3f(EPSILON, 0, 0)) - GetDistance(point - Vec3f(EPSILON, 0, 0)),
		GetDistance(point + Vec3f(0, EPSILON, 0)) - GetDistance(point - Vec3f(0, EPSILON, 0)),
		GetDistance(point + Vec3f(0, 0, EPSILON)) - GetDistance(point - Vec3f(0, 0, EPSILON)));
	return (normalVec);
}

// ========= CSG ===

Difference::Difference(Object *object_a, Object *object_b)
{
	pType=DIFFERENCE;
	if(object_a==nullptr)
	{
		return;
	}
	ObjectA=object_a;
	if(object_b==nullptr)
	{
		return;
	}
	ObjectB=object_b;
	pPosition=object_a->Position();
	SetColor((object_a->Color()+object_b->Color())/2.0);
	SetProperty(ObjectProperty::SPECULARITY, (object_a->Specularity()+object_b->Specularity())/2.0);
	SetProperty(ObjectProperty::TRASPARENCY, (object_a->Transparency()+object_b->Transparency())/2.0);
	SetProperty(ObjectProperty::BRIGHTNESS, (object_a->Brightness()+object_b->Brightness())/2.0);
	object_a->SetProperty(ObjectProperty::VISIBILITY, 0.0);
	object_b->SetProperty(ObjectProperty::VISIBILITY, 0.0);
}

float Difference::GetDistance(const Vec3f &from) const
{
	float DistA=ObjectA->GetDistance(from);
	float DistB=ObjectB->GetDistance(from);
	return fmax(DistA, -DistB);
}

Union::Union(Object *object_a, Object *object_b)
{
	pType=UNION;
	if(object_a==nullptr)
	{
		return;
	}
	ObjectA=object_a;
	if(object_b==nullptr)
	{
		return;
	}
	ObjectB=object_b;
	pPosition=(object_a->Position()+object_b->Position())/2.0;
	SetColor((object_a->Color()+object_b->Color())/2.0);
	SetProperty(ObjectProperty::SPECULARITY, (object_a->Specularity()+object_b->Specularity())/2.0);
	SetProperty(ObjectProperty::TRASPARENCY, (object_a->Transparency()+object_b->Transparency())/2.0);
	SetProperty(ObjectProperty::BRIGHTNESS, (object_a->Brightness()+object_b->Brightness())/2.0);
	object_a->SetProperty(ObjectProperty::VISIBILITY, 0.0);
	object_b->SetProperty(ObjectProperty::VISIBILITY, 0.0);
}

float Union::GetDistance(const Vec3f &from) const
{
	float DistA=ObjectA->GetDistance(from);
	float DistB=ObjectB->GetDistance(from);
	return (fmin(DistA, DistB));
}

Intersection::Intersection(Object *object_a, Object *object_b)
{
	pType=INTERSECTION;
	if(object_a==nullptr)
	{
		return;
	}
	ObjectA=object_a;
	if(object_b==nullptr)
	{
		return;
	}
	ObjectB=object_b;
	pPosition=(object_a->Position()+object_b->Position())/2.0;
	SetColor((object_a->Color()+object_b->Color())/2.0);
	SetProperty(ObjectProperty::SPECULARITY, (object_a->Specularity()+object_b->Specularity())/2.0);
	SetProperty(ObjectProperty::TRASPARENCY, (object_a->Transparency()+object_b->Transparency())/2.0);
	SetProperty(ObjectProperty::BRIGHTNESS, (object_a->Brightness()+object_b->Brightness())/2.0);
	object_a->SetProperty(ObjectProperty::VISIBILITY, 0.0);
	object_b->SetProperty(ObjectProperty::VISIBILITY, 0.0);
}

float Intersection::GetDistance(const Vec3f &from) const
{
	float DistA=ObjectA->GetDistance(from);
	float DistB=ObjectB->GetDistance(from);
	return (fmax(DistA, DistB));
}

// ========= SPHERE ===

Sphere::Sphere()
{
	pType=SPHERE;
	pProperties[ObjectProperty::DIAMETER]=0.5;
}

float Sphere::GetDistance(const Vec3f &from) const
{
	return ((from-pPosition).Length()-pProperties[ObjectProperty::DIAMETER]);
}

Vec3f Sphere::GetNormalVector(const Vec3f &point) const
{
	return (point-pPosition);
}

// ========= CUBE ===

Cube::Cube()
{
	pType=CUBE;
	pProperties[ObjectProperty::LENGTH]=0.5;
}

float Cube::GetDistance(const Vec3f &from) const
{
	Vec3f d = WorldToLocal(from).Abs() - Vec3f(pProperties[ObjectProperty::LENGTH], pProperties[ObjectProperty::LENGTH], pProperties[ObjectProperty::LENGTH]);
	return (Vec3f::Max(d, Vec3f(0.0F, 0.0F, 0.0F)).Length() + fmin(fmax(d.X, fmax(d.Y, d.Z)), 0.0F));
}

Vec3f Cube::GetNormalVector(const Vec3f &from) const
{
	Vec3f localFrom = WorldToLocal(from);
	float hl = pProperties[ObjectProperty::LENGTH];
	Vec3f sPoint(
		fmaxf(-hl, fminf(localFrom.X, hl)),
		fmaxf(-hl, fminf(localFrom.Y, hl)),
		fmaxf(-hl, fminf(localFrom.Z, hl)));
	Vec3f localNormal = localFrom - sPoint;
	return (pVRight * localNormal.X) + (pVUp * localNormal.Y) + (pVForward * localNormal.Z);
}

// ========= CUBOID ===

Cuboid::Cuboid()
{
	pType=CUBOID;
	pProperties[ObjectProperty::LENGTH_X]=0.5;
	pProperties[ObjectProperty::LENGTH_Y]=0.5;
	pProperties[ObjectProperty::LENGTH_Z]=0.5;
}

float Cuboid::GetDistance(const Vec3f &from) const
{
	Vec3f d = WorldToLocal(from).Abs() - Vec3f(pProperties[ObjectProperty::LENGTH_X], pProperties[ObjectProperty::LENGTH_Y], pProperties[ObjectProperty::LENGTH_Z]);
	return (Vec3f::Max(d, Vec3f(0.0F, 0.0F, 0.0F)).Length() + fmin(fmax(d.X, fmax(d.Y, d.Z)), 0.0F));
}

Vec3f Cuboid::GetNormalVector(const Vec3f &from) const
{
	Vec3f localFrom = WorldToLocal(from);
	float hlX = pProperties[ObjectProperty::LENGTH_X];
	float hlY = pProperties[ObjectProperty::LENGTH_Y];
	float hlZ = pProperties[ObjectProperty::LENGTH_Z];
	Vec3f sPoint(
		fmaxf(-hlX, fminf(localFrom.X, hlX)),
		fmaxf(-hlY, fminf(localFrom.Y, hlY)),
		fmaxf(-hlZ, fminf(localFrom.Z, hlZ)));
	Vec3f localNormal = localFrom - sPoint;
	return (pVRight * localNormal.X) + (pVUp * localNormal.Y) + (pVForward * localNormal.Z);
}

// ========= CYLINDER ===

Cylinder::Cylinder()
{
	pType=CYLINDER;
	pProperties[ObjectProperty::DIAMETER]=0.5;
	pProperties[ObjectProperty::LENGTH]=0.5;
}

float Cylinder::GetDistance(const Vec3f &from) const
{
	Vec3f localFrom = WorldToLocal(from);
	float dXY=Vec2f(localFrom.X, localFrom.Y).Length() - pProperties[ObjectProperty::DIAMETER];
	float dZ=fabs(localFrom.Z) - pProperties[ObjectProperty::LENGTH];
	return fmin(fmax(dXY, dZ), 0.0) + Vec2f::Max({dXY, dZ}, {0.0, 0.0}).Length();
}

// ========= INFINITE CYLINDER ===

InfiniteCylinder::InfiniteCylinder()
{
	pType=INFINITE_CYLINDER;
	pProperties[ObjectProperty::DIAMETER]=0.5;
}

float InfiniteCylinder::GetDistance(const Vec3f &from) const
{
	Vec3f localFrom=WorldToLocal(from);
	return (Vec2f(localFrom.X, localFrom.Y).Length() - pProperties[ObjectProperty::DIAMETER]);
}

// ========= ELLIPTIC CYLINDER ===

EllipticCylinder::EllipticCylinder()
{
	pType=ELLIPTIC_CYLINDER;
	pProperties[ObjectProperty::DIAMETER_1]=0.5;
	pProperties[ObjectProperty::DIAMETER_2]=1.0;
	pProperties[ObjectProperty::LENGTH]=1.0;
}

float EllipticCylinder::GetDistance(const Vec3f &from) const
{
	Vec3f localFrom=WorldToLocal(from);
	float Rx=pProperties[Object::ObjectProperty::DIAMETER_1];
	float StretchFactor=Rx/pProperties[Object::ObjectProperty::DIAMETER_2];
	float dXY=Vec2f(localFrom.X, localFrom.Y*StretchFactor).Length() - Rx;
	float dZ=fabs(localFrom.Z) - pProperties[ObjectProperty::LENGTH];
	return fmin(fmax(dXY, dZ), 0.0) + Vec2f::Max({dXY, dZ}, {0.0, 0.0}).Length();
}

// ========= INFINITE ELLIPTIC CYLINDER ===

InfiniteEllipticCylinder::InfiniteEllipticCylinder()
{
	pType=INFINITE_ELLIPTIC_CYLINDER;
	pProperties[ObjectProperty::DIAMETER_1]=0.5;
	pProperties[ObjectProperty::DIAMETER_2]=1.0;
}

float InfiniteEllipticCylinder::GetDistance(const Vec3f &from) const
{
	Vec3f localFrom=WorldToLocal(from);
	float Rx=pProperties[Object::ObjectProperty::DIAMETER_1];
	float StretchFactor=Rx/pProperties[Object::ObjectProperty::DIAMETER_2];
	return (Vec2f(localFrom.X, localFrom.Y*StretchFactor).Length() - Rx);
}

// ========= TORUS ===

Torus::Torus()
{
	pType=TORUS;
	pProperties[ObjectProperty::DIAMETER_1]=0.5;
	pProperties[ObjectProperty::DIAMETER_2]=1.0;
}

float Torus::GetDistance(const Vec3f &from) const
{
	Vec3f localFrom=WorldToLocal(from);
	Vec2f d=Vec2f(Vec2f(localFrom.X, localFrom.Y).Length()-pProperties[ObjectProperty::DIAMETER_2], localFrom.Z);
	return (d.Length()-pProperties[ObjectProperty::DIAMETER_1]);
}

// ========= PLANE ===

Plane::Plane()
{
	pType=PLANE;
}

float Plane::GetDistance(const Vec3f &from) const
{
	return (from-pPosition).Dot(pVForward);
}

Vec3f Plane::GetNormalVector(const Vec3f &point) const
{
	return (pVForward);
}

// ========= GYROID ===

Gyroid::Gyroid()
{
	pType=GYROID;
	pProperties[ObjectProperty::SCALE]=1.0;
}

float Gyroid::GetDistance(const Vec3f &from) const
{
	Vec3f localFrom=WorldToLocal(from)/pProperties[ObjectProperty::SCALE];
	return (cos(localFrom.X)*sin(localFrom.Y) + cos(localFrom.Y)*sin(localFrom.Z) + cos(localFrom.Z)*sin(localFrom.X));
}

// ========= SCHWARZ PRIMITIVE ===

SchwarzPrimitive::SchwarzPrimitive()
{
	pType=SCHWARZ_PRIMITIVE;
	pProperties[ObjectProperty::SCALE]=1.0;
}

float SchwarzPrimitive::GetDistance(const Vec3f &from) const
{
	Vec3f localFrom=WorldToLocal(from)/pProperties[ObjectProperty::SCALE];
	return (cos(localFrom.X) + cos(localFrom.Y) + cos(localFrom.Z));
}

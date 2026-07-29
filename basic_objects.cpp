#include <cstdio>
#include <cfloat>
#include <cmath>
#include "basic_objects.hpp"

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

bool Object::Visibility() const
{
	return (pVisibility);
}

void Object::SetVisibility(bool visibility)
{
	pVisibility=visibility;
}

float Object::Brightness() const
{
	return (pBrightness);
}

void Object::SetBrightness(float brightness)
{
	if(brightness<0.0)
	{
		brightness=0.0;
	}
	pBrightness=brightness;
}

float Object::Specularity() const
{
	return (pSpecularity);
}

void Object::SetSpecularity(float specularity)
{
	if(specularity<0.0)
	{
		specularity=0.0;
	}
	if(specularity>1.0)
	{
		specularity=1.0;
	}
	pSpecularity=specularity;
	uint64_t multiplicationTrick=(1.0-specularity)*512.0;
	uint64_t remainder=(1.0-specularity)*511.0;
	pDiffusionChance=UINT64_MAX>>9;
	pDiffusionChance*=multiplicationTrick;
	pDiffusionChance+=remainder;
}

float Object::Transparency() const
{
	return (pTransparency);
}

void Object::SetTransparency(float transparency)
{
	if(transparency<0.0)
	{
		transparency=0.0;
	}
	if(transparency>1.0)
	{
		transparency=1.0;
	}
	if(transparency==1.0)
	{
		pVisibility=false;
	}
	pTransparency=transparency;
	uint64_t multiplicationTrick=transparency*512.0;
	uint64_t remainder=transparency*511.0;
	pPassthroughChance=UINT64_MAX>>9;
	pPassthroughChance*=multiplicationTrick;
	pPassthroughChance+=remainder;
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
	return (pProperties[property]);
}

void Object::SetProperty(uint32_t property, float value)
{
	pProperties[property]=value;
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
	pColor=(object_a->Color()+object_b->Color())/2.0;
	SetBrightness((object_a->Brightness()+object_b->Brightness())/2.0);
	SetSpecularity((object_a->Specularity()+object_b->Specularity())/2.0);
	SetTransparency((object_a->Transparency()+object_b->Transparency())/2.0);
	object_a->SetVisibility(false);
	object_b->SetVisibility(false);
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
	pColor=(object_a->Color()+object_b->Color())/2.0;
	SetBrightness((object_a->Brightness()+object_b->Brightness())/2.0);
	SetSpecularity((object_a->Specularity()+object_b->Specularity())/2.0);
	SetTransparency((object_a->Transparency()+object_b->Transparency())/2.0);
	object_a->SetVisibility(false);
	object_b->SetVisibility(false);
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
	pColor=(object_a->Color()+object_b->Color())/2.0;
	SetBrightness((object_a->Brightness()+object_b->Brightness())/2.0);
	SetSpecularity((object_a->Specularity()+object_b->Specularity())/2.0);
	SetTransparency((object_a->Transparency()+object_b->Transparency())/2.0);
	object_a->SetVisibility(false);
	object_b->SetVisibility(false);
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
	pProperties[0]=1.0; // Radius
}

void Sphere::SetRadius(float radius)
{
	pProperties[0]=radius;
}

float Sphere::GetDistance(const Vec3f &from) const
{
	return ((from-pPosition).Length()-pProperties[0]);
}

Vec3f Sphere::GetNormalVector(const Vec3f &point) const
{
	return (point-pPosition);
}

// ========= CUBE ===

Cube::Cube()
{
	pType=CUBE;
	pProperties[0]=0.5; // HalfLength
}

void Cube::SetLength(float length)
{
	pProperties[0]=length/2.0;
}

float Cube::GetDistance(const Vec3f &from) const
{
	Vec3f d=WorldToLocal(from).Abs()-Vec3f(pProperties[0], pProperties[0], pProperties[0]);
	return (Vec3f::Max(d, Vec3f(0, 0, 0)).Length() + fmin(fmax(d.X, fmax(d.Y, d.Z)), 0.0));
}

// ========= CYLINDER ===

Cylinder::Cylinder()
{
	pType=CYLINDER;
	pProperties[0]=1.0; // Radius
	pProperties[1]=0.5; // HalfLength
}

void Cylinder::SetRadius(float radius)
{
	pProperties[0]=radius;
}

void Cylinder::SetLength(float length)
{
	pProperties[1]=length/2.0;
}

float Cylinder::GetDistance(const Vec3f &from) const
{
	Vec3f localFrom=WorldToLocal(from);
	float dXY=Vec2f(localFrom.X, localFrom.Y).Length() - pProperties[0];
	float dZ=fabs(localFrom.Z) - pProperties[1];
	Vec2f d(fmax(dXY, dZ), fmax(dXY, -dZ));
	return fmin(d.Length(), fmax(dXY, dZ));
}

// ========= INFINITE CYLINDER ===

InfiniteCylinder::InfiniteCylinder()
{
	pType=INFINITE_CYLINDER;
	pProperties[0]=1.0; // Radius
}

void InfiniteCylinder::SetRadius(float radius)
{
	pProperties[0]=radius;
}

float InfiniteCylinder::GetDistance(const Vec3f &from) const
{
	Vec3f localFrom=WorldToLocal(from);
	return (Vec2f(localFrom.X, localFrom.Y).Length() - pProperties[0]);
}

// ========= TORUS ===

Torus::Torus()
{
	pType=TORUS;
	pProperties[0]=2.0; // Radius1
	pProperties[1]=1.0; // Radius2
}

void Torus::SetRadius1(float radius)
{
	pProperties[0]=radius;
}

void Torus::SetRadius2(float radius)
{
	pProperties[1]=radius;
}

float Torus::GetDistance(const Vec3f &from) const
{
	Vec3f localFrom=WorldToLocal(from);
	Vec2f d=Vec2f(Vec2f(localFrom.X, localFrom.Y).Length()-pProperties[0], localFrom.Z);
	return (d.Length()-pProperties[1]);
}

// ========= PLANE ===

Plane::Plane()
{
	pType=PLANE;
}

float Plane::GetDistance(const Vec3f &from) const
{
	return ((from-pPosition).Dot(pVForward));
}

Vec3f Plane::GetNormalVector(const Vec3f &point) const
{
	return (pVForward);
}

// ========= GYROID ===

Gyroid::Gyroid()
{
	pType=GYROID;
	pProperties[0]=1.0;  // Scale
}

void Gyroid::SetScale(float scale)
{
	pProperties[0]=scale;
}

float Gyroid::GetDistance(const Vec3f &from) const
{
	Vec3f localFrom=WorldToLocal(from)/pProperties[0];
	return (cos(localFrom.X)*sin(localFrom.Y) + cos(localFrom.Y)*sin(localFrom.Z) + cos(localFrom.Z)*sin(localFrom.X));
}

// ========= Schwarz primitive ===

SchwarzPrimitive::SchwarzPrimitive()
{
	pType=SCHWARZ_PRIMITIVE;
	pProperties[0]=1.0; // Scale
}

void SchwarzPrimitive::SetScale(float scale)
{
	pProperties[0]=scale;
}

float SchwarzPrimitive::GetDistance(const Vec3f &from) const
{
	Vec3f localFrom=WorldToLocal(from)/pProperties[0];
	return (cos(localFrom.X) + cos(localFrom.Y) + cos(localFrom.Z));
}

#include <cstdio>
#include <cmath>
#include "basicObjects.hpp"

static const double UINT64_DIV2_D=(double)(UINT64_MAX/2);
static const float UINT32_DIV2_F=(float)(UINT32_MAX/2);

Object::Object()
{
	SceneObjects=nullptr;
	pVisible=1;
	pID=0;
	pBrightness=0.0;
	pReflectivity=0.25;
	pName=new string("Object");
}

bool Object::Visible() const
{
	return(pVisible);
}

void Object::SetVisible(bool visible)
{
	pVisible=visible;
}

uint64_t Object::ID() const
{
	return(pID);
}

void Object::SetID(uint64_t id)
{
	pID=id;
}

double Object::Brightness() const
{
	return(pBrightness);
}

void Object::SetBrightness(double brightness)
{
	if(brightness<0.0)
	{
		brightness=0.0;
	}
	pBrightness=brightness;
}

double Object::Reflectivity()
{
	return(pReflectivity);
}

void Object::SetReflectivity(double reflectivity)
{
	if(reflectivity<0.0)
	{
		reflectivity=0.0;
	}
	pReflectivity=reflectivity;
}

Vec3f Object::Color() const
{
	return(pColor);
}

void Object::SetColor(Vec3f color)
{
	if(color.X>255.0F)
	{
		color.X=255.0F;
	}
	if(color.Y>255.0F)
	{
		color.Y=255.0F;
	}
	if(color.Z>255.0F)
	{
		color.Z=255.0F;
	}
	pColor.X=color.X;
	pColor.Y=color.Y;
	pColor.Z=color.Z;
}

void Object::SetColor(float r, float g, float b)
{
	if(r>255.0F)
	{
		r=255.0F;
	}
	if(g>255.0F)
	{
		g=255.0F;
	}
	if(b>255.0F)
	{
		b=255.0F;
	}
	pColor.X=r;
	pColor.Y=g;
	pColor.Z=b;
}

Vec3d Object::Position() const
{
	return(pPosition);
}

void Object::SetPosition(Vec3d position)
{
	pPosition=position;
}

void Object::SetPosition(double x, double y, double z)
{
	pPosition.X=x;
	pPosition.Y=y;
	pPosition.Z=z;
}

string Object::Name()
{
	return(*pName);
}

void Object::SetName(string name)
{
	*pName=name;
}

double Object::GetDistance(Vec3d from) const
{
	return((from-pPosition).Length());
}

// Using the gradient of the SDF, estimate the normal vector on the surface at given point
Vec3d Object::GetNormalVector(Vec3d point) const
{
	double b=GetDistance(Vec3d(point.X - NORMAL_CALCULATION_D, point.Y - NORMAL_CALCULATION_D, point.Z - NORMAL_CALCULATION_D));
	Vec3d normalV(
		GetDistance(Vec3d(point.X + NORMAL_CALCULATION_D, point.Y - NORMAL_CALCULATION_D, point.Z - NORMAL_CALCULATION_D)) - b,
		GetDistance(Vec3d(point.X - NORMAL_CALCULATION_D, point.Y + NORMAL_CALCULATION_D, point.Z - NORMAL_CALCULATION_D)) - b,
		GetDistance(Vec3d(point.X - NORMAL_CALCULATION_D, point.Y - NORMAL_CALCULATION_D, point.Z + NORMAL_CALCULATION_D)) - b);
	return(normalV);
}

// ========= CSG ===

Difference::Difference(Object *object_a, Object *object_b)
{
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
	SetName("Difference");
	pColor=(ObjectA->Color()+ObjectB->Color())/2.0;
	pBrightness=(ObjectA->Brightness()+ObjectB->Brightness())/2.0;
	pReflectivity=(ObjectA->Reflectivity()+ObjectB->Reflectivity())/2.0;
	ObjectA->SetVisible(false);
	ObjectB->SetVisible(false);
}

double Difference::GetDistance(Vec3d from) const
{
	double DistA=ObjectA->GetDistance(from);
	double DistB=0.0-ObjectB->GetDistance(from);
	return max(DistA, DistB);
}

Union::Union(Object *object_a, Object *object_b)
{
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
	SetName("Union");
	pColor=(ObjectA->Color()+ObjectB->Color())/2.0;
	pBrightness=(ObjectA->Brightness()+ObjectB->Brightness())/2.0;
	pReflectivity=(ObjectA->Reflectivity()+ObjectB->Reflectivity())/2.0;
	ObjectA->SetVisible(false);
	ObjectB->SetVisible(false);
}

double Union::GetDistance(Vec3d from) const
{
	double DistA=ObjectA->GetDistance(from);
	double DistB=ObjectB->GetDistance(from);
	return min(DistA, DistB);
}

Intersection::Intersection(Object *object_a, Object *object_b)
{
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
	SetName("Intersection");
	pColor=(ObjectA->Color()+ObjectB->Color())/2.0;
	pBrightness=(ObjectA->Brightness()+ObjectB->Brightness())/2.0;
	pReflectivity=(ObjectA->Reflectivity()+ObjectB->Reflectivity())/2.0;
	ObjectA->SetVisible(false);
	ObjectB->SetVisible(false);
}

double Intersection::GetDistance(Vec3d from) const
{
	double DistA=ObjectA->GetDistance(from);
	double DistB=ObjectB->GetDistance(from);
	return max(DistA, DistB);
}

// ========= RAY ===

void Ray::RandomizeVector3D(Vec3d &vector)
{
	vector.X=pPRNG.generate_mms();
	vector.X/=UINT64_DIV2_D;
	vector.X-=1.0;
	vector.Y=pPRNG.generate_mms();
	vector.Y/=UINT64_DIV2_D;
	vector.Y-=1.0;
	vector.Z=pPRNG.generate_mms();
	vector.Z/=UINT64_DIV2_D;
	vector.Z-=1.0;
}

Ray::Ray()
{
	pObjectToIgnore=nullptr;
	SetName("Ray");
}

prng_u64 Ray::PRNG()
{
	return(pPRNG);
}

void Ray::SetPRNG(prng_u64 &prng)
{
	pPRNG=prng;
}

void Ray::SetDefaultDirection(double x, double y, double z)
{
	pDefaultDirection.X=x;
	pDefaultDirection.Y=y;
	pDefaultDirection.Z=z;
	pDefaultDirection.Normalize();
}

void Ray::SetDirection(Vec3d direction)
{
	pDirection=direction;
	pDirection.Normalize();
}

void Ray::SetDirection(Vec3d *direction)
{
	pDirection=*direction;
	pDirection.Normalize();
}

void Ray::SetDirection(double x, double y, double z)
{
	pDirection.X=x;
	pDirection.Y=y;
	pDirection.Z=z;
	pDirection.Normalize();
}

void Ray::Reset()
{

	pDirection=pDefaultDirection;
	SetPosition(0, 0, 0);
}

void Ray::Run()
{
	Vec3d SurfaceNormalVec;
	Vec3d RandomDirectionVec;
	Vec3d ReflectionVec;
	Vec3f ColorAcc(255, 255, 255);
	Vec3f IlluminationAcc(0, 0, 0);
	Object *Obstacle=nullptr;
	uint64_t CollisionsHappened=0;

	while(CollisionsHappened<RAY_COLLISIONS_MAX)
	{
		Obstacle=RunOnce();
		if(Obstacle==nullptr)
		{
			break;
		}
		pObjectToIgnore=Obstacle;
		CollisionsHappened++;

		IlluminationAcc += Obstacle->Color() * Obstacle->Brightness();
		// IlluminationAcc += Obstacle->Color();

		ColorAcc=ColorAcc * Obstacle->Color() / 255.0;

		// Obstacle->Reflectivity();
		SurfaceNormalVec=Obstacle->GetNormalVector(pPosition);
		SurfaceNormalVec.Normalize();

		RandomizeVector3D(RandomDirectionVec);
		RandomDirectionVec.Normalize();

		// ===
		// ReflectionVec=pDirection - (SurfaceNormalVec*2.0) * pDirection.Dot(SurfaceNormalVec);
		// SetDirection(ReflectionVec);
		// ===
		SetDirection(SurfaceNormalVec + RandomDirectionVec);
		// SetDirection(SurfaceNormalVec);
	}
	pColor=pColor + (ColorAcc * IlluminationAcc);
}

Object *Ray::RunOnce()
{
	double mindist=UINT32_MAX, dist;
	uint64_t stepsDone=0;
	while(stepsDone<RAY_STEPS_PER_RUN_MAX)
	{
		for(Object *Object: *SceneObjects)
		{
			if(!Object->Visible())
			{
				continue;
			}
			if(Object==pObjectToIgnore)
			{
				pObjectToIgnore=nullptr;
				continue;
			}
			dist=Object->GetDistance(pPosition);
			if(mindist>dist)
			{
				mindist=dist;
				if(mindist<RAY_COLLISION_THRESHOLD)
				{
					return(Object);
				}
			}
		}
		pPosition=pPosition+pDirection*mindist;
		stepsDone++;
	}
	return(nullptr);
}

// ========= SPHERE ===

Sphere::Sphere()
{
	pRadius=1.0;
	SetName("Sphere");
}

void Sphere::SetRadius(double radius)
{
	pRadius=radius;
}

double Sphere::GetDistance(Vec3d from) const
{
	return((from-pPosition).Length()-pRadius);
}

// ========= CUBE ===

Cube::Cube()
{
	pLength=1.0;
	SetName("Cube");
}

void Cube::SetLength(double length)
{
	pLength=length/2;
}

double Cube::GetDistance(Vec3d from) const
{
	Vec3d d=(from-pPosition).Abs() - Vec3d(pLength, pLength, pLength);
	return d.Max(Vec3d(0, 0, 0)).Length() + min(max(d.X, max(d.Y, d.Z)), 0.0);
}

// ========= TORUS ===

Torus::Torus()
{
	pRadius1=2.0;
	pRadius2=1.0;
	SetName("Torus");
}

void Torus::SetRadius1(double radius)
{
	pRadius1=radius;
}

void Torus::SetRadius2(double radius)
{
	pRadius2=radius;
}

double Torus::GetDistance(Vec3d from) const
{
	Vec3d s=from-pPosition;
	Vec2d d=Vec2d(Vec2d(s.X, s.Z).Length()-pRadius1, s.Y);
	return(d.Length()-pRadius2);
}

// ========= PLANE ===

Plane::Plane()
{
	pOrientation=Vec3d(0, 0, 1);
	SetName("Plane");
}

void Plane::SetOrientation(Vec3d orientation)
{
	pOrientation=orientation.Normal();
}

void Plane::SetOrientation(double x, double y, double z)
{
	pOrientation.X=x;
	pOrientation.Y=y;
	pOrientation.Z=z;
	pOrientation.Normalize();
}

double Plane::GetDistance(Vec3d from) const
{
	return((from-pPosition).Dot(pOrientation));
}

#include <cstdio>
#include <cmath>
#include "basicObjects.hpp"

uint64_t Object::sLastKnownObjectID=0;

Vec3d Object::WorldToLocal(const Vec3d &point) const
{
	Vec3d dir=point - pPosition;
	Vec3d right, up, forward=pOrientation;
	if(abs(forward.X) < 1.0)
	{
		right=Vec3d(1, 0, 0).Cross(forward);
	}
	else
	{
		right=Vec3d(0, 1, 0).Cross(forward);
	}
	right.Normalize();
	up=forward.Cross(right);
	up.Normalize();
	return Vec3d(
		dir.Dot(right),
		dir.Dot(up),
		dir.Dot(forward)
		);
}

Object::Object()
{
	SetName("Object");
	SceneObjects=nullptr;
	pVisible=1;
	pID=sLastKnownObjectID++;
	pBrightness=0.0;
	pSpecularity=0.0;
	pDiffusionChance=UINT64_MAX;
	pOrientation=Vec3d(0, 0, 1);
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

uint64_t Object::DiffusionChance() const
{
	return(pDiffusionChance);
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

double Object::Specularity() const
{
	return(pSpecularity);
}

void Object::SetSpecularity(double specularity)
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

const Vec3f &Object::Color() const
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

const Vec3d &Object::Position() const
{
	return(pPosition);
}

void Object::SetPosition(const Vec3d &position)
{
	pPosition=position;
}

void Object::SetPosition(double x, double y, double z)
{
	pPosition.X=x;
	pPosition.Y=y;
	pPosition.Z=z;
}

const Vec3d &Object::Orientation() const
{
	return(pOrientation);
}

void Object::SetOrientation(const Vec3d &orientation)
{
	pOrientation=orientation;
	pOrientation.Normalize();
}

void Object::SetOrientation(double x, double y, double z)
{
	pOrientation.X=x;
	pOrientation.Y=y;
	pOrientation.Z=z;
	pOrientation.Normalize();
}

const string &Object::Name() const
{
	return(pName);
}

void Object::SetName(const string &name)
{
	pName=name;
}

double Object::GetDistance(Vec3d from) const
{
	from=from-pPosition;
	return(from.Length());
}

// Using the gradient of the SDF, estimate the normal vector on the surface at given point
Vec3d Object::GetNormalVector(Vec3d point) const
{
	double b=GetDistance(Vec3d(point.X - NORMAL_CALCULATION_DIST, point.Y - NORMAL_CALCULATION_DIST, point.Z - NORMAL_CALCULATION_DIST));
	Vec3d normalVec(
		GetDistance(Vec3d(point.X + NORMAL_CALCULATION_DIST, point.Y - NORMAL_CALCULATION_DIST, point.Z - NORMAL_CALCULATION_DIST)) - b,
		GetDistance(Vec3d(point.X - NORMAL_CALCULATION_DIST, point.Y + NORMAL_CALCULATION_DIST, point.Z - NORMAL_CALCULATION_DIST)) - b,
		GetDistance(Vec3d(point.X - NORMAL_CALCULATION_DIST, point.Y - NORMAL_CALCULATION_DIST, point.Z + NORMAL_CALCULATION_DIST)) - b);
	return(normalVec);
}

// ========= CSG ===

Difference::Difference(Object *object_a, Object *object_b)
{
	SetName("Difference");
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
	pColor=(ObjectA->Color()+ObjectB->Color())/2.0;
	pBrightness=(ObjectA->Brightness()+ObjectB->Brightness())/2.0;
	pSpecularity=(ObjectA->Specularity()+ObjectB->Specularity())/2.0;
	ObjectA->SetVisible(false);
	ObjectB->SetVisible(false);
}

double Difference::GetDistance(Vec3d from) const
{
	double DistA=ObjectA->GetDistance(from);
	double DistB=ObjectB->GetDistance(from);
	return max(DistA, -DistB);
}

Union::Union(Object *object_a, Object *object_b)
{
	SetName("Union");
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
	pColor=(ObjectA->Color()+ObjectB->Color())/2.0;
	pBrightness=(ObjectA->Brightness()+ObjectB->Brightness())/2.0;
	pSpecularity=(ObjectA->Specularity()+ObjectB->Specularity())/2.0;
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
	SetName("Intersection");
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
	pColor=(ObjectA->Color()+ObjectB->Color())/2.0;
	pBrightness=(ObjectA->Brightness()+ObjectB->Brightness())/2.0;
	pSpecularity=(ObjectA->Specularity()+ObjectB->Specularity())/2.0;
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

Vec3d Ray::createRandomVector3d()
{
	constexpr double div=(double)(UINT64_MAX>>12);
	Vec3d randomVector;
	do
	{
		randomVector.X=pPRNG.generate_xorshift_star()>>11;
		randomVector.X=randomVector.X/div-1.0;
		randomVector.Y=pPRNG.generate_xorshift_star()>>11;
		randomVector.Y=randomVector.Y/div-1.0;
		randomVector.Z=pPRNG.generate_xorshift_star()>>11;
		randomVector.Z=randomVector.Z/div-1.0;
	} while(randomVector.LengthSquared()>1.0);
	return(randomVector);
}

Ray::Ray()
{
	SetName("Ray");
	pReflectionsLimit=7;
	pStepsPerRunLimit=1024;
	uint64_t prngSeed=pPRNG.get_seed_value()+this->ID();
	pPRNG.set_seed_value(prngSeed);
}

void Ray::SetDefaultOrientation(double x, double y, double z)
{
	Vec3d newDefaultOrientation(x, y, z);
	newDefaultOrientation.Normalize();
	pDefaultOrientation=newDefaultOrientation;
}

void Ray::SetReflectionsLimit(uint64_t limit)
{
	pReflectionsLimit=limit;
}

void Ray::SetStepsPerRunLimit(uint64_t limit)
{
	pStepsPerRunLimit=limit;
}

void Ray::Run()
{
	uint64_t ReflectionsHappened=0, ReflectionsLimit=pReflectionsLimit;
	Object *Obstacle=nullptr;
	Vec3d SurfaceNormalVec;
	Vec3f ColorSample(1.0, 1.0, 1.0);

	SetOrientation(pDefaultOrientation);
	SetPosition(0, 0, 0);

	while(ReflectionsHappened<ReflectionsLimit)
	{
		Obstacle=RunOnce();
		if(Obstacle==nullptr)
		{
			break;
		}
		ReflectionsHappened++;

		if(Obstacle->Brightness()>0.0)
		{
			ColorSample=ColorSample * Obstacle->Color() * Obstacle->Brightness();
			break;
		}
		else
		{
			ColorSample=ColorSample * Obstacle->Color() / 255.0;
		}

		SurfaceNormalVec=Obstacle->GetNormalVector(pPosition);
		SurfaceNormalVec.Normalize();

		Vec3d NewDirection;
		if(pPRNG.generate_xorshift_star()<Obstacle->DiffusionChance())
		{
			NewDirection=SurfaceNormalVec + createRandomVector3d();
		}
		else
		{
			NewDirection=pOrientation - (SurfaceNormalVec*2.0) * SurfaceNormalVec.Dot(pOrientation);
		}
		SetOrientation(NewDirection);
		pPosition=pPosition+pOrientation;
	}
	pColor=pColor + ColorSample;
}

Object *Ray::RunOnce()
{
	uint64_t StepsTaken=0, StepsPerRunLimit=pStepsPerRunLimit;
	Vec3d Position=pPosition, Orientation=pOrientation;
	while(StepsTaken<StepsPerRunLimit)
	{
		double minDistance=(double)(UINT64_MAX>>12), Distance;
		for(Object *sceneObject: *SceneObjects)
		{
			if(!sceneObject->Visible())
			{
				continue;
			}
			Distance=sceneObject->GetDistance(Position);
			if(Distance<minDistance)
			{
				minDistance=Distance;
				if(Distance<RAY_COLLISION_DISTANCE)
				{
					pPosition=Position;
					return(sceneObject);
				}
			}
		}
		StepsTaken++;
		Position=Position+Orientation*minDistance;
	}
	return(nullptr);
}

// ========= SPHERE ===

Sphere::Sphere()
{
	SetName("Sphere");
	pRadius=1.0;
}

void Sphere::SetRadius(double radius)
{
	pRadius=radius;
}

double Sphere::GetDistance(Vec3d from) const
{
	from=from-pPosition;
	return(from.Length()-pRadius);
}

Vec3d Sphere::GetNormalVector(Vec3d point) const
{
	return(point-pPosition);
}

// ========= CUBE ===

Cube::Cube()
{
	SetName("Cube");
	pLength=1.0;
}

void Cube::SetLength(double length)
{
	pLength=length/2;
}

double Cube::GetDistance(Vec3d from) const
{
	from=WorldToLocal(from);
	Vec3d d=from.Abs()-Vec3d(pLength, pLength, pLength);
	return d.Max(Vec3d(0, 0, 0)).Length() + min(max(d.X, max(d.Y, d.Z)), 0.0);
}

// ========= CYLINDER ===

Cylinder::Cylinder()
{
	SetName("Cylinder");
	pLength=1.0;
	pRadius=1.0;
}

void Cylinder::SetLength(double length)
{
	pLength=length;
}

void Cylinder::SetRadius(double radius)
{
	pRadius=radius;
}

double Cylinder::GetDistance(Vec3d from) const
{
	from=WorldToLocal(from);
	double dXY=Vec2d(from.X, from.Y).Length() - pRadius;
	double dZ=abs(from.Z) - pLength / 2.0;
	Vec2d d(max(dXY, dZ), max(dXY, -dZ));
	return min(d.Length(), max(dXY, dZ));
}

// ========= TORUS ===

Torus::Torus()
{
	SetName("Torus");
	pRadius1=2.0;
	pRadius2=1.0;
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
	from=WorldToLocal(from);
	Vec2d d=Vec2d(Vec2d(from.X, from.Y).Length()-pRadius1, from.Z);
	return(d.Length()-pRadius2);
}

// ========= PLANE ===

Plane::Plane()
{
	SetName("Plane");
}

double Plane::GetDistance(Vec3d from) const
{
	from=from-pPosition;
	return(from.Dot(pOrientation));
}

Vec3d Plane::GetNormalVector(Vec3d point) const
{
	return(pOrientation);
}

#include <cstdio>
#include <cmath>
#include "basicObjects.hpp"

Vec3d Object::WorldToLocal(const Vec3d &point) const
{
	Vec3d dir = point - pPosition;
	Vec3d right, up, forward = pOrientation;

	if (abs(forward.X) < 1.0)
	{
		right = Vec3d(1, 0, 0).Cross(forward);
	}
	else
	{
		right = Vec3d(0, 1, 0).Cross(forward);
	}
	right.Normalize();

	up = forward.Cross(right);
	up.Normalize();

	return Vec3d(
		dir.Dot(right),
		dir.Dot(up),
		dir.Dot(forward)
		);
}

Object::Object()
{
	pName=new string("Object");
	SceneObjects=nullptr;
	pVisible=1;
	pID=0;
	pBrightness=0.0;
	pReflectivity=0.5;
	pSpecularity=0.5;
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
	if(reflectivity>1.0)
	{
		reflectivity=1.0;
	}
	pReflectivity=reflectivity;
}

double Object::Specularity()
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

Vec3d Object::Orientation() const
{
	return(pOrientation);
}

void Object::SetOrientation(const Vec3d *orientation)
{
	Vec3d newOrientation(orientation);
	newOrientation.Normalize();
	pOrientation=newOrientation;
}

void Object::SetOrientation(const Vec3d &orientation)
{
	Vec3d newOrientation(orientation);
	newOrientation.Normalize();
	pOrientation=newOrientation;
}

void Object::SetOrientation(double x, double y, double z)
{
	Vec3d newOrientation(x, y, z);
	newOrientation.Normalize();
	pOrientation=newOrientation;
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
	from=from-pPosition;
	return(from.Length());
}

// Using the gradient of the SDF, estimate the normal vector on the surface at given point
Vec3d Object::GetNormalVector(Vec3d point) const
{
	double b=GetDistance(Vec3d(point.X - NORMAL_CALCULATION_DIST, point.Y - NORMAL_CALCULATION_DIST, point.Z - NORMAL_CALCULATION_DIST));
	Vec3d normalV(
		GetDistance(Vec3d(point.X + NORMAL_CALCULATION_DIST, point.Y - NORMAL_CALCULATION_DIST, point.Z - NORMAL_CALCULATION_DIST)) - b,
		GetDistance(Vec3d(point.X - NORMAL_CALCULATION_DIST, point.Y + NORMAL_CALCULATION_DIST, point.Z - NORMAL_CALCULATION_DIST)) - b,
		GetDistance(Vec3d(point.X - NORMAL_CALCULATION_DIST, point.Y - NORMAL_CALCULATION_DIST, point.Z + NORMAL_CALCULATION_DIST)) - b);
	return(normalV);
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
	pReflectivity=(ObjectA->Reflectivity()+ObjectB->Reflectivity())/2.0;
	pSpecularity=(ObjectA->Specularity()+ObjectB->Specularity())/2.0;
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
	pReflectivity=(ObjectA->Reflectivity()+ObjectB->Reflectivity())/2.0;
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
	pReflectivity=(ObjectA->Reflectivity()+ObjectB->Reflectivity())/2.0;
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
	constexpr double div=(double)(UINT64_MAX)/(double)(2.0);
	Vec3d randomVector;
	do
	{
		randomVector.X=pPRNG.generate_xorshift();
		randomVector.X/=div;
		randomVector.X-=1.0;
		randomVector.Y=pPRNG.generate_xorshift();
		randomVector.Y/=div;
		randomVector.Y-=1.0;
		randomVector.Z=pPRNG.generate_xorshift();
		randomVector.Z/=div;
		randomVector.Z-=1.0;
	} while (randomVector.LengthSquared() > 1.0);
	return(randomVector);
}

Ray::Ray()
{
	SetName("Ray");
	uint64_t prngSeed=(uint64_t)(this);
	prngSeed+=pPRNG.generate_xorshift();
	pPRNG.set_seed_value(prngSeed);
	pObjectToIgnore=nullptr;
}

void Ray::SetDefaultDirection(double x, double y, double z)
{
	Vec3d newDefaultDirection(x, y, z);
	newDefaultDirection.Normalize();
	pDefaultDirection=newDefaultDirection;
}

void Ray::SetDirection(const Vec3d *direction)
{
	Vec3d newDirection(direction);
	newDirection.Normalize();
	pDirection=newDirection;
}

void Ray::SetDirection(const Vec3d &direction)
{
	Vec3d newDirection(direction);
	newDirection.Normalize();
	pDirection=newDirection;
}

void Ray::SetDirection(double x, double y, double z)
{
	Vec3d newDirection(x, y, z);
	newDirection.Normalize();
	pDirection=newDirection;
}

void Ray::Reset()
{
	pDirection=pDefaultDirection;
	SetPosition(0, 0, 0);
}

void Ray::Run()
{
	Vec3d SurfaceNormalVec;
	Vec3d reflectionVec;
	Vec3f colorAcc(255, 255, 255);
	Vec3f illuminationAcc(0, 0, 0);
	Object *Obstacle=nullptr;
	uint64_t CollisionsHappened=0;

	while(CollisionsHappened<RAY_COLLISIONS_MAX)
	{
		Obstacle=RunOnce();
		if(Obstacle==nullptr)
		{
			colorAcc=Vec3f(0, 0, 0);
			break;
		}
		pObjectToIgnore=Obstacle;
		CollisionsHappened++;

		illuminationAcc = illuminationAcc + Obstacle->Color() * Obstacle->Brightness();

		colorAcc=colorAcc * Obstacle->Color() / 255.0;

		// TODO:
		// Obstacle->Reflectivity();

		SurfaceNormalVec=Obstacle->GetNormalVector(pPosition);
		SurfaceNormalVec.Normalize();

		double specularity=Obstacle->Specularity();

		reflectionVec=pDirection - (SurfaceNormalVec*2.0) * pDirection.Dot(SurfaceNormalVec);
		reflectionVec.Normalize(specularity);

		Vec3d diffusionVec=createRandomVector3d();
		diffusionVec=diffusionVec+SurfaceNormalVec;
		diffusionVec.Normalize(1.0-specularity);

		SetDirection(reflectionVec + diffusionVec);
	}
	pColor=pColor + (colorAcc + illuminationAcc);
	// pColor=pColor + ColorAcc;
}

Object *Ray::RunOnce()
{
	double mindist=UINT32_MAX, dist;
	uint64_t stepsDone=0;
	Object *prettyCloseObject=nullptr;
	while(stepsDone<RAY_STEPS_PER_RUN_MAX)
	{
		for(Object *sceneObject: *SceneObjects)
		{
			if(!sceneObject->Visible())
			{
				continue;
			}
			if(sceneObject==pObjectToIgnore)
			{
				pObjectToIgnore=nullptr;
				continue;
			}
			dist=sceneObject->GetDistance(pPosition);
			if(dist<mindist)
			{
				mindist=dist;
				if(dist<RAY_COLLISION_THRESHOLD)
				{
					return(sceneObject);
				}
				else if(dist<RAY_COLLISION_THRESHOLD2)
				{
					prettyCloseObject=sceneObject;
				}
			}
		}
		pPosition=pPosition+pDirection*mindist;
		stepsDone++;
	}
	return(prettyCloseObject);
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
	from=from-pPosition;
	return(from.Length()-pRadius);
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
	from=from-pPosition;
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
	from=from-pPosition;
	double dXZ = Vec2d(from.X, from.Z).Length() - pRadius;
	double dY = abs(from.Y) - pLength / 2.0;
	Vec2d d(max(dXZ, dY), max(dXZ, -dY));
	return min(d.Length(), max(dXZ, dY));
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
	from=from-pPosition;
	Vec2d d=Vec2d(Vec2d(from.X, from.Z).Length()-pRadius1, from.Y);
	return(d.Length()-pRadius2);
}

// ========= PLANE ===

Plane::Plane()
{
	pOrientation=Vec3d(0, 0, 1);
	SetName("Plane");
}

double Plane::GetDistance(Vec3d from) const
{
	from=from-pPosition;
	return(from.Dot(pOrientation));
}

#include "basicObjects.hpp"
#include <cstdio>
#include <cmath>

Vec3d RandomDirection()
{
	double prnX, prnY, prnZ;
	prnX=mms_prng_32();
	// prnX=fnv_prng_32();
	prnX/=UINT32_MAX/2.0;
	prnX-=1.0;
	prnY=mms_prng_32();
	// prnY=fnv_prng_32();
	prnY/=UINT32_MAX/2.0;
	prnY-=1.0;
	prnZ=mms_prng_32();
	// prnZ=fnv_prng_32();
	prnZ/=UINT32_MAX/2.0;
	prnZ-=1.0;
	return(Vec3d(prnX, prnY, prnZ));
}

Object::Object()
{
	SceneObjects=nullptr;
	pVisible=1;
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
	return(normalV.Normal());
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
	SetColor((ObjectA->Color()+ObjectB->Color())/2.0);
	SetBrightness((ObjectA->Brightness()+ObjectB->Brightness())/2.0);
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
	SetColor((ObjectA->Color()+ObjectB->Color())/2.0);
	SetBrightness((ObjectA->Brightness()+ObjectB->Brightness())/2.0);
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
	SetColor((ObjectA->Color()+ObjectB->Color())/2.0);
	SetBrightness((ObjectA->Brightness()+ObjectB->Brightness())/2.0);
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

Ray::Ray()
{
	pStepsDone=0;
	pCollisionsHappened=0;
	SetName("Ray");
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
	pDirection=direction.Normal();
}

void Ray::SetDirection(Vec3d *direction)
{
	pDirection=direction->Normal();
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
	pStepsDone=0;
	pCollisionsHappened=0;
	pDirection=pDefaultDirection;
	SetPosition(0, 0, 0);
}

void Ray::Run()
{
	Vec3d SurfaceNormalVec;
	Vec3d ReflectionVec;
	Vec3f ColorAcc(255, 255, 255);
	Vec3f IlluminationAcc(0, 0, 0);
	Object *Obstacle=nullptr;

	while(pCollisionsHappened<RAY_COLLISIONS_MAX)
	{
		Obstacle=RunOnce();
		if(Obstacle==nullptr)
		{
			break;
		}
		pCollisionsHappened++;

		IlluminationAcc += Obstacle->Color() * Obstacle->Brightness();

		ColorAcc = ColorAcc * Obstacle->Color();
		ColorAcc = ColorAcc / 255.0;
		// ColorAccK*=Obstacle->Reflectivity();

		SurfaceNormalVec=Obstacle->GetNormalVector(pPosition);
		// ===
		// ReflectionVec=pDirection - (SurfaceNormalVec*2.0) * pDirection.Dot(SurfaceNormalVec);
		// SetDirection(ReflectionVec);
		// ===
		SetDirection(SurfaceNormalVec + RandomDirection());
		// ===
		pPosition=pPosition+SurfaceNormalVec*RAY_COLLISION_STEPOUT;
	}
	pColor=pColor + (ColorAcc*IlluminationAcc);
}

Object *Ray::RunOnce()
{
	double mindist=UINT32_MAX, dist=0;
	Object *NearestObject=nullptr;
	while(pStepsDone<RAY_STEPS_MAX)
	{
		for(Object *Obj: *SceneObjects)
		{
			if(!Obj->Visible())
			{
				continue;
			}
			dist=Obj->GetDistance(pPosition);
			if(mindist>dist)
			{
				mindist=dist;
			}
			if(mindist<RAY_COLLISION_THRESHOLD)
			{
				NearestObject=Obj;
				break;
			}
		}
		if(NearestObject)
		{
			break;
		}
		pPosition=pPosition+pDirection*mindist;
		pStepsDone++;
	}
	return(NearestObject);
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
	pOrientation=Vec3d(0, -1, 0);
	SetName("Plane");
}

void Plane::SetOrientation(Vec3d orientation)
{
	pOrientation=orientation.Normal();
}

double Plane::GetDistance(Vec3d from) const
{
	return((from-pPosition).Dot(pOrientation));
}

#include "basicObjects.hpp"
#include <cstdio>
#include <cmath>

Object::Object()
{
	pVisible=1;
	pBrightness=0;
	pReflectivity=0.5;
	Position=new Vec3d(0, 0, 0);
	pColor=new Vec3uc(DEFAULT_OBJECT_COLOR);
	pName=new string(DEFAULT_OBJECT_NAME);
}

bool Object::Visible() const
{
	return(pVisible);
}

void Object::SetVisible(bool visible)
{
	pVisible=visible;
}

uint Object::Brightness() const
{
	return(pBrightness);
}

void Object::SetBrightness(uint brightness)
{
	pBrightness=brightness;
}

double Object::Reflectivity()
{
	return(pReflectivity);
}

void Object::SetReflectivity(double reflectivity)
{
	pReflectivity=reflectivity;
}

Vec3f Object::Color() const
{
	Vec3f color(pColor->X, pColor->Y, pColor->Z);
	return(color);
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
	pColor->X=color.X;
	pColor->Y=color.Y;
	pColor->Z=color.Z;
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
	pColor->X=r;
	pColor->Y=g;
	pColor->Z=b;
}

void Object::SetPosition(Vec3d position)
{
	*Position=position;
}

void Object::SetPosition(double x, double y, double z)
{
	Position->X=x;
	Position->Y=y;
	Position->Z=z;
}

string Object::Name()
{
	return(*pName);
}

void Object::SetName(string name)
{
	*pName=name;
}

void Object::AttachExternalColorBuffer(Vec3uc *buffer)
{
	pColor=buffer;
}

double Object::GetDistance(Vec3d from) const
{
	from=from-*Position;
	return(from.Length());
}

// Using the gradient of the SDF, estimate the normal vector on the surface at given point
Vec3d Object::GetNormalVector(Vec3d point) const
{
	Vec3d normalV(
			GetDistance(Vec3d(point.X + RAY_COLLISION_THRESHOLD, point.Y, point.Z)) - GetDistance(Vec3d(point.X - RAY_COLLISION_THRESHOLD, point.Y, point.Z)),
			GetDistance(Vec3d(point.X, point.Y + RAY_COLLISION_THRESHOLD, point.Z)) - GetDistance(Vec3d(point.X, point.Y - RAY_COLLISION_THRESHOLD, point.Z)),
			GetDistance(Vec3d(point.X, point.Y, point.Z  + RAY_COLLISION_THRESHOLD)) - GetDistance(Vec3d(point.X, point.Y, point.Z - RAY_COLLISION_THRESHOLD)));
	normalV.Normalize();
	return(normalV);
}

Vec3d Object::GetNormalVector(Vec3d *point) const
{
	Vec3d normalV(
			GetDistance(Vec3d(point->X + RAY_COLLISION_THRESHOLD, point->Y, point->Z)) - GetDistance(Vec3d(point->X - RAY_COLLISION_THRESHOLD, point->Y, point->Z)),
			GetDistance(Vec3d(point->X, point->Y + RAY_COLLISION_THRESHOLD, point->Z)) - GetDistance(Vec3d(point->X, point->Y - RAY_COLLISION_THRESHOLD, point->Z)),
			GetDistance(Vec3d(point->X, point->Y, point->Z  + RAY_COLLISION_THRESHOLD)) - GetDistance(Vec3d(point->X, point->Y, point->Z - RAY_COLLISION_THRESHOLD)));
	normalV.Normalize();
	return(normalV);
}

bool Object::ItsALightSource()
{
	return(pBrightness>0);
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
	SetColor((ObjectA->Color()+ObjectB->Color())/2.0);
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
	SetColor((ObjectA->Color()+ObjectB->Color())/2.0);
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
	SetColor((ObjectA->Color()+ObjectB->Color())/2.0);
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
	pDirection=new Vec3d(0, 0, 0);
	pStepsDone=0;
	pCollisionsHappened=0;
	SetName("Ray");
}

Vec3d Ray::Direction() const
{
	return(*pDirection);
}

void Ray::SetDirection(Vec3d direction)
{
	*pDirection=direction;
	pDirection->Normalize();
}

void Ray::SetDirection(Vec3d *direction)
{
	*pDirection=*direction;
	pDirection->Normalize();
}

void Ray::SetDirection(double x, double y, double z)
{
	pDirection->X=x;
	pDirection->Y=y;
	pDirection->Z=z;
	pDirection->Normalize();
}

void Ray::Reset()
{
	pStepsDone=0;
	pCollisionsHappened=0;
	pObjectToSkip=nullptr;
	SetPosition(0, 0, 0);
}

void Ray::Run()
{
	double illuninationLevel=0.0, ColorAccK=1.0;
	double diffusedLighting, diffusedLightingHV;
	Vec3d SurfaceNormalVec;
	Vec3d fromCollisionPointToLightSource;
	Vec3d FirstCollisionPoint;
	Vec3f NewColor;
	Object *Obstacle=nullptr, *FirstCollisionObstacle=nullptr;

	while(pCollisionsHappened<RAY_COLLISIONS_MAX)
	{
		Obstacle=RunOnce();
		if(Obstacle==nullptr) // finished in space
		{
			break;
		}
		pCollisionsHappened++;

		if(!FirstCollisionObstacle)
		{
			FirstCollisionObstacle=Obstacle;
			FirstCollisionPoint=*Position;
			// NewColor=Obstacle->Color();
		}
		// else
		{
			// NewColor=NewColor*(1.0-Obstacle->Reflectivity()) + Obstacle->Color()*Obstacle->Reflectivity();
			NewColor=NewColor*(1.0-ColorAccK) + Obstacle->Color()*ColorAccK;
			ColorAccK/=2.0;
		}

		if(Obstacle->ItsALightSource())
		{
			pCollisionsHappened=RAY_COLLISIONS_MAX;
		}
	}

	if(FirstCollisionObstacle)
	{
		pObjectToSkip=FirstCollisionObstacle;
		SurfaceNormalVec=FirstCollisionObstacle->GetNormalVector(FirstCollisionPoint);

		diffusedLightingHV=0.0;
		for(Object *LightSource: *SceneLights)
		{
			SetPosition(FirstCollisionPoint);

			fromCollisionPointToLightSource=*LightSource->Position-FirstCollisionPoint;
			fromCollisionPointToLightSource.Normalize();

			diffusedLighting=SurfaceNormalVec*fromCollisionPointToLightSource;
			if(diffusedLighting<0.0)
			{
				diffusedLighting=0.0;
			}
			if(diffusedLightingHV<diffusedLighting)
			{
				diffusedLightingHV=diffusedLighting;
			}

			SetDirection(&fromCollisionPointToLightSource);
			Obstacle=RunOnce();
			if(Obstacle==LightSource)
			{
				// illuninationLevel=diffusedLightingHV;
			}
			else
			{
				// illuninationLevel=diffusedLightingHV/2.0;
			}
			illuninationLevel=diffusedLightingHV;
		}
	}

	SetColor(NewColor*illuninationLevel);
}

Object *Ray::RunOnce()
{
	double mindist=__INT_MAX__, dist=0;
	Object *NearestObject=nullptr;
	Vec3d SurfaceNormalVec, ReflectionVec;
	while(pStepsDone<RAY_STEPS_MAX)
	{
		for(Object *Obj: *SceneObjects)
		{
			if(!Obj->Visible())
			{
				continue;
			}
			if(Obj==pObjectToSkip)
			{
				continue;
			}
			dist=Obj->GetDistance(*Position);
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
			pObjectToSkip=NearestObject;
			SurfaceNormalVec=NearestObject->GetNormalVector(this->Position);
			ReflectionVec=*pDirection-SurfaceNormalVec*2.0*(*pDirection*SurfaceNormalVec);
			SetDirection(ReflectionVec);
			break;
		}
		SetPosition(*Position+*pDirection*mindist);
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
	from=from-*Position;
	return(from.Length()-pRadius);
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
	from=from-*Position;
	Vec3d d=from.Abs() - Vec3d(pLength, pLength, pLength);
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
	from=from-*Position;
	Vec2d d=Vec2d(Vec2d(from.X, from.Z).Length()-pRadius1, from.Y);
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
	pOrientation=orientation;
	pOrientation.Normalize();
}

double Plane::GetDistance(Vec3d from) const
{
	from=from-*Position;
	return(from*pOrientation);
}

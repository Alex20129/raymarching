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

double Object::GetDistance(Vec3d from)
{
	from=from-*Position;
	return(from.Length());
}

bool Object::ItsALightSource()
{
	return(pBrightness>0);
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
	double illuninationLevel=0.125;
	double diffusedLighting, diffusedLightingHV;
	Vec3d SurfaceNormal;
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
			NewColor=Obstacle->Color();
		}
		else
		{
			NewColor=NewColor*(1.0-Obstacle->Reflectivity()) + Obstacle->Color()*Obstacle->Reflectivity();
		}

		if(Obstacle->ItsALightSource())
		{
			break;
		}
	}

	if(FirstCollisionObstacle)
	{
		pObjectToSkip=FirstCollisionObstacle;
		// WIP
		//incorrect, it works only for spheres
		SurfaceNormal=FirstCollisionPoint-*FirstCollisionObstacle->Position;
		SurfaceNormal.Normalize();

		diffusedLightingHV=0.0;
		for(Object *LightSource: *SceneLights)
		{
			SetPosition(FirstCollisionPoint);

			fromCollisionPointToLightSource=*LightSource->Position-FirstCollisionPoint;
			fromCollisionPointToLightSource.Normalize();

			diffusedLighting=SurfaceNormal*fromCollisionPointToLightSource;
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
			if(mindist<0.25)
			{
				NearestObject=Obj;
				break;
			}
		}
		if(NearestObject)
		{
			pObjectToSkip=NearestObject;
			// WIP
			//incorrect, it works only for spheres
			SurfaceNormalVec=(*this->Position-*NearestObject->Position).Normal();
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

double Sphere::GetDistance(Vec3d from)
{
	from=from-Position;
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
	pLength=length;
}

double Cube::GetDistance(Vec3d from)
{
	from=from-Position;
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

double Torus::GetDistance(Vec3d from)
{
	from=from-Position;
	Vec2d d=Vec2d(Vec2d(from.X, from.Z).Length()-pRadius1, from.Y);
	return(d.Length()-pRadius2);
}

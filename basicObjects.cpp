#include "basicObjects.hpp"
#include <cstdio>
#include <cmath>

Object::Object()
{
	pVisible=1;
	pBrightness=0;
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
	pReflectionsHappened=0;
	SetName("Ray");
}

Vec3d Ray::Direction() const
{
	return(*pDirection);
}

void Ray::SetDirection(Vec3d direction)
{
	*pDirection=direction.Normal();
}

void Ray::SetDirection(double x, double y, double z)
{
	*pDirection=Vec3d(x, y, z).Normal();
}

void Ray::Reset()
{
	pStepsDone=0;
	pReflectionsHappened=0;
	pObjectToSkipOnce=nullptr;
	SetPosition(0, 0, 0);
}

void Ray::Run()
{
	double illuninationLevel=1.0;
	double diffused_lighting, color_acc_k=1.0;
	Vec3d SurfaceNormal, Reflection;
	Vec3d fromPointToLightSource;
	Vec3d CollisionPoint, FirstCollisionPoint;
	Vec3f NewColor(DEFAULT_OBJECT_COLOR);
	Object *Obstacle=nullptr, *FirstCollisionObstacle=nullptr;

	while(pReflectionsHappened<RAY_REFLECTIONS_MAX)
	{
		Obstacle=RunTo(*pDirection);
		if(Obstacle==nullptr) // finished in space
		{
			break;
		}

		CollisionPoint=*Position;
		if(!FirstCollisionObstacle)
		{
			if(!Obstacle->ItsALightSource())
			{
				FirstCollisionObstacle=Obstacle;
				FirstCollisionPoint=*Position;
			}
		}

		NewColor=NewColor*(1.0-color_acc_k) + Obstacle->Color()*color_acc_k;
		color_acc_k/=2.0;

		if(Obstacle->ItsALightSource())
		{
			break;
		}

		pObjectToSkipOnce=Obstacle;

		// WIP
		//incorrect, it works only for spheres
		SurfaceNormal=(CollisionPoint-*Obstacle->Position).Normal();
		Reflection=(*pDirection-SurfaceNormal*2.0*(*pDirection*SurfaceNormal)).Normal();

		SetDirection(Reflection);
		pReflectionsHappened++;
	}

	if(FirstCollisionObstacle)
	{
		illuninationLevel=0.0;
		SurfaceNormal=(FirstCollisionPoint-*FirstCollisionObstacle->Position).Normal();
		for(Object *LightSource: *SceneLights)
		{
			fromPointToLightSource=(*LightSource->Position-FirstCollisionPoint).Normal();

//			diffused_lighting=(SurfaceNormal*fromPointToLightSource);
//			if(diffused_lighting<0.0)
//			{
//				diffused_lighting=0.0;
//			}

			SetPosition(FirstCollisionPoint);
			pObjectToSkipOnce=FirstCollisionObstacle;

			Obstacle=RunTo(fromPointToLightSource);
			if(Obstacle)
			{
				if(Obstacle->ItsALightSource())
				{
//					illuninationLevel+=diffused_lighting*2.0;
					illuninationLevel+=1.25;
				}
				else
				{
//					illuninationLevel+=diffused_lighting/2.0;
					illuninationLevel+=0.75;
				}
			}
		}
		illuninationLevel/=SceneLights->size();
	}

	SetColor(NewColor*illuninationLevel);
}

Object *Ray::RunTo(Vec3d direction)
{
	double mindist=__INT_MAX__, dist=0;
	Object *NearestObject=nullptr;
	while(pStepsDone<RAY_STEPS_MAX)
	{
		for(Object *Obj: *SceneObjects)
		{
			if(!Obj->Visible())
			{
				continue;
			}
			if(Obj==pObjectToSkipOnce)
			{
				pObjectToSkipOnce=nullptr;
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
			break;
		}
		SetPosition(*Position+direction*mindist);
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

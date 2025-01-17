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

uint64_t Object::Brightness() const
{
	return(pBrightness);
}

void Object::SetBrightness(uint64_t brightness)
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
	double b=GetDistance(Vec3d(point.X - NORMAL_CALCULATION_D, point.Y - NORMAL_CALCULATION_D, point.Z - NORMAL_CALCULATION_D));
	Vec3d normalV(
		GetDistance(Vec3d(point.X + NORMAL_CALCULATION_D, point.Y - NORMAL_CALCULATION_D, point.Z - NORMAL_CALCULATION_D)) - b,
		GetDistance(Vec3d(point.X - NORMAL_CALCULATION_D, point.Y + NORMAL_CALCULATION_D, point.Z - NORMAL_CALCULATION_D)) - b,
		GetDistance(Vec3d(point.X - NORMAL_CALCULATION_D, point.Y - NORMAL_CALCULATION_D, point.Z + NORMAL_CALCULATION_D)) - b);
	return(normalV.Normal());
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
	double IlluminationAcc=0.0, ColorAccK=1.0;
	Vec3d SurfaceNormalVec, ReflectionVec;
	Vec3d fromLightSourceToCollisionPoint;
	Vec3d FirstCollisionPoint;
	Vec3f PixelColor;
	Object *Obstacle=nullptr, *FirstCollisionObstacle=nullptr, *ShadowObstacle;

	// while(pCollisionsHappened<RAY_COLLISIONS_MAX)
	while(pCollisionsHappened<1)
	{
		Obstacle=RunOnce();
		if(Obstacle==nullptr)
		{
			break;
		}
		pCollisionsHappened++;

		if(!FirstCollisionObstacle)
		{
			FirstCollisionObstacle=Obstacle;
			FirstCollisionPoint=*Position;
			PixelColor=Obstacle->Color();
		}
		else
		{
			ColorAccK/=2.0;
			PixelColor+=Obstacle->Color()*ColorAccK;
		}

		if(Obstacle->ItsALightSource())
		{
			pCollisionsHappened=RAY_COLLISIONS_MAX;
		}
		else
		{
			pObjectToSkip=Obstacle;
			SurfaceNormalVec=Obstacle->GetNormalVector(*Position);
			ReflectionVec=*pDirection - (2.0*SurfaceNormalVec) * (*pDirection*SurfaceNormalVec);
			SetDirection(ReflectionVec);
		}
	}

	// PixelColor.Normalize(255);

	if(FirstCollisionObstacle)
	{
		for(Object *LightSource: *SceneLights)
		{
			pObjectToSkip = LightSource;
			pStepsDone = 0;

			fromLightSourceToCollisionPoint = FirstCollisionPoint - *LightSource->Position;
			double distanceToLight = fromLightSourceToCollisionPoint.Length();

			SetPosition(*LightSource->Position);
			SetDirection(fromLightSourceToCollisionPoint);

			ShadowObstacle = RunOnce();
			double PointIllumination, miss=(FirstCollisionPoint - *this->Position).Length();

			if(ShadowObstacle==FirstCollisionObstacle && miss<2.0)
			{
				PointIllumination = LightSource->Brightness();
				PointIllumination /= distanceToLight*distanceToLight + 1.0F;
				IlluminationAcc += PointIllumination;
			}
		}

		IlluminationAcc = min(2.0, IlluminationAcc);
		IlluminationAcc = max(0.1, IlluminationAcc);
	}

	SetColor(PixelColor * IlluminationAcc);
}

Object *Ray::RunOnce()
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

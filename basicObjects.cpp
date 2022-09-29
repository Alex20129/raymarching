#include "basicObjects.hpp"
#include <cstdio>
#include <cmath>

Object::Object()
{
	pVisible=1;
	Position=new Vec3d(0, 0, 0);
	pColor=nullptr;
	pName=new string("Object");
}

bool Object::Visible()
{
	return(pVisible);
}

void Object::SetVisible(bool visible)
{
	pVisible=visible;
}

Vec3f Object::Color()
{
	Vec3f retColor;
	if(pColor!=nullptr)
	{
		retColor.X=pColor->X;
		retColor.Y=pColor->Y;
		retColor.Z=pColor->Z;
	}
	return(retColor);
}

void Object::SetColor(Color_t new_color)
{
	if(pColor==nullptr)
	{
		pColor=new Vec3uc(DEFAULT_OBJECT_COLOR);
	}
	pColor->X=new_color.X;
	pColor->Y=new_color.Y;
	pColor->Z=new_color.Z;
}

void Object::SetColor(Vec3f new_color)
{
	if(new_color.X>255)
	{
		new_color.X=255;
	}
	if(new_color.Y>255)
	{
		new_color.Y=255;
	}
	if(new_color.Z>255)
	{
		new_color.Z=255;
	}
	SetColor(Vec3uc(new_color.X, new_color.Y, new_color.Z));
}

void Object::SetColor(float r, float g, float b)
{
	if(r>255)
	{
		r=255;
	}
	if(g>255)
	{
		g=255;
	}
	if(b>255)
	{
		b=255;
	}
	SetColor(Vec3uc(r, g, b));
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
	return(false);
}

// ========= LIGHT SOURCE ===

LightSource::LightSource()
{
	Brightness=DEFAULT_LIGHTSOURCE_BRIGHTNESS;
	SetName("LightSource");
}

bool LightSource::ItsALightSource()
{
	return(true);
}

// ========= RAY ===

Ray::Ray()
{
	pDefaultDirection=new Vec3d(0, 0, 0);
	pStepsDone=0;
	pIsFinished=0;
	SetName("Ray");
}

Vec3d Ray::Direction()
{
	return(*pDefaultDirection);
}

void Ray::SetDirection(Vec3d direction)
{
	*pDefaultDirection=direction.Normal();
}

void Ray::SetDirection(double x, double y, double z)
{
	*pDefaultDirection=Vec3d(x, y, z).Normal();
}

void Ray::Reload()
{
	pStepsDone=0;
	pIsFinished=0;
	pObjectToIgnore=nullptr;
}

void Ray::Reset()
{
	Reload();
	SetPosition(0, 0, 0);
}

void Ray::Run()
{
	double diff, spec;
	Vec3d SurfaceNormal, toLightSource, reflection, firstCollisionPoint, lightDirection;
	LightSource *NearestLightSource;
	Object *firstCollisionObject=RunTo(pDefaultDirection);
	if(firstCollisionObject==nullptr) // finished in space
	{
		SetColor(DEFAULT_OBJECT_COLOR);
		return;
	}
	firstCollisionPoint=Position;
	NearestLightSource=GetNearestLightSource();
	toLightSource=(*NearestLightSource->Position-*this->Position).Normal();
	//incorrect, it works only for spheres WIP
	SurfaceNormal=(firstCollisionPoint-*firstCollisionObject->Position).Normal();
	//
	reflection=(*pDefaultDirection-SurfaceNormal*2.0*(*pDefaultDirection*SurfaceNormal)).Normal();
	diff=(SurfaceNormal*toLightSource);
	if(diff<0)
	{
		diff=0;
	}
	spec=(reflection*toLightSource);
	if(spec<0)
	{
		spec=0;
	}
	spec=pow(spec, 16);
	pObjectToIgnore=NearestLightSource;
	this->SetPosition(NearestLightSource->Position);
	lightDirection=(firstCollisionPoint-*NearestLightSource->Position).Normal();
	if(RunTo(&lightDirection))
	{
		if(pNrstObj==firstCollisionObject)
		{
			SetColor(firstCollisionObject->Color()*diff + firstCollisionObject->Color()*spec);
		}
		else
		{
			SetColor(firstCollisionObject->Color()*diff*0.5);
		}
	}
	else
	{
		SetColor(firstCollisionObject->Color()*diff*0.5);
	}
}

Object *Ray::RunTo(Vec3d *direction)
{
	double mindist=-1U, dist=0;
	size_t objectID, objectsTotal=SceneObjects->size();
	Object *Obj;
	pNrstObj=nullptr;
//	vector <double> distanceMap;
//	distanceMap.resize(objectsTotal, 999);
	if(direction==nullptr)
	{
		direction=pDefaultDirection;
	}
	while(mindist>0.125)
	{
		pStepsDone++;
		if(pStepsDone>RAY_STEPS_MAX)
		{
			return(pNrstObj);
		}
		for(objectID=0; objectID<objectsTotal; objectID++)
		{
			Obj=SceneObjects->at(objectID);
			if(!Obj->Visible())
			{
				continue;
			}
			if(Obj==pObjectToIgnore)
			{
				continue;
			}
			dist=Obj->GetDistance(*Position);
//			distanceMap.at(objectid)=dist;
			if(mindist>dist)
			{
				mindist=dist;
				pNrstObj=Obj;
			}
		}
		SetPosition(*Position+*direction*mindist);
	}
	return(pNrstObj);
}

// totally incorrect. NTF, WIP
LightSource *Ray::GetNearestLightSource()
{
	double mindist=-1U, dist;
	LightSource *lsource=nullptr;
	for(Object *obj: *SceneLights)
	{
		dist=(*obj->Position-*this->Position).Length();
		if(dist<mindist)
		{
			mindist=dist;
			lsource=(LightSource *)obj;
		}
	}
	return(lsource);
}

// ========= SPHERE ===

Sphere::Sphere()
{
	pRadius=1;
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
	pLength=1;
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
	pRadius1=2;
	pRadius2=1;
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

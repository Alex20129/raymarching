#include <cstdio>
#include <cfloat>
#include <cmath>
#include "octree.hpp"
#include "prng.hpp"

uint64_t Object::sLastKnownObjectID=0;

Vec3f Object::WorldToLocal(const Vec3f &point) const
{
	Vec3f dir=point-pPosition;
	return Vec3f(dir.Dot(pVRight), dir.Dot(pVUp), dir.Dot(pVForward));
}

void Object::UpdateBasis(const Vec3f &forward)
{
	pVForward=forward;
	pVForward.Normalize();
	if(abs(pVForward.X) < 1.0)
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
	pID=sLastKnownObjectID++;
	UpdateBasis(Vec3f(0, 0, 1));
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

uint64_t Object::PassthroughChance() const
{
	return(pPassthroughChance);
}

float Object::Brightness() const
{
	return(pBrightness);
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
	return(pSpecularity);
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
	return(pTransparency);
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
	pTransparency=transparency;
	uint64_t multiplicationTrick=transparency*512.0;
	uint64_t remainder=transparency*511.0;
	pPassthroughChance=UINT64_MAX>>9;
	pPassthroughChance*=multiplicationTrick;
	pPassthroughChance+=remainder;
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

const Vec3f &Object::Position() const
{
	return(pPosition);
}

void Object::SetPosition(const Vec3f &position)
{
	pPosition=position;
}

void Object::SetPosition(float x, float y, float z)
{
	pPosition.X=x;
	pPosition.Y=y;
	pPosition.Z=z;
}

const Vec3f &Object::Orientation() const
{
	return(pVForward);
}

void Object::SetOrientation(const Vec3f &orientation)
{
	UpdateBasis(orientation);
}

void Object::SetOrientation(float x, float y, float z)
{
	UpdateBasis(Vec3f(x, y, z));
}

float Object::GetDistance(const Vec3f &from) const
{
	return((from-pPosition).Length());
}

// Using the gradient of the SDF, estimate the normal vector on the surface at given point
Vec3f Object::GetNormalVector(const Vec3f &point) const
{
	Vec3f normalVec(
		GetDistance(point + Vec3f(NORMAL_CALCULATION_DIST, 0, 0)) - GetDistance(point - Vec3f(NORMAL_CALCULATION_DIST, 0, 0)),
		GetDistance(point + Vec3f(0, NORMAL_CALCULATION_DIST, 0)) - GetDistance(point - Vec3f(0, NORMAL_CALCULATION_DIST, 0)),
		GetDistance(point + Vec3f(0, 0, NORMAL_CALCULATION_DIST)) - GetDistance(point - Vec3f(0, 0, NORMAL_CALCULATION_DIST)));
	return(normalVec);
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
	pPosition=object_a->Position();
	pColor=(object_a->Color()+object_b->Color())/2.0;
	pBrightness=(object_a->Brightness()+object_b->Brightness())/2.0;
	pSpecularity=(object_a->Specularity()+object_b->Specularity())/2.0;
	pTransparency=(object_a->Transparency()+object_b->Transparency())/2.0;
	object_a->SetVisible(false);
	object_b->SetVisible(false);
}

float Difference::GetDistance(const Vec3f &from) const
{
	float DistA=ObjectA->GetDistance(from);
	float DistB=ObjectB->GetDistance(from);
	return fmax(DistA, -DistB);
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
	pPosition=(object_a->Position()+object_b->Position())/2.0;
	pColor=(object_a->Color()+object_b->Color())/2.0;
	pBrightness=(object_a->Brightness()+object_b->Brightness())/2.0;
	pSpecularity=(object_a->Specularity()+object_b->Specularity())/2.0;
	pTransparency=(object_a->Transparency()+object_b->Transparency())/2.0;
	object_a->SetVisible(false);
	object_b->SetVisible(false);
}

float Union::GetDistance(const Vec3f &from) const
{
	float DistA=ObjectA->GetDistance(from);
	float DistB=ObjectB->GetDistance(from);
	return(fmin(DistA, DistB));
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
	pPosition=(object_a->Position()+object_b->Position())/2.0;
	pColor=(object_a->Color()+object_b->Color())/2.0;
	pBrightness=(object_a->Brightness()+object_b->Brightness())/2.0;
	pSpecularity=(object_a->Specularity()+object_b->Specularity())/2.0;
	pTransparency=(object_a->Transparency()+object_b->Transparency())/2.0;
	object_a->SetVisible(false);
	object_b->SetVisible(false);
}

float Intersection::GetDistance(const Vec3f &from) const
{
	float DistA=ObjectA->GetDistance(from);
	float DistB=ObjectB->GetDistance(from);
	return(fmax(DistA, DistB));
}

// ========= RAY ===

Ray::Ray()
{
	prng64 tempPRNG;
	pPrngSeedValue=tempPRNG.get_rn_uint()+this->ID();
	pReflectionsLimit=7;
	pStepsPerRunLimit=1024;
}

void Ray::SetDefaultOrientation(float x, float y, float z)
{
	Vec3f newDefaultOrientation(x, y, z);
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

void Ray::Trace()
{
	uint64_t ReflectionsHappened=0, ReflectionsLimit=pReflectionsLimit;
	Vec3f ColorSample(1.0, 1.0, 1.0);
	prng64 StackLocalPRNG;
	StackLocalPRNG.set_seed_value(pPrngSeedValue);

	SetPosition(pFirstCollisionPoint);
	SetOrientation(pDefaultOrientation);

	const Object *TransparentObject=nullptr;
	Vec3f Direction=pVForward;

	if(pFirstCollisionPoint.X==0 && pFirstCollisionPoint.Y==0 && pFirstCollisionPoint.Z==0)
	{
		RunOnce(Direction);
		pFirstCollisionPoint=pPosition;
	}

	while(ReflectionsHappened++<ReflectionsLimit)
	{
		StackLocalPRNG.generate_xorshift_star();

		const Object *Obstacle=RunOnce(Direction, TransparentObject);
		if(Obstacle==nullptr)
		{
			break;
		}

		if(Obstacle->Brightness()>0.0)
		{
			ColorSample=ColorSample * Obstacle->Color() * Obstacle->Brightness();
			break;
		}
		else
		{
			ColorSample=ColorSample * Obstacle->Color() / 255.0;
		}

		if(StackLocalPRNG.get_rn_uint()<Obstacle->PassthroughChance())
		{
			TransparentObject=Obstacle;
			continue;
		}
		else
		{
			TransparentObject=nullptr;
		}
		Vec3f SurfaceNormalVec=Obstacle->GetNormalVector(pPosition);
		SurfaceNormalVec.Normalize();
		if(StackLocalPRNG.get_rn_uint()<Obstacle->DiffusionChance())
		{
			Vec3f randomVector;
			do
			{
				randomVector.X=StackLocalPRNG.get_rn_fp();
				StackLocalPRNG.generate_xorshift();
				randomVector.Y=StackLocalPRNG.get_rn_fp();
				StackLocalPRNG.generate_xorshift();
				randomVector.Z=StackLocalPRNG.get_rn_fp();
				StackLocalPRNG.generate_xorshift();
			}
			while(randomVector.LengthSquared()>1.0);
			Direction=SurfaceNormalVec + randomVector;
		}
		else
		{
			Direction=Direction - (SurfaceNormalVec*2.0) * SurfaceNormalVec.Dot(Direction);
		}
		Direction.Normalize();
		pPosition=pPosition+Direction;
	}
	pPrngSeedValue=StackLocalPRNG.get_rn_uint();
	pColor=pColor+ColorSample;
}

const Object *Ray::RunOnce(const Vec3f &direction, const Object *skip)
{
	uint64_t StepsTaken=0, StepsPerRunLimit=pStepsPerRunLimit;
	Vec3f Position=pPosition;
	while(StepsTaken<StepsPerRunLimit)
	{
		float minDistance=FLT_MAX, Distance;
		const Object *ClosestObject=nullptr;
		const OctreeNode *Node=SceneTree->GetClosestLeafNode(Position);
		for(int obj=0; Node->objects[obj] && obj<4; obj++)
		{
			if(Node->objects[obj]==skip)
			{
				continue;
			}
			Distance=Node->objects[obj]->GetDistance(Position);
			if(minDistance>Distance)
			{
				minDistance=Distance;
				ClosestObject=Node->objects[obj];
			}
		}
		if(minDistance<RAY_COLLISION_DISTANCE)
		{
			pPosition=Position;
			return(ClosestObject);
		}
		StepsTaken++;
		Position=Position+direction*minDistance;
	}
	pPosition=Position;
	return(nullptr);
}

// ========= SPHERE ===

Sphere::Sphere()
{
	pRadius=1.0;
}

void Sphere::SetRadius(float radius)
{
	pRadius=radius;
}

float Sphere::GetDistance(const Vec3f &from) const
{
	return((from-pPosition).Length()-pRadius);
}

Vec3f Sphere::GetNormalVector(const Vec3f &point) const
{
	return(point-pPosition);
}

// ========= CUBE ===

Cube::Cube()
{
	pHalfLength=0.5;
}

void Cube::SetLength(float length)
{
	pHalfLength=length/2.0;
}

float Cube::GetDistance(const Vec3f &from) const
{
	Vec3f d=WorldToLocal(from).Abs()-Vec3f(pHalfLength, pHalfLength, pHalfLength);
	return(Vec3f::Max(d, Vec3f(0, 0, 0)).Length() + fmin(fmax(d.X, fmax(d.Y, d.Z)), 0.0));
}

// ========= CYLINDER ===

Cylinder::Cylinder()
{
	pLength=1.0;
	pRadius=1.0;
}

void Cylinder::SetLength(float length)
{
	pLength=length;
}

void Cylinder::SetRadius(float radius)
{
	pRadius=radius;
}

float Cylinder::GetDistance(const Vec3f &from) const
{
	Vec3f localFrom=WorldToLocal(from);
	float dXY=Vec2f(localFrom.X, localFrom.Y).Length() - pRadius;
	float dZ=abs(localFrom.Z) - pLength / 2.0;
	Vec2f d(fmax(dXY, dZ), fmax(dXY, -dZ));
	return fmin(d.Length(), fmax(dXY, dZ));
}

// ========= TORUS ===

Torus::Torus()
{
	pRadius1=2.0;
	pRadius2=1.0;
}

void Torus::SetRadius1(float radius)
{
	pRadius1=radius;
}

void Torus::SetRadius2(float radius)
{
	pRadius2=radius;
}

float Torus::GetDistance(const Vec3f &from) const
{
	Vec3f localFrom=WorldToLocal(from);
	Vec2f d=Vec2f(Vec2f(localFrom.X, localFrom.Y).Length()-pRadius1, localFrom.Z);
	return(d.Length()-pRadius2);
}

// ========= PLANE ===

float Plane::GetDistance(const Vec3f &from) const
{
	return((from-pPosition).Dot(pVForward));
}

Vec3f Plane::GetNormalVector(const Vec3f &point) const
{
	return(pVForward);
}

// ========= GYROID ===

Gyroid::Gyroid()
{
	pScale=1.0;
}

void Gyroid::SetScale(float scale)
{
	pScale=scale;
}

float Gyroid::GetDistance(const Vec3f &from) const
{
	Vec3f localFrom=WorldToLocal(from)/pScale;
	return(cos(localFrom.X)*sin(localFrom.Y) + cos(localFrom.Y)*sin(localFrom.Z) + cos(localFrom.Z)*sin(localFrom.X));
}

// ========= Schwarz primitive ===

SchwarzPrimitive::SchwarzPrimitive()
{
	pScale=1.0;
}

void SchwarzPrimitive::SetScale(float scale)
{
	pScale=scale;
}

float SchwarzPrimitive::GetDistance(const Vec3f &from) const
{
	Vec3f localFrom=WorldToLocal(from)/pScale;
	return(cos(localFrom.X) + cos(localFrom.Y) + cos(localFrom.Z));
}

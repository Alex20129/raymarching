#include <cstdio>
#include <cfloat>
#include <cmath>
#include <thread>
#include <queue>
#include <chrono>
#include "prng.hpp"
#include "scene.hpp"

// ========= RAY ===

uint32_t Ray::STEPS_PER_RUN_LIMIT = 1024u;
uint32_t Ray::REFLECTIONS_LIMIT = 5u;

void Ray::SetDefaultDirection(float x, float y, float z)
{
	Vec3f newDefaultOrientation(x, y, z);
	newDefaultOrientation.Normalize();
	pDefaultDirection=newDefaultOrientation;
}

static inline void ui64toVec3f(uint64_t uval, Vec3f &result)
{
	union fpConverter
	{
		uint32_t uv;
		float fpv;
	} rn;

	rn.uv=(uval & 0xFFFFF)<<3;
	rn.uv=rn.uv | 0x3F800000;
	rn.fpv=rn.fpv-1.0f;
	rn.uv|=(uval & 0x100000)<<11;
	result.X=rn.fpv;

	uval=uval>>21;

	rn.uv=(uval & 0xFFFFF)<<3;
	rn.uv=rn.uv | 0x3F800000;
	rn.fpv=rn.fpv-1.0f;
	rn.uv|=(uval & 0x100000)<<11;
	result.Y=rn.fpv;

	uval=uval>>21;

	rn.uv=(uval & 0xFFFFF)<<3;
	rn.uv=rn.uv | 0x3F800000;
	rn.fpv=rn.fpv-1.0f;
	rn.uv|=(uval & 0x100000)<<11;
	result.Z=rn.fpv;
}

void Ray::Reset()
{
	Color.X=
	Color.Y=
	Color.Z=0.0f;
	pFirstCollisionPoint.X=
	pFirstCollisionPoint.Y=
	pFirstCollisionPoint.Z=0.0f;
}

void Ray::Trace()
{
	uint32_t ReflectionsLimit=Ray::REFLECTIONS_LIMIT;
	Vec3f ColorSample(1.0, 1.0, 1.0);

	prng64 StackLocalPRNG;
	StackLocalPRNG.set_seed_value(PRNGSeedValue);

	const Object *TransparentObject=nullptr;
	Vec3f Position=pFirstCollisionPoint, Direction=pDefaultDirection;

	if(pFirstCollisionPoint.X==0.0f && pFirstCollisionPoint.Y==0.0f && pFirstCollisionPoint.Z==0.0f)
	{
		if(nullptr==RunOnce(pFirstCollisionPoint, Direction, TransparentObject))
		{
			pFirstCollisionPoint.X=
			pFirstCollisionPoint.Y=
			pFirstCollisionPoint.Z=0.0f;
		}
	}

	for(uint32_t ReflectionsHappened=0; ReflectionsHappened<ReflectionsLimit; ReflectionsHappened++)
	{
		const Object *Obstacle=RunOnce(Position, Direction, TransparentObject);
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
		StackLocalPRNG.generate_xorshift_star();
		if(StackLocalPRNG.get_rn_uint()<Obstacle->PassthroughChance())
		{
			TransparentObject=Obstacle;
			continue;
		}
		else
		{
			TransparentObject=nullptr;
		}
		Vec3f SurfaceNormalVec=Obstacle->GetNormalVector(Position);
		SurfaceNormalVec.Normalize();
		if(StackLocalPRNG.get_rn_uint()<Obstacle->DiffusionChance())
		{
			Vec3f randomVector;
			do
			{
				ui64toVec3f(StackLocalPRNG.get_rn_uint(), randomVector);
				StackLocalPRNG.generate_xorshift_star();
			}
			while(randomVector.LengthSquared()>1.0);
			Direction=SurfaceNormalVec + randomVector;
		}
		else
		{
			Direction=Direction - (SurfaceNormalVec*2.0) * SurfaceNormalVec.Dot(Direction);
		}
		Direction.Normalize();
		Position=Position+Direction;
	}
	PRNGSeedValue=StackLocalPRNG.get_rn_uint();
	Color=Color+ColorSample;
}

const Object *Ray::RunOnce(Vec3f &position, Vec3f direction, const Object *skip)
{
	vector <const Object *> *Objects=this->VisibleObjects;
	uint32_t StepsPerRunLimit=Ray::STEPS_PER_RUN_LIMIT;
	for (uint32_t StepsTaken=0; StepsTaken<StepsPerRunLimit; StepsTaken++)
	{
		float minDistance=FLT_MAX, Distance;
		for (const Object *ThisObject : *Objects)
		{
			if (skip!=ThisObject)
			{
				Distance=ThisObject->GetDistance(position);
				if (minDistance>Distance)
				{
					minDistance=Distance;
				}
			}
			if (minDistance<EPSILON)
			{
				return (ThisObject);
			}
		}
		position=position+direction*minDistance;
	}
	return (nullptr);
}

// ========= SCENE ===

uint64_t Scene::SAMPLES_PER_PIXEL = 8u;

Scene::Scene()
{
	pSceneObjects=new vector <Object *>;
	pVisibleObjects=new vector <const Object *>;
	if(pRenderThreads<thread::hardware_concurrency())
	{
		pRenderThreads=thread::hardware_concurrency();
	}
	SetScreenSize(DefaultScreenWidth, DefaultScreenHeight);
	SceneRays.reserve(pScreenHeight*pScreenWidth+1);
	uint64_t X, Y;
	for(Y=0; Y<pScreenHeight; Y++)
	{
		for(X=0; X<pScreenWidth; X++)
		{
			SceneRays.push_back(Ray());
			SceneRays.back().SetDefaultDirection(X-pScreenWidth/2.0, Y-pScreenHeight/2.0, pScreenWidth);
			SceneRays.back().PRNGSeedValue=pScreenHeight*pScreenWidth+X*X+Y*Y+X*Y;
			SceneRays.back().VisibleObjects=pVisibleObjects;
		}
	}
}

Scene::~Scene()
{
	while(!pSceneObjects->empty())
	{
		Object *object=pSceneObjects->back();
		pSceneObjects->pop_back();
		delete object;
	}
	delete(pVisibleObjects);
	delete(pSceneObjects);
}

uint32_t Scene::AddObject(Object *object)
{
	uint32_t ObjectID=UINT32_MAX;
	if(nullptr!=object)
	{
		ObjectID=pSceneObjects->size();
		pSceneObjects->push_back(object);
	}
	return (ObjectID);
}

uint32_t Scene::AddObject(Object::ObjectType object_type, uint32_t parent_a_id, uint32_t parent_b_id)
{
	uint32_t ObjectID=UINT32_MAX;
	switch (object_type)
	{
		default:
		case Object::ObjectType::OBJECT:
		{
			return (AddObject(new Object));
		}
		case Object::ObjectType::DIFFERENCE:
		{
			if(parent_a_id==UINT32_MAX)
			{
				break;
			}
			if(parent_b_id==UINT32_MAX)
			{
				break;
			}
			Object *ParentObjectA=(*pSceneObjects)[parent_a_id];
			Object *ParentObjectB=(*pSceneObjects)[parent_b_id];
			return (AddObject(new Difference(ParentObjectA, ParentObjectB)));
		}
		case Object::ObjectType::UNION:
		{
			if(parent_a_id==UINT32_MAX)
			{
				break;
			}
			if(parent_b_id==UINT32_MAX)
			{
				break;
			}
			Object *ParentObjectA=(*pSceneObjects)[parent_a_id];
			Object *ParentObjectB=(*pSceneObjects)[parent_b_id];
			return (AddObject(new Union(ParentObjectA, ParentObjectB)));
		}
		case Object::ObjectType::INTERSECTION:
		{
			if(parent_a_id==UINT32_MAX)
			{
				break;
			}
			if(parent_b_id==UINT32_MAX)
			{
				break;
			}
			Object *ParentObjectA=(*pSceneObjects)[parent_a_id];
			Object *ParentObjectB=(*pSceneObjects)[parent_b_id];
			return (AddObject(new Intersection(ParentObjectA, ParentObjectB)));
		}
		case Object::ObjectType::SPHERE:
		{
			return (AddObject(new Sphere));
		}
		case Object::ObjectType::CUBE:
		{
			return (AddObject(new Cube));
		}
		case Object::ObjectType::CUBOID:
		{
			return (AddObject(new Cuboid));
		}
		case Object::ObjectType::CYLINDER:
		{
			return (AddObject(new Cylinder));
		}
		case Object::ObjectType::INFINITE_CYLINDER:
		{
			return (AddObject(new InfiniteCylinder));
		}
		case Object::ObjectType::ELLIPTIC_CYLINDER:
		{
			return (AddObject(new EllipticCylinder));
		}
		case Object::ObjectType::INFINITE_ELLIPTIC_CYLINDER:
		{
			return (AddObject(new InfiniteEllipticCylinder));
		}
		case Object::ObjectType::TORUS:
		{
			return (AddObject(new Torus));
		}
		case Object::ObjectType::PLANE:
		{
			return (AddObject(new Plane));
		}
		case Object::ObjectType::GYROID:
		{
			return (AddObject(new Gyroid));
		}
		case Object::ObjectType::SCHWARZ_PRIMITIVE:
		{
			return (AddObject(new SchwarzPrimitive));
		}
	}
	return (ObjectID);
}

static void RayRunningWrapper(vector <Ray> *rays, uint64_t thread_id, uint64_t rays_per_thread, uint64_t samples_per_pixel)
{
	uint64_t rayid, sample;
	for(rayid=thread_id*rays_per_thread; rayid<(thread_id+1)*rays_per_thread; rayid++)
	{
		(*rays)[rayid].Reset();
		for(sample=0; sample<samples_per_pixel; sample++)
		{
			(*rays)[rayid].Trace();
		}
	}
}

void Scene::Render()
{
	uint64_t threadid, samplesPerPixel=Scene::SAMPLES_PER_PIXEL;
	float colorDiv=samplesPerPixel;
	thread *renderThread;
	queue<thread *> renderThreads;

	pVisibleObjects->clear();
	for(Object *object : *pSceneObjects)
	{
		if(object->Visibility())
		{
			pVisibleObjects->push_back(object);
		}
	}

	chrono::time_point <chrono::high_resolution_clock> finish;
	chrono::time_point <chrono::high_resolution_clock> start=chrono::high_resolution_clock::now();

	for(threadid=0; threadid<pRenderThreads; threadid++)
	{
		renderThread=new thread(RayRunningWrapper, &SceneRays, threadid, SceneRays.size()/pRenderThreads, samplesPerPixel);
		renderThreads.push(renderThread);
	}
	while(!renderThreads.empty())
	{
		renderThreads.front()->join();
		delete renderThreads.front();
		renderThreads.pop();
	}
	for(uint64_t y=0; y < pScreenHeight; y++)
	{
		for(uint64_t x=0; x < pScreenWidth; x++)
		{
			int64_t rayID=x+y*pScreenWidth;
			Vec3f color=SceneRays.at(rayID).Color/colorDiv;
			uint8_t r = fmin(color.X, 255.0f);
			uint8_t g = fmin(color.Y, 255.0f);
			uint8_t b = fmin(color.Z, 255.0f);
			RenderedImage[y][x] = png::rgb_pixel(r, g, b);
		}
	}
	finish=chrono::high_resolution_clock::now();
	pRenderTime=(finish - start).count()/1000000;
	fprintf(stdout, "SamplesPerPixel: %lu\n", samplesPerPixel);
	fprintf(stdout, "RenderThreads: %lu\n", pRenderThreads);
	fprintf(stdout, "RenderTime: %li ms\n", pRenderTime);
}

uint64_t Scene::ScreenWidth() const
{
	return (pScreenWidth);
}

uint64_t Scene::ScreenHeight() const
{
	return (pScreenHeight);
}

uint64_t Scene::RenderThreads() const
{
	return (pRenderThreads);
}

int64_t Scene::RenderTime() const
{
	return (pRenderTime);
}

void Scene::SetScreenWidth(uint64_t width)
{
	SetScreenSize(width, pScreenHeight);
}

void Scene::SetScreenHeight(uint64_t height)
{
	SetScreenSize(pScreenWidth, height);
}

void Scene::SetScreenSize(uint64_t width, uint64_t height)
{
	pScreenWidth=width;
	pScreenHeight=height;
	RenderedImage.resize(width, height);
}

void Scene::SetRenderThreads(uint64_t render_threads)
{
	pRenderThreads=render_threads;
}

// ========= OBJECT MANIPULATION ===

Object::ObjectType Scene::GetObjectType(uint32_t object_id) const
{
	return ((*pSceneObjects)[object_id]->Type());
}

bool Scene::GetObjectVisibility(uint32_t object_id) const
{
	return ((*pSceneObjects)[object_id]->Visibility());
}

void Scene::SetObjectVisibility(uint32_t object_id, bool visibility)
{
	(*pSceneObjects)[object_id]->SetVisibility(visibility);
}

float Scene::GetObjectBrightness(uint32_t object_id) const
{
	return ((*pSceneObjects)[object_id]->Brightness());
}

void Scene::SetObjectBrightness(uint32_t object_id, float brightness)
{
	(*pSceneObjects)[object_id]->SetBrightness(brightness);
}

float Scene::GetObjectSpecularity(uint32_t object_id) const
{
	return ((*pSceneObjects)[object_id]->Specularity());
}

void Scene::SetObjectSpecularity(uint32_t object_id, float specularity)
{
	(*pSceneObjects)[object_id]->SetSpecularity(specularity);
}

float Scene::GetObjectTransparency(uint32_t object_id) const
{
	return ((*pSceneObjects)[object_id]->Transparency());
}

void Scene::SetObjectTransparency(uint32_t object_id, float transparency)
{
	(*pSceneObjects)[object_id]->SetTransparency(transparency);
}

const Vec3f &Scene::GetObjectColor(uint32_t object_id) const
{
	return ((*pSceneObjects)[object_id]->Color());
}

void Scene::SetObjectColor(uint32_t object_id, const Vec3f &color)
{
	(*pSceneObjects)[object_id]->SetColor(color);
}

void Scene::SetObjectColor(uint32_t object_id, float r, float g, float b)
{
	(*pSceneObjects)[object_id]->SetColor(r, g, b);
}

const Vec3f &Scene::GetObjectPosition(uint32_t object_id) const
{
	return ((*pSceneObjects)[object_id]->Position());
}

void Scene::SetObjectPosition(uint32_t object_id, const Vec3f &position)
{
	(*pSceneObjects)[object_id]->SetPosition(position);
}

void Scene::SetObjectPosition(uint32_t object_id, float x, float y, float z)
{
	(*pSceneObjects)[object_id]->SetPosition(x, y, z);
}

const Vec3f &Scene::GetObjectOrientation(uint32_t object_id) const
{
	return ((*pSceneObjects)[object_id]->Orientation());
}

void Scene::SetObjectOrientation(uint32_t object_id, const Vec3f &orientation, float roll)
{
	(*pSceneObjects)[object_id]->SetOrientation(orientation, roll);
}

void Scene::SetObjectOrientation(uint32_t object_id, float x, float y, float z, float roll)
{
	(*pSceneObjects)[object_id]->SetOrientation(x, y, z, roll);
}

float Scene::GetObjectProperty(uint32_t object_id, uint32_t property) const
{
	return ((*pSceneObjects)[object_id]->Property(property));
}

void Scene::SetObjectProperty(uint32_t object_id, uint32_t property, float value)
{
	(*pSceneObjects)[object_id]->SetProperty(property, value);
}

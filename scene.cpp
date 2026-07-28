#include <cstdio>
#include <cfloat>
#include <cmath>
#include <thread>
#include <queue>
#include <chrono>
#include "scene.hpp"

Scene::Scene()
{
	pSceneObjects=new vector <Object *>;

	if(pRenderThreads<thread::hardware_concurrency())
	{
		pRenderThreads=thread::hardware_concurrency();
	}

	SetScreenSize(DefaultScreenWidth, DefaultScreenHeight);

	SceneRays.reserve(pScreenHeight*pScreenWidth+1);

	uint64_t X, Y;
	for(Y=0; Y<pScreenHeight; ++Y)
	{
		for(X=0; X<pScreenWidth; ++X)
		{
			SceneRays.push_back(Ray());
			SceneRays.back().SetDefaultDirection(X-pScreenWidth/2.0, Y-pScreenHeight/2.0, pScreenWidth);
			SceneRays.back().SceneObjects=this->pSceneObjects;
			SceneRays.back().PRNGSeedValue=pScreenHeight*pScreenWidth+X*X+Y*Y+X*Y;
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
}

void Scene::AddObject(Object *object)
{
	this->pSceneObjects->push_back(object);
	this->pSceneObjectsIndex[object->ID()]=object;
}

uint64_t Scene::AddObject(Object::ObjectType object_type, uint64_t parent_a, uint64_t parent_b)
{
	uint64_t ObjectID=UINT64_MAX;
	switch (object_type)
	{
		default:
		case Object::ObjectType::OBJECT:
		{
			Object *NewObject=new Object;
			ObjectID=NewObject->ID();
			this->pSceneObjects->push_back(NewObject);
			this->pSceneObjectsIndex[ObjectID]=NewObject;
			break;
		}
		case Object::ObjectType::DIFFERENCE:
		{
			break;
		}
		case Object::ObjectType::UNION:
		{
			break;
		}
		case Object::ObjectType::INTERSECTION:
		{
			break;
		}
		case Object::ObjectType::SPHERE:
		{
			Object *NewSphere=new Sphere;
			ObjectID=NewSphere->ID();
			this->pSceneObjects->push_back(NewSphere);
			this->pSceneObjectsIndex[ObjectID]=NewSphere;
			break;
		}
		case Object::ObjectType::CUBE:
		{
			Object *NewCube=new Cube;
			ObjectID=NewCube->ID();
			this->pSceneObjects->push_back(NewCube);
			this->pSceneObjectsIndex[ObjectID]=NewCube;
			break;
		}
		case Object::ObjectType::CYLINDER:
		{
			Object *NewCylinder=new Cylinder;
			ObjectID=NewCylinder->ID();
			this->pSceneObjects->push_back(NewCylinder);
			this->pSceneObjectsIndex[ObjectID]=NewCylinder;
			break;
		}
		case Object::ObjectType::TORUS:
		{
			Object *NewTorus=new Torus;
			ObjectID=NewTorus->ID();
			this->pSceneObjects->push_back(NewTorus);
			this->pSceneObjectsIndex[ObjectID]=NewTorus;
			break;
		}
		case Object::ObjectType::PLANE:
		{
			Object *NewPlane=new Plane;
			ObjectID=NewPlane->ID();
			this->pSceneObjects->push_back(NewPlane);
			this->pSceneObjectsIndex[ObjectID]=NewPlane;
			break;
		}
		case Object::ObjectType::GYROID:
		{
			Object *NewGyroid=new Gyroid;
			ObjectID=NewGyroid->ID();
			this->pSceneObjects->push_back(NewGyroid);
			this->pSceneObjectsIndex[ObjectID]=NewGyroid;
			break;
		}
		case Object::ObjectType::SCHWARZ_PRIMITIVE:
		{
			Object *NewSchwarzPrimitive=new SchwarzPrimitive;
			ObjectID=NewSchwarzPrimitive->ID();
			this->pSceneObjects->push_back(NewSchwarzPrimitive);
			this->pSceneObjectsIndex[ObjectID]=NewSchwarzPrimitive;
			break;
		}
	}
	return(ObjectID);
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
	uint64_t threadid;
	thread *renderThread;
	float colorDiv=pSamplesPerPixel;
	queue<thread *> renderThreads;

	chrono::time_point <chrono::high_resolution_clock> finish;
	chrono::time_point <chrono::high_resolution_clock> start=chrono::high_resolution_clock::now();

	for(threadid=0; threadid<pRenderThreads; threadid++)
	{
		renderThread=new thread(RayRunningWrapper, &SceneRays, threadid, SceneRays.size()/pRenderThreads, pSamplesPerPixel);
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
	fprintf(stdout, "SamplesPerPixel: %lu\n", pSamplesPerPixel);
	fprintf(stdout, "RenderThreads: %lu\n", pRenderThreads);
	fprintf(stdout, "RenderTime: %li ms\n", pRenderTime);
}

uint64_t Scene::ScreenWidth() const
{
	return(pScreenWidth);
}

uint64_t Scene::ScreenHeight() const
{
	return(pScreenHeight);
}

uint64_t Scene::RenderThreads() const
{
	return(pRenderThreads);
}

uint64_t Scene::SamplesPerPixel() const
{
	return(pSamplesPerPixel);
}

int64_t Scene::RenderTime() const
{
	return(pRenderTime);
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

void Scene::SetSamplesPerPixel(uint64_t samples_per_pixel)
{
	pSamplesPerPixel=samples_per_pixel;
}

#include <cstdio>
#include <cfloat>
#include <cmath>
#include <thread>
#include <queue>
#include <chrono>
#include "scene.hpp"

Scene::Scene()
{
	uint64_t X, Y;
	Ray *newRay;

	SceneTree=new Octree;
	SceneObjects=new vector <Object *>;
	SceneRays=new vector <Ray *>;

	if(pRenderThreads<thread::hardware_concurrency())
	{
		pRenderThreads=thread::hardware_concurrency();
	}

	SetScreenSize(DefaultScreenWidth, DefaultScreenHeight);

	for(Y=0; Y<pScreenHeight; ++Y)
	{
		for(X=0; X<pScreenWidth; ++X)
		{
			newRay=new Ray();
			newRay->SetDefaultOrientation(X-pScreenWidth/2.0, Y-pScreenHeight/2.0, pScreenWidth);
			newRay->SceneTree=this->SceneTree;
			SceneRays->push_back(newRay);
		}
	}
}

Scene::~Scene()
{
	while(!SceneObjects->empty())
	{
		Object *object=SceneObjects->back();
		SceneObjects->pop_back();
		delete object;
	}
	while(!SceneRays->empty())
	{
		Ray *ray=SceneRays->back();
		SceneRays->pop_back();
		delete ray;
	}
}

void Scene::RebuildSceneTree()
{
	SceneTree->Build((vector <const Object *> *)(SceneObjects));
}

void Scene::AddObject(Object *object)
{
	this->SceneObjects->push_back(object);
	object->SceneTree=this->SceneTree;
}

static void RayRunningWrapper(const vector <Ray *> *rays, uint64_t thread_id, uint64_t rays_per_thread, uint64_t samples_per_pixel)
{
	uint64_t rayid, sample;
	for(rayid=thread_id*rays_per_thread; rayid<(thread_id+1)*rays_per_thread; rayid++)
	{
		Ray *rayPtr=(*rays)[rayid];
		rayPtr->SetColor(0, 0, 0);
		for(sample=0; sample<samples_per_pixel; sample++)
		{
			rayPtr->Trace();
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
		renderThread=new thread(RayRunningWrapper, SceneRays, threadid, SceneRays->size()/pRenderThreads, pSamplesPerPixel);
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
			Vec3f color=SceneRays->at(rayID)->Color()/colorDiv;
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

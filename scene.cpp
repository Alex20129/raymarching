#include <cstdio>
#include "scene.hpp"

Scene::Scene()
{
	int X, Y;
	Ray *newRay;

	SceneObjects=new vector <Object *>;
	SceneLights=new vector <Object *>;
	SceneRays=new vector <Ray *>;
	ImageData=new vector <uint8_t>;
	pRayRunsPerPixel=1;

	pRenderThreads=thread::hardware_concurrency();
	if(pRenderThreads<1)
	{
		pRenderThreads=1;
	}

	SetScreenSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);

	for(Y=0; Y<pScreenHeight; ++Y)
	{
		for(X=0; X<pScreenWidth; ++X)
		{
			newRay=new Ray();
			newRay->SetID(SceneRays->size());
			newRay->SetDefaultDirection(X-pScreenWidth/2.0, Y-pScreenHeight/2.0, pScreenWidth);
			newRay->SceneObjects=this->SceneObjects;
			SceneRays->push_back(newRay);
		}
	}
}

Scene::~Scene()
{
	while(!SceneObjects->empty())
	{
		delete SceneObjects->back();
		SceneObjects->pop_back();
	}
	while(!SceneRays->empty())
	{
		delete SceneRays->back();
		SceneRays->pop_back();
	}
}

void Scene::AddObject(Object *object)
{
	this->SceneObjects->push_back(object);
	if(object->Brightness()>0.0)
	{
		this->SceneLights->push_back(object);
	}
	object->SceneObjects=this->SceneObjects;
}

static void RayRunningWrapperFun(vector <Ray *> *rays, uint64_t thread_id, uint64_t rays_per_thread, uint64_t ray_runs_per_pixel)
{
	uint64_t rayid, run;
	vector <Ray *> threadLocalRays=*rays;
	Ray *rayPtr;
	for(rayid=thread_id*rays_per_thread; rayid<(thread_id+1)*rays_per_thread; rayid++)
	{
		rayPtr=threadLocalRays.at(rayid);
		rayPtr->SetColor(0, 0, 0);
		for(run=0; run<ray_runs_per_pixel; run++)
		{
			rayPtr->Reset();
			rayPtr->Run();
		}
	}
}

void Scene::Render()
{
	uint64_t threadid;
	thread *renderThread;
	float colorDiv=pRayRunsPerPixel;
	queue<thread *> renderThreads;

	chrono::time_point <chrono::high_resolution_clock> start=chrono::high_resolution_clock::now(), end;
	for(threadid=0; threadid<pRenderThreads; threadid++)
	{
		renderThread=new thread(RayRunningWrapperFun, SceneRays, threadid, SceneRays->size()/pRenderThreads, pRayRunsPerPixel);
		renderThreads.push(renderThread);
	}
	while(!renderThreads.empty())
	{
		renderThreads.front()->join();
		delete renderThreads.front();
		renderThreads.pop();
	}
	for(size_t ray_id=0; ray_id<SceneRays->size(); ray_id++)
	{
		int32_t c;
		Vec3f color=SceneRays->at(ray_id)->Color();

		c=color.X/colorDiv;
		c=std::min(c, 255);
		ImageData->data()[ray_id*4]=c;

		c=color.Y/colorDiv;
		c=std::min(c, 255);
		ImageData->data()[ray_id*4+1]=c;

		c=color.Z/colorDiv;
		c=std::min(c, 255);
		ImageData->data()[ray_id*4+2]=c;
	}
	end=chrono::high_resolution_clock::now();
	FrameRenderTime=chrono::duration_cast <chrono::milliseconds>(end - start);

	fprintf(stdout, "RayRunsPerPixel: %lu\n", pRayRunsPerPixel);
	fprintf(stdout, "FrameRenderTime: %li ms\n", FrameRenderTime.count());
}

uint64_t Scene::ScreenWidth()
{
	return(pScreenWidth);
}

uint64_t Scene::ScreenHeight()
{
	return(pScreenHeight);
}

uint64_t Scene::RenderThreadsNum()
{
	return(pRenderThreads);
}

void Scene::SetScreenWidth(uint64_t width)
{
	SetScreenSize(width, pScreenHeight);
}

void Scene::SetScreenHeight(uint64_t height)
{
	SetScreenSize(pScreenWidth, height);
}

void Scene::SetNumOfRenderThreads(uint64_t render_threads)
{
	pRenderThreads=render_threads;
}

void Scene::SetNumOfRayRunsPerPixel(uint64_t ray_runs_per_pixel)
{
	pRayRunsPerPixel=ray_runs_per_pixel;
}

void Scene::SetScreenSize(uint64_t width, uint64_t height)
{
	pScreenWidth=width;
	pScreenHeight=height;
	ImageData->resize(width*height*4, 255);
}

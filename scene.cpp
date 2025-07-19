#include "scene.hpp"
#include <cstdio>

Scene::Scene()
{
	int X, Y;
	Ray *newRay;

	SceneObjects=new vector <Object *>;
	SceneLights=new vector <Object *>;
	SceneRays=new vector <Ray *>;
	pRenderThreads=new queue<thread *>;

	ImageData=new vector <uint8_t>;

	if(thread::hardware_concurrency()>1)
	{
		pRenderThreadsNum=thread::hardware_concurrency();
	}
	else
	{
		pRenderThreadsNum=DEFAULT_RENDER_THREADS_NUM;
	}

	SetScreenSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);

	for(Y=0; Y<pScreenHeight; ++Y)
	{
		for(X=0; X<pScreenWidth; ++X)
		{
			newRay=new Ray();
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

static void RayRunningWrapperFun(Ray **rays, unsigned int rays_per_thread, unsigned int thread_id)
{
	unsigned int rayid;
	for(rayid=0; rayid<rays_per_thread; rayid++)
	{
		rays[thread_id*rays_per_thread+rayid]->Reset();
		rays[thread_id*rays_per_thread+rayid]->Run();
	}
}

void Scene::Render(unsigned int rays_per_pixel)
{
	thread *newThread;
	unsigned int threadid;
	float colorDiv=rays_per_pixel;
	chrono::time_point <chrono::high_resolution_clock> start=chrono::high_resolution_clock::now(), end;
	while(rays_per_pixel)
	{
		rays_per_pixel--;
		for(threadid=0; threadid<pRenderThreadsNum; threadid++)
		{
			newThread=new thread(RayRunningWrapperFun, SceneRays->data(), SceneRays->size()/pRenderThreadsNum, threadid);
			pRenderThreads->push(newThread);
		}
		while(!pRenderThreads->empty())
		{
			pRenderThreads->front()->join();
			delete pRenderThreads->front();
			pRenderThreads->pop();
		}
	}
	for(size_t ray_id=0; ray_id<SceneRays->size(); ray_id++)
	{
		Vec3f color=SceneRays->at(ray_id)->Color();
		ImageData->data()[ray_id*4]=(uint8_t)(color.X/colorDiv);
		ImageData->data()[ray_id*4+1]=(uint8_t)(color.Y/colorDiv);
		ImageData->data()[ray_id*4+2]=(uint8_t)(color.Z/colorDiv);
	}
	end=chrono::high_resolution_clock::now();
	FrameRenderTime=chrono::duration_cast <chrono::milliseconds>(end - start);
	fprintf(stdout, "FrameRenderTime: %li ms\n", FrameRenderTime.count());
}

unsigned int Scene::ScreenWidth()
{
	return(pScreenWidth);
}

unsigned int Scene::ScreenHeight()
{
	return(pScreenHeight);
}

unsigned int Scene::RenderThreadsNum()
{
	return(pRenderThreadsNum);
}

void Scene::SetScreenWidth(unsigned int width)
{
	SetScreenSize(width, pScreenHeight);
}

void Scene::SetScreenHeight(unsigned int height)
{
	SetScreenSize(pScreenWidth, height);
}

void Scene::SetRenderThreadsNum(unsigned int threads_num)
{
	pRenderThreadsNum=threads_num;
}

void Scene::SetScreenSize(unsigned int width, unsigned int height)
{
	pScreenWidth=width;
	pScreenHeight=height;
	ImageData->resize(width*height*4, 255);
}

#include "scene.hpp"
#include <thread>
#include <cstdio>

Scene::Scene()
{
	int X, Y;
	Ray *newRay;

	SceneObjects=new vector <Object *>;
	SceneLights=new vector <Object *>;
	SceneRays=new vector <Ray *>;

	ImageData=new vector <unsigned char>;

	SetScreenSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);

	for(Y=0; Y<pScreenHeight; ++Y)
	{
		for(X=0; X<pScreenWidth; ++X)
		{
			newRay=new Ray();
			newRay->SetDirection(X-pScreenWidth/2.0, Y-pScreenHeight/2.0, pScreenWidth);
			newRay->AttachExternalColorBuffer((Vec3uc *)&ImageData->data()[SceneRays->size()*4]);
			newRay->SceneObjects=this->SceneObjects;
			newRay->SceneLights=this->SceneLights;
			SceneRays->push_back(newRay);
		}
	}
}

Scene::~Scene()
{
	while(SceneObjects->size())
	{
		delete SceneObjects->back();
		SceneObjects->pop_back();
	}
	while(SceneRays->size())
	{
		delete SceneRays->back();
		SceneRays->pop_back();
	}
}

void Scene::AddObject(Object *object)
{
	if(object->ItsALightSource())
	{
		this->SceneLights->push_back(object);
	}
	else
	{
		this->SceneObjects->push_back(object);
	}

	object->SceneObjects=this->SceneObjects;
	object->SceneLights=this->SceneLights;
}

static void RayRunningWrapperFun(Ray **rays, unsigned int numofrays, unsigned int threadid)
{
	unsigned int rayid, raysPerThread=numofrays/RENDER_THREADS_NUM;
	for(rayid=0; rayid<raysPerThread; rayid++)
	{
		rays[threadid*raysPerThread+rayid]->Reset();
		rays[threadid*raysPerThread+rayid]->Run();
	}
}

void Scene::Render()
{
	unsigned int threadid;
	thread *threadsToRun[RENDER_THREADS_NUM];
	chrono::time_point <chrono::high_resolution_clock> start=chrono::high_resolution_clock::now(), end;
	for(threadid=0; threadid<RENDER_THREADS_NUM; threadid++)
	{
		threadsToRun[threadid]=new thread(RayRunningWrapperFun, SceneRays->data(), SceneRays->size(), threadid);
	}
	for(threadid=0; threadid<RENDER_THREADS_NUM; threadid++)
	{
		threadsToRun[threadid]->join();
	}
	for(threadid=0; threadid<RENDER_THREADS_NUM; threadid++)
	{
		delete threadsToRun[threadid];
	}
	end=chrono::high_resolution_clock::now();
	FrameRenderTime=chrono::duration_cast <chrono::milliseconds>(end - start);
}

unsigned int Scene::ScreenWidth()
{
	return(pScreenWidth);
}
unsigned int Scene::ScreenHeight()
{
	return(pScreenHeight);
}
void Scene::SetScreenWidth(unsigned int width)
{
	SetScreenSize(width, pScreenHeight);
}
void Scene::SetScreenHeight(unsigned int height)
{
	SetScreenSize(pScreenWidth, height);
}

void Scene::SetScreenSize(unsigned int width, unsigned int height)
{
	pScreenWidth=width;
	pScreenHeight=height;
	ImageData->resize(pScreenWidth*pScreenHeight*4, 255);
}

void Scene::SavePPMImage(const char *filename)
{
	unsigned int color, colornum=ImageData->size();
	FILE *fd=fopen(filename, "w");
	if(fd)
	{
		fprintf(fd, "P3 %i %i 255\n", pScreenWidth, pScreenHeight);
		fprintf(fd, "%i", ImageData->data()[0]);
		for(color=1; color<colornum; color++)
		{
			fprintf(fd, " %i", ImageData->data()[color]);
		}
		fprintf(fd, "\n");
		fclose(fd);
	}
}

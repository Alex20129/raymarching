#ifndef SCENE_HPP
#define SCENE_HPP

#include "basicObjects.hpp"
#include <thread>
#include <chrono>

#define DEFAULT_SCREEN_WIDTH		1024
#define DEFAULT_SCREEN_HEIGHT		1024
#define DEFAULT_RENDER_THREADS_NUM	4

using namespace std;

class Scene
{
public:
	Scene();
	~Scene();
	vector <Object *> *SceneObjects, *SceneLights;
	vector <unsigned char> *ImageData;
	chrono::milliseconds FrameRenderTime;
	void AddObject(Object *object);
	void Render();
	unsigned int ScreenWidth();
	unsigned int ScreenHeight();
	unsigned int RenderThreadsNum();
	void SetScreenWidth(unsigned int width);
	void SetScreenHeight(unsigned int height);
	void SetRenderThreadsNum(unsigned int threads_num);
	void SetScreenSize(unsigned int width, unsigned int height);
	void SavePPMImage(const char *filename);
private:
	vector <thread *> *pRenderThreads;
	vector <Ray *> *SceneRays;
	unsigned int pScreenWidth, pScreenHeight, pRenderThreadsNum;
};

#endif // SCENE_HPP

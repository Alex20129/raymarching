#ifndef SCENE_HPP
#define SCENE_HPP

#include "basicObjects.hpp"
#include <thread>
#include <queue>
#include <chrono>

#define DEFAULT_SCREEN_WIDTH		2048
#define DEFAULT_SCREEN_HEIGHT		1536

using namespace std;

class Scene
{
	vector <Ray *> *SceneRays;
	uint64_t pScreenWidth, pScreenHeight;
	uint64_t pRenderThreads;
	uint64_t pRayRunsPerPixel;
public:
	Scene();
	~Scene();
	vector <Object *> *SceneObjects, *SceneLights;
	vector <unsigned char> *ImageData;
	chrono::milliseconds FrameRenderTime;
	void AddObject(Object *object);
	void Render();
	uint64_t ScreenWidth();
	uint64_t ScreenHeight();
	uint64_t RenderThreadsNum();
	void SetScreenWidth(uint64_t width);
	void SetScreenHeight(uint64_t height);
	void SetNumOfRenderThreads(uint64_t render_threads);
	void SetNumOfRayRunsPerPixel(uint64_t ray_runs_per_pixel);
	void SetScreenSize(uint64_t width, uint64_t height);
};

#endif // SCENE_HPP

#ifndef SCENE_HPP
#define SCENE_HPP

#include "octree.hpp"

#define DEFAULT_SCREEN_WIDTH	2048
#define DEFAULT_SCREEN_HEIGHT	1536

class Scene
{
	vector <Ray *> *SceneRays;
	vector <Object *> *SceneObjects;
	int64_t pScreenWidth, pScreenHeight;
	uint64_t pRenderThreads;
	uint64_t pSamplesPerPixel;
public:
	Scene();
	~Scene();
	Octree *SceneTree;
	vector <unsigned char> *ImageData;
	int64_t FrameRenderTime;
	void RebuildSceneTree();
	void AddObject(Object *object);
	void Render();
	int64_t ScreenWidth();
	int64_t ScreenHeight();
	uint64_t RenderThreadsNum();
	void SetScreenWidth(int64_t width);
	void SetScreenHeight(int64_t height);
	void SetNumOfRenderThreads(uint64_t render_threads);
	void SetNumOfSamplesPerPixel(uint64_t samples_per_pixel);
	void SetScreenSize(int64_t width, int64_t height);
};

#endif // SCENE_HPP

#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <png++/png.hpp>
#include "octree.hpp"

#define DEFAULT_SCREEN_WIDTH	2048
#define DEFAULT_SCREEN_HEIGHT	1536

class Scene
{
	Octree *SceneTree;
	vector <Object *> *SceneObjects;
	vector <Ray *> *SceneRays;
	int64_t pScreenWidth, pScreenHeight;
	uint64_t pRenderThreads;
	uint64_t pSamplesPerPixel;
	int64_t pRenderTime;
public:
	vector <unsigned char> *ImageData;
	png::image<png::rgb_pixel> RenderedResult;
	Scene();
	~Scene();
	void RebuildSceneTree();
	void AddObject(Object *object);
	void Render();
	int64_t ScreenWidth() const;
	int64_t ScreenHeight() const;
	uint64_t RenderThreads() const;
	int64_t RenderTime() const;
	void SetScreenWidth(int64_t width);
	void SetScreenHeight(int64_t height);
	void SetNumOfRenderThreads(uint64_t render_threads);
	void SetNumOfSamplesPerPixel(uint64_t samples_per_pixel);
	void SetScreenSize(int64_t width, int64_t height);
};

#endif // SCENE_HPP

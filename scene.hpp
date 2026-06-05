#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <png++/png.hpp>
#include "octree.hpp"

static constexpr uint64_t DefaultScreenWidth=2048;
static constexpr uint64_t DefaultScreenHeight=1536;
static constexpr uint64_t DefaultRenderThreads=1;
static constexpr uint64_t DefaultpSamplesPerPixel=8;

class Scene
{
	Octree *SceneTree;
	vector <Object *> *SceneObjects;
	vector <Ray *> *SceneRays;
	uint64_t pScreenWidth=DefaultScreenWidth;
	uint64_t pScreenHeight=DefaultScreenHeight;
	uint64_t pRenderThreads=DefaultRenderThreads;
	uint64_t pSamplesPerPixel=DefaultpSamplesPerPixel;
	int64_t pRenderTime;
public:
	png::image<png::rgb_pixel> RenderedImage;
	Scene();
	~Scene();
	void RebuildSceneTree();
	void AddObject(Object *object);
	void Render();
	uint64_t ScreenWidth() const;
	uint64_t ScreenHeight() const;
	uint64_t RenderThreads() const;
	uint64_t SamplesPerPixel() const;
	int64_t RenderTime() const;
	void SetScreenWidth(uint64_t width);
	void SetScreenHeight(uint64_t height);
	void SetScreenSize(uint64_t width, uint64_t height);
	void SetRenderThreads(uint64_t render_threads);
	void SetSamplesPerPixel(uint64_t samples_per_pixel);
};

#endif // SCENE_HPP

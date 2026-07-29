#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <png++/png.hpp>
#include "basic_objects.hpp"

static constexpr uint64_t DefaultScreenWidth=2048;
static constexpr uint64_t DefaultScreenHeight=1536;
static constexpr uint64_t DefaultRenderThreads=1;
static constexpr uint64_t DefaultSamplesPerPixel=8;

using namespace std;

class Ray
{
	Vec3f pDefaultDirection;
	Vec3f pFirstCollisionPoint;
public:
	static uint32_t STEPS_PER_RUN_LIMIT;
	static uint32_t REFLECTIONS_LIMIT;
	Vec3f Color;
	vector <Object *> *SceneObjects;
	uint64_t PRNGSeedValue=0;
	void SetDefaultDirection(float x, float y, float z);
	void Reset();
	void Trace();
	const Object *RunOnce(Vec3f &position, Vec3f direction, const Object *skip);
};

class Scene
{
	vector <Ray> SceneRays;
	vector <Object *> *pSceneObjects;
	uint64_t pScreenWidth=DefaultScreenWidth;
	uint64_t pScreenHeight=DefaultScreenHeight;
	uint64_t pRenderThreads=DefaultRenderThreads;
	uint64_t pSamplesPerPixel=DefaultSamplesPerPixel;
	int64_t pRenderTime;
public:
	png::image<png::rgb_pixel> RenderedImage;
	Scene();
	~Scene();
	uint32_t AddObject(Object *object);
	uint32_t AddObject(Object::ObjectType object_type, uint32_t parent_a_id=UINT32_MAX, uint32_t parent_b_id=UINT32_MAX);
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

	Object::ObjectType GetObjectType(uint32_t object_id) const;

	bool GetObjectVisibility(uint32_t object_id) const;
	void SetObjectVisibility(bool visible);

	float GetObjectBrightness(uint32_t object_id) const;
	void SetObjectBrightness(float brightness);

	float GetObjectSpecularity(uint32_t object_id) const;
	void SetObjectSpecularity(float specularity);

	float GetObjectTransparency(uint32_t object_id) const;
	void SetObjectTransparency(float transparency);

	const Vec3f &GetObjectColor(uint32_t object_id) const;
	void SetObjectColor(Vec3f color);
	void SetObjectColor(float r, float g, float b);

	const Vec3f &GetObjectPosition(uint32_t object_id) const;
	void SetObjectPosition(const Vec3f &position);
	void SetObjectPosition(float x, float y, float z);

	const Vec3f &GetObjectOrientation(uint32_t object_id) const;
	void SetObjectOrientation(const Vec3f &orientation);
	void SetObjectOrientation(float x, float y, float z);

	float GetObjectProperty(uint32_t object_id, uint32_t property) const;
	void SetObjectProperty(uint32_t object_id, uint32_t property, float value);
};

#endif // SCENE_HPP

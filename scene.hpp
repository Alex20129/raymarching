#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <png++/png.hpp>
#include "basic_objects.hpp"

static constexpr uint64_t DefaultScreenWidth=2048;
static constexpr uint64_t DefaultScreenHeight=1536;
static constexpr uint64_t DefaultRenderThreads=1;

using namespace std;

class Ray
{
	Vec3f pDefaultDirection;
	Vec3f pFirstCollisionPoint;
	const Object *RunOnce(Vec3f &position, Vec3f direction, const Object *skip);
public:
	static uint32_t STEPS_PER_RUN_LIMIT;
	static uint32_t REFLECTIONS_LIMIT;
	vector <const Object *> *VisibleObjects;
	Vec3f Color;
	uint64_t PRNGSeedValue=0;
	void SetDefaultDirection(float x, float y, float z);
	void Reset();
	void Trace();
};

class Scene
{
	vector <Ray> SceneRays;
	vector <Object *> *pSceneObjects;
	vector <const Object *> *pVisibleObjects;
	uint64_t pScreenWidth=DefaultScreenWidth;
	uint64_t pScreenHeight=DefaultScreenHeight;
	uint64_t pRenderThreads=DefaultRenderThreads;
	int64_t pRenderTime;
public:
	static uint64_t SAMPLES_PER_PIXEL;
	png::image<png::rgb_pixel> RenderedImage;
	Scene();
	~Scene();
	uint32_t AddObject(Object *object);
	uint32_t AddObject(Object::ObjectType object_type, uint32_t parent_a_id=UINT32_MAX, uint32_t parent_b_id=UINT32_MAX);
	void Render();
	uint64_t ScreenWidth() const;
	uint64_t ScreenHeight() const;
	uint64_t RenderThreads() const;
	int64_t RenderTime() const;
	void SetScreenWidth(uint64_t width);
	void SetScreenHeight(uint64_t height);
	void SetScreenSize(uint64_t width, uint64_t height);
	void SetRenderThreads(uint64_t render_threads);

	// ========= OBJECT MANIPULATION ===

	Object::ObjectType GetObjectType(uint32_t object_id) const;

	bool GetObjectVisibility(uint32_t object_id) const;
	float GetObjectBrightness(uint32_t object_id) const;

	float GetObjectSpecularity(uint32_t object_id) const;
	void SetObjectSpecularity(uint32_t object_id, float specularity);

	float GetObjectTransparency(uint32_t object_id) const;
	void SetObjectTransparency(uint32_t object_id, float transparency);

	const Vec3f &GetObjectColor(uint32_t object_id) const;
	void SetObjectColor(uint32_t object_id, const Vec3f &color);
	void SetObjectColor(uint32_t object_id, float r, float g, float b);

	const Vec3f &GetObjectPosition(uint32_t object_id) const;
	void SetObjectPosition(uint32_t object_id, const Vec3f &position);
	void SetObjectPosition(uint32_t object_id, float x, float y, float z);

	const Vec3f &GetObjectOrientation(uint32_t object_id) const;
	void SetObjectOrientation(uint32_t object_id, const Vec3f &orientation, float roll=0.0F);
	void SetObjectOrientation(uint32_t object_id, float x, float y, float z, float roll=0.0F);

	float GetObjectProperty(uint32_t object_id, uint32_t property) const;
	void SetObjectProperty(uint32_t object_id, uint32_t property, float value);
};

#endif // SCENE_HPP

#ifndef SCENE_HPP
#define SCENE_HPP

#include "basicObjects.hpp"
#include <chrono>

#define DEFAULT_SCREEN_WIDTH	1024
#define DEFAULT_SCREEN_HEIGHT	1024
#define RENDER_THREADS_NUM		16

using namespace std;

class Scene
{
public:
	Scene();
	~Scene();
	vector <Object *> *SceneObjects, *SceneLights;
	vector <Ray *> *SceneRays;
	vector <unsigned char> *ImageData;
	chrono::milliseconds FrameRenderTime;
	void AddObject(Object *object);
	void Render();
	unsigned int ScreenWidth();
	unsigned int ScreenHeight();
	void SetScreenWidth(unsigned int width);
	void SetScreenHeight(unsigned int height);
	void SetScreenSize(unsigned int width, unsigned int height);
	void SavePPMImage(const char *filename);
private:
	unsigned int pScreenWidth, pScreenHeight;
};

#endif // SCENE_HPP

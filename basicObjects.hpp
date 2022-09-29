#ifndef BASICOBJECTS_HPP
#define BASICOBJECTS_HPP

#include <vector>
#include <string>
#include "commonVectorFun.hpp"

#define RAY_STEPS_MAX					512

#define DEFAULT_LIGHTSOURCE_BRIGHTNESS	0xFFFF
#define DEFAULT_OBJECT_COLOR			0xCC,0xCC,0xCC

using namespace std;

class Object
{
public:
	Object();
	bool Visible();
	void SetVisible(bool visible);

	Vec3f Color();
	void SetColor(Color_t new_color);
	void SetColor(Vec3f new_color);
	void SetColor(float r, float g, float b);

	void SetPosition(Vec3d position);
	void SetPosition(double x, double y, double z);

	string Name();
	void SetName(string name);

	void AttachExternalColorBuffer(Vec3uc *buffer);
	virtual double GetDistance(Vec3d from);
	virtual bool ItsALightSource();

	vector <Object *> *SceneObjects, *SceneLights;
	Vec3d *Position;
protected:
private:
	bool pVisible;
	string *pName;
	Color_t *pColor;
};

// ========= SPHERE ===

class Sphere : public Object
{
public:
	Sphere();
	void SetRadius(double radius);
	double GetDistance(Vec3d from);
private:
	double pRadius;
};

// ========= LIGHT SOURCE ===

class LightSource : public Sphere
{
public:
	LightSource();
	bool ItsALightSource();
	unsigned int Brightness;
private:
};

// ========= RAY ===

class Ray : public Object
{
public:
	Ray();
	Vec3d Direction();
	void SetDirection(Vec3d direction);
	void SetDirection(double x, double y, double z);
	void Reload();
	void Reset();
	void Run();
	Object *RunTo(Vec3d *direction);
	LightSource *GetNearestLightSource();
private:
	Object *pObjectToIgnore, *pNrstObj;
	Vec3d *pDefaultDirection;
	unsigned int pStepsDone;
	unsigned char pIsFinished;
};

// ========= CUBE ===

class Cube : public Object
{
public:
	Cube();
	void SetLength(double length);
	double GetDistance(Vec3d from);
private:
	double pLength;
};

// ========= TORUS ===

class Torus : public Object
{
public:
	Torus();
	void SetRadius1(double radius);
	void SetRadius2(double radius);
	double GetDistance(Vec3d from);
private:
	double pRadius1, pRadius2;
};

#endif // BASICOBJECTS_HPP

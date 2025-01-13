#ifndef BASICOBJECTS_HPP
#define BASICOBJECTS_HPP

#include <vector>
#include <string>
#include "commonVectorFun.hpp"

#define RAY_STEPS_MAX			1024
#define RAY_REFLECTIONS_MAX		4

#define DEFAULT_OBJECT_COLOR	0xFF,0xFF,0xFF
#define DEFAULT_OBJECT_NAME		"Default object"

using namespace std;

class Object
{
public:
	Object();
	bool Visible() const;
	void SetVisible(bool visible);

	uint Brightness() const;
	void SetBrightness(uint brightness);

	double Reflectivity();
	void SetReflectivity(double reflectivity);

	Vec3f Color() const;
	void SetColor(Vec3f color);
	void SetColor(float r, float g, float b);

	void SetPosition(Vec3d position);
	void SetPosition(double x, double y, double z);

	string Name();
	void SetName(string name);

	void AttachExternalColorBuffer(Vec3uc *buffer);
	virtual double GetDistance(Vec3d from);
	bool ItsALightSource();

	vector <Object *> *SceneObjects, *SceneLights;
	Vec3d *Position;
protected:
private:
	bool pVisible;
	uint pBrightness;
	double pReflectivity;
	string *pName;
	Vec3uc *pColor;
};

// ========= RAY ===

class Ray : public Object
{
public:
	Ray();
	Vec3d Direction() const;
	void SetDirection(Vec3d direction);
	void SetDirection(double x, double y, double z);
	void Reset();
	void Run();
	Object *RunTo(Vec3d direction);
private:
	Object *pObjectToSkipOnce;
	Vec3d *pDirection;
	unsigned int pStepsDone, pReflectionsHappened;
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

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <zmq.hpp>
#include "scene.hpp"

Scene *gScene;

int main(int argc, char *argv[])
{
	float ObjectZpos=320;
	gScene=new Scene;

	// ======== CSG: classic example
	// uint32_t Sphere1ObjectID=gScene->AddObject(Object::SPHERE);
	// gScene->SetObjectProperty(Sphere1ObjectID, Object::ObjectProperty::DIAMETER, 88.0);
	// gScene->SetObjectPosition(Sphere1ObjectID, 0, 65, ObjectZpos);

	// uint32_t Cube1ObjectID=gScene->AddObject(Object::CUBE);
	// gScene->SetObjectProperty(Cube1ObjectID, Object::ObjectProperty::LENGTH, 70.0);
	// gScene->SetObjectPosition(Cube1ObjectID, 0, 65, ObjectZpos);
	// gScene->SetObjectOrientation(Cube1ObjectID, 1, 0, 1, 0);

	// uint32_t IntersectionObjectID=gScene->AddObject(Object::INTERSECTION, Sphere1ObjectID, Cube1ObjectID);
	// gScene->SetObjectColor(IntersectionObjectID, 30, 130, 130);

	// uint32_t InfiniteCylinder1ObjectID=gScene->AddObject(Object::INFINITE_CYLINDER);
	// gScene->SetObjectProperty(InfiniteCylinder1ObjectID, Object::ObjectProperty::DIAMETER, 44.0);
	// gScene->SetObjectPosition(InfiniteCylinder1ObjectID, 0, 65, ObjectZpos);
	// gScene->SetObjectOrientation(InfiniteCylinder1ObjectID, 1, 0, 1, 0);

	// uint32_t InfiniteCylinder2ObjectID=gScene->AddObject(Object::INFINITE_CYLINDER);
	// gScene->SetObjectProperty(InfiniteCylinder2ObjectID, Object::ObjectProperty::DIAMETER, 44.0);
	// gScene->SetObjectPosition(InfiniteCylinder2ObjectID, 0, 65, ObjectZpos);
	// gScene->SetObjectOrientation(InfiniteCylinder2ObjectID, 1, 0, -1, 0);

	// uint32_t InfiniteCylinder3ObjectID=gScene->AddObject(Object::INFINITE_CYLINDER);
	// gScene->SetObjectProperty(InfiniteCylinder3ObjectID, Object::ObjectProperty::DIAMETER, 44.0);
	// gScene->SetObjectPosition(InfiniteCylinder3ObjectID, 0, 65, ObjectZpos);
	// gScene->SetObjectOrientation(InfiniteCylinder3ObjectID, 0, 1, 0, 0);

	// uint32_t Difference1ObjectID=gScene->AddObject(Object::DIFFERENCE, IntersectionObjectID, InfiniteCylinder1ObjectID);
	// uint32_t Difference2ObjectID=gScene->AddObject(Object::DIFFERENCE, Difference1ObjectID, InfiniteCylinder2ObjectID);

	// uint32_t ConstructObjectID=gScene->AddObject(Object::DIFFERENCE, Difference2ObjectID, InfiniteCylinder3ObjectID);
	// gScene->SetObjectColor(ConstructObjectID, 30, 130, 130);
	// gScene->SetObjectSpecularity(ConstructObjectID, 0.1);

	// ======== CSG: gyroid in sphere
	uint32_t Sphere2ObjectID=gScene->AddObject(Object::SPHERE);
	gScene->SetObjectProperty(Sphere2ObjectID, Object::ObjectProperty::DIAMETER, 100.0);
	gScene->SetObjectPosition(Sphere2ObjectID, 0, 50, ObjectZpos);

	uint32_t GyroidObjectID=gScene->AddObject(Object::GYROID);
	gScene->SetObjectProperty(GyroidObjectID, Object::ObjectProperty::SCALE, 10.0);
	gScene->SetObjectPosition(GyroidObjectID, 0, 50, ObjectZpos);

	uint32_t GyroidInSphereObjectID=gScene->AddObject(Object::INTERSECTION, Sphere2ObjectID, GyroidObjectID);
	gScene->SetObjectColor(GyroidInSphereObjectID, 30, 130, 130);
	gScene->SetObjectSpecularity(GyroidInSphereObjectID, 0.1);

	// ======== CSG: Schwarz primitive in sphere
	Sphere *NewSphere4=new Sphere();
	NewSphere4->SetDiameter(100);
	NewSphere4->SetPosition(0, 50, ObjectZpos);

	SchwarzPrimitive *NewSchwarzPrimitive=new SchwarzPrimitive();
	NewSchwarzPrimitive->SetScale(6.5);
	NewSchwarzPrimitive->SetPosition(0, 50, ObjectZpos);
	NewSchwarzPrimitive->SetOrientation(1, 0, 1);

	Intersection *SphereSchwarzIntersection=new Intersection(NewSphere4, NewSchwarzPrimitive);
	SphereSchwarzIntersection->SetColor(30, 130, 130);
	SphereSchwarzIntersection->SetSpecularity(0.25);

	// ======== primitives
	Cylinder *Cylinder2=new Cylinder();
	Cylinder2->SetLength(56);
	Cylinder2->SetDiameter(56);
	Cylinder2->SetPosition(80, 50, ObjectZpos);
	Cylinder2->SetSpecularity(0.25);
	Cylinder2->SetColor(240, 18, 240);

	Cube *Cube2=new Cube();
	Cube2->SetLength(56);
	Cube2->SetPosition(80, 50, ObjectZpos);
	Cube2->SetSpecularity(0.25);
	Cube2->SetColor(150, 10, 150);

	Torus *Torus1=new Torus();
	Torus1->SetDiameter1(20);
	Torus1->SetDiameter2(68);
	Torus1->SetPosition(0, 50, ObjectZpos);
	Torus1->SetSpecularity(0.25);
	Torus1->SetColor(200, 200, 80);

	Sphere *BlueSphere=new Sphere();
	BlueSphere->SetDiameter(64);
	BlueSphere->SetPosition(-80, 50, ObjectZpos);
	BlueSphere->SetSpecularity(1.0);
	BlueSphere->SetColor(52, 52, 255);
	// BlueSphere->SetTransparency(0.5);

	Sphere *GreenSphere=new Sphere();
	GreenSphere->SetDiameter(64);
	GreenSphere->SetPosition(0, 50, ObjectZpos);
	GreenSphere->SetSpecularity(1.0);
	GreenSphere->SetColor(52, 255, 52);
	// GreenSphere->SetTransparency(0.5);

	Sphere *RedSphere=new Sphere();
	RedSphere->SetDiameter(64);
	RedSphere->SetPosition(80, 50, ObjectZpos);
	RedSphere->SetSpecularity(1.0);
	RedSphere->SetColor(255, 52, 52);
	// RedSphere->SetTransparency(0.5);

	InfiniteCylinder *NewInfiniteCylinder=new InfiniteCylinder();
	NewInfiniteCylinder->SetDiameter(40);
	NewInfiniteCylinder->SetPosition(0, 50, ObjectZpos);
	NewInfiniteCylinder->SetOrientation({1, 0, 0}, 0);
	NewInfiniteCylinder->SetColor(30, 130, 130);

	// ======== box
	Cube *Cube3=new Cube();
	Cube3->SetLength(80);
	Cube3->SetPosition(0, -139, ObjectZpos);

	Plane *Plane1=new Plane();
	Plane1->SetPosition(0, -100, 0);
	Plane1->SetOrientation(0, 1, 0);

	Difference *Ceiling=new Difference(Plane1, Cube3);
	Ceiling->SetColor(240, 240, 240);

	Plane *Floor=new Plane();
	Floor->SetPosition(0, 100, 0);
	Floor->SetOrientation(0, -1, 0);
	Floor->SetColor(240, 240, 240);

	Plane *Plane3=new Plane();
	Plane3->SetPosition(0, 0, 400);
	Plane3->SetOrientation(0, 0, -1);
	Plane3->SetColor(240, 240, 240);

	Plane *RedWall=new Plane();
	RedWall->SetPosition(-135, 0, 0);
	RedWall->SetOrientation(1, 0, 0);
	RedWall->SetColor(255, 120, 120);

	Plane *BlueWall=new Plane();
	BlueWall->SetPosition(135, 0, 0);
	BlueWall->SetOrientation(-1, 0, 0);
	BlueWall->SetColor(120, 120, 255);

	// ======== lights
	Cube *LightSource1=new Cube();
	LightSource1->SetLength(80);
	LightSource1->SetPosition(0, -150, ObjectZpos);
	LightSource1->SetColor(255, 255, 255);
	LightSource1->SetBrightness(11.5);

	Sphere *LightSource2=new Sphere();
	LightSource2->SetDiameter(20);
	LightSource2->SetPosition(80, -100, ObjectZpos);
	LightSource2->SetColor(255, 255, 255);
	LightSource2->SetBrightness(9.0);

	// ========
	gScene->AddObject(Ceiling);
	gScene->AddObject(Floor);
	gScene->AddObject(Plane3);
	gScene->AddObject(RedWall);
	gScene->AddObject(BlueWall);

	gScene->AddObject(LightSource1);
	// gScene->AddObject(LightSource2);

	// gScene->AddObject(SphereSchwarzIntersection);

	// gScene->AddObject(BlueSphere);
	// gScene->AddObject(GreenSphere);
	// gScene->AddObject(RedSphere);
	// gScene->AddObject(NewInfiniteCylinder);
	// gScene->AddObject(Cube2);
	// gScene->AddObject(Torus1);

	char fileName[128];
	int32_t i;
	Ray::REFLECTIONS_LIMIT=6;
	Scene::SAMPLES_PER_PIXEL=128;
	// for(i=0; i<8; i++, samples_per_pixel*=2)
	for(i=0; i<10; i++)
	{
		// float spec=i*0.1;

		// Cylinder1->SetOrientation(0, std::sin(i*M_PI_2/10.0), std::cos(i*M_PI_2/10.0));
		// Cube2->SetOrientation(std::sin(i*M_PI_2/10.0), std::cos(i*M_PI_2/10.0), 0);
		// Torus1->SetOrientation(0, std::sin(i*M_PI_2/10.0), std::cos(i*M_PI_2/10.0));
		// NewSchwarzPrimitive->SetOrientation(std::sin(i*M_PI_2/50.0), 0, std::cos(i*M_PI_2/50.0));

		// RedSphere->SetSpecularity(spec);
		// GreenSphere->SetSpecularity(spec);
		// BlueSphere->SetSpecularity(spec);
		// Construct->SetSpecularity(spec);

		gScene->Render();

		sprintf(fileName, "render_%02i_%luspp.png", i, Scene::SAMPLES_PER_PIXEL);
		gScene->RenderedImage.write(fileName);

		Scene::SAMPLES_PER_PIXEL*=2;
	}

	return 0;
}

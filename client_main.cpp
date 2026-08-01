#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cmath>
#include <zmq.hpp>
#include "scene.hpp"

Scene *gScene;

int main(int argc, char *argv[])
{
	float ObjectZpos=320;
	gScene=new Scene;

	// ======== CSG: classic example
	// uint32_t Sphere1ObjectID=gScene->AddObject(Object::ObjectType::SPHERE);
	// gScene->SetObjectProperty(Sphere1ObjectID, Object::ObjectProperty::DIAMETER, 88.0);
	// gScene->SetObjectPosition(Sphere1ObjectID, 0, 65, ObjectZpos);

	// uint32_t Cube1ObjectID=gScene->AddObject(Object::ObjectType::CUBE);
	// gScene->SetObjectProperty(Cube1ObjectID, Object::ObjectProperty::LENGTH, 70.0);
	// gScene->SetObjectPosition(Cube1ObjectID, 0, 65, ObjectZpos);
	// gScene->SetObjectOrientation(Cube1ObjectID, 1, 0, 1, 0);

	// uint32_t IntersectionObjectID=gScene->AddObject(Object::ObjectType::INTERSECTION, Sphere1ObjectID, Cube1ObjectID);
	// gScene->SetObjectColor(IntersectionObjectID, 30, 130, 130);

	// uint32_t InfiniteCylinder1ObjectID=gScene->AddObject(Object::ObjectType::INFINITE_CYLINDER);
	// gScene->SetObjectProperty(InfiniteCylinder1ObjectID, Object::ObjectProperty::DIAMETER, 44.0);
	// gScene->SetObjectPosition(InfiniteCylinder1ObjectID, 0, 65, ObjectZpos);
	// gScene->SetObjectOrientation(InfiniteCylinder1ObjectID, 1, 0, 1, 0);

	// uint32_t InfiniteCylinder2ObjectID=gScene->AddObject(Object::ObjectType::INFINITE_CYLINDER);
	// gScene->SetObjectProperty(InfiniteCylinder2ObjectID, Object::ObjectProperty::DIAMETER, 44.0);
	// gScene->SetObjectPosition(InfiniteCylinder2ObjectID, 0, 65, ObjectZpos);
	// gScene->SetObjectOrientation(InfiniteCylinder2ObjectID, 1, 0, -1, 0);

	// uint32_t InfiniteCylinder3ObjectID=gScene->AddObject(Object::ObjectType::INFINITE_CYLINDER);
	// gScene->SetObjectProperty(InfiniteCylinder3ObjectID, Object::ObjectProperty::DIAMETER, 44.0);
	// gScene->SetObjectPosition(InfiniteCylinder3ObjectID, 0, 65, ObjectZpos);
	// gScene->SetObjectOrientation(InfiniteCylinder3ObjectID, 0, 1, 0, 0);

	// uint32_t Difference1ObjectID=gScene->AddObject(Object::ObjectType::DIFFERENCE, IntersectionObjectID, InfiniteCylinder1ObjectID);
	// uint32_t Difference2ObjectID=gScene->AddObject(Object::ObjectType::DIFFERENCE, Difference1ObjectID, InfiniteCylinder2ObjectID);

	// uint32_t ConstructObjectID=gScene->AddObject(Object::ObjectType::DIFFERENCE, Difference2ObjectID, InfiniteCylinder3ObjectID);
	// gScene->SetObjectColor(ConstructObjectID, 30, 130, 130);
	// gScene->SetObjectSpecularity(ConstructObjectID, 0.1);

	// ======== CSG: gyroid in sphere
	uint32_t Sphere2ObjectID=gScene->AddObject(Object::ObjectType::SPHERE);
	gScene->SetObjectProperty(Sphere2ObjectID, Object::ObjectProperty::DIAMETER, 100.0);
	gScene->SetObjectPosition(Sphere2ObjectID, 0, 50, ObjectZpos);

	uint32_t GyroidObjectID=gScene->AddObject(Object::ObjectType::GYROID);
	gScene->SetObjectProperty(GyroidObjectID, Object::ObjectProperty::SCALE, 12.0);
	gScene->SetObjectPosition(GyroidObjectID, 0, 50, ObjectZpos);
	gScene->SetObjectOrientation(GyroidObjectID, 1, 0, 0, 0);

	uint32_t GyroidInSphereObjectID=gScene->AddObject(Object::ObjectType::INTERSECTION, Sphere2ObjectID, GyroidObjectID);
	gScene->SetObjectColor(GyroidInSphereObjectID, 30, 130, 130);

	// ======== CSG: Schwarz primitive in sphere
	// uint32_t Sphere3ObjectID=gScene->AddObject(Object::ObjectType::SPHERE);
	// gScene->SetObjectProperty(Sphere3ObjectID, Object::ObjectProperty::DIAMETER, 100.0);
	// gScene->SetObjectPosition(Sphere3ObjectID, 0, 50, ObjectZpos);

	// uint32_t SchwarzPrimitive1ObjectID=gScene->AddObject(Object::ObjectType::SCHWARZ_PRIMITIVE);
	// gScene->SetObjectProperty(SchwarzPrimitive1ObjectID, Object::ObjectProperty::SCALE, 13.0);
	// gScene->SetObjectPosition(SchwarzPrimitive1ObjectID, 0, 50, ObjectZpos);

	// uint32_t SchwarzPrimitiveInSphereObjectID=gScene->AddObject(Object::ObjectType::INTERSECTION, Sphere3ObjectID, SchwarzPrimitive1ObjectID);
	// gScene->SetObjectColor(SchwarzPrimitiveInSphereObjectID, 30, 130, 130);

	// ======== primitives
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

	// uint32_t Cylinder1ObjectID=gScene->AddObject(Object::ObjectType::CYLINDER);
	// gScene->SetObjectProperty(Cylinder1ObjectID, Object::ObjectProperty::DIAMETER, 60);
	// gScene->SetObjectProperty(Cylinder1ObjectID, Object::ObjectProperty::LENGTH, 60);
	// gScene->SetObjectPosition(Cylinder1ObjectID, 0, 50, ObjectZpos);
	// gScene->SetObjectOrientation(Cylinder1ObjectID, 1, 0, 1, 0);
	// gScene->SetObjectColor(Cylinder1ObjectID, 120, 20, 120);
	// gScene->SetObjectSpecularity(Cylinder1ObjectID, 1.0);

	// uint32_t InfiniteCylinder1ObjectID=gScene->AddObject(Object::ObjectType::INFINITE_CYLINDER);
	// gScene->SetObjectProperty(InfiniteCylinder1ObjectID, Object::ObjectProperty::DIAMETER, 40);
	// gScene->SetObjectPosition(InfiniteCylinder1ObjectID, 0, 50, ObjectZpos);
	// gScene->SetObjectOrientation(InfiniteCylinder1ObjectID, 1, 0, 0, 0);
	// gScene->SetObjectColor(InfiniteCylinder1ObjectID, 130, 30, 130);

	// uint32_t EllipticCylinder1ObjectID=gScene->AddObject(Object::ObjectType::ELLIPTIC_CYLINDER);
	// gScene->SetObjectProperty(EllipticCylinder1ObjectID, Object::ObjectProperty::DIAMETER_1, 30);
	// gScene->SetObjectProperty(EllipticCylinder1ObjectID, Object::ObjectProperty::DIAMETER_2, 60);
	// gScene->SetObjectProperty(EllipticCylinder1ObjectID, Object::ObjectProperty::LENGTH, 60);
	// gScene->SetObjectPosition(EllipticCylinder1ObjectID, 0, 50, ObjectZpos);
	// gScene->SetObjectOrientation(EllipticCylinder1ObjectID, 1, 0, 1, 0);
	// gScene->SetObjectColor(EllipticCylinder1ObjectID, 130, 30, 130);

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

	// gScene->AddObject(BlueSphere);
	// gScene->AddObject(GreenSphere);
	// gScene->AddObject(RedSphere);
	// gScene->AddObject(Cube2);
	// gScene->AddObject(Torus1);

	char fileName[128];
	int32_t i;
	Ray::REFLECTIONS_LIMIT=7;
	Scene::SAMPLES_PER_PIXEL=1024;
	for(i=0; i<50; i++)
	{
		// float spec=i*0.1;

		// RedSphere->SetSpecularity(spec);
		// GreenSphere->SetSpecularity(spec);
		// BlueSphere->SetSpecularity(spec);
		// Construct->SetSpecularity(spec);

		// Cube2->SetOrientation(std::sin(i*M_PI_2/10.0), std::cos(i*M_PI_2/10.0), 0);
		// Torus1->SetOrientation(0, std::sin(i*M_PI_2/10.0), std::cos(i*M_PI_2/10.0));

		// gScene->SetObjectOrientation(Cylinder1ObjectID, std::sin(i*M_PI_2/50.0), 0, std::cos(i*M_PI_2/50.0), 0);
		// gScene->SetObjectOrientation(GyroidObjectID, std::sin(i*M_PI_2/50.0), 0, std::cos(i*M_PI_2/50.0), 0);
		gScene->SetObjectPosition(GyroidObjectID, i/2.0, 50, ObjectZpos);

		gScene->Render();

		sprintf(fileName, "render_%02i_%luspp.png", i, Scene::SAMPLES_PER_PIXEL);
		gScene->RenderedImage.write(fileName);

		// Scene::SAMPLES_PER_PIXEL*=2;
	}

	return 0;
}

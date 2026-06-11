#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "scene.hpp"

Scene *NewScene;

int main(int argc, char *argv[])
{
	float ObjectZpos=320;
	NewScene=new Scene;

	// ======== CSG
	Sphere *NewSphere1=new Sphere();
	NewSphere1->SetRadius(44);
	NewSphere1->SetPosition(0, 65, ObjectZpos);

	Cube *NewCube1=new Cube();
	NewCube1->SetLength(70);
	NewCube1->SetPosition(0, 65, ObjectZpos);
	NewCube1->SetOrientation(1, 0, 1);

	Intersection *ShpereCubeIntersection=new Intersection(NewSphere1, NewCube1);

	Cylinder *NewCylinder1=new Cylinder();
	NewCylinder1->SetLength(71);
	NewCylinder1->SetRadius(22);
	NewCylinder1->SetPosition(0, 65, ObjectZpos);
	NewCylinder1->SetOrientation(1, 0, 1);

	Cylinder *NewCylinder2=new Cylinder();
	NewCylinder2->SetLength(71);
	NewCylinder2->SetRadius(22);
	NewCylinder2->SetPosition(0, 65, ObjectZpos);
	NewCylinder2->SetOrientation(1, 0, -1);

	Cylinder *NewCylinder3=new Cylinder();
	NewCylinder3->SetLength(71);
	NewCylinder3->SetRadius(22);
	NewCylinder3->SetPosition(0, 65, ObjectZpos);
	NewCylinder3->SetOrientation(0, 1, 0);

	Union *Cylinders1=new Union(NewCylinder1, NewCylinder2);
	Union *Cylinders2=new Union(Cylinders1, NewCylinder3);

	Difference *Construct=new Difference(ShpereCubeIntersection, Cylinders2);
	Construct->SetColor(30, 130, 130);

	// ======== CSG: gyroid in sphere
	Sphere *NewSphere3=new Sphere();
	NewSphere3->SetRadius(50);
	NewSphere3->SetPosition(0, 50, ObjectZpos);

	Gyroid *NewGyroid=new Gyroid();
	NewGyroid->SetScale(5.0);
	NewGyroid->SetPosition(0, 50, ObjectZpos);

	Intersection *SphereGyroidIntersection=new Intersection(NewSphere3, NewGyroid);
	SphereGyroidIntersection->SetColor(30, 130, 130);

	// ======== CSG: Schwarz primitive in sphere
	Sphere *NewSphere4=new Sphere();
	NewSphere4->SetRadius(50);
	NewSphere4->SetPosition(0, 50, ObjectZpos);

	SchwarzPrimitive *NewSchwarzPrimitive=new SchwarzPrimitive();
	NewSchwarzPrimitive->SetScale(5.0);
	NewSchwarzPrimitive->SetPosition(0, 50, ObjectZpos);
	NewSchwarzPrimitive->SetOrientation(1, 0, 1);

	Intersection *SphereSchwarzIntersection=new Intersection(NewSphere4, NewSchwarzPrimitive);
	SphereSchwarzIntersection->SetColor(30, 130, 130);

	// ======== primitives
	Cylinder *Cylinder2=new Cylinder();
	Cylinder2->SetLength(56);
	Cylinder2->SetRadius(28);
	Cylinder2->SetPosition(80, 50, ObjectZpos);
	Cylinder2->SetSpecularity(0.5);
	Cylinder2->SetColor(240, 18, 240);

	Cube *Cube2=new Cube();
	Cube2->SetLength(56);
	Cube2->SetPosition(80, 50, ObjectZpos);
	Cube2->SetSpecularity(0.5);
	Cube2->SetColor(150, 10, 150);

	Torus *Torus1=new Torus();
	Torus1->SetRadius1(34);
	Torus1->SetRadius2(10);
	Torus1->SetPosition(0, 50, ObjectZpos);
	Torus1->SetSpecularity(0.5);
	Torus1->SetColor(200, 200, 80);

	Sphere *BlueSphere=new Sphere();
	BlueSphere->SetRadius(32);
	BlueSphere->SetPosition(-80, 50, ObjectZpos);
	BlueSphere->SetSpecularity(1.0);
	BlueSphere->SetColor(52, 52, 255);
	// BlueSphere->SetTransparency(0.5);

	Sphere *GreenSphere=new Sphere();
	GreenSphere->SetRadius(32);
	GreenSphere->SetPosition(0, 50, ObjectZpos);
	GreenSphere->SetSpecularity(1.0);
	GreenSphere->SetColor(52, 255, 52);
	// GreenSphere->SetTransparency(0.5);

	Sphere *RedSphere=new Sphere();
	RedSphere->SetRadius(32);
	RedSphere->SetPosition(80, 50, ObjectZpos);
	RedSphere->SetSpecularity(1.0);
	RedSphere->SetColor(255, 52, 52);
	// RedSphere->SetTransparency(0.5);

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
	LightSource2->SetRadius(10);
	LightSource2->SetPosition(80, -100, ObjectZpos);
	LightSource2->SetColor(255, 255, 255);
	LightSource2->SetBrightness(9.0);

	// ========
	NewScene->AddObject(Ceiling);
	NewScene->AddObject(Floor);
	NewScene->AddObject(Plane3);
	NewScene->AddObject(RedWall);
	NewScene->AddObject(BlueWall);

	NewScene->AddObject(LightSource1);
	// NewScene->AddObject(LightSource2);

	NewScene->AddObject(Construct);
	// NewScene->AddObject(SphereGyroidIntersection);
	// NewScene->AddObject(SphereSchwarzIntersection);

	// NewScene->AddObject(BlueSphere);
	// NewScene->AddObject(RedSphere);
	// NewScene->AddObject(GreenSphere);
	// NewScene->AddObject(Cylinder2);
	// NewScene->AddObject(Cube2);
	// NewScene->AddObject(Torus1);

	NewScene->RebuildSceneTree();

	uint32_t i, samples_per_pixel=16;
	for(i=0; i<10; i++, samples_per_pixel*=2)
	{
		// float spec=i*0.1;

		// Cylinder1->SetOrientation(0, std::sin(i*M_PI_2/10.0), std::cos(i*M_PI_2/10.0));
		// Cube2->SetOrientation(std::sin(i*M_PI_2/10.0), std::cos(i*M_PI_2/10.0), 0);
		// Torus1->SetOrientation(0, std::sin(i*M_PI_2/10.0), std::cos(i*M_PI_2/10.0));

		// RedSphere->SetSpecularity(spec);
		// GreenSphere->SetSpecularity(spec);
		// BlueSphere->SetSpecularity(spec);
		// Construct->SetSpecularity(spec);

		NewScene->SetSamplesPerPixel(samples_per_pixel);
		NewScene->Render();

		string fileName=string("render_")+to_string(samples_per_pixel)+string("_spp.png");
		NewScene->RenderedImage.write(fileName);
	}

	return 0;
}

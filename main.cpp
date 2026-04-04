#include <QApplication>
#include <QTimer>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "widget.hpp"
#include "controlwidget.hpp"
#include "scene.hpp"

Scene *NewScene;
Widget *SceneW;
ControlWidget *ControlsW;

#include <cmath>

int main(int argc, char *argv[])
{
	// QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
	// QApplication RayMarchingApp(argc, argv);

	double ObjectZpos=320;
	NewScene=new Scene;

	// ======== CSG
	Sphere *NewSphere1=new Sphere();
	NewSphere1->SetRadius(44);
	NewSphere1->SetPosition(0, 50, ObjectZpos);

	Cube *NewCube1=new Cube();
	NewCube1->SetLength(68);
	NewCube1->SetPosition(0, 50, ObjectZpos);

	Intersection *ShpereCubeIntersection=new Intersection(NewSphere1, NewCube1);

	Cylinder *NewCylinder1=new Cylinder();
	NewCylinder1->SetLength(70);
	NewCylinder1->SetRadius(20);
	NewCylinder1->SetPosition(0, 50, ObjectZpos);
	NewCylinder1->SetOrientation(0, 0, 1);

	Cylinder *NewCylinder2=new Cylinder();
	NewCylinder2->SetLength(70);
	NewCylinder2->SetRadius(20);
	NewCylinder2->SetPosition(0, 50, ObjectZpos);
	NewCylinder2->SetOrientation(1, 0, 0);

	Union *Cylinders=new Union(NewCylinder1, NewCylinder2);

	Difference *Construct=new Difference(ShpereCubeIntersection, Cylinders);
	Construct->SetColor(18, 200, 200);

	// ======== CSG: gyroid in sphere
	Sphere *NewSphere2=new Sphere();
	NewSphere2->SetRadius(50);
	NewSphere2->SetPosition(0, 40, ObjectZpos);

	Gyroid *NewGyroid=new Gyroid();
	NewGyroid->SetScale(5.0);
	NewGyroid->SetPosition(0, 40, ObjectZpos);

	Intersection *SphereGyroidIntersection=new Intersection(NewSphere2, NewGyroid);
	SphereGyroidIntersection->SetColor(80, 80, 255);

	// ======== CSG: Schwarz primitive in sphere
	Sphere *NewSphere3=new Sphere();
	NewSphere3->SetRadius(50);
	NewSphere3->SetPosition(0, 40, ObjectZpos);

	SchwarzPrimitive *NewSchwarzPrimitive=new SchwarzPrimitive();
	NewSchwarzPrimitive->SetScale(5.0);
	NewSchwarzPrimitive->SetPosition(0, 40, ObjectZpos);

	Intersection *SphereSPIntersection=new Intersection(NewSphere2, NewSchwarzPrimitive);
	SphereSPIntersection->SetColor(80, 80, 255);

	// ========
	Sphere *BlueSphere=new Sphere();
	BlueSphere->SetRadius(32);
	BlueSphere->SetPosition(-80, 50, ObjectZpos);
	BlueSphere->SetSpecularity(1.0);
	BlueSphere->SetColor(52, 52, 255);

	Sphere *RedSphere=new Sphere();
	RedSphere->SetRadius(32);
	RedSphere->SetPosition(80, 50, ObjectZpos);
	RedSphere->SetSpecularity(1.0);
	RedSphere->SetColor(255, 52, 52);

	Sphere *GreenSphere=new Sphere();
	GreenSphere->SetRadius(32);
	GreenSphere->SetPosition(0, 50, ObjectZpos);
	GreenSphere->SetSpecularity(1.0);
	GreenSphere->SetColor(52, 255, 52);

	Cylinder *Cylinder2=new Cylinder();
	Cylinder2->SetLength(56);
	Cylinder2->SetRadius(28);
	Cylinder2->SetPosition(80, 50, ObjectZpos);
	Cylinder2->SetSpecularity(1.0);
	Cylinder2->SetColor(240, 18, 240);

	Cube *Cube2=new Cube();
	Cube2->SetLength(56);
	Cube2->SetPosition(80, 50, ObjectZpos);
	//Cube2->SetSpecularity(0.85);
	//Cube2->SetBrightness(0.25);
	Cube2->SetColor(150, 10, 150);

	Torus *Torus1=new Torus();
	Torus1->SetRadius1(34);
	Torus1->SetRadius2(10);
	Torus1->SetPosition(0, 50, ObjectZpos);
	//Torus1->SetSpecularity(0.85);
	//Torus1->SetBrightness(0.25);
	Torus1->SetColor(200, 200, 80);

	// ======== box
	Cube *Cube3=new Cube();
	Cube3->SetLength(90);
	Cube3->SetPosition(0, -144, ObjectZpos);

	Plane *Plane1=new Plane();
	Plane1->SetPosition(0, -100, 0);
	Plane1->SetOrientation(0, 1, 0);

	Difference *Ceiling=new Difference(Plane1, Cube3);
	Ceiling->SetColor(180, 180, 180);

	Plane *Floor=new Plane();
	Floor->SetPosition(0, 100, 0);
	Floor->SetOrientation(0, -1, 0);
	Floor->SetColor(180, 180, 180);

	Plane *Plane3=new Plane();
	Plane3->SetPosition(0, 0, 400);
	Plane3->SetOrientation(0, 0, -1);
	Plane3->SetColor(180, 180, 180);

	Plane *RedWall=new Plane();
	RedWall->SetPosition(-140, 0, 0);
	RedWall->SetOrientation(1, 0, 0);
	RedWall->SetColor(255, 100, 100);

	Plane *GreenWall=new Plane();
	GreenWall->SetPosition(140, 0, 0);
	GreenWall->SetOrientation(-1, 0, 0);
	GreenWall->SetColor(100, 255, 100);

	// ======== lights
	Cube *LightSource1=new Cube();
	LightSource1->SetLength(90);
	LightSource1->SetPosition(0, -155, ObjectZpos);
	LightSource1->SetColor(255, 255, 255);
	LightSource1->SetBrightness(10.0);

	Sphere *LightSource2=new Sphere();
	LightSource2->SetRadius(10);
	LightSource2->SetPosition(80, -100, ObjectZpos);
	LightSource2->SetColor(255, 255, 255);
	LightSource2->SetBrightness(5.0);

	// NewScene->AddObject(Ceiling);
	NewScene->AddObject(Floor);
	NewScene->AddObject(Plane3);
	// NewScene->AddObject(RedWall);
	// NewScene->AddObject(GreenWall);

	NewScene->AddObject(LightSource1);
	// NewScene->AddObject(LightSource2);

	// NewScene->AddObject(Construct);
	// NewScene->AddObject(SphereGyroidIntersection);
	// NewScene->AddObject(SphereSPIntersection);

	NewScene->AddObject(BlueSphere);
	NewScene->AddObject(RedSphere);
	NewScene->AddObject(GreenSphere);
	// NewScene->AddObject(Cylinder2);
	// NewScene->AddObject(Cube2);
	// NewScene->AddObject(Torus1);

	NewScene->RebuildSceneTree();
	// return 42;

	uint32_t samples_per_pixel=1;
	for(int i=0; i<11; i++)
	{
		// double spec=i*0.1;

		// Cylinder1->SetOrientation(0, std::sin(i*M_PI_2/10.0), std::cos(i*M_PI_2/10.0));
		// Cube2->SetOrientation(std::sin(i*M_PI_2/10.0), std::cos(i*M_PI_2/10.0), 0);
		// Torus1->SetOrientation(0, std::sin(i*M_PI_2/10.0), std::cos(i*M_PI_2/10.0));
		// NewCube1->SetOrientation(std::sin(i*M_PI_2/10.0), 0, std::cos(i*M_PI_2/10.0));

		// Sphere2->SetSpecularity(spec);
		// Sphere3->SetSpecularity(spec);
		// Cylinder2->SetSpecularity(spec);
		// Construct->SetSpecularity(spec);
		// SphereGyroidIntersection->SetSpecularity(spec);
		// SphereSPIntersection->SetSpecularity(spec);

		samples_per_pixel*=2;

		NewScene->SetNumOfSamplesPerPixel(samples_per_pixel);
		NewScene->Render();

		QImage img1(NewScene->ImageData->data(), NewScene->ScreenWidth(), NewScene->ScreenHeight(), QImage::Format_RGBA8888);
		img1.save(QString("render_") + QString::number(i) + QString(".png"));
	}

//	SceneW=new Widget;
//	ControlsW=new ControlWidget;
//	ControlsW->setWindowModality(Qt::ApplicationModal);

//	SceneW->setImage(&img1);

//	QTimer::singleShot(0, SceneW, SLOT(showNormal()));
//	QTimer::singleShot(0, SceneW, SLOT(showFullScreen()));
//	QTimer::singleShot(0, SceneW, SLOT(showMaximized()));
//	QTimer::singleShot(0, ControlsW, SLOT(show()));

//	return RayMarchingApp.exec();
	return 0;
}

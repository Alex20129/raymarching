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

int main(int argc, char *argv[])
{
	QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
	QApplication RayMarchingApp(argc, argv);

	NewScene=new Scene;

	Sphere *NewSphere1=new Sphere();
	NewSphere1->SetName("Sphere 1");
	NewSphere1->SetRadius(44);
	NewSphere1->SetReflectivity(0.4);
	NewSphere1->SetPosition(0, -40, 300);
	NewSphere1->SetColor(200, 20, 20);

	Cube *NewCube1=new Cube();
	NewCube1->SetName("Cube 1");
	NewCube1->SetLength(68);
	NewCube1->SetReflectivity(0.6);
	NewCube1->SetPosition(0, -40, 300);
	NewCube1->SetColor(20, 180, 180);

	double test_brightness=0.0;

	Intersection *ShpereCubeIntersection=new Intersection(NewSphere1, NewCube1);
	ShpereCubeIntersection->SetReflectivity(0.25);
	ShpereCubeIntersection->SetBrightness(test_brightness);
	ShpereCubeIntersection->SetColor(0, 180, 180);

	Sphere *NewSphere2=new Sphere();
	NewSphere2->SetName("Sphere 2");
	NewSphere2->SetRadius(46);
	NewSphere2->SetReflectivity(0.25);
	NewSphere2->SetPosition(-50, 44, 300);
	NewSphere2->SetBrightness(test_brightness);
	NewSphere2->SetColor(20, 20, 200);

	Cube *NewCube2=new Cube();
	NewCube2->SetName("Cube 2");
	NewCube2->SetLength(80);
	NewCube2->SetReflectivity(0.25);
	NewCube2->SetPosition(50, 44, 300);
	NewCube2->SetBrightness(test_brightness);
	NewCube2->SetColor(150, 10, 150);

	// Torus *NewTorus2=new Torus();
	// NewTorus2->SetName("Torus 2");
	// NewTorus2->SetPosition(0, 20, 120);
	// NewTorus2->SetRadius1(40);
	// NewTorus2->SetRadius2(10);
	// NewTorus2->SetBrightness(test_brightness);
	// NewTorus2->SetColor(200, 200, 80);

	Plane *NewPlane1=new Plane();
	NewPlane1->SetPosition(0, 100, 0);
	NewPlane1->SetOrientation(0, -1, 0);
	NewPlane1->SetColor(100, 100, 100);

	Plane *NewPlane2=new Plane();
	NewPlane2->SetPosition(0, 0, 400);
	NewPlane2->SetOrientation(0, 0, -1);
	NewPlane2->SetColor(100, 100, 100);

	Plane *NewPlane3=new Plane();
	NewPlane3->SetPosition(-140, 0, 0);
	NewPlane3->SetOrientation(1, 0, 0);
	NewPlane3->SetColor(140, 80, 80);

	Plane *NewPlane4=new Plane();
	NewPlane4->SetPosition(140, 0, 0);
	NewPlane4->SetOrientation(-1, 0, 0);
	NewPlane4->SetColor(80, 140, 80);

	Sphere *NewLightSource1=new Sphere();
	NewLightSource1->SetName("Spherical light source");
	NewLightSource1->SetRadius(30);
	NewLightSource1->SetPosition(-30, -300, 10);
	NewLightSource1->SetBrightness(1.0);
	NewLightSource1->SetColor(255, 255, 255);

	Cube *NewLightSource2=new Cube();
	NewLightSource2->SetName("Cubical light source");
	NewLightSource2->SetLength(60);
	NewLightSource2->SetPosition(30, -300, 10);
	NewLightSource2->SetBrightness(1.0);
	NewLightSource2->SetColor(255, 255, 255);

	NewScene->AddObject(ShpereCubeIntersection);
	NewScene->AddObject(NewSphere2);
	NewScene->AddObject(NewCube2);
	// NewScene->AddObject(NewTorus2);
	NewScene->AddObject(NewLightSource1);
	NewScene->AddObject(NewLightSource2);
	NewScene->AddObject(NewPlane1);
	NewScene->AddObject(NewPlane2);
	NewScene->AddObject(NewPlane3);
	NewScene->AddObject(NewPlane4);

	uint32_t rays_per_pixel=4;
	while(rays_per_pixel)
	{
		NewScene->SetNumOfRayRunsPerPixel(rays_per_pixel);
		NewScene->Render();
		QImage img1(NewScene->ImageData->data(), NewScene->ScreenWidth(), NewScene->ScreenHeight(), QImage::Format_RGBA8888);
		img1.save(QString::number(rays_per_pixel)+QString(".png"));
		rays_per_pixel=rays_per_pixel>>1;
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

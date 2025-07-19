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

	double test_brightness=0.0;

	NewScene=new Scene;

	Sphere *NewSphere1=new Sphere();
	NewSphere1->SetName("Small sphere 1");
	NewSphere1->SetRadius(22);
	NewSphere1->SetReflectivity(0.4);
	NewSphere1->SetPosition(0, 10, 160);
	NewSphere1->SetBrightness(test_brightness);
	NewSphere1->SetColor(200, 20, 20);

	Cube *NewCube1=new Cube();
	NewCube1->SetName("Cube 1");
	NewCube1->SetLength(34);
	NewCube1->SetReflectivity(0.6);
	NewCube1->SetPosition(0, 10, 160);
	NewCube1->SetBrightness(test_brightness);
	NewCube1->SetColor(20, 180, 180);

	Intersection *ShpereCubeIntersection=new Intersection(NewSphere1, NewCube1);
	ShpereCubeIntersection->SetReflectivity(0.25);
	ShpereCubeIntersection->SetPosition(0, 10, 160);
	ShpereCubeIntersection->SetBrightness(test_brightness);
	ShpereCubeIntersection->SetColor(0, 180, 180);

	Sphere *NewSphere2=new Sphere();
	NewSphere2->SetName("Small sphere 2");
	NewSphere2->SetRadius(25);
	NewSphere2->SetReflectivity(0.25);
	NewSphere2->SetPosition(-55, 10, 180);
	NewSphere2->SetBrightness(test_brightness);
	NewSphere2->SetColor(20, 20, 200);

	Cube *NewCube2=new Cube();
	NewCube2->SetName("Cube 2");
	NewCube2->SetLength(40);
	NewCube2->SetReflectivity(0.25);
	NewCube2->SetPosition(55, 10, 180);
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
	NewPlane1->SetPosition(0, 35, 0);
	NewPlane1->SetOrientation(0, -1, 0);
	NewPlane1->SetColor(100, 100, 100);

	Plane *NewPlane2=new Plane();
	NewPlane2->SetPosition(0, 0, 360);
	NewPlane2->SetOrientation(0, 0, -1);
	NewPlane2->SetColor(100, 100, 100);

	Sphere *NewLightSource1=new Sphere();
	NewLightSource1->SetName("Spherical light source");
	NewLightSource1->SetRadius(40);
	NewLightSource1->SetPosition(-400, -400, 0);
	NewLightSource1->SetBrightness(1.0);
	NewLightSource1->SetColor(255, 255, 255);

	Cube *NewLightSource2=new Cube();
	NewLightSource2->SetName("Cubical light source");
	NewLightSource2->SetLength(80);
	NewLightSource2->SetPosition(400, -400, 180);
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

	NewScene->Render(16);
	QImage img1(NewScene->ImageData->data(), NewScene->ScreenWidth(), NewScene->ScreenHeight(), QImage::Format_RGBA8888);
	img1.save(QString("016.png"));

	SceneW=new Widget;
//	ControlsW=new ControlWidget;
//	ControlsW->setWindowModality(Qt::ApplicationModal);

	SceneW->setImage(&img1);

//	QTimer::singleShot(0, SceneW, SLOT(showNormal()));
//	QTimer::singleShot(0, SceneW, SLOT(showFullScreen()));
	QTimer::singleShot(0, SceneW, SLOT(showMaximized()));
//	QTimer::singleShot(0, ControlsW, SLOT(show()));

	return RayMarchingApp.exec();
}

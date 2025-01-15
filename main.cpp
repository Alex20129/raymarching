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
	NewSphere1->SetName("Small sphere 1");
	NewSphere1->SetRadius(22);
	NewSphere1->SetReflectivity(0.5);
	NewSphere1->SetPosition(0, 10, 160);
	NewSphere1->SetColor(200, 0, 0);

	Cube *NewCube1=new Cube();
	NewCube1->SetName("Cube 1");
	NewCube1->SetLength(30);
	NewCube1->SetReflectivity(0.5);
	NewCube1->SetPosition(0, 10, 160);
	NewCube1->SetColor(0, 180, 180);

	Intersection *ShpereCubeIntersection=new Intersection(NewSphere1, NewCube1);
	ShpereCubeIntersection->SetPosition(0, 10, 160);
	ShpereCubeIntersection->SetColor(0, 180, 180);

	Sphere *NewSphere2=new Sphere();
	NewSphere2->SetName("Small sphere 2");
	NewSphere2->SetRadius(25);
	NewSphere2->SetReflectivity(0.5);
	NewSphere2->SetPosition(-55, 10, 180);
	NewSphere2->SetColor(0, 0, 200);

	Cube *NewCube2=new Cube();
	NewCube2->SetName("Cube 2");
	NewCube2->SetLength(40);
	NewCube2->SetReflectivity(0.5);
	NewCube2->SetPosition(55, 10, 180);
	NewCube2->SetColor(180, 0, 180);

	// Torus *NewTorus1=new Torus();
	// NewTorus1->SetName("Torus 1");
	// NewTorus1->SetPosition(0, 20, 120);
	// NewTorus1->SetRadius1(40);
	// NewTorus1->SetRadius2(10);
	// NewTorus1->SetColor(180, 200, 80);

	Sphere *NewLightSource1=new Sphere();
	NewLightSource1->SetName("Spherical light source");
	NewLightSource1->SetRadius(20);
	NewLightSource1->SetPosition(-100, -120, 120);
	NewLightSource1->SetBrightness(100);
	NewLightSource1->SetColor(255, 255, 255);

	Cube *NewLightSource2=new Cube();
	NewLightSource2->SetName("Cubical light source");
	NewLightSource2->SetLength(40);
	NewLightSource2->SetPosition(100, -120, 200);
	NewLightSource2->SetBrightness(100);
	NewLightSource2->SetColor(255, 255, 255);

	Plane *NewPlane=new Plane();
	NewPlane->SetPosition(0, 35, 0);
	NewPlane->SetColor(50, 50, 50);

	NewScene->AddObject(ShpereCubeIntersection);
	// NewScene->AddObject(NewSphere1);
	NewScene->AddObject(NewSphere2);
	// NewScene->AddObject(NewCube1);
	NewScene->AddObject(NewCube2);
	// NewScene->AddObject(NewTorus1);
	NewScene->AddObject(NewLightSource1);
	NewScene->AddObject(NewLightSource2);
	NewScene->AddObject(NewPlane);

//	Object *testObj=new Object;
//	testObj->SetPosition(100, 0, 50);

//	fprintf(stdout, "%8.1lf\n", (testObj->Position()-newCube1->Position()).Length());
//	fprintf(stdout, "%8.1lf\n", newCube1->GetDistance(testObj->Position()));

	NewScene->Render();
	QImage img1(NewScene->ImageData->data(), NewScene->ScreenWidth(), NewScene->ScreenHeight(), QImage::Format_RGBA8888);
	img1.save(QString("011.png"));

	SceneW=new Widget;
	ControlsW=new ControlWidget;
//	ControlsW->setWindowModality(Qt::ApplicationModal);

	SceneW->setImage(&img1);

//	QTimer::singleShot(0, SceneW, SLOT(showNormal()));
//	QTimer::singleShot(0, SceneW, SLOT(showFullScreen()));
	QTimer::singleShot(0, SceneW, SLOT(showMaximized()));
//	QTimer::singleShot(0, ControlsW, SLOT(show()));

	return RayMarchingApp.exec();
}

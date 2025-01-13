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
	QApplication app(argc, argv);

	NewScene=new Scene;

	Sphere *NewSphere1=new Sphere();
	NewSphere1->SetName("Small sphere 1");
	NewSphere1->SetRadius(12);
	NewSphere1->SetReflectivity(0.0);
	NewSphere1->SetPosition(14, 0, 60);
	NewSphere1->SetColor(120, 20, 20);

	Sphere *NewSphere2=new Sphere();
	NewSphere2->SetName("Small sphere 2");
	NewSphere2->SetRadius(12);
	NewSphere1->SetReflectivity(0.5);
	NewSphere2->SetPosition(-14, 0, 60);
	NewSphere2->SetColor(20, 20, 120);

	Sphere *NewSphere3=new Sphere();
	NewSphere3->SetName("Big sphere");
	NewSphere3->SetRadius(700);
	NewSphere3->SetPosition(0, 712, 60);
	NewSphere3->SetColor(90, 90, 90);

	Sphere *NewLightSource1=new Sphere();
	NewLightSource1->SetName("Spherical light source");
	NewLightSource1->SetRadius(20);
	NewLightSource1->SetPosition(100, -100, 60);
	NewLightSource1->SetBrightness(100);
	NewLightSource1->SetColor(255, 255, 255);

	Cube *NewLightSource2=new Cube();
	NewLightSource2->SetName("Cubical light source");
	NewLightSource2->SetLength(13);
	NewLightSource2->SetPosition(-100, -100, 60);
	NewLightSource2->SetBrightness(100);
	NewLightSource2->SetColor(255, 255, 255);

	NewScene->AddObject(NewSphere1);
	NewScene->AddObject(NewSphere2);
	NewScene->AddObject(NewSphere3);
	NewScene->AddObject(NewLightSource1);
	NewScene->AddObject(NewLightSource2);

//	Cube *newCube1=new Cube();
//	newCube1->SetPosition(-30, 30, 120);
//	newCube1->SetLength(15);
//	newCube1->SetColor(100, 200, 100);
//	MainScene->AddObject(newCube1);

//	Torus *newTorus1=new Torus();
//	newTorus1->SetPosition(0, 30, 140);
//	newTorus1->SetRadius1(40);
//	newTorus1->SetRadius2(10);
//	newTorus1->SetColor(180, 200, 80);
//	MainScene->AddObject(newTorus1);

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
	QTimer::singleShot(0, ControlsW, SLOT(show()));

	return app.exec();
}

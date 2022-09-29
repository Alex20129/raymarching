#include <QApplication>
#include <QTimer>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "widget.h"
#include "controlwidget.h"
#include "scene.hpp"

Scene *MainScene;
Widget *SceneW;
ControlWidget *ControlsW;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	MainScene=new Scene;

	LightSource *newLightSource1=new LightSource();
	newLightSource1->SetVisible(0);
	newLightSource1->SetPosition(100, -100, 100);
	newLightSource1->SetColor(255, 255, 255);
	MainScene->AddObject(newLightSource1);

	LightSource *newLightSource2=new LightSource();
	newLightSource2->SetVisible(0);
	newLightSource2->SetPosition(-100, -100, 100);
	newLightSource2->SetColor(255, 255, 255);
	MainScene->AddObject(newLightSource2);

	Sphere *newSphere1=new Sphere();
	newSphere1->SetPosition(-300, 300, 300);
	newSphere1->SetRadius(380);
	newSphere1->SetColor(0, 200, 200);
	MainScene->AddObject(newSphere1);

	Sphere *newSphere2=new Sphere();
	newSphere2->SetPosition(25, 0, 120);
	newSphere2->SetRadius(20);
	newSphere2->SetColor(200, 0, 200);
	MainScene->AddObject(newSphere2);

	Cube *newCube1=new Cube();
	newCube1->SetPosition(-30, 30, 120);
	newCube1->SetLength(15);
	newCube1->SetColor(100, 200, 100);
	MainScene->AddObject(newCube1);

	Torus *newTorus1=new Torus();
	newTorus1->SetPosition(0, 30, 140);
	newTorus1->SetRadius1(40);
	newTorus1->SetRadius2(10);
	newTorus1->SetColor(180, 200, 80);
	MainScene->AddObject(newTorus1);

//	Object *testObj=new Object;
//	testObj->SetPosition(100, 0, 50);

//	fprintf(stdout, "%8.1lf\n", (testObj->Position()-newCube1->Position()).Length());
//	fprintf(stdout, "%8.1lf\n", newCube1->GetDistance(testObj->Position()));

	MainScene->Render();
	//fprintf(stdout, "RenderTime: %lims\n", FrameRenderTime.count());
	//MainScene->SavePPMImage("009.ppm");
	QImage img1(MainScene->ImageData->data(), MainScene->ScreenWidth(), MainScene->ScreenHeight(), QImage::Format_RGBA8888);
	img1.save(QString("010.png"));

	SceneW=new Widget;
	ControlsW=new ControlWidget;

	QTimer::singleShot(0, SceneW, SLOT(show()));
	QTimer::singleShot(0, ControlsW, SLOT(show()));
//	ControlsW->setWindowModality(Qt::ApplicationModal);

	return app.exec();
}

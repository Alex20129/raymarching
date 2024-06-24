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
	NewSphere1->SetName("Sphere 1");
	NewSphere1->SetRadius(12);
	NewSphere1->SetPosition(15, 0, 80);
	NewSphere1->SetColor(0, 200, 200);

	Sphere *NewSphere2=new Sphere();
	NewSphere2->SetName("Sphere 2");
	NewSphere2->SetRadius(12);
	NewSphere2->SetPosition(-15, 0, 80);
	NewSphere2->SetColor(200, 0, 200);

	Sphere *NewSphere3=new Sphere();
	NewSphere3->SetName("Big sphere");
	NewSphere3->SetRadius(500);
	NewSphere3->SetPosition(0, 512, 80);
	NewSphere3->SetColor(150, 150, 150);

	Sphere *NewLightSource1=new Sphere();
	NewLightSource1->SetName("Light source 1");
	NewLightSource1->SetRadius(10);
	NewLightSource1->SetPosition(100, -100, 50);
	NewLightSource1->SetBrightness(100);
	NewLightSource1->SetColor(255, 255, 255);

	Sphere *NewLightSource2=new Sphere();
	NewLightSource2->SetName("Light source 2");
	NewLightSource2->SetRadius(10);
	NewLightSource2->SetPosition(-100, -100, 50);
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
	//fprintf(stdout, "RenderTime: %lims\n", FrameRenderTime.count());
	//MainScene->SavePPMImage("009.ppm");
	QImage img1(NewScene->ImageData->data(), NewScene->ScreenWidth(), NewScene->ScreenHeight(), QImage::Format_RGBA8888);
	img1.save(QString("011.png"));

	SceneW=new Widget;
	ControlsW=new ControlWidget;

	SceneW->showFullScreen();

	QTimer::singleShot(0, SceneW, SLOT(show()));
	QTimer::singleShot(0, ControlsW, SLOT(show()));
//	ControlsW->setWindowModality(Qt::ApplicationModal);

	return app.exec();
}

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
	// QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
	// QApplication RayMarchingApp(argc, argv);

	double ObjectZpos=320;
	NewScene=new Scene;

	// ======== CSG
	Sphere *NewSphere1=new Sphere();
	NewSphere1->SetName("Sphere 1");
	NewSphere1->SetRadius(44);
	NewSphere1->SetPosition(0, 50, ObjectZpos);
	//NewSphere1->SetSpecularity(0.4);
	//NewSphere1->SetBrightness(1.0);

	Cube *NewCube1=new Cube();
	NewCube1->SetName("Cube 1");
	NewCube1->SetLength(68);
	NewCube1->SetPosition(0, 50, ObjectZpos);
	//NewCube1->SetSpecularity(0.4);
	//NewCube1->SetBrightness(1.0);

	Intersection *ShpereCubeIntersection=new Intersection(NewSphere1, NewCube1);

	Cylinder *NewCylinder1=new Cylinder();
	NewCylinder1->SetName("Cylinder 1");
	NewCylinder1->SetLength(70);
	NewCylinder1->SetRadius(20);
	NewCylinder1->SetPosition(0, 50, ObjectZpos);
	//NewCylinder1->SetSpecularity(0.5);
	//NewCylinder1->SetBrightness(0.25);

	Difference *Construct=new Difference(ShpereCubeIntersection, NewCylinder1);
	Construct->SetColor(20, 180, 180);

	// ========
	Sphere *BlueSphere=new Sphere();
	BlueSphere->SetName("Blue sphere");
	BlueSphere->SetRadius(32);
	BlueSphere->SetPosition(-80, 50, ObjectZpos);
	BlueSphere->SetSpecularity(1.0);
	//BlueSphere->SetBrightness(0.5);
	BlueSphere->SetColor(52, 52, 255);

	Sphere *BlackSphere=new Sphere();
	BlackSphere->SetName("Black sphere");
	BlackSphere->SetRadius(32);
	BlackSphere->SetPosition(-80, 50, ObjectZpos);
	BlackSphere->SetSpecularity(1.0);
	//BlackSphere->SetBrightness(0.5);
	BlackSphere->SetColor(20, 20, 20);

	Sphere *YellowSphere=new Sphere();
	YellowSphere->SetName("Yellow sphere");
	YellowSphere->SetRadius(32);
	YellowSphere->SetPosition(80, 50, ObjectZpos);
	YellowSphere->SetSpecularity(1.0);
	//YellowSphere->SetBrightness(0.5);
	YellowSphere->SetColor(255, 255, 14);

	Sphere *RedSphere=new Sphere();
	RedSphere->SetName("Red sphere");
	RedSphere->SetRadius(32);
	RedSphere->SetPosition(80, 50, ObjectZpos);
	RedSphere->SetSpecularity(1.0);
	//RedSphere->SetBrightness(0.5);
	RedSphere->SetColor(255, 48, 48);

	Cylinder *Cylinder2=new Cylinder();
	Cylinder2->SetName("Cylinder 2");
	Cylinder2->SetLength(56);
	Cylinder2->SetRadius(28);
	Cylinder2->SetPosition(80, 50, ObjectZpos);
	Cylinder2->SetSpecularity(1.0);
	//Cylinder2->SetBrightness(0.25);
	Cylinder2->SetColor(240, 18, 240);

	Cube *Cube2=new Cube();
	Cube2->SetName("Cube 2");
	Cube2->SetLength(56);
	Cube2->SetPosition(80, 50, ObjectZpos);
	//Cube2->SetSpecularity(0.85);
	//Cube2->SetBrightness(0.25);
	Cube2->SetColor(150, 10, 150);

	Torus *Torus1=new Torus();
	Torus1->SetName("Torus 2");
	Torus1->SetRadius1(34);
	Torus1->SetRadius2(10);
	Torus1->SetPosition(0, 50, ObjectZpos);
	//Torus1->SetSpecularity(0.85);
	//Torus1->SetBrightness(0.25);
	Torus1->SetColor(200, 200, 80);

	// ======== box
	Plane *Plane1=new Plane();
	Plane1->SetName("Ceiling");
	Plane1->SetPosition(0, -100, 0);
	Plane1->SetOrientation(0, 1, 0);

	Plane *Plane2=new Plane();
	Plane1->SetName("Back wall");
	Plane2->SetPosition(0, 0, 400);
	Plane2->SetOrientation(0, 0, -1);

	Plane *Plane3=new Plane();
	Plane1->SetName("Floor");
	Plane3->SetPosition(0, 100, 0);
	Plane3->SetOrientation(0, -1, 0);

	Plane *Plane4=new Plane();
	Plane4->SetPosition(-140, 0, 0);
	Plane4->SetOrientation(1, 0, 0);

	Plane *Plane5=new Plane();
	Plane5->SetPosition(140, 0, 0);
	Plane5->SetOrientation(-1, 0, 0);

	Union *BoxUnion1=new Union(Plane1, Plane2);
	Union *BoxUnion2=new Union(Plane3, Plane4);
	Union *BoxUnion3=new Union(BoxUnion1, BoxUnion2);
	Union *Box=new Union(BoxUnion3, Plane5);
	Box->SetColor(180, 180, 180);

	// ======== lights
	Cube *LightSource1=new Cube();
	LightSource1->SetLength(90);
	LightSource1->SetPosition(0, -144, ObjectZpos);

	Difference *LightPanel=new Difference(LightSource1, Plane1);
	LightPanel->SetName("Light panel");
	LightPanel->SetColor(255, 255, 255);
	LightPanel->SetBrightness(10.0);

	Sphere *LightSource2=new Sphere();
	LightSource2->SetName("Spherical light source");
	LightSource2->SetRadius(10);
	LightSource2->SetPosition(80, -50, ObjectZpos);
	LightSource2->SetBrightness(2.0);
	LightSource2->SetColor(255, 255, 255);

	NewScene->AddObject(Construct);
	NewScene->AddObject(BlueSphere);
	// NewScene->AddObject(BlackSphere);
	//NewScene->AddObject(YellowSphere);
	NewScene->AddObject(RedSphere);
	// NewScene->AddObject(Cylinder2);
	// NewScene->AddObject(Cube2);
	// NewScene->AddObject(Torus1);
	NewScene->AddObject(LightPanel);
	// NewScene->AddObject(LightSource2);
	NewScene->AddObject(Box);

	uint32_t samples_per_pixel;
	for(samples_per_pixel=2; samples_per_pixel<1025; samples_per_pixel*=2)
	{
		// double spec=i*0.1;
		// ShpereCubeIntersection->SetSpecularity(spec);
		// Sphere2->SetSpecularity(spec);
		// Sphere3->SetSpecularity(spec);
		// Cylinder2->SetSpecularity(spec);
		// Cylinder1->SetOrientation(0, std::sin(i*M_PI_2/10.0), std::cos(i*M_PI_2/10.0));
		// Cube2->SetOrientation(std::sin(i*M_PI_2/10.0), std::cos(i*M_PI_2/10.0), 0);
		// Torus1->SetOrientation(0, std::sin(i*M_PI_2/10.0), std::cos(i*M_PI_2/10.0));
		// NewCube1->SetOrientation(std::sin(i*M_PI_2/10.0), 0, std::cos(i*M_PI_2/10.0));

		NewScene->SetNumOfSamplesPerPixel(samples_per_pixel);
		NewScene->Render();
		QImage img1(NewScene->ImageData->data(), NewScene->ScreenWidth(), NewScene->ScreenHeight(), QImage::Format_RGBA8888);
		img1.save(QString("xorshift_star_")+
				QString::number(samples_per_pixel)+
				QString(".png"));
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

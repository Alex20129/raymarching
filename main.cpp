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

	NewScene=new Scene;

	Plane *Plane1=new Plane();
	Plane1->SetPosition(0, 100, 0);
	Plane1->SetOrientation(0, -1, 0);
	Plane1->SetSpecularity(0);
	Plane1->SetColor(150, 150, 150);

	Plane *Plane2=new Plane();
	Plane2->SetPosition(0, -100, 0);
	Plane2->SetOrientation(0, 1, 0);
	Plane2->SetSpecularity(0);
	Plane2->SetColor(150, 150, 150);

	Plane *Plane3=new Plane();
	Plane3->SetPosition(0, 0, 400);
	Plane3->SetOrientation(0, 0, -1);
	Plane3->SetSpecularity(0);
	Plane3->SetColor(150, 150, 150);

	Plane *Plane4=new Plane();
	Plane4->SetName("Red wall");
	Plane4->SetPosition(-140, 0, 0);
	Plane4->SetOrientation(1, 0, 0);
	Plane4->SetSpecularity(0);
	Plane4->SetColor(255, 80, 80);

	Plane *Plane5=new Plane();
	Plane4->SetName("Green wall");
	Plane5->SetPosition(140, 0, 0);
	Plane5->SetOrientation(-1, 0, 0);
	Plane5->SetSpecularity(0);
	Plane5->SetColor(80, 255, 80);

	//======== Intersection

	Sphere *NewSphere1=new Sphere();
	NewSphere1->SetName("Sphere 1");
	NewSphere1->SetRadius(44);
	NewSphere1->SetPosition(0, 50, 300);
	//NewSphere1->SetReflectivity(0.4);
	//NewSphere1->SetBrightness(0.5);
	NewSphere1->SetColor(200, 20, 20);

	Cube *NewCube1=new Cube();
	NewCube1->SetName("Cube 1");
	NewCube1->SetLength(68);
	NewCube1->SetPosition(0, 50, 300);
	//NewCube1->SetReflectivity(0.6);
	//NewCube1->SetSpecularity(0.5);
	//NewCube1->SetBrightness(0.5);
	NewCube1->SetColor(20, 180, 180);

	Intersection *ShpereCubeIntersection=new Intersection(NewSphere1, NewCube1);
	ShpereCubeIntersection->SetReflectivity(0.25);
	ShpereCubeIntersection->SetSpecularity(0.75);
	ShpereCubeIntersection->SetColor(0, 180, 180);

	Cylinder *NewCylinder1=new Cylinder();
	NewCylinder1->SetName("Cylinder 1");
	NewCylinder1->SetLength(70);
	NewCylinder1->SetRadius(20);
	NewCylinder1->SetPosition(0, 50, 300);
	//NewCylinder1->SetReflectivity(0.6);
	//NewCylinder1->SetSpecularity(0.5);
	//NewCylinder1->SetBrightness(0.25);
	NewCylinder1->SetColor(20, 180, 150);

	Difference *ShpereCylinderDifference=new Difference(ShpereCubeIntersection, NewCylinder1);

	//========

	Sphere *Sphere2=new Sphere();
	Sphere2->SetName("Blue sphere");
	Sphere2->SetRadius(32);
	Sphere2->SetPosition(-80, 50, 300);
	Sphere2->SetReflectivity(0.25);
	//Sphere2->SetSpecularity(0.85);
	//Sphere2->SetBrightness(0.5);
	Sphere2->SetColor(0, 0, 255);

	Sphere *Sphere3=new Sphere();
	Sphere3->SetName("Black sphere");
	Sphere3->SetRadius(32);
	Sphere3->SetPosition(0, 50, 300);
	Sphere3->SetReflectivity(0.25);
	//Sphere3->SetSpecularity(0.85);
	//Sphere3->SetBrightness(0.5);
	Sphere3->SetColor(0, 0, 0);

	Cylinder *Cylinder2=new Cylinder();
	Cylinder2->SetName("Cylinder");
	Cylinder2->SetLength(56);
	Cylinder2->SetRadius(28);
	Cylinder2->SetPosition(80, 50, 300);
	//Cylinder2->SetSpecularity(0.5);
	//Cylinder2->SetBrightness(0.25);
	Cylinder2->SetColor(150, 10, 150);

	Cube *Cube2=new Cube();
	Cube2->SetName("Cube 2");
	Cube2->SetLength(56);
	Cube2->SetPosition(80, 50, 300);
	Cube2->SetReflectivity(0.25);
	//Cube2->SetSpecularity(0.85);
	//Cube2->SetBrightness(0.0);
	Cube2->SetColor(150, 10, 150);

	Torus *Torus1=new Torus();
	Torus1->SetName("Torus 2");
	Torus1->SetRadius1(34);
	Torus1->SetRadius2(10);
	Torus1->SetPosition(0, 50, 300);
	Torus1->SetReflectivity(0.25);
	//Torus1->SetSpecularity(0.85);
	//Torus1->SetBrightness(0.0);
	Torus1->SetColor(200, 200, 80);

	Cube *LightSource1=new Cube();
	LightSource1->SetName("Cubical light source");
	LightSource1->SetLength(92);
	LightSource1->SetPosition(0, -144, 300);
	LightSource1->SetBrightness(1.4);
	LightSource1->SetColor(255, 255, 255);

	Sphere *LightSource2=new Sphere();
	LightSource2->SetName("Spherical light source");
	LightSource2->SetRadius(50);
	LightSource2->SetPosition(0, -120, 0);
	LightSource2->SetBrightness(1.2);
	LightSource2->SetColor(255, 255, 255);

	//NewScene->AddObject(ShpereCubeIntersection);
	NewScene->AddObject(ShpereCylinderDifference);

	NewScene->AddObject(Sphere2);
	// NewScene->AddObject(Sphere3);
	NewScene->AddObject(Cylinder2);
	// NewScene->AddObject(Cube2);
	//NewScene->AddObject(Torus1);
	NewScene->AddObject(LightSource1);
	// NewScene->AddObject(LightSource2);
	NewScene->AddObject(Plane1);
	NewScene->AddObject(Plane2);
	NewScene->AddObject(Plane3);
	NewScene->AddObject(Plane4);
	NewScene->AddObject(Plane5);

	uint32_t rays_per_pixel=4;

	for(int32_t i=0; i<8; i++)
	{
		// double spec=i*0.1;
		// ShpereCubeIntersection->SetSpecularity(spec);
		Sphere2->SetSpecularity(1);
		// Sphere3->SetSpecularity(spec);
		Cylinder2->SetSpecularity(1);
		// Cylinder1->SetOrientation(0, std::sin(i*M_PI_2/10.0), std::cos(i*M_PI_2/10.0));
		// Cube2->SetOrientation(std::sin(i*M_PI_2/10.0), std::cos(i*M_PI_2/10.0), 0);
		// Torus1->SetOrientation(0, std::sin(i*M_PI_2/10.0), std::cos(i*M_PI_2/10.0));
		// NewCube1->SetOrientation(std::sin(i*M_PI_2/10.0), 0, std::cos(i*M_PI_2/10.0));

		NewScene->SetNumOfRayRunsPerPixel(rays_per_pixel);
		NewScene->Render();
		QImage img1(NewScene->ImageData->data(), NewScene->ScreenWidth(), NewScene->ScreenHeight(), QImage::Format_RGBA8888);
		img1.save(QString::number(rays_per_pixel)+
					QString("-")+
					QString::number(i)+
					QString(".png"));

		rays_per_pixel*=2;
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

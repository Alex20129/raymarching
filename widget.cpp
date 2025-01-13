#include "./ui_widget.h"
#include "widget.hpp"
#include "controlwidget.hpp"

#include <QPaintEvent>
#include <QPainter>

extern Scene *NewScene;
extern Widget *SceneW;
extern ControlWidget *ControlsW;

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
	ui->setupUi(this);
	this->resize(NewScene->ScreenWidth()+100, NewScene->ScreenHeight()+100);
}

Widget::~Widget()
{
	delete ui;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
	if(event->key()==Qt::Key_F11)
	{
		event->accept();
		if(this->isFullScreen())
		{
			this->showNormal();
		}
		else
		{
			this->showFullScreen();
		}
	}
}

void Widget::closeEvent(QCloseEvent *event)
{
	event->accept();
	QApplication::exit(0);
}

void Widget::paintEvent(QPaintEvent *event)
{
	int X, Y;
	QPainter myPainter(this);
	event->accept();
	if(!pImage)
	{
		return;
	}
	X=this->width()/2-pImage->width()/2;
	Y=this->height()/2-pImage->height()/2;
	myPainter.drawImage(X, Y, *pImage);
}

void Widget::setImage(QImage *new_image)
{
	pImage=new_image;
	// this->repaint(); // immediate repaint
	this->update(); // deferred repaint
}

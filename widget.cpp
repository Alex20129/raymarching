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
	event->accept();
	QPainter(this).drawImage(QRectF(this->rect().width()/2-NewScene->ScreenWidth()/2, this->rect().height()/2-NewScene->ScreenHeight()/2, NewScene->ScreenWidth(), NewScene->ScreenHeight()),
							 QImage(NewScene->ImageData->data(), NewScene->ScreenWidth(), NewScene->ScreenHeight(), QImage::Format_RGBA8888));
}

void Widget::resizeEvent(QResizeEvent *event)
{

}

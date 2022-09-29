#include "./ui_widget.h"
#include "widget.h"
#include "controlwidget.h"

#include <QPaintEvent>
#include <QPainter>

extern Scene *MainScene;
extern Widget *SceneW;
extern ControlWidget *ControlsW;

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
	ui->setupUi(this);
	this->resize(MainScene->ScreenWidth()+100, MainScene->ScreenHeight()+100);
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

void Widget::paintEvent(QPaintEvent *event)
{
	event->accept();
	QPainter(this).drawImage(QRectF(this->rect().width()/2-MainScene->ScreenWidth()/2, this->rect().height()/2-MainScene->ScreenHeight()/2, MainScene->ScreenWidth(), MainScene->ScreenHeight()),
							 QImage(MainScene->ImageData->data(), MainScene->ScreenWidth(), MainScene->ScreenHeight(), QImage::Format_RGBA8888));
}

void Widget::resizeEvent(QResizeEvent *event)
{

}

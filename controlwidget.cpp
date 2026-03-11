#include "./ui_controlwidget.h"
#include "controlwidget.hpp"
#include "widget.hpp"

#include <QPaintEvent>
#include <QPainter>
#include <QLineEdit>
#include <QComboBox>

extern Scene *NewScene;
extern Widget *SceneW;

ControlWidget::ControlWidget(QWidget *parent) : QWidget(parent), ui(new Ui::ControlWidget)
{
	ui->setupUi(this);
	obID=0;
}

ControlWidget::~ControlWidget()
{
	delete ui;
}

void ControlWidget::on_Zdial_valueChanged(int value)
{
	ui->lineEditZ->setText(QString::number(value));
	objpos.Z=value;
}

void ControlWidget::on_YScrollBar_valueChanged(int value)
{
	ui->lineEditY->setText(QString::number(value));
	objpos.Y=value;
}

void ControlWidget::on_XScrollBar_valueChanged(int value)
{
	ui->lineEditX->setText(QString::number(value));
	objpos.X=value;
}

void ControlWidget::on_comboBox_currentIndexChanged(int index)
{
	obID=index;
	ui->lineEditX->setText(QString::number(objpos.X));
	ui->XScrollBar->setValue(objpos.X);
	ui->lineEditY->setText(QString::number(objpos.Y));
	ui->YScrollBar->setValue(objpos.Y);
	ui->lineEditZ->setText(QString::number(objpos.Z));
	ui->Zdial->setValue(objpos.Z);
}

void ControlWidget::on_renderButton_clicked()
{
	obID=ui->comboBox->currentIndex();

	objpos.X=ui->lineEditX->text().toDouble();
	objpos.Y=ui->lineEditY->text().toDouble();
	objpos.Z=ui->lineEditZ->text().toDouble();

	ui->XScrollBar->setValue(objpos.X);
	ui->YScrollBar->setValue(objpos.Y);
	ui->Zdial->setValue(objpos.Z);

	NewScene->Render();
	ui->rendertime->setText(QString::number(NewScene->FrameRenderTime));
	SceneW->update();
}

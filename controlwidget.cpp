#include "./ui_controlwidget.h"
#include "controlwidget.h"
#include "widget.h"

#include <QPaintEvent>
#include <QPainter>
#include <QLineEdit>
#include <QComboBox>

extern Scene *MainScene;
extern Widget *SceneW;
extern ControlWidget *ControlsW;

ControlWidget::ControlWidget(QWidget *parent) : QWidget(parent), ui(new Ui::ControlWidget)
{
	ui->setupUi(this);
	for(int obj=0; obj<MainScene->SceneObjects->size(); obj++)
	{
		ui->comboBox->addItem(QString::fromStdString(MainScene->SceneObjects->at(obj)->Name()));
	}
	obID=0;
}

ControlWidget::~ControlWidget()
{
	delete ui;
}

void ControlWidget::on_Zdial_valueChanged(int value)
{
	objpos=MainScene->SceneObjects->at(obID)->Position;
	ui->lineEditZ->setText(QString::number(value));
	objpos->Z=value;
}

void ControlWidget::on_YScrollBar_valueChanged(int value)
{
	objpos=MainScene->SceneObjects->at(obID)->Position;
	ui->lineEditY->setText(QString::number(value));
	objpos->Y=value;
}

void ControlWidget::on_XScrollBar_valueChanged(int value)
{
	objpos=MainScene->SceneObjects->at(obID)->Position;
	ui->lineEditX->setText(QString::number(value));
	objpos->X=value;
}

void ControlWidget::on_comboBox_currentIndexChanged(int index)
{
	obID=index;
	objpos=MainScene->SceneObjects->at(obID)->Position;
	ui->lineEditX->setText(QString::number(objpos->X));
	ui->XScrollBar->setValue(objpos->X);
	ui->lineEditY->setText(QString::number(objpos->Y));
	ui->YScrollBar->setValue(objpos->Y);
	ui->lineEditZ->setText(QString::number(objpos->Z));
	ui->Zdial->setValue(objpos->Z);
}

void ControlWidget::on_renderButton_clicked()
{
	obID=ui->comboBox->currentIndex();
	objpos=MainScene->SceneObjects->at(obID)->Position;

	objpos->X=ui->lineEditX->text().toDouble();
	objpos->Y=ui->lineEditY->text().toDouble();
	objpos->Z=ui->lineEditZ->text().toDouble();

	ui->XScrollBar->setValue(objpos->X);
	ui->YScrollBar->setValue(objpos->Y);
	ui->Zdial->setValue(objpos->Z);

	MainScene->Render();
	ui->rendertime->setText(QString::number(MainScene->FrameRenderTime.count()));
	SceneW->update();
}

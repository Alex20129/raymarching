#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include <QBrush>

#include "scene.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class ControlWidget; }
QT_END_NAMESPACE

class ControlWidget : public QWidget
{
	Q_OBJECT
signals:
public:
	ControlWidget(QWidget *parent=nullptr);
	~ControlWidget();
public slots:
private slots:
	void on_Zdial_valueChanged(int value);
	void on_YScrollBar_valueChanged(int value);
	void on_XScrollBar_valueChanged(int value);
	void on_comboBox_currentIndexChanged(int index);
	void on_renderButton_clicked();

private:
	Vec3d *objpos;
	uint obID;
	Ui::ControlWidget *ui;
};
#endif // CONTROLWIDGET_H

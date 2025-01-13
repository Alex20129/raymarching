#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <QWidget>
#include <QBrush>
#include <QImage>

#include "scene.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
	Q_OBJECT
	QImage *pImage;
	Ui::Widget *ui;
signals:
public:
	Widget(QWidget *parent=nullptr);
	~Widget();
	void keyPressEvent(QKeyEvent *event);
	void closeEvent(QCloseEvent *event);
	void paintEvent(QPaintEvent *event);
public slots:
	void setImage(QImage *new_image);
};
#endif // WIDGET_HPP

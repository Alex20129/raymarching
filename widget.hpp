#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <QWidget>
#include <QBrush>

#include "scene.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
	Q_OBJECT
signals:
public:
	Widget(QWidget *parent=nullptr);
	~Widget();
	void keyPressEvent(QKeyEvent *event);
	void closeEvent(QCloseEvent *event);
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
public slots:
private:
	Ui::Widget *ui;
private slots:
};
#endif // WIDGET_HPP

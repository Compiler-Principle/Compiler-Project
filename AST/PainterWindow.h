#ifndef COMPILER_PROJECT_PAINTERWINDOW_H
#define COMPILER_PROJECT_PAINTERWINDOW_H
#include <QtWidgets/QApplication>
#include <QtWidgets/QtWidgets>
#include <QPainter>
#include <QFont>
#include <QBrush>
#include <QColor>
#include <QPaintDevice>
#include <QPaintEngine>
#include <QPixmap>
#include <QPainterPath>
#include <QDebug>
#include <QLCDNumber>
#include <QRect>
#include <QRectF>
#include <QPoint>
#include <QPointF>
#include <QLabel>
#include "AST.h"


class PainterWindow : public QWidget {
public:
    explicit PainterWindow(QWidget *parent = 0);

    PainterWindow(baseAST *node);

    ~PainterWindow();
    void paintEvent(QPaintEvent *);
    baseAST *ast;

private:
};


#endif //COMPILER_PROJECT_PAINTERWINDOW_H

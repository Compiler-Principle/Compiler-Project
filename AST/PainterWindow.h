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
#include "json.hpp"

using json_t = nlohmann::json;

class PainterWindow : public QWidget {
public:
    baseAST *ast;
    explicit PainterWindow(QWidget *parent = 0);
    PainterWindow(baseAST *node);

    ~PainterWindow();
    void paintEvent(QPaintEvent *);
    std::vector<baseAST *> traversal() const;

private:
};


#endif //COMPILER_PROJECT_PAINTERWINDOW_H

#include <queue>
#include "PainterWindow.h"
#include "AST.h"

PainterWindow::PainterWindow(QWidget *parent) {

}


PainterWindow::~PainterWindow() {

}

void PainterWindow::paintEvent(QPaintEvent *){
//    QPainter painter(this);
//    painter.setPen(Qt::blue);
//    painter.setFont(QFont("Arial", 50));
////    painter->drawText(rect(), Qt::AlignCenter, "Qt");
//    painter.drawRect(20,20,160,160);

}

PainterWindow::PainterWindow(baseAST *node) {
    this->ast = node;
}

std::vector<baseAST *> PainterWindow::traversal() const {
    // TODO: implement 2D return vector with level index
    std::vector<baseAST *> ret;
    std::queue<baseAST *> q;
    q.push(this->ast);
    while(!q.empty()) {
        baseAST *top =q.front();
        for(auto t: top->children) {
            ret.push_back(t);
        }
        q.pop();
    }

    return ret;
}

int baseAST::IDAccumulate = 0;

int main(int argc, char ** argv){
    baseAST a;
    a.print();


    return 0;
}
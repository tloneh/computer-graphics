#include "myglwidget.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MyGLWidget widget;
    widget.setWindowTitle("Ray Tracing");
    widget.resize(800, 450); // 设置初始窗口大小
    widget.show();

    return app.exec();
}
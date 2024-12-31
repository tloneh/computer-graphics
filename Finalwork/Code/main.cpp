#include "myglwidget.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MyGLWidget widget;
    widget.setWindowTitle("Ray Tracing");
    widget.resize(800, 450); // ���ó�ʼ���ڴ�С
    widget.show();

    return app.exec();
}
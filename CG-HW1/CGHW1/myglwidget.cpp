#include "myglwidget.h"
float rotationX = 0.0f;
float rotationY = 0.0f;
float rotationZ = 0.0f;
MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent),
	scene_id(0)
{
}

MyGLWidget::~MyGLWidget()
{

}

void MyGLWidget::initializeGL()
{
	glViewport(0, 0, width(), height());
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glDisable(GL_DEPTH_TEST);
}

void MyGLWidget::paintGL()
{
	if (scene_id==0) {
		scene_0();
	}
	else if(scene_id == 1){
		scene_1();
	}
    else {
        scene_2();
    }
}

void MyGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	update();
}

void MyGLWidget::keyPressEvent(QKeyEvent *e) {
	//Press 0 or 1 to switch the scene
	if (e->key() == Qt::Key_0) {
		scene_id = 0;
		update();
	}
	else if (e->key() == Qt::Key_1) {
		scene_id = 1;
		update();
	}
    else if(e->key() == Qt::Key_2){
        scene_id = 2;
        update();
    }
    else if (e->key() == Qt::Key_X) {
        rotationX += 5.0f;  // 每次按键增加旋转角度
        //rotationX -= 5.0f;  //即可实现旋转方向相反
        update();
    }
    else if (e->key() == Qt::Key_Y) {
        rotationY += 5.0f;  // 每次按键增加旋转角度
        update();
    }
    else if (e->key() == Qt::Key_Z) {
        rotationZ += 5.0f;  // 每次按键增加旋转角度
        update();
    }
}

void MyGLWidget::scene_0()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, 100.0f, 0.0f, 100.0f, -1000.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(50.0f, 50.0f, 0.0f);
	
	//draw a diagonal "I"
	glPushMatrix();
	glColor3f(0.839f, 0.153f, 0.157f);
	glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(-2.5f, -22.5f, 0.0f);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(5.0f, 0.0f);
	glVertex2f(0.0f, 45.0f);

	glVertex2f(5.0f, 0.0f);
	glVertex2f(0.0f, 45.0f);
	glVertex2f(5.0f, 45.0f);

	glEnd();
	glPopMatrix();	
}

void MyGLWidget::scene_1()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width(), 0.0f, height(), -1000.0f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.5 * width(), 0.5 * height(), 0.0f);  // 移动到窗口中心

    // 绘制字母 H
    glPushMatrix();
    glTranslatef(-200.0f, 0.0f, 0.0f);  // 移动到合适位置
    drawH();
    glPopMatrix();

    // 绘制字母 G
    glPushMatrix();
    glTranslatef(0.0f, -50.0f, 0.0f);  // 适当平移以防重叠
    drawG();
    glPopMatrix();

    // 绘制字母 Y
    glPushMatrix();
    glTranslatef(200.0f, -50.0f, 0.0f);  // 适当平移以防重叠
    drawY();
    glPopMatrix();
}

// 绘制字母 H
void MyGLWidget::drawH()
{
    glColor4f(1.0f, 0.0f, 0.0f, 0.9f); // 红色，透明度0.9
    // 左竖线
    glBegin(GL_QUAD_STRIP);
    glVertex2f(-50.0f, -100.0f);
    glVertex2f(-70.0f, -100.0f);
    glVertex2f(-50.0f, 100.0f);
    glVertex2f(-70.0f, 100.0f);
    glEnd();
    // 横杠
    glBegin(GL_QUAD_STRIP);
    glVertex2f(-50.0f, -10.0f);
    glVertex2f(50.0f, -10.0f);
    glVertex2f(-50.0f, 10.0f);
    glVertex2f(50.0f, 10.0f);
    glEnd();
    //右竖线
    glBegin(GL_QUAD_STRIP);
    glVertex2f(70.0f, -100.0f);
    glVertex2f(50.0f, -100.0f);
    glVertex2f(70.0f, 100.0f);
    glVertex2f(50.0f, 100.0f);
    glEnd();
}

// 绘制字母 G
void MyGLWidget::drawG()
{
    glColor4f(0.0f, 1.0f, 0.0f, 0.6f); // 绿色，0.6透明度
    //中间横
    glBegin(GL_QUAD_STRIP);
    glVertex2f(-50.0f, 0.0f);
    glVertex2f(-70.0f, 0.0f);
    glVertex2f(-50.0f, 100.0f);
    glVertex2f(-70.0f, 100.0f);
    glEnd();
    //左下斜边
    glBegin(GL_TRIANGLES);
    glVertex2f(-50.0f, 0.0f);
    glVertex2f(-70.0f, 0.0f);
    glVertex2f(0.0f, -50.0f);

    glVertex2f(0.0f, -50.0f);
    glVertex2f(0.0f, -70.0f);
    glVertex2f(-70.0f, -0.0f);
    glEnd();
    //左上斜边
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 150.0f);
    glVertex2f(0.0f, 170.0f);
    glVertex2f(-50.0f, 100.0f);

    glVertex2f(-70.0f, 100.0f);
    glVertex2f(-50.0f, 100.0f);
    glVertex2f(0.0f, 170.0f);
    glEnd();
    //下横
    glBegin(GL_QUAD_STRIP);
    glVertex2f(0.0f, -50.0f);
    glVertex2f(0.0f, -70.0f);
    glVertex2f(100.0f, -50.0f);
    glVertex2f(100.0f, -70.0f);
    glEnd();
    //上横
    glBegin(GL_QUAD_STRIP);
    glVertex2f(0.0f, 150.0f);
    glVertex2f(0.0f, 170.0f);
    glVertex2f(100.0f, 150.0f);
    glVertex2f(100.0f, 170.0f);
    glEnd();
    //T竖
    glBegin(GL_QUAD_STRIP);
    glVertex2f(100.0f, -50.0f);
    glVertex2f(100.0f, -10.0f);
    glVertex2f(80.0f, -50.0f);
    glVertex2f(80.0f, -10.0f);
    glEnd();
    //T横
    glBegin(GL_QUAD_STRIP);
    glVertex2f(60.0f, -10.0f);
    glVertex2f(60.0f, 10.0f);
    glVertex2f(120.0f, -10.0f);
    glVertex2f(120.0f, 10.0f);
    glEnd();
}

// 绘制字母 Y
void MyGLWidget::drawY()
{
    glColor4f(0.0f, 0.0f, 1.0f, 0.3f); // 蓝色
    // 下半
    glBegin(GL_QUAD_STRIP);
    glVertex2f(10.0f, -50.0f);
    glVertex2f(10.0f, 50.0f);
    glVertex2f(-10.0f, -50.0f);
    glVertex2f(-10.0f, 50.0f);
    glEnd();
    //右上
    glBegin(GL_QUAD_STRIP);
    glVertex2f(10.0f, 50.0f);
    glVertex2f(0.0f, 65.0f);
    glVertex2f(60.0f, 136.5f);
    glVertex2f(42.7f, 146.5f);
    glEnd();
    //左上
    glBegin(GL_QUAD_STRIP);
    glVertex2f(-10.0f, 50.0f);
    glVertex2f(0.0f, 65.0f);
    glVertex2f(-60.0f, 136.5f);
    glVertex2f(-42.7f, 146.5f);
    glEnd();
    // 三角形
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 65.0f);
    glVertex2f(-10.0f, 50.0f);
    glVertex2f(10.0f, 50.0f);
    glEnd();
}

void MyGLWidget::scene_2()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width(), 0.0f, height(), -1000.0f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.5 * width(), 0.5 * height(), 0.0f);  // 移动到窗口中心

    // 设置字母 H 的厚度
    float thickness = 20.0f;

    //// 绘制字母 H（绕 X 轴旋转）
    //glPushMatrix();
    //glRotatef(rotationX, 1.0f, 0.0f, 0.0f);  // 绕 X 轴旋转
    //glTranslatef(0.0f, 0.0f, 0.0f);  // 平移到合适位置
    //draw3DH(thickness);
    //glPopMatrix();
    //
    // 绘制字母 H（绕 Y 轴旋转）
    //glPushMatrix();
    //glRotatef(rotationY, 0.0f, 1.0f, 0.0f);  // 绕 Y 轴旋转
    //glTranslatef(0.0f, 0.0f, 0.0f); 
    //draw3DH(thickness);
    //glPopMatrix();

    // 绘制字母 H（绕 Z 轴旋转）
    //glPushMatrix();
    //glRotatef(rotationZ, 0.0f, 0.0f, 1.0f);  // 绕 Z 轴旋转
    //glTranslatef(0.0f, 0.0f, 0.0f); 
    //draw3DH(thickness);
    //glPopMatrix();
}

void MyGLWidget::draw3DH(float thickness)
{
    // 绘制字母 H 的 3D 版本
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -thickness / 2.0f);
    for (int i = -thickness; i < thickness; i += 1.0f) {
        glTranslatef(0.0f, 0.0f, 0.5f);
        drawH();
    }
    glPopMatrix();

    
}



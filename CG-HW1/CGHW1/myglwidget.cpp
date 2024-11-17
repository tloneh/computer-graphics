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
        rotationX += 5.0f;  // ÿ�ΰ���������ת�Ƕ�
        //rotationX -= 5.0f;  //����ʵ����ת�����෴
        update();
    }
    else if (e->key() == Qt::Key_Y) {
        rotationY += 5.0f;  // ÿ�ΰ���������ת�Ƕ�
        update();
    }
    else if (e->key() == Qt::Key_Z) {
        rotationZ += 5.0f;  // ÿ�ΰ���������ת�Ƕ�
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
    glTranslatef(0.5 * width(), 0.5 * height(), 0.0f);  // �ƶ�����������

    // ������ĸ H
    glPushMatrix();
    glTranslatef(-200.0f, 0.0f, 0.0f);  // �ƶ�������λ��
    drawH();
    glPopMatrix();

    // ������ĸ G
    glPushMatrix();
    glTranslatef(0.0f, -50.0f, 0.0f);  // �ʵ�ƽ���Է��ص�
    drawG();
    glPopMatrix();

    // ������ĸ Y
    glPushMatrix();
    glTranslatef(200.0f, -50.0f, 0.0f);  // �ʵ�ƽ���Է��ص�
    drawY();
    glPopMatrix();
}

// ������ĸ H
void MyGLWidget::drawH()
{
    glColor4f(1.0f, 0.0f, 0.0f, 0.9f); // ��ɫ��͸����0.9
    // ������
    glBegin(GL_QUAD_STRIP);
    glVertex2f(-50.0f, -100.0f);
    glVertex2f(-70.0f, -100.0f);
    glVertex2f(-50.0f, 100.0f);
    glVertex2f(-70.0f, 100.0f);
    glEnd();
    // ���
    glBegin(GL_QUAD_STRIP);
    glVertex2f(-50.0f, -10.0f);
    glVertex2f(50.0f, -10.0f);
    glVertex2f(-50.0f, 10.0f);
    glVertex2f(50.0f, 10.0f);
    glEnd();
    //������
    glBegin(GL_QUAD_STRIP);
    glVertex2f(70.0f, -100.0f);
    glVertex2f(50.0f, -100.0f);
    glVertex2f(70.0f, 100.0f);
    glVertex2f(50.0f, 100.0f);
    glEnd();
}

// ������ĸ G
void MyGLWidget::drawG()
{
    glColor4f(0.0f, 1.0f, 0.0f, 0.6f); // ��ɫ��0.6͸����
    //�м��
    glBegin(GL_QUAD_STRIP);
    glVertex2f(-50.0f, 0.0f);
    glVertex2f(-70.0f, 0.0f);
    glVertex2f(-50.0f, 100.0f);
    glVertex2f(-70.0f, 100.0f);
    glEnd();
    //����б��
    glBegin(GL_TRIANGLES);
    glVertex2f(-50.0f, 0.0f);
    glVertex2f(-70.0f, 0.0f);
    glVertex2f(0.0f, -50.0f);

    glVertex2f(0.0f, -50.0f);
    glVertex2f(0.0f, -70.0f);
    glVertex2f(-70.0f, -0.0f);
    glEnd();
    //����б��
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 150.0f);
    glVertex2f(0.0f, 170.0f);
    glVertex2f(-50.0f, 100.0f);

    glVertex2f(-70.0f, 100.0f);
    glVertex2f(-50.0f, 100.0f);
    glVertex2f(0.0f, 170.0f);
    glEnd();
    //�º�
    glBegin(GL_QUAD_STRIP);
    glVertex2f(0.0f, -50.0f);
    glVertex2f(0.0f, -70.0f);
    glVertex2f(100.0f, -50.0f);
    glVertex2f(100.0f, -70.0f);
    glEnd();
    //�Ϻ�
    glBegin(GL_QUAD_STRIP);
    glVertex2f(0.0f, 150.0f);
    glVertex2f(0.0f, 170.0f);
    glVertex2f(100.0f, 150.0f);
    glVertex2f(100.0f, 170.0f);
    glEnd();
    //T��
    glBegin(GL_QUAD_STRIP);
    glVertex2f(100.0f, -50.0f);
    glVertex2f(100.0f, -10.0f);
    glVertex2f(80.0f, -50.0f);
    glVertex2f(80.0f, -10.0f);
    glEnd();
    //T��
    glBegin(GL_QUAD_STRIP);
    glVertex2f(60.0f, -10.0f);
    glVertex2f(60.0f, 10.0f);
    glVertex2f(120.0f, -10.0f);
    glVertex2f(120.0f, 10.0f);
    glEnd();
}

// ������ĸ Y
void MyGLWidget::drawY()
{
    glColor4f(0.0f, 0.0f, 1.0f, 0.3f); // ��ɫ
    // �°�
    glBegin(GL_QUAD_STRIP);
    glVertex2f(10.0f, -50.0f);
    glVertex2f(10.0f, 50.0f);
    glVertex2f(-10.0f, -50.0f);
    glVertex2f(-10.0f, 50.0f);
    glEnd();
    //����
    glBegin(GL_QUAD_STRIP);
    glVertex2f(10.0f, 50.0f);
    glVertex2f(0.0f, 65.0f);
    glVertex2f(60.0f, 136.5f);
    glVertex2f(42.7f, 146.5f);
    glEnd();
    //����
    glBegin(GL_QUAD_STRIP);
    glVertex2f(-10.0f, 50.0f);
    glVertex2f(0.0f, 65.0f);
    glVertex2f(-60.0f, 136.5f);
    glVertex2f(-42.7f, 146.5f);
    glEnd();
    // ������
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
    glTranslatef(0.5 * width(), 0.5 * height(), 0.0f);  // �ƶ�����������

    // ������ĸ H �ĺ��
    float thickness = 20.0f;

    //// ������ĸ H���� X ����ת��
    //glPushMatrix();
    //glRotatef(rotationX, 1.0f, 0.0f, 0.0f);  // �� X ����ת
    //glTranslatef(0.0f, 0.0f, 0.0f);  // ƽ�Ƶ�����λ��
    //draw3DH(thickness);
    //glPopMatrix();
    //
    // ������ĸ H���� Y ����ת��
    //glPushMatrix();
    //glRotatef(rotationY, 0.0f, 1.0f, 0.0f);  // �� Y ����ת
    //glTranslatef(0.0f, 0.0f, 0.0f); 
    //draw3DH(thickness);
    //glPopMatrix();

    // ������ĸ H���� Z ����ת��
    //glPushMatrix();
    //glRotatef(rotationZ, 0.0f, 0.0f, 1.0f);  // �� Z ����ת
    //glTranslatef(0.0f, 0.0f, 0.0f); 
    //draw3DH(thickness);
    //glPopMatrix();
}

void MyGLWidget::draw3DH(float thickness)
{
    // ������ĸ H �� 3D �汾
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -thickness / 2.0f);
    for (int i = -thickness; i < thickness; i += 1.0f) {
        glTranslatef(0.0f, 0.0f, 0.5f);
        drawH();
    }
    glPopMatrix();

    
}



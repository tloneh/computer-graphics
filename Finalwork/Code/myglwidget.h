#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "utils.h"

#define MAX_Z_BUFFER 99999999.0f
#define MIN_FLOAT 1e-10f

using namespace glm;




class MyGLWidget : public QOpenGLWidget{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void keyPressEvent(QKeyEvent* e);

private:
    void scene_0();
    void clearBuffer(vec3* now_render_buffer);
    void clearBuffer(int* now_buffer);
    void clearZBuffer(float* now_buffer);
    void resizeBuffer(int newW, int newH);

    int WindowSizeH = 0;
    int WindowSizeW = 0;
    int scene_id;
    int degree = 0;

    // buffers
    vec3* render_buffer;
    vec3* temp_render_buffer;
    float* temp_z_buffer;
    float* z_buffer;
    vec2 offset;

    Model objModel;

    vec3 camPosition;
    vec3 camLookAt;
    vec3 camUp;
    mat4 projMatrix;
    vec3 lightPosition;


};

vec3 reflect(const vec3& incident, const vec3& normal);
vec3 random_in_unit_sphere();
vec3 random_unit_vector();
float random_float();
// ��Ȧ�ڵ����������
vec3 random_in_unit_disk();

class Ray {
public:
    vec3 origin;    // �������
    vec3 direction; // ���߷���
    Ray() : origin(0.0f, 0.0f, 0.0f), direction(0.0f, 0.0f, 0.0f) {} // Ĭ�Ϲ��캯��
    Ray(const vec3& o, const vec3& d) : origin(o), direction(normalize(d)) {}

    vec3 at(float t) const {
        return origin + t * direction; // �������̼�����λ��
    }
};
class Camera {
public:
    vec3 origin;            // �����λ��
    vec3 lower_left_corner; // �Ӵ����½�
    vec3 horizontal;        // �Ӵ�ˮƽ��Χ
    vec3 vertical;          // �Ӵ���ֱ��Χ
    float lens_radius;      // ��ͷ�뾶�����ڹ�Ȧģ��
    float focus_dist;       // ���࣬���ڹ�Ȧģ��

    // ������캯��
    Camera(vec3 position, vec3 look_at, vec3 up, float aspect_ratio, float aperture, float focus_dist) {
        origin = position;
        this->focus_dist = focus_dist;  // ���潹��
        vec3 w = normalize(position - look_at);
        vec3 u = normalize(cross(up, w));
        vec3 v = cross(w, u);

        // �趨�Ӵ��ĸ߶ȺͿ��
        float viewport_height = 2.0f;
        float viewport_width = aspect_ratio * viewport_height;

        // �����Ӵ��ķ�Χ
        horizontal = viewport_width * u;
        vertical = viewport_height * v;

        // �����Ӵ������½�λ��
        lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - w;

        // �����������Ӵ���λ��
        horizontal *= focus_dist;
        vertical *= focus_dist;
        lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - w;

        lens_radius = aperture / 2.0f;  // ��Ȧ�뾶
    }

    // ��ȡ���ߣ�֧�ֹ�Ȧģ��
    Ray getRay(float u, float v) const {
        // �������һ����Ȧ�ϵĵ�ƫ��
        vec3 rd = lens_radius * random_in_unit_disk();  // �ڵ�λԲ�����������һ����
        vec3 offset = horizontal * rd.x + vertical * rd.y;  // ��ƫ����Ӧ�õ�����λ��

        // �������ߵķ��򣬷����Ǵ����ԭ�㵽�Ӵ��ϵ�ĳ����
        vec3 direction = lower_left_corner + u * horizontal + v * vertical - origin - offset;

        // �������߶���
        return Ray(origin + offset, direction);
    }
};






// ����������͵�ö��
enum MaterialType {
	LAMBERTIAN,
	METAL,
	DIELECTRIC,
};

// ������
class Material {
public:
	MaterialType type;
	vec3 albedo;       // ������������ɫ
	float fuzz;        // �����ֲڶ�
	float ref_idx;     // �����ʣ����ڵ���ʣ�

	Material(MaterialType t, vec3 color, float f = 0.0f, float ri = 1.0f)
		: type(t), albedo(color), fuzz(f), ref_idx(ri) {}

	bool scatter(const Ray& r_in, const vec3& hit_point, const vec3& normal, vec3& attenuation, Ray& scattered) const;
		


private:
	static float reflectance(float cosine, float ref_idx) {
		float r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};
class Hittable {
public:
    virtual bool hit(const Ray& r, float t_min, float t_max, float& t, vec3& hit_point, vec3& normal, Material& mat) const = 0;
    virtual ~Hittable() {}
};
class Sphere : public Hittable {
public:
    vec3 center;
    float radius;
    Material material;

    Sphere(vec3 c, float r, Material m) : center(c), radius(r), material(m) {}

    bool hit(const Ray& r, float t_min, float t_max, float& t, vec3& hit_point, vec3& normal, Material& mat) const override {
        // ������ཻ�����߼�
        vec3 oc = r.origin - center;
        float a = dot(r.direction, r.direction);
        float b = dot(oc, r.direction);
        float c = dot(oc, oc) - radius * radius;
        float discriminant = b * b - a * c;

        if (discriminant > 0) {
            float root = sqrt(discriminant);
            float temp = (-b - root) / a;
            if (temp < t_max && temp > t_min) {
                t = temp;
                hit_point = r.at(t);
                normal = (hit_point - center) / radius;
                mat = material;
                return true;
            }

            temp = (-b + root) / a;
            if (temp < t_max && temp > t_min) {
                t = temp;
                hit_point = r.at(t);
                normal = (hit_point - center) / radius;
                mat = material;
                return true;
            }
        }
        return false;
    }
};


vec3 rayColor(const Ray& r, const std::vector<Sphere>& world, int depth);


#endif // MYGLWIDGET_H

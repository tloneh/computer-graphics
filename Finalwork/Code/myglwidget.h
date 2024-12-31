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
// 光圈内的随机点生成
vec3 random_in_unit_disk();

class Ray {
public:
    vec3 origin;    // 光线起点
    vec3 direction; // 光线方向
    Ray() : origin(0.0f, 0.0f, 0.0f), direction(0.0f, 0.0f, 0.0f) {} // 默认构造函数
    Ray(const vec3& o, const vec3& d) : origin(o), direction(normalize(d)) {}

    vec3 at(float t) const {
        return origin + t * direction; // 参数方程计算点的位置
    }
};
class Camera {
public:
    vec3 origin;            // 摄像机位置
    vec3 lower_left_corner; // 视窗左下角
    vec3 horizontal;        // 视窗水平范围
    vec3 vertical;          // 视窗垂直范围
    float lens_radius;      // 镜头半径，用于光圈模糊
    float focus_dist;       // 焦距，用于光圈模糊

    // 相机构造函数
    Camera(vec3 position, vec3 look_at, vec3 up, float aspect_ratio, float aperture, float focus_dist) {
        origin = position;
        this->focus_dist = focus_dist;  // 保存焦距
        vec3 w = normalize(position - look_at);
        vec3 u = normalize(cross(up, w));
        vec3 v = cross(w, u);

        // 设定视窗的高度和宽度
        float viewport_height = 2.0f;
        float viewport_width = aspect_ratio * viewport_height;

        // 计算视窗的范围
        horizontal = viewport_width * u;
        vertical = viewport_height * v;

        // 计算视窗的左下角位置
        lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - w;

        // 焦点距离调整视窗的位置
        horizontal *= focus_dist;
        vertical *= focus_dist;
        lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - w;

        lens_radius = aperture / 2.0f;  // 光圈半径
    }

    // 获取射线，支持光圈模糊
    Ray getRay(float u, float v) const {
        // 随机生成一个光圈上的点偏移
        vec3 rd = lens_radius * random_in_unit_disk();  // 在单位圆盘上随机生成一个点
        vec3 offset = horizontal * rd.x + vertical * rd.y;  // 将偏移量应用到像素位置

        // 计算射线的方向，方向是从相机原点到视窗上的某个点
        vec3 direction = lower_left_corner + u * horizontal + v * vertical - origin - offset;

        // 返回射线对象
        return Ray(origin + offset, direction);
    }
};






// 定义材质类型的枚举
enum MaterialType {
	LAMBERTIAN,
	METAL,
	DIELECTRIC,
};

// 材质类
class Material {
public:
	MaterialType type;
	vec3 albedo;       // 漫反射或金属颜色
	float fuzz;        // 金属粗糙度
	float ref_idx;     // 折射率（用于电介质）

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
        // 球体的相交计算逻辑
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

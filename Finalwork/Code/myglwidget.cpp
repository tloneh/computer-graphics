#include "myglwidget.h"
#include <GL/glew.h>
#include <algorithm>
#include <thread>
#include <mutex>
#include <vector>


vec3 camera_position(0.0f, 1.0f, 2.0f); // 初始摄像机位置
vec3 camera_look_at(0.0f, 0.0f, -1.0f); // 初始观察目标点
vec3 camera_up(0.0f, 1.0f, 0.0f);

// 光圈半径和焦距设置
float aperture = 0.008f;    // 控制离焦模糊程度
float focus_dist = 3.0f; // 焦点距离

// 生成 [0, 1) 范围内的随机浮点数
vec3 random_in_unit_sphere() {
	while (true) {
		vec3 p = vec3(random_float(), random_float(), random_float()) * 2.0f - vec3(1.0f);
		if (length(p) < 1.0f) return p;
	}
}

vec3 random_unit_vector() {
	return normalize(random_in_unit_sphere());
}

void MyGLWidget::paintGL()
{
	switch (scene_id) {
	case 0:scene_0(); break;

	}
}

vec3 random_in_unit_disk() {
	vec3 p;
	do {
		p = 2.0f * vec3(random_float(), random_float(), 0.0f) - vec3(1.0f, 1.0f, 0.0f); // 随机生成单位圆内的点
	} while (dot(p, p) >= 1.0f);  // 确保点在单位圆内
	return p;
}


bool hitGround(const Ray& r, float& t) {
	const float ground_y = 0.0f; // 地面高度为 0
	if (fabs(r.direction.y) < 1e-6) {
		return false; // 射线平行于地面
	}
	t = (ground_y - r.origin.y) / r.direction.y; // 计算交点
	return t > 0; // 只考虑向下的射线
}


// 生成 [0, 1) 范围内的随机浮点数
float random_float() {
	return rand() / (RAND_MAX + 1.0f);
}



vec3 rayColor(const Ray& r, const std::vector<Sphere>& world, int depth) {
	if (depth <= 0) {
		return vec3(0, 0, 0); // 递归深度为0时返回黑色
	}

	float t;
	vec3 hit_point, normal;
	Material mat(MaterialType::LAMBERTIAN, vec3(0));

	// 先检查光线与球体的相交
	for (const auto& object : world) {
		if (object.hit(r, 0.001f, MAX_Z_BUFFER, t, hit_point, normal, mat)) {
			Ray scattered;
			vec3 attenuation;
			if (mat.scatter(r, hit_point, normal, attenuation, scattered)) {
				return attenuation * rayColor(scattered, world, depth - 1); // 递归调用
			}
			return vec3(0, 0, 0); // 如果没有散射，则返回黑色
		}
	}

	// 检测光线是否与地面相交
	if (hitGround(r, t)) {
		vec3 hit_point = r.at(t);
		if (fabs(hit_point.x) <= 5 && fabs(hit_point.z) <= 5) { // 地面范围
			// 棋盘格纹理
			bool checker = (int(floor(hit_point.x) + floor(hit_point.z)) % 2 == 0);
			return checker ? vec3(0.9f, 0.9f, 0.9f) : vec3(0.3f, 0.3f, 0.3f); // 黑白棋盘
			//return vec3(0.5f, 0.5f, 0.5f);
		}
	}
	// 如果光线没有与任何物体相交，返回渐变背景色
	vec3 unit_direction = normalize(r.direction);
	t = 0.5f * (unit_direction.y + 1.0f);
	return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f); // 渐变背景
}

// 使用多线程进行渲染
void renderScene(int start, int end, int image_width, int image_height, const Camera& cam,
	const std::vector<Sphere>& world, std::vector<vec3>& framebuffer, int max_depth, int samples_per_pixel) {
	for (int j = start; j < end; ++j) {  // 遍历垂直方向的像素行
		for (int i = 0; i < image_width; ++i) {  // 遍历每行中的每个像素
			vec3 pixel_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; ++s) {  // 对每个像素进行多次采样
				float u = (i + random_float()) / (image_width - 1);  // 计算屏幕空间坐标
				float v = (j + random_float()) / (image_height - 1);
				Ray r = cam.getRay(u, v);  // 从相机发射一条光线
				pixel_color += rayColor(r, world, max_depth);  // 计算光线与场景相交后的颜色
			}
			// 每个像素的颜色是所有采样的平均值
			framebuffer[j * image_width + i] = pixel_color / float(samples_per_pixel);
		}
	}
}

void MyGLWidget::scene_0() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 清除帧缓冲和深度缓冲

	int image_width = WindowSizeW;  // 图像宽度
	int image_height = WindowSizeH;  // 图像高度
	float aspect_ratio = float(image_width) / image_height;  // 计算宽高比

	

	Camera camera(camera_position, camera_look_at, camera_up, aspect_ratio, aperture, focus_dist);  // 创建相机

	// 创建场景中的物体
	std::vector<Sphere> world = {
		Sphere(vec3(0, 0.5, -1), 0.5f, Material(LAMBERTIAN, vec3(0.7f, 0.3f, 0.3f), 0.3f)),
		Sphere(vec3(1, 0.5, -1), 0.5f, Material(METAL, vec3(0.8f, 0.6f, 0.2f), 0.3f)),
		Sphere(vec3(-3, 0.5, -1), 0.5f, Material(DIELECTRIC, vec3(1.0f), 0.0f, 1.5f)),
		Sphere(vec3(-1.5, 1, -1), 1.0f, Material(DIELECTRIC, vec3(1.0f), 0.0f, 1.5f)),
		Sphere(vec3(-1.0, 1, -2), 1.0f, Material(METAL, vec3(1.0f, 0.713f, 0.757f), 0.2f)),
		Sphere(vec3(-0.5, 1, -3), 1.0f, Material(DIELECTRIC, vec3(1.0f), 0.0f, 1.5f)),
		Sphere(vec3(3, 0.2, -1), 0.2f, Material(LAMBERTIAN, vec3(0.7f, 0.5f, 0.3f), 0.3f)),
		Sphere(vec3(4, 0.2, 1), 0.2f, Material(LAMBERTIAN, vec3(0.9f, 0.3f, 0.3f), 0.5f)),
		Sphere(vec3(4, 0.2, -1), 0.2f, Material(LAMBERTIAN, vec3(0.5f, 0.3f, 0.3f), 0.3f)),
		Sphere(vec3(-2, 0.2, -1), 0.2f, Material(LAMBERTIAN, vec3(0.2f, 0.5f, 0.3f), 0.7f)),
		Sphere(vec3(-1, 0.2, 1), 0.2f, Material(LAMBERTIAN, vec3(0.2f, 0.3f, 0.8f), 0.3f)),
	};

	const int samples_per_pixel = 100;  // 每个像素的采样次数
	const int max_depth = 50;  // 最大递归深度
	std::vector<vec3> framebuffer(image_width * image_height);  // 用于存储图像像素的缓冲区

	// 多线程渲染
	int num_threads = std::thread::hardware_concurrency();  // 获取硬件并发线程数
	std::vector<std::thread> threads;
	int rows_per_thread = image_height / num_threads;  // 每个线程处理的行数

	for (int t = 0; t < num_threads; ++t) {
		int start = t * rows_per_thread;
		int end = (t == num_threads - 1) ? image_height : start + rows_per_thread;
		threads.emplace_back(renderScene, start, end, image_width, image_height, camera, std::cref(world), std::ref(framebuffer), max_depth, samples_per_pixel);
	}

	for (auto& thread : threads) {
		thread.join();  // 等待所有线程完成渲染
	}

	// 绘制像素到屏幕
	glDrawPixels(image_width, image_height, GL_RGB, GL_FLOAT, framebuffer.data());
	glFlush();  // 刷新显示
}

vec3 reflect(const vec3& incident, const vec3& normal) {
	return incident - 2.0f * dot(incident, normal) * normal;
}

// 计算折射光线的方向，返回 true 表示折射成功，false 表示折射不可行（全反射）
bool refract(const vec3& v, const vec3& n, float etai_over_etat, vec3& refracted) {
	vec3 unit_direction = normalize(v);
	float dt = dot(unit_direction, n);
	float discriminant = 1.0f - etai_over_etat * etai_over_etat * (1.0f - dt * dt);

	if (discriminant > 0.0f) {
		refracted = etai_over_etat * (unit_direction - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else {
		return false;  // 全反射
	}
}

// Schlick's approximation to Fresnel reflectance
float schlick(float cosine, float ref_idx) {
	float r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
	r0 = r0 * r0;
	return r0 + (1.0f - r0) * pow((1.0f - cosine), 5.0f);
}


bool Material::scatter(const Ray& r_in, const vec3& hit_point, const vec3& normal, vec3& attenuation, Ray& scattered) const {

	// 漫反射材质
	if (type == LAMBERTIAN) {
		vec3 target = hit_point + normal + random_in_unit_sphere(); // 漫反射目标点
		scattered = Ray(hit_point, target - hit_point); // 发射新的反射光线
		attenuation = albedo; // 漫反射衰减为材质颜色
		return true;
	}

	// 金属材质的反射
	else if (type == METAL) {
		vec3 reflected = reflect(normalize(r_in.direction), normal); // 计算反射光线
		scattered = Ray(hit_point, reflected + fuzz * random_in_unit_sphere()); // 模糊反射
		attenuation = albedo; // 金属材质的衰减通常为其颜色
		return (dot(scattered.direction, normal) > 0); // 确保反射光线与法线方向一致
	}

	// 电解质材质（折射玻璃球）
	else if (type == DIELECTRIC) {
		vec3 reflected = reflect(r_in.direction, normal); // 计算反射光线
		vec3 refracted;
		float reflect_prob;
		attenuation = vec3(1.0f, 1.0f, 1.0f); // 折射材质的衰减为常数

		// 计算折射率
		float etai_over_etat = (r_in.direction.y > 0) ? (1.0f / ref_idx) : ref_idx;

		// 计算光线与法线的夹角cos值
		float cos_theta = dot(r_in.direction, normal);
		cos_theta = fmin(cos_theta, 1.0f); // 限制cos_theta在[-1, 1]区间

		// 尝试折射，如果成功，计算反射的概率
		if (refract(r_in.direction, normal, etai_over_etat, refracted)) {
			reflect_prob = schlick(cos_theta, ref_idx); // 使用Schlick近似计算反射概率
		}
		else {
			scattered = Ray(hit_point + normal * 0.001f, reflected); // 如果折射失败，则使用反射
			return true;
		}

		// 根据反射概率决定使用反射光线还是折射光线
		if (random_float() < reflect_prob) {
			scattered = Ray(hit_point + normal * 0.001f, reflected); // 使用反射光线
		}
		else {
			scattered = Ray(hit_point + normal * 0.001f, refracted); // 使用折射光线
		}
		return true;
	}

	return false;
}








MyGLWidget::MyGLWidget(QWidget* parent)
	:QOpenGLWidget(parent)
{
}

MyGLWidget::~MyGLWidget()
{
	delete[] render_buffer;
	delete[] temp_render_buffer;
	delete[] temp_z_buffer;
	delete[] z_buffer;
}

void MyGLWidget::resizeBuffer(int newW, int newH) {
	delete[] render_buffer;
	delete[] temp_render_buffer;
	delete[] temp_z_buffer;
	delete[] z_buffer;
	WindowSizeW = newW;
	WindowSizeH = newH;
	render_buffer = new vec3[WindowSizeH * WindowSizeW];
	temp_render_buffer = new vec3[WindowSizeH * WindowSizeW];
	temp_z_buffer = new float[WindowSizeH * WindowSizeW];
	z_buffer = new float[WindowSizeH * WindowSizeW];
}


void MyGLWidget::initializeGL()
{
	WindowSizeW = width();
	WindowSizeH = height();
	glViewport(0, 0, WindowSizeW, WindowSizeH);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glDisable(GL_DEPTH_TEST);
	offset = vec2(WindowSizeH / 2, WindowSizeW / 2);
	// 对定义的数组初始化
	render_buffer = new vec3[WindowSizeH * WindowSizeW];
	temp_render_buffer = new vec3[WindowSizeH * WindowSizeW];
	temp_z_buffer = new float[WindowSizeH * WindowSizeW];
	z_buffer = new float[WindowSizeH * WindowSizeW];
	for (int i = 0; i < WindowSizeH * WindowSizeW; i++) {
		render_buffer[i] = vec3(0, 0, 0);
		temp_render_buffer[i] = vec3(0, 0, 0);
		temp_z_buffer[i] = MAX_Z_BUFFER;
		z_buffer[i] = MAX_Z_BUFFER;
	}
}

void MyGLWidget::keyPressEvent(QKeyEvent* e) {
	float step_size = 0.5f;
	switch (e->key()) {
	case Qt::Key_0:
		scene_id = 0;
		update();
		break;
	case Qt::Key_F:
		if (isFullScreen()) {
			showNormal(); // 切换回普通窗口
		}
		else {
			showFullScreen(); // 切换到全屏
		}
		break;
	case Qt::Key_W:
		camera_position += vec3(0, 0, -step_size); // 向前
		update();
		break;
	case Qt::Key_S:
		camera_position += vec3(0, 0, step_size);  // 向后
		update();
		break;
	case Qt::Key_A:
		camera_position += vec3(-step_size, 0, 0); // 向左
		update();
		break;
	case Qt::Key_D:
		camera_position += vec3(step_size, 0, 0);  // 向右
		update();
		break;
	case Qt::Key_Q:
		camera_position += vec3(0, step_size, 0);  // 向上
		update();
		break;
	case Qt::Key_E:
		camera_position += vec3(0, -step_size, 0); // 向下
		update();
		break;
	case Qt::Key_Up:
		focus_dist += 0.5f; // 焦距增大
		update();
		break;
	case Qt::Key_Down:
		focus_dist -= 0.5f; // 焦距减少
		update();
		break;
	case Qt::Key_Right:
		aperture += 0.002f; // 模糊程度增加
		update();
		break;
	case Qt::Key_Left:
		aperture -= 0.002f; // 模糊程度减少
		update();
		break;
	default:
		QWidget::keyPressEvent(e); // 调用默认键盘处理
	}

	
}


void MyGLWidget::clearBuffer(vec3* now_buffer) {
	for (int i = 0; i < WindowSizeH * WindowSizeW; i++) {
		now_buffer[i] = vec3(0, 0, 0);
	}
}

void MyGLWidget::clearBuffer(int* now_buffer) {
	memset(now_buffer, 0, WindowSizeW * WindowSizeH * sizeof(int));
}


void MyGLWidget::clearZBuffer(float* now_buffer) {
	std::fill(now_buffer, now_buffer + WindowSizeW * WindowSizeH, MAX_Z_BUFFER);
}


void MyGLWidget::resizeGL(int w, int h) {
	WindowSizeW = w; // 更新窗口宽度
	WindowSizeH = h; // 更新窗口高度
	glViewport(0, 0, w, h); // 设置新的视口大小
	resizeBuffer(w, h); // 调整缓冲区大小
	clearBuffer(render_buffer); // 清空缓冲区
	clearZBuffer(z_buffer);     // 清空深度缓冲区
}








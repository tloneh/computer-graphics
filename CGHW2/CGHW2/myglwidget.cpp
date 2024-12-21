#include "myglwidget.h"
#include <GL/glew.h>
#include <algorithm>

MyGLWidget::MyGLWidget(QWidget *parent)
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
	render_buffer = new vec3[WindowSizeH*WindowSizeW];
	temp_render_buffer = new vec3[WindowSizeH*WindowSizeW];
	temp_z_buffer = new float[WindowSizeH*WindowSizeW];
	z_buffer = new float[WindowSizeH*WindowSizeW];
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
	render_buffer = new vec3[WindowSizeH*WindowSizeW];
	temp_render_buffer = new vec3[WindowSizeH*WindowSizeW];
	temp_z_buffer = new float[WindowSizeH*WindowSizeW];
	z_buffer = new float[WindowSizeH*WindowSizeW];
	for (int i = 0; i < WindowSizeH*WindowSizeW; i++) {
		render_buffer[i] = vec3(0, 0, 0);
		temp_render_buffer[i] = vec3(0, 0, 0);
		temp_z_buffer[i] = MAX_Z_BUFFER;			
		z_buffer[i] = MAX_Z_BUFFER;
	}
}

void MyGLWidget::keyPressEvent(QKeyEvent *e) {
	
	switch (e->key()) {
		case Qt::Key_0: scene_id = 0;update(); break;
		case Qt::Key_1: scene_id = 1;update(); break;
		case Qt::Key_9: degree += 35;update(); break;
	}
}

void MyGLWidget::paintGL()
{
	switch (scene_id) {
		case 0:scene_0(); break;
		case 1:scene_1(); break;
	}
}
void MyGLWidget::clearBuffer(vec3* now_buffer) {
	for (int i = 0; i < WindowSizeH*WindowSizeW; i++) {
		now_buffer[i] = vec3(0,0,0);
	}
}

void MyGLWidget::clearBuffer(int* now_buffer) {
	memset(now_buffer, 0, WindowSizeW * WindowSizeH * sizeof(int));
}


void MyGLWidget::clearZBuffer(float* now_buffer) {
	std::fill(now_buffer,now_buffer+WindowSizeW * WindowSizeH, MAX_Z_BUFFER);
}


// 窗口大小变动后，需要重新生成render_buffer等数组
void MyGLWidget::resizeGL(int w, int h)
{
	resizeBuffer(w, h);
	offset = vec2(WindowSizeH / 2, WindowSizeW / 2);
	clearBuffer(render_buffer);
}

//void MyGLWidget::scene_0()
//{
//	// 选择要加载的model
//	objModel.loadModel("./objs/singleTriangle.obj");
//
//	// 自主设置变换矩阵
//	camPosition = vec3(100 * sin(degree * 3.14 / 180.0) + objModel.centralPoint.y, 100 * cos(degree * 3.14 / 180.0) + objModel.centralPoint.x, 10+ objModel.centralPoint.z);
//	camLookAt = objModel.centralPoint;     // 例如，看向物体中心
//	camUp = vec3(0, 1, 0);         // 上方向向量
//	projMatrix = glm::perspective(radians(20.0f), 1.0f, 0.1f, 2000.0f);
//
//	// 单一点光源，可以改为数组实现多光源
//	lightPosition = objModel.centralPoint + vec3(0,100,100);
//	clearBuffer(render_buffer);
//	clearZBuffer(z_buffer);
//	for (int i = 0; i < objModel.triangleCount; i++) {
//		Triangle nowTriangle = objModel.getTriangleByID(i);
//		drawTriangle(nowTriangle);
//	}
//	glClear(GL_COLOR_BUFFER_BIT);
//	renderWithTexture(render_buffer,WindowSizeH,WindowSizeW);
//}
void MyGLWidget::scene_0()
{
	// 选择要加载的model
	objModel.loadModel("./objs/singleTriangle.obj");

	// 自主设置变换矩阵
	camPosition = vec3(100 * sin(degree * 3.14 / 180.0) + objModel.centralPoint.y, 100 * cos(degree * 3.14 / 180.0) + objModel.centralPoint.x, 10 + objModel.centralPoint.z);
	camLookAt = objModel.centralPoint;     // 例如，看向物体中心
	camUp = vec3(0, 1, 0);         // 上方向向量
	projMatrix = glm::perspective(radians(20.0f), 1.0f, 0.1f, 2000.0f);
	lightStrength = 100.0f;
	// 设置两个光源
	lightPosition[0] = objModel.centralPoint + vec3(0, 100, 100);  // 第一个光源位置
	lightColor[0] = vec3(0.8f, 0.3f, 0.0f) * lightStrength;  // 光源1

	lightPosition[1] = objModel.centralPoint + vec3(100, 100, 100);  // 第二个光源位置
	lightColor[1] = vec3(0.0f, 0.6f, 0.5f) * lightStrength;  // 光源2

	clearBuffer(render_buffer);
	clearZBuffer(z_buffer);
	for (int i = 0; i < objModel.triangleCount; i++) {
		Triangle nowTriangle = objModel.getTriangleByID(i);
		drawTriangle(nowTriangle);
	}
	glClear(GL_COLOR_BUFFER_BIT);
	renderWithTexture(render_buffer, WindowSizeH, WindowSizeW);
}

void MyGLWidget::scene_1()
{
	// 选择要加载的model
	//objModel.loadModel("./objs/teapot_600.obj");
	//objModel.loadModel("./objs/teapot_8000.obj");
	//objModel.loadModel("./objs/rock.obj");
	objModel.loadModel("./objs/cube.obj");
	//objModel.loadModel("./objs/singleTriangle.obj");
	
	// 自主设置变换矩阵
	camPosition = vec3(100 * sin(degree * 3.14 / 180.0) + objModel.centralPoint.y, 100 * cos(degree * 3.14 / 180.0) + objModel.centralPoint.x, 10+ objModel.centralPoint.z);
	camLookAt = objModel.centralPoint;     // 例如，看向物体中心
	camUp = vec3(0, 1, 0);         // 上方向向量
	projMatrix = glm::perspective(radians(20.0f), 1.0f, 0.1f, 2000.0f);
	// 单一点光源，可以改为数组实现多光源
	// lightPosition[0] = objModel.centralPoint + vec3(0, 100, 100);
	// 设置两个光源
	lightStrength = 1000.0f;
	lightPosition[0] = objModel.centralPoint + vec3(100, 100, -100);  // 第一个光源位置
	lightColor[0] = vec3(0.0f, 0.0f, 1.0f) * lightStrength;  // 光源1

	lightPosition[1] = objModel.centralPoint + vec3(-100, 100, -100);  // 第二个光源位置
	lightColor[1] = vec3(0.0f, 1.0f, 1.0f) * lightStrength;  // 光源2
	clearBuffer(render_buffer);
	clearZBuffer(z_buffer);
	for (int i = 0; i < objModel.triangleCount; i++) {
		Triangle nowTriangle = objModel.getTriangleByID(i);
		drawTriangle(nowTriangle);
	}
	glClear(GL_COLOR_BUFFER_BIT);
	renderWithTexture(render_buffer, WindowSizeH, WindowSizeW);
}


void MyGLWidget::drawTriangle(Triangle triangle) {
	// 三维顶点映射到二维平面
	vec3* vertices = triangle.triangleVertices;
	vec3* normals = triangle.triangleNormals;
	FragmentAttr transformedVertices[3];
	clearBuffer(this->temp_render_buffer);
	clearZBuffer(this->temp_z_buffer);
	mat4 viewMatrix = glm::lookAt(camPosition, camLookAt, camUp);

    for (int i = 0; i < 3; ++i) {
		vec4 ver_mv = viewMatrix * vec4(vertices[i], 1.0f);
		float nowz = glm::length(camPosition - vec3(ver_mv));
		vec4 ver_proj = projMatrix * ver_mv;
		transformedVertices[i].x = ver_proj.x + offset.x;
		transformedVertices[i].y = ver_proj.y + offset.y;
		transformedVertices[i].z = nowz;
		transformedVertices[i].pos_mv = ver_mv;  
		mat3 normalMatrix = mat3(viewMatrix);
		vec3 normal_mv = normalMatrix * normals[i];
		transformedVertices[i].normal = normal_mv;
    }

	// 将当前三角形渲染在temp_buffer中
		
	// HomeWork: 1、绘制三角形三边
	bresenham(transformedVertices[0], transformedVertices[1], 1);
    bresenham(transformedVertices[1], transformedVertices[2], 2);
    bresenham(transformedVertices[2], transformedVertices[0], 3);
	// DDA
	/*DDA(transformedVertices[0], transformedVertices[1], 1);
	DDA(transformedVertices[1], transformedVertices[2], 2);
	DDA(transformedVertices[2], transformedVertices[0], 3);*/
    // HomeWork: 2: 用edge-walking填充三角形内部到temp_buffer中
    int firstChangeLine = edge_walking(transformedVertices);

	// 合并temp_buffer 到 render_buffer, 深度测试
	// 从firstChangeLine开始遍历，可以稍快
	for(int h = firstChangeLine; h < WindowSizeH ; h++){
		auto render_row = &render_buffer[h * WindowSizeW];
		auto temp_render_row = &temp_render_buffer[h * WindowSizeW];
		auto z_buffer_row = &z_buffer[h*WindowSizeW];
		auto temp_z_buffer_row = &temp_z_buffer[h*WindowSizeW];
		for (int i = 0 ; i < WindowSizeW ; i++){
			if (z_buffer_row[i] < temp_z_buffer_row[i])
				continue;
			else
			{
				z_buffer_row[i] = temp_z_buffer_row[i];
				render_row[i] = temp_render_row[i];
			}
		}

	}
}

void MyGLWidget::DDA(FragmentAttr& start, FragmentAttr& end, int id) {
	int x1 = start.x;
	int y1 = start.y;
	int x2 = end.x;
	int y2 = end.y;

	int dx = x2 - x1;
	int dy = y2 - y1;
	int steps = std::max(abs(dx), abs(dy));  // 计算步数
	float xIncrement = dx / float(steps);    // 每步增量
	float yIncrement = dy / float(steps);    // 每步增量

	float x = float(x1);
	float y = float(y1);

	for (int i = 0; i <= steps; i++) {
		// 如果像素在画布内，填充颜色
		int px = static_cast<int>(x);
		int py = static_cast<int>(y);

		// 防止超出画布的边界
		if (px >= 0 && px < WindowSizeW && py >= 0 && py < WindowSizeH) {
			// 在这里可以插值颜色、光照等信息
			temp_render_buffer[py * WindowSizeW + px] = PhoneShading(start);
			//temp_render_buffer[py * WindowSizeW + px] = vec3(0.0f, 1.0f, 0.0f);
			// 如果需要插值深度，可以在这里更新 temp_z_buffer
			temp_z_buffer[py * WindowSizeW + px] = (start.z + end.z) / 2.0f;
		}

		// 递增坐标
		x += xIncrement;
		y += yIncrement;
	}
}


int MyGLWidget::edge_walking(FragmentAttr transformedVertices[3]) { //光照强度1000.0f
	int firstChangeLine = WindowSizeH;

	// 遍历屏幕上的每一行（y轴）
	for (int y = 0; y < WindowSizeH; y++) {
		int xStart = WindowSizeW, xEnd = -1;

		// 计算y行与三角形每条边的交点（通过线段与扫描线的交点）
		for (int i = 0; i < 3; ++i) {
			int next = (i + 1) % 3;  // 下一个顶点的索引
			FragmentAttr v1 = transformedVertices[i];
			FragmentAttr v2 = transformedVertices[next];

			// 检查是否有交点（y值在这两点之间）
			if ((v1.y <= y && v2.y > y) || (v1.y > y && v2.y <= y)) {
				// 计算交点的x坐标
				float t = float(y - v1.y) / float(v2.y - v1.y);
				int xIntersection = int(v1.x + t * (v2.x - v1.x));

				// 更新扫描线的xStart和xEnd
				if (v1.y <= v2.y) {
					xStart = std::min(xStart, xIntersection);
				}
				else {
					xEnd = std::max(xEnd, xIntersection);
				}
			}
		}

		// 如果有效的x范围，开始填充像素
		if (xStart <= xEnd) {
			for (int x = xStart; x <=xEnd; x++) {
				FragmentAttr pixel;

				// 计算插值系数
				float alpha = float(x - xStart) / float(xEnd - xStart);
				float beta = 1.0f - alpha;
				float gamma = 1.0f - alpha - beta;

				// 使用Barycentric插值计算位置、法线和深度值
				pixel.pos_mv = alpha * transformedVertices[0].pos_mv +
					beta * transformedVertices[1].pos_mv +
					gamma * transformedVertices[2].pos_mv;

				pixel.normal = alpha * transformedVertices[0].normal +
					beta * transformedVertices[1].normal +
					gamma * transformedVertices[2].normal;

				// 插值深度
				pixel.pos_mv.z = alpha * transformedVertices[0].pos_mv.z +
					beta * transformedVertices[1].pos_mv.z +
					gamma * transformedVertices[2].pos_mv.z;

				// 着色：计算光照
				//vec3 color = PhoneShading(pixel);  // 计算当前像素的最终颜色
				vec3 color = BlinnPhongShading(pixel);

				// 深度测试：如果当前像素的深度小于已存储深度，则更新颜色和深度缓冲
				int index = y * WindowSizeW + x;
				if (pixel.pos_mv.z < temp_z_buffer[index]) {
					temp_render_buffer[index] = color;
					temp_z_buffer[index] = pixel.pos_mv.z;
				}
			}

			// 更新 firstChangeLine 为第一个有效的行
			firstChangeLine = std::min(firstChangeLine, y);
		}
	}

	return firstChangeLine;
}


vec3 MyGLWidget::PhoneShading(FragmentAttr& nowPixelResult) {
	// 环境光强度
	float ambient = 0.1f;

	// 初始化最终颜色为环境光
	vec3 finalColor = vec3(0.1f, 0.0713f, 0.0757f) * 4.0f;  // 淡粉色
	//vec3 finalColor = vec3(ambient, ambient, ambient);

	// 遍历每个光源
	for (int i = 0; i < 2; ++i) {  // 现在假设有两个光源
		// 获取光源到当前像素的方向
		vec3 lightDir = glm::normalize(lightPosition[i] - vec3(nowPixelResult.pos_mv));  // 将pos_mv转换为vec3

		// 计算法向量与光源方向的点积，模拟漫反射光照
		float diff = std::max(glm::dot(nowPixelResult.normal, lightDir), 0.0f);

		// 高光反射
		vec3 viewDir = glm::normalize(camPosition - vec3(nowPixelResult.pos_mv));
		vec3 halfDir = glm::normalize(lightDir + viewDir);
		float spec = glm::pow(glm::max(glm::dot(nowPixelResult.normal, halfDir), 0.0f), 16);

		// 计算当前光源的光照贡献
		vec3 lightContrib = lightColor[i] * (diff + spec);  // 光源颜色和光照贡献加权

		// 计算光源衰减
		float distance = glm::length(lightPosition[i] - vec3(nowPixelResult.pos_mv));
		float attenuation = 1.0f / (1.0f + 0.01f * distance * distance);  // 衰减公式
		lightContrib *= attenuation;

		// 将当前光源的贡献累加到最终颜色
		finalColor += lightContrib;
	}

	return finalColor;  // 返回最终计算的颜色
}



void MyGLWidget::bresenham(FragmentAttr& start, FragmentAttr& end, int id) {
	// 根据起点、终点，计算当前边在画布上的像素
	int x1 = start.x, y1 = start.y;
	int x2 = end.x, y2 = end.y;
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int sx = (x1 < x2) ? 1 : -1;
	int sy = (y1 < y2) ? 1 : -1;
	int err = dx - dy;

	// 确保边界检查，防止访问越界
	auto setColor = [this](int x, int y, const vec3& color) {
		if (x >= 0 && x < WindowSizeW && y >= 0 && y < WindowSizeH) {
			temp_render_buffer[y * WindowSizeW + x] = color;
		}
		};

	// 使用Bresenham算法遍历两点之间的所有像素
	while (true) {
		// 使用start和end之间的插值来计算颜色
		float alpha = float(x1 - start.x) / float(x2 - start.x + 1);
		float beta = 1.0f - alpha;
		FragmentAttr pixel;
		pixel.pos_mv = alpha * start.pos_mv + beta * end.pos_mv;
		pixel.normal = alpha * start.normal + beta * end.normal;

		// 调用PhoneShading计算最终颜色
		vec3 color = PhoneShading(pixel);
		//vec3 color = vec3(0.0f, 0.0f, 0.0f);

		// 将颜色设置到渲染缓冲区
		setColor(x1, y1, color);

		// 检查是否已到达终点
		if (x1 == x2 && y1 == y2) break;

		int e2 = err * 2;
		if (e2 > -dy) {
			err -= dy;
			x1 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y1 += sy;
		}
	}
}

vec3 MyGLWidget::GouraudShading(FragmentAttr& vertex) {
	// 环境光强度
	float ambient = 5.0f;

	// 初始化最终颜色为环境光
	vec3 finalColor = vec3(0.0f);

	// 遍历每个光源，计算每个光源对顶点的贡献
	for (int i = 0; i < 2; ++i) {  // 假设有两个光源
		// 获取光源到当前顶点的方向
		vec3 lightDir = glm::normalize(lightPosition[i] - vec3(vertex.pos_mv));

		// 计算法向量与光源方向的点积，模拟漫反射光照
		float diff = std::max(glm::dot(vertex.normal, lightDir), 0.0f);

		// 高光反射
		vec3 viewDir = glm::normalize(camPosition - vec3(vertex.pos_mv));
		vec3 halfDir = glm::normalize(lightDir + viewDir);
		float spec = glm::pow(glm::max(glm::dot(vertex.normal, halfDir), 0.0f), 16);

		// 计算当前光源的光照贡献
		vec3 lightContrib = lightColor[i] * (diff + spec);  // 光源颜色和光照贡献加权

		// 将当前光源的贡献累加到最终颜色
		finalColor += lightContrib;
	}

	// 环境光的影响
	finalColor += ambient * vec3(0.1f, 0.1f, 0.1f);  // 假设一个小的环境光强度

	return finalColor;
}
//int MyGLWidget::edge_walking(FragmentAttr transformedVertices[3]) {
//	int firstChangeLine = WindowSizeH;
//
//	// 计算每个顶点的颜色，考虑多个光源的影响
//	vec3 colors[3];
//	for (int i = 0; i < 3; ++i) {
//		colors[i] = GouraudShading(transformedVertices[i]);  // 每个顶点的颜色计算
//	}
//
//	// 遍历屏幕上的每一行（y轴）
//	for (int y = 0; y < WindowSizeH; y++) {
//		int xStart = WindowSizeW, xEnd = -1;
//
//		// 计算y行与三角形每条边的交点（通过线段与扫描线的交点）
//		for (int i = 0; i < 3; ++i) {
//			int next = (i + 1) % 3;  // 下一个顶点的索引
//			FragmentAttr v1 = transformedVertices[i];
//			FragmentAttr v2 = transformedVertices[next];
//
//			// 检查是否有交点（y值在这两点之间）
//			if ((v1.y <= y && v2.y > y) || (v1.y > y && v2.y <= y)) {
//				// 计算交点的x坐标
//				float t = float(y - v1.y) / float(v2.y - v1.y);
//				int xIntersection = int(v1.x + t * (v2.x - v1.x));
//
//				// 更新扫描线的xStart和xEnd
//				if (v1.y <= v2.y) {
//					xStart = std::min(xStart, xIntersection);
//				}
//				else {
//					xEnd = std::max(xEnd, xIntersection);
//				}
//			}
//		}
//
//		// 如果有效的x范围，开始填充像素
//		if (xStart <= xEnd) {
//			for (int x = xStart; x <= xEnd; x++) {
//				FragmentAttr pixel;
//
//				// 计算插值系数
//				float alpha = float(x - xStart) / float(xEnd - xStart);
//				float beta = 1.0f - alpha;
//				float gamma = 1.0f - alpha - beta;
//
//				// 使用Barycentric插值计算颜色
//				vec3 color = alpha * colors[0] + beta * colors[1] + gamma * colors[2];
//
//				// 计算光照并更新颜色
//				temp_render_buffer[y * WindowSizeW + x] = color;
//
//				// 插值深度
//				pixel.pos_mv.z = alpha * transformedVertices[0].pos_mv.z +
//					beta * transformedVertices[1].pos_mv.z +
//					gamma * transformedVertices[2].pos_mv.z;
//
//				// 更新深度缓冲
//				int index = y * WindowSizeW + x;
//				if (pixel.pos_mv.z < temp_z_buffer[index]) {
//					temp_z_buffer[index] = pixel.pos_mv.z;
//				}
//			}
//
//			// 更新 firstChangeLine 为第一个有效的行
//			firstChangeLine = std::min(firstChangeLine, y);
//		}
//	}
//
//	return firstChangeLine;
//}

vec3 MyGLWidget::BlinnPhongShading(FragmentAttr& nowPixelResult) {
	// 环境光强度
	float ambient = 0.1f;

	// 初始化最终颜色为环境光
	vec3 finalColor = vec3(0.1f, 0.0713f, 0.0757f) * 4.0f;  // 淡粉色
	//vec3 finalColor = vec3(ambient, ambient, ambient);

	// 遍历每个光源
	for (int i = 0; i < 2; ++i) {  // 有2个光源
		// 获取光源到当前像素的方向
		vec3 lightDir = glm::normalize(lightPosition[i] - vec3(nowPixelResult.pos_mv));

		// 计算法向量与光源方向的点积，模拟漫反射光照
		float diff = glm::max(glm::dot(nowPixelResult.normal, lightDir), 0.0f);
		vec3 diffuse = diff * lightColor[i];

		// 计算视角方向
		vec3 viewDir = glm::normalize(camPosition - vec3(nowPixelResult.pos_mv));

		// 计算半程向量 H
		vec3 halfwayDir = glm::normalize(lightDir + viewDir);

		// 镜面反射光照（Blinn-Phong 模型）
		float spec = glm::pow(glm::max(glm::dot(nowPixelResult.normal, halfwayDir), 0.0f), 16);  // n=16，控制高光反射的锐度
		vec3 specular = spec * lightColor[i];

		// 计算光源衰减
		float distance = glm::length(lightPosition[i] - vec3(nowPixelResult.pos_mv));
		float attenuation = 1.0f / (1.0f + 0.1f * distance + 0.1f * distance * distance);  // 可以根据需要调整衰减公式

		// 计算光源的贡献
		vec3 lightContrib = (diffuse + specular) * attenuation;

		// 将当前光源的贡献累加到最终颜色
		finalColor += lightContrib;
	}

	return finalColor;  // 返回最终计算的颜色
}

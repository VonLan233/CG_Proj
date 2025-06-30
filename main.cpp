#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// 全局变量用于鼠标控制视角
float rotateX = -20.0f;  // 初始向下倾斜一点
float rotateY = 0.0f;
int lastMouseX, lastMouseY;
int mousePressed = 0;
bool redShow = true, yellowShow = true, LosiShow = true, baseShow = true, ShoeFace = true;

// 动画控制变量
static float redPluginOffsetX = 150.0f;      // 红色插件X轴偏移
static float yellowPluginOffsetX = -150.0f;   // 黄色插件X轴偏移
static float screwOffsetY = -30.0f;          // 螺丝钉Y轴偏移
static bool screwDirection = true;         // 螺丝钉方向控制 (true=拔出, false=插入)
static float shoeFaceOffsetY = 30.0f;         // 鞋面Y轴偏移
static bool shoeFaceDirection = true;        // 鞋面方向控制 (true=向上, false=向下)

// 添加纹理变量
GLuint shoeTexture;                  // 鞋面纹理
GLuint underTexture;                 // 鞋底纹理
GLuint bindTexture;                  // 连接部分纹理
GLuint shoePrintTexture;             // 鞋印纹理
GLuint shoeFaceTexture;              // 鞋面纹理
GLuint bangTexture;                  // Bang纹理
GLuint basicTexture;                 // Basic纹理
GLuint luosiTexture;                 // 螺丝纹理
GLuint magicTexture;                 // Magic纹理

// 添加纹理加载函数
static void loadTextures()
{
    glEnable(GL_TEXTURE_2D);        // 启用2D纹理
    int width = 0, height = 0, channels = 0;

    // 加载鞋面纹理
    const char *filename = "C:/Users/LJT/Desktop/DMT2309228/Upper.png";
    stbi_uc *image = stbi_load(filename, &width, &height, &channels, 0);
    if (image) {
        printf("鞋面纹理加载成功: %dx%d, channels: %d\n", width, height, channels);
        glGenTextures(1, &shoeTexture);
        glBindTexture(GL_TEXTURE_2D, shoeTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        GLenum format = GL_RGB;
        if (channels == 1) format = GL_LUMINANCE;
        else if (channels == 3) format = GL_RGB;
        else if (channels == 4) format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
        stbi_image_free(image);
    } else {
        printf("鞋面纹理加载失败: %s\n", filename);
    }

    // 加载鞋底纹理
    const char *underFilename = "C:/Users/LJT/Desktop/DMT2309228/Under.png";
    stbi_uc *underImage = stbi_load(underFilename, &width, &height, &channels, 0);
    if (underImage) {
        printf("鞋底纹理加载成功: %dx%d, channels: %d\n", width, height, channels);
        glGenTextures(1, &underTexture);
        glBindTexture(GL_TEXTURE_2D, underTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        GLenum format = GL_RGB;
        if (channels == 1) format = GL_LUMINANCE;
        else if (channels == 3) format = GL_RGB;
        else if (channels == 4) format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, underImage);
        stbi_image_free(underImage);
    } else {
        printf("鞋底纹理加载失败: %s\n", underFilename);
    }

    // 加载连接部分纹理
    const char *bindFilename = "C:/Users/LJT/Desktop/DMT2309228/Bind.png";
    stbi_uc *bindImage = stbi_load(bindFilename, &width, &height, &channels, 0);
    if (bindImage) {
        printf("连接部分纹理加载成功: %dx%d, channels: %d\n", width, height, channels);
        glGenTextures(1, &bindTexture);
        glBindTexture(GL_TEXTURE_2D, bindTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        GLenum format = GL_RGB;
        if (channels == 1) format = GL_LUMINANCE;
        else if (channels == 3) format = GL_RGB;
        else if (channels == 4) format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, bindImage);
        stbi_image_free(bindImage);
    } else {
        printf("连接部分纹理加载失败: %s\n", bindFilename);
    }

    // 加载鞋印纹理
    const char *shoePrintFilename = "C:/Users/LJT/Desktop/DMT2309228/ShoePrint.png";
    stbi_uc *shoePrintImage = stbi_load(shoePrintFilename, &width, &height, &channels, 0);
    if (shoePrintImage) {
        printf("鞋印纹理加载成功: %dx%d, channels: %d\n", width, height, channels);
        glGenTextures(1, &shoePrintTexture);
        glBindTexture(GL_TEXTURE_2D, shoePrintTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        GLenum format = GL_RGB;
        if (channels == 1) format = GL_LUMINANCE;
        else if (channels == 3) format = GL_RGB;
        else if (channels == 4) format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, shoePrintImage);
        stbi_image_free(shoePrintImage);

    } else {
        printf("鞋印纹理加载失败: %s\n", shoePrintFilename);
    }

    // 加载鞋面纹理
    const char *shoeFaceFilename = "C:/Users/LJT/Desktop/DMT2309228/ShoeFace.png";
    stbi_uc *shoeFaceImage = stbi_load(shoeFaceFilename, &width, &height, &channels, 0);
    if (shoeFaceImage) {
        printf("鞋面纹理加载成功: %dx%d, channels: %d\n", width, height, channels);
        glGenTextures(1, &shoeFaceTexture);
        glBindTexture(GL_TEXTURE_2D, shoeFaceTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        GLenum format = GL_RGB;
        if (channels == 1) format = GL_LUMINANCE;
        else if (channels == 3) format = GL_RGB;
        else if (channels == 4) format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, shoeFaceImage);
        stbi_image_free(shoeFaceImage);
        printf("鞋面纹理加载完成\n");
    } else {
        printf("鞋面纹理加载失败: %s\n", shoeFaceFilename);
    }
    const char *bangFilename = "C:/Users/LJT/Desktop/DMT2309228/Bang.png";
    stbi_uc *bangImage = stbi_load(bangFilename, &width, &height, &channels, 0);
    if (bangImage) {
        printf("Bang纹理加载成功: %dx%d, channels: %d\n", width, height, channels);
        glGenTextures(1, &bangTexture);
        glBindTexture(GL_TEXTURE_2D, bangTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        GLenum format = GL_RGB;
        if (channels == 1) format = GL_LUMINANCE;
        else if (channels == 3) format = GL_RGB;
        else if (channels == 4) format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, bangImage);
        stbi_image_free(bangImage);
        printf("Bang纹理加载完成\n");
    } else {
        printf("Bang纹理加载失败: %s\n", bangFilename);
    }
    const char *basicFilename = "C:/Users/LJT/Desktop/DMT2309228/Basic.png";
    stbi_uc *basicImage = stbi_load(basicFilename, &width, &height, &channels, 0);
    if (basicImage) {
        printf("Basic纹理加载成功: %dx%d, channels: %d\n", width, height, channels);
        glGenTextures(1, &basicTexture);
        glBindTexture(GL_TEXTURE_2D, basicTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        GLenum format = GL_RGB;
        if (channels == 1) format = GL_LUMINANCE;
        else if (channels == 3) format = GL_RGB;
        else if (channels == 4) format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, basicImage);
        stbi_image_free(basicImage);
        printf("Basic纹理加载完成\n");
    } else {
        printf("Basic纹理加载失败: %s\n", basicFilename);
    }
    const char *luosiFilename = "C:/Users/LJT/Desktop/DMT2309228/Luosi.png";
    stbi_uc *luosiImage = stbi_load(luosiFilename, &width, &height, &channels, 0);
    if (luosiImage) {
        printf("螺丝纹理加载成功: %dx%d, channels: %d\n", width, height, channels);
        glGenTextures(1, &luosiTexture);
        glBindTexture(GL_TEXTURE_2D, luosiTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        GLenum format = GL_RGB;
        if (channels == 1) format = GL_LUMINANCE;
        else if (channels == 3) format = GL_RGB;
        else if (channels == 4) format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, luosiImage);
        stbi_image_free(luosiImage);
        printf("螺丝纹理加载完成\n");
    } else {
        printf("螺丝纹理加载失败: %s\n", luosiFilename);
    }
    const char *magicFilename = "C:/Users/LJT/Desktop/DMT2309228/Magic.png";
    stbi_uc *magicImage = stbi_load(magicFilename, &width, &height, &channels, 0);
    if (magicImage) {
        printf("Magic纹理加载成功: %dx%d, channels: %d\n", width, height, channels);
        glGenTextures(1, &magicTexture);
        glBindTexture(GL_TEXTURE_2D, magicTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        GLenum format = GL_RGB;
        if (channels == 1) format = GL_LUMINANCE;
        else if (channels == 3) format = GL_RGB;
        else if (channels == 4) format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, magicImage);
        stbi_image_free(magicImage);
        printf("Magic纹理加载完成\n");
    } else {
        printf("Magic纹理加载失败: %s\n", magicFilename);
    }
}

// 修改椭圆柱体绘制函数，添加纹理坐标
void drawEllipticCylinderWithTexture(float centerX, float centerY, float centerZ,
                         float radiusX, float radiusZ, float height, int slices) {

    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);

    // 绘制侧面（带纹理坐标）
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * M_PI * i / slices;
        float x = radiusX * cos(angle);
        float z = radiusZ * sin(angle);

        // 计算法向量（椭圆柱侧面）
        float nx = cos(angle) / radiusX;
        float nz = sin(angle) / radiusZ;
        float len = sqrt(nx*nx + nz*nz);
        glNormal3f(nx/len, 0.0f, nz/len);

        // 纹理坐标
        float u = (float)i / slices;  // 水平方向纹理坐标

        // 底面点
        glTexCoord2f(u, 0.0f);
        glVertex3f(x, 0.0f, z);
        // 顶面点
        glTexCoord2f(u, 1.0f);
        glVertex3f(x, height, z);
    }
    glEnd();

    // 绘制底面（带纹理坐标）
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, -1.0f, 0.0f);  // 向下的法向量
    glTexCoord2f(0.5f, 0.5f);  // 中心纹理坐标
    glVertex3f(0.0f, 0.0f, 0.0f);   // 中心点
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * M_PI * i / slices;
        float x = radiusX * cos(angle);
        float z = radiusZ * sin(angle);

        // 圆形纹理坐标映射
        float u = 0.5f + 0.5f * cos(angle);
        float v = 0.5f + 0.5f * sin(angle);
        glTexCoord2f(u, v);
        glVertex3f(x, 0.0f, z);
    }
    glEnd();

    // 绘制顶面（带纹理坐标）
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 1.0f, 0.0f);   // 向上的法向量
    glTexCoord2f(0.5f, 0.5f);  // 中心纹理坐标
    glVertex3f(0.0f, height, 0.0f); // 中心点
    for (int i = slices; i >= 0; i--) {  // 逆时针顺序
        float angle = 2.0f * M_PI * i / slices;
        float x = radiusX * cos(angle);
        float z = radiusZ * sin(angle);

        // 圆形纹理坐标映射
        float u = 0.5f + 0.5f * cos(angle);
        float v = 0.5f + 0.5f * sin(angle);
        glTexCoord2f(u, v);
        glVertex3f(x, height, z);
    }
    glEnd();

    glPopMatrix();
}

// 绘制实心椭圆柱体
void drawEllipticCylinder(float centerX, float centerY, float centerZ,
                         float radiusX, float radiusZ, float height, int slices) {

    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);

    // 绘制侧面
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * M_PI * i / slices;
        float x = radiusX * cos(angle);
        float z = radiusZ * sin(angle);

        // 计算法向量（椭圆柱侧面）
        float nx = cos(angle) / radiusX;
        float nz = sin(angle) / radiusZ;
        float len = sqrt(nx*nx + nz*nz);
        glNormal3f(nx/len, 0.0f, nz/len);

        // 底面点
        glVertex3f(x, 0.0f, z);
        // 顶面点
        glVertex3f(x, height, z);
    }
    glEnd();

    // 绘制底面
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, -1.0f, 0.0f);  // 向下的法向量
    glVertex3f(0.0f, 0.0f, 0.0f);   // 中心点
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * M_PI * i / slices;
        float x = radiusX * cos(angle);
        float z = radiusZ * sin(angle);
        glVertex3f(x, 0.0f, z);
    }
    glEnd();

    // 绘制顶面
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 1.0f, 0.0f);   // 向上的法向量
    glVertex3f(0.0f, height, 0.0f); // 中心点
    for (int i = slices; i >= 0; i--) {  // 逆时针顺序
        float angle = 2.0f * M_PI * i / slices;
        float x = radiusX * cos(angle);
        float z = radiusZ * sin(angle);
        glVertex3f(x, height, z);
    }
    glEnd();

    glPopMatrix();
}

// 绘制梯形截面柱体（X轴为梯形高，Y轴为柱体高）
// centerX, centerY, centerZ: 底面下底中点坐标
// bottomLength: 下底长度（Z方向）
// topLength: 上底长度（Z方向）
// trapHeight: 梯形的高度（X方向）
// prismHeight: 柱体的高度（Y方向）
void drawTrapezoidalPrism(float centerX, float centerY, float centerZ,
                         float bottomLength, float topLength,
                         float trapHeight, float prismHeight) {

    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);

    // 计算梯形的关键点
    float bl = bottomLength * 0.5f;  // 下底半长
    float tl = topLength * 0.5f;     // 上底半长

    // 定义梯形的8个顶点 (底面4个 + 顶面4个)
    float bottomVerts[4][3] = {
        {0.0f,       0.0f, -bl},      // 底面下底左
        {0.0f,       0.0f,  bl},      // 底面下底右
        {trapHeight, 0.0f,  tl},      // 底面上底右
        {trapHeight, 0.0f, -tl}       // 底面上底左
    };

    float topVerts[4][3] = {
        {0.0f,       prismHeight, -bl},      // 顶面下底左
        {0.0f,       prismHeight,  bl},      // 顶面下底右
        {trapHeight, prismHeight,  tl},      // 顶面上底右
        {trapHeight, prismHeight, -tl}       // 顶面上底左
    };

    // 绘制底面梯形
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3fv(bottomVerts[0]);
    glVertex3fv(bottomVerts[1]);
    glVertex3fv(bottomVerts[2]);
    glVertex3fv(bottomVerts[3]);
    glEnd();

    // 绘制顶面梯形
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3fv(topVerts[0]);
    glVertex3fv(topVerts[3]);
    glVertex3fv(topVerts[2]);
    glVertex3fv(topVerts[1]);
    glEnd();

    // 绘制四个侧面
    glBegin(GL_QUADS);

    // 下底面 (X=0的面)
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3fv(bottomVerts[0]);
    glVertex3fv(topVerts[0]);
    glVertex3fv(topVerts[1]);
    glVertex3fv(bottomVerts[1]);

    // 上底面 (X=trapHeight的面)
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3fv(bottomVerts[3]);
    glVertex3fv(bottomVerts[2]);
    glVertex3fv(topVerts[2]);
    glVertex3fv(topVerts[3]);

    // 左侧面 (斜面，Z负方向)
    float nx_left = trapHeight;
    float nz_left = bl - tl;  // Z方向的倾斜
    float len_left = sqrt(nx_left*nx_left + nz_left*nz_left);
    glNormal3f(-nx_left/len_left, 0.0f, -nz_left/len_left);
    glVertex3fv(bottomVerts[0]);
    glVertex3fv(bottomVerts[3]);
    glVertex3fv(topVerts[3]);
    glVertex3fv(topVerts[0]);

    // 右侧面 (斜面，Z正方向)
    float nx_right = trapHeight;
    float nz_right = tl - bl;  // Z方向的倾斜
    float len_right = sqrt(nx_right*nx_right + nz_right*nz_right);
    glNormal3f(-nx_right/len_right, 0.0f, -nz_right/len_right);
    glVertex3fv(bottomVerts[1]);
    glVertex3fv(topVerts[1]);
    glVertex3fv(topVerts[2]);
    glVertex3fv(bottomVerts[2]);

    glEnd();

    glPopMatrix();
}

// 绘制1/4椭球体（保留X-Z平面下面，X轴正方向的1/4）
// centerX, centerY, centerZ: 椭圆体中心坐标
// radiusX, radiusY, radiusZ: 三个轴向的半径
// slices: 经线分段数
// stacks: 纬线分段数
void drawQuarterEllipsoid(float centerX, float centerY, float centerZ,
                         float radiusX, float radiusY, float radiusZ,
                         int slices, int stacks) {

    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);

    // 绘制椭球体表面（Y负半球，X正方向的半圆）
    for (int i = 0; i < stacks/2; i++) {  // 只绘制下半部分
        float phi1 = M_PI * (-0.5 + (float)i / stacks);        // 当前纬度
        float phi2 = M_PI * (-0.5 + (float)(i + 1) / stacks);  // 下一个纬度

        glBegin(GL_QUAD_STRIP);
        // 从Z负轴到Z正轴，但只要X正方向的部分
        for (int j = -slices/4; j <= slices/4; j++) {  // θ从-π/2到π/2
            float theta = M_PI * j / (slices/2);  // 经度：-π/2到π/2

            // 第一个点 (当前纬度)
            float x1 = radiusX * cos(phi1) * cos(theta);
            float y1 = radiusY * sin(phi1);
            float z1 = radiusZ * cos(phi1) * sin(theta);

            // 第二个点 (下一个纬度)
            float x2 = radiusX * cos(phi2) * cos(theta);
            float y2 = radiusY * sin(phi2);
            float z2 = radiusZ * cos(phi2) * sin(theta);

            // 计算法向量用于光照
            float nx1 = x1 / (radiusX * radiusX);
            float ny1 = y1 / (radiusY * radiusY);
            float nz1 = z1 / (radiusZ * radiusZ);
            float len1 = sqrt(nx1*nx1 + ny1*ny1 + nz1*nz1);

            float nx2 = x2 / (radiusX * radiusX);
            float ny2 = y2 / (radiusY * radiusY);
            float nz2 = z2 / (radiusZ * radiusZ);
            float len2 = sqrt(nx2*nx2 + ny2*ny2 + nz2*nz2);

            glNormal3f(nx1/len1, ny1/len1, nz1/len1);
            glVertex3f(x1, y1, z1);

            glNormal3f(nx2/len2, ny2/len2, nz2/len2);
            glVertex3f(x2, y2, z2);
        }
        glEnd();
    }

    // 绘制X-Z平面的切面（Y=0的面）- 半椭圆
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 1.0f, 0.0f);  // 向上的法向量
    glVertex3f(0.0f, 0.0f, 0.0f);  // 中心点

    for (int j = -slices/4; j <= slices/4; j++) {
        float theta = M_PI * j / (slices/2);  // θ从-π/2到π/2
        float x = radiusX * cos(theta);
        float z = radiusZ * sin(theta);
        glVertex3f(x, 0.0f, z);
    }
    glEnd();

    // 绘制Z正方向的切面（X正半轴到中心）
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, -1.0f);  // 向Z负方向的法向量
    glVertex3f(0.0f, 0.0f, 0.0f);  // 中心点

    for (int i = 0; i <= stacks/2; i++) {
        float phi = M_PI * (-0.5 + (float)i / stacks);
        float x = radiusX * cos(phi);
        float y = radiusY * sin(phi);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    // 绘制Z负方向的切面（X正半轴到中心）
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, 1.0f);  // 向Z正方向的法向量
    glVertex3f(0.0f, 0.0f, 0.0f);  // 中心点

    for (int i = stacks/2; i >= 0; i--) {  // 逆时针顺序
        float phi = M_PI * (-0.5 + (float)i / stacks);
        float x = radiusX * cos(phi);
        float y = radiusY * sin(phi);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    glPopMatrix();
}

// 绘制中空半椭圆柱体（去掉Y轴负半轴，柱体高度沿X轴正方向）
// centerX, centerY, centerZ: 底面椭圆中心坐标
// outerRadiusY, outerRadiusZ: 外椭圆的Y轴和Z轴半径
// innerRadiusY, innerRadiusZ: 内椭圆的Y轴和Z轴半径
// height: 柱体高度（沿X轴正方向）
// slices: 圆周分段数
void drawHollowHalfEllipticCylinderX(float centerX, float centerY, float centerZ,
                                   float outerRadiusY, float outerRadiusZ,
                                   float innerRadiusY, float innerRadiusZ,
                                   float height, int slices) {

    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);

    // 绘制外表面（Y轴正半轴）
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices/2; i++) {  // 只绘制Y正半轴，从Z负轴到Z正轴
        float angle = M_PI * i / (slices/2);  // 角度从0到π
        float y = outerRadiusY * sin(angle);  // sin确保Y≥0
        float z = outerRadiusZ * cos(angle);

        // 计算外表面法向量
        float nx = 0.0f;
        float ny = sin(angle) / outerRadiusY;  // 对应sin
        float nz = cos(angle) / outerRadiusZ;  // 对应cos
        float len = sqrt(ny*ny + nz*nz);

        glNormal3f(nx, ny/len, nz/len);

        // 底面点（X=0）
        glVertex3f(0.0f, y, z);
        // 顶面点（X=height）
        glVertex3f(height, y, z);
    }
    glEnd();

    // 绘制内表面（Y轴正半轴）
    glBegin(GL_QUAD_STRIP);
    for (int i = slices/2; i >= 0; i--) {  // 逆序绘制内表面
        float angle = M_PI * i / (slices/2);  // 角度从π到0
        float y = innerRadiusY * sin(angle);  // sin确保Y≥0
        float z = innerRadiusZ * cos(angle);

        // 计算内表面法向量（向内）
        float nx = 0.0f;
        float ny = -sin(angle) / innerRadiusY;  // 对应sin，向内
        float nz = -cos(angle) / innerRadiusZ;  // 对应cos，向内
        float len = sqrt(ny*ny + nz*nz);

        glNormal3f(nx, ny/len, nz/len);

        // 底面点（X=0）
        glVertex3f(0.0f, y, z);
        // 顶面点（X=height）
        glVertex3f(height, y, z);
    }
    glEnd();

    // 绘制底面环形（X=0的面）
    glBegin(GL_QUAD_STRIP);
    glNormal3f(-1.0f, 0.0f, 0.0f);  // 向X负方向的法向量
    for (int i = 0; i <= slices/2; i++) {
        float angle = M_PI * i / (slices/2);

        // 外椭圆点
        float outerY = outerRadiusY * sin(angle);  // sin确保Y≥0
        float outerZ = outerRadiusZ * cos(angle);
        glVertex3f(0.0f, outerY, outerZ);

        // 内椭圆点
        float innerY = innerRadiusY * sin(angle);  // sin确保Y≥0
        float innerZ = innerRadiusZ * cos(angle);
        glVertex3f(0.0f, innerY, innerZ);
    }
    glEnd();

    // 绘制顶面环形（X=height的面）
    glBegin(GL_QUAD_STRIP);
    glNormal3f(1.0f, 0.0f, 0.0f);  // 向X正方向的法向量
    for (int i = 0; i <= slices/2; i++) {
        float angle = M_PI * i / (slices/2);

        // 内椭圆点
        float innerY = innerRadiusY * sin(angle);  // sin确保Y≥0
        float innerZ = innerRadiusZ * cos(angle);
        glVertex3f(height, innerY, innerZ);

        // 外椭圆点
        float outerY = outerRadiusY * sin(angle);  // sin确保Y≥0
        float outerZ = outerRadiusZ * cos(angle);
        glVertex3f(height, outerY, outerZ);
    }
    glEnd();

    // 绘制左端面（Z负方向的切面）
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);  // 向Z负方向的法向量
    // 底面
    glVertex3f(0.0f, 0.0f, -outerRadiusZ);
    glVertex3f(0.0f, 0.0f, -innerRadiusZ);
    glVertex3f(height, 0.0f, -innerRadiusZ);
    glVertex3f(height, 0.0f, -outerRadiusZ);
    glEnd();

    // 绘制右端面（Z正方向的切面）
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);  // 向Z正方向的法向量
    // 底面
    glVertex3f(0.0f, 0.0f, innerRadiusZ);
    glVertex3f(0.0f, 0.0f, outerRadiusZ);
    glVertex3f(height, 0.0f, outerRadiusZ);
    glVertex3f(height, 0.0f, innerRadiusZ);
    glEnd();

    glPopMatrix();
}

// 绘制中空椭圆柱体
void drawHollowEllipticalCylinder(float centerX, float centerY, float centerZ,
                                 float outerRadiusX, float outerRadiusZ,
                                 float innerCenterX, float innerCenterZ,
                                 float innerRadiusX, float innerRadiusZ,
                                 float height, int slices) {
    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);

    // 绘制外壁
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * M_PI * i / slices;
        float x = outerRadiusX * cos(angle);
        float z = outerRadiusZ * sin(angle);

        // 计算外壁法向量
        float nx = cos(angle) / outerRadiusX;
        float nz = sin(angle) / outerRadiusZ;
        float length = sqrt(nx * nx + nz * nz);
        nx /= length;
        nz /= length;

        glNormal3f(nx, 0.0f, nz);

        // 底面点
        glVertex3f(x, 0.0f, z);
        // 顶面点
        glVertex3f(x, height, z);
    }
    glEnd();

    // 绘制内壁
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * M_PI * i / slices;
        float x = innerCenterX + innerRadiusX * cos(angle);
        float z = innerCenterZ + innerRadiusZ * sin(angle);

        // 计算内壁法向量（向内）
        float nx = -cos(angle) / innerRadiusX;
        float nz = -sin(angle) / innerRadiusZ;
        float length = sqrt(nx * nx + nz * nz);
        nx /= length;
        nz /= length;

        glNormal3f(nx, 0.0f, nz);

        // 底面点
        glVertex3f(x, 0.0f, z);
        // 顶面点
        glVertex3f(x, height, z);
    }
    glEnd();

    // 绘制底面环形面
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.0f, -1.0f, 0.0f);  // 向下的法向量
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * M_PI * i / slices;

        // 外椭圆点
        float outerX = outerRadiusX * cos(angle);
        float outerZ = outerRadiusZ * sin(angle);
        glVertex3f(outerX, 0.0f, outerZ);

        // 内椭圆点
        float innerX = innerCenterX + innerRadiusX * cos(angle);
        float innerZ = innerCenterZ + innerRadiusZ * sin(angle);
        glVertex3f(innerX, 0.0f, innerZ);
    }
    glEnd();

    // 绘制顶面环形面
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.0f, 1.0f, 0.0f);   // 向上的法向量
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * M_PI * i / slices;

        // 外椭圆点
        float outerX = outerRadiusX * cos(angle);
        float outerZ = outerRadiusZ * sin(angle);
        glVertex3f(outerX, height, outerZ);

        // 内椭圆点
        float innerX = innerCenterX + innerRadiusX * cos(angle);
        float innerZ = innerCenterZ + innerRadiusZ * sin(angle);
        glVertex3f(innerX, height, innerZ);
    }
    glEnd();

    glPopMatrix();
}

// 绘制中空半椭圆柱体（去掉X轴负半轴，柱体高度沿Y轴正方向，内外椭圆可以不同心）
// centerX, centerY, centerZ: 外椭圆底面中心坐标
// outerRadiusX, outerRadiusZ: 外椭圆的X轴和Z轴半径
// innerCenterX, innerCenterZ: 内椭圆的X轴和Z轴中心偏移
// innerRadiusX, innerRadiusZ: 内椭圆的X轴和Z轴半径
// height: 柱体高度（沿Y轴正方向）
// slices: 圆周分段数
void drawHollowHalfEllipticCylinderY(float centerX, float centerY, float centerZ,
                                   float outerRadiusX, float outerRadiusZ,
                                   float innerCenterX, float innerCenterZ,
                                   float innerRadiusX, float innerRadiusZ,
                                   float height, int slices) {
    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);

    // 绘制外表面（X轴正半轴）
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices/2; i++) {  // 只绘制X正半轴，从Z负轴到Z正轴
        float angle = M_PI * i / (slices/2);  // 角度从0到π
        float x = outerRadiusX * sin(angle);  // sin确保X≥0
        float z = outerRadiusZ * cos(angle);

        // 计算外表面法向量
        float nx = sin(angle) / outerRadiusX;  // 对应sin
        float ny = 0.0f;
        float nz = cos(angle) / outerRadiusZ;  // 对应cos
        float len = sqrt(nx*nx + nz*nz);
        glNormal3f(nx/len, ny, nz/len);

        // 底面点（Y=0）
        glVertex3f(x, 0.0f, z);
        // 顶面点（Y=height）
        glVertex3f(x, height, z);
    }
    glEnd();

    // 绘制内表面（X轴正半轴，使用内椭圆的中心偏移）
    glBegin(GL_QUAD_STRIP);
    for (int i = slices/2; i >= 0; i--) {  // 逆序绘制内表面
        float angle = M_PI * i / (slices/2);  // 角度从π到0
        float x = innerCenterX + innerRadiusX * sin(angle);  // 加上内椭圆的X偏移
        float z = innerCenterZ + innerRadiusZ * cos(angle);  // 加上内椭圆的Z偏移

        // 计算内表面法向量（向内，相对于内椭圆中心）
        float nx = -sin(angle) / innerRadiusX;  // 对应sin，向内
        float ny = 0.0f;
        float nz = -cos(angle) / innerRadiusZ;  // 对应cos，向内
        float len = sqrt(nx*nx + nz*nz);
        glNormal3f(nx/len, ny, nz/len);

        // 底面点（Y=0）
        glVertex3f(x, 0.0f, z);
        // 顶面点（Y=height）
        glVertex3f(x, height, z);
    }
    glEnd();

    // 绘制底面环形（Y=0的面）
    glBegin(GL_QUAD_STRIP);
    glNormal3f(0.0f, -1.0f, 0.0f);  // 向Y负方向的法向量
    for (int i = 0; i <= slices/2; i++) {
        float angle = M_PI * i / (slices/2);
        // 外椭圆点
        float outerX = outerRadiusX * sin(angle);  // sin确保X≥0
        float outerZ = outerRadiusZ * cos(angle);
        glVertex3f(outerX, 0.0f, outerZ);
        // 内椭圆点（使用偏移中心）
        float innerX = innerCenterX + innerRadiusX * sin(angle);
        float innerZ = innerCenterZ + innerRadiusZ * cos(angle);
        glVertex3f(innerX, 0.0f, innerZ);
    }
    glEnd();

    // 绘制顶面环形（Y=height的面）
    glBegin(GL_QUAD_STRIP);
    glNormal3f(0.0f, 1.0f, 0.0f);  // 向Y正方向的法向量
    for (int i = 0; i <= slices/2; i++) {
        float angle = M_PI * i / (slices/2);
        // 内椭圆点（使用偏移中心）
        float innerX = innerCenterX + innerRadiusX * sin(angle);
        float innerZ = innerCenterZ + innerRadiusZ * cos(angle);
        glVertex3f(innerX, height, innerZ);
        // 外椭圆点
        float outerX = outerRadiusX * sin(angle);  // sin确保X≥0
        float outerZ = outerRadiusZ * cos(angle);
        glVertex3f(outerX, height, outerZ);
    }
    glEnd();

    // 绘制左端面（Z负方向的切面）
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);  // 向Z负方向的法向量
    glVertex3f(0.0f, 0.0f, -outerRadiusZ);
    glVertex3f(innerCenterX, 0.0f, innerCenterZ - innerRadiusZ);
    glVertex3f(innerCenterX, height, innerCenterZ - innerRadiusZ);
    glVertex3f(0.0f, height, -outerRadiusZ);
    glEnd();

    // 绘制右端面（Z正方向的切面）
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);  // 向Z正方向的法向量
    glVertex3f(innerCenterX, 0.0f, innerCenterZ + innerRadiusZ);
    glVertex3f(0.0f, 0.0f, outerRadiusZ);
    glVertex3f(0.0f, height, outerRadiusZ);
    glVertex3f(innerCenterX, height, innerCenterZ + innerRadiusZ);
    glEnd();

    glPopMatrix();
}

// 绘制中空梯形柱体（正方形孔）
// bottomCenterX, bottomCenterY, bottomCenterZ: 梯形底面下底边中点坐标
// bottomLength: 下底长度（沿Z轴）
// topLength: 上底长度（沿Z轴）
// trapezoidHeight: 梯形的高（沿X轴正方向）
// cylinderHeight: 柱体的高（沿Y轴正方向）
// holeCenterX, holeCenterZ: 正方形孔中心的X,Z坐标
// holeSize: 正方形孔的边长
void drawHollowTrapezoidCylinder(float bottomCenterX, float bottomCenterY, float bottomCenterZ,
                                float bottomLength, float topLength, float trapezoidHeight,
                                float cylinderHeight, float holeCenterX, float holeCenterZ,
                                float holeSize) {

    // 计算梯形的四个顶点
    float bottomZ1 = bottomCenterZ - bottomLength / 2.0f;
    float bottomZ2 = bottomCenterZ + bottomLength / 2.0f;
    float topZ1 = bottomCenterZ - topLength / 2.0f;
    float topZ2 = bottomCenterZ + topLength / 2.0f;

    float bottomX = bottomCenterX;
    float topX = bottomCenterX + trapezoidHeight;

    // 计算正方形孔的四个顶点
    float halfSize = holeSize / 2.0f;
    float holeX1 = holeCenterX - halfSize;
    float holeX2 = holeCenterX + halfSize;
    float holeZ1 = holeCenterZ - halfSize;
    float holeZ2 = holeCenterZ + halfSize;

    // 1. 绘制梯形柱体的四个外表面
    glBegin(GL_QUADS);

    // 前面（Z轴负方向）
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(bottomX, bottomCenterY, bottomZ1);
    glVertex3f(bottomX, bottomCenterY + cylinderHeight, bottomZ1);
    glVertex3f(topX, bottomCenterY + cylinderHeight, topZ1);
    glVertex3f(topX, bottomCenterY, topZ1);

    // 后面（Z轴正方向）
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(topX, bottomCenterY, topZ2);
    glVertex3f(topX, bottomCenterY + cylinderHeight, topZ2);
    glVertex3f(bottomX, bottomCenterY + cylinderHeight, bottomZ2);
    glVertex3f(bottomX, bottomCenterY, bottomZ2);

    // 左侧斜面
    float nx1, ny1, nz1;
    float v1x = 0.0f, v1y = cylinderHeight, v1z = 0.0f;
    float v2x = trapezoidHeight, v2y = 0.0f, v2z = (topZ1 - bottomZ1);
    nx1 = v1y * v2z - v1z * v2y;
    ny1 = v1z * v2x - v1x * v2z;
    nz1 = v1x * v2y - v1y * v2x;
    float len1 = sqrt(nx1*nx1 + ny1*ny1 + nz1*nz1);
    nx1 /= len1; ny1 /= len1; nz1 /= len1;

    glNormal3f(nx1, ny1, nz1);
    glVertex3f(bottomX, bottomCenterY, bottomZ1);
    glVertex3f(topX, bottomCenterY, topZ1);
    glVertex3f(topX, bottomCenterY + cylinderHeight, topZ1);
    glVertex3f(bottomX, bottomCenterY + cylinderHeight, bottomZ1);

    // 右侧斜面
    float nx2, ny2, nz2;
    float v3x = trapezoidHeight, v3y = 0.0f, v3z = (topZ2 - bottomZ2);
    float v4x = 0.0f, v4y = cylinderHeight, v4z = 0.0f;
    nx2 = v3y * v4z - v3z * v4y;
    ny2 = v3z * v4x - v3x * v4z;
    nz2 = v3x * v4y - v3y * v4x;
    float len2 = sqrt(nx2*nx2 + ny2*ny2 + nz2*nz2);
    nx2 /= len2; ny2 /= len2; nz2 /= len2;

    glNormal3f(nx2, ny2, nz2);
    glVertex3f(topX, bottomCenterY, topZ2);
    glVertex3f(bottomX, bottomCenterY, bottomZ2);
    glVertex3f(bottomX, bottomCenterY + cylinderHeight, bottomZ2);
    glVertex3f(topX, bottomCenterY + cylinderHeight, topZ2);

    glEnd();

    // 2. 绘制正方形孔的四个内表面
    glBegin(GL_QUADS);

    // 正方形孔的前面（Z轴负方向）
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(holeX1, bottomCenterY, holeZ1);
    glVertex3f(holeX2, bottomCenterY, holeZ1);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, holeZ1);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, holeZ1);

    // 正方形孔的后面（Z轴正方向）
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(holeX2, bottomCenterY, holeZ2);
    glVertex3f(holeX1, bottomCenterY, holeZ2);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, holeZ2);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, holeZ2);

    // 正方形孔的左面（X轴负方向）
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(holeX1, bottomCenterY, holeZ2);
    glVertex3f(holeX1, bottomCenterY, holeZ1);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, holeZ1);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, holeZ2);

    // 正方形孔的右面（X轴正方向）
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(holeX2, bottomCenterY, holeZ1);
    glVertex3f(holeX2, bottomCenterY, holeZ2);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, holeZ2);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, holeZ1);

    glEnd();

    // 3. 绘制底面（8个四边形区域）
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);

    // 底面左下角区域
    glVertex3f(bottomX, bottomCenterY, bottomZ1);
    glVertex3f(bottomX, bottomCenterY, holeZ1);
    glVertex3f(holeX1, bottomCenterY, holeZ1);
    glVertex3f(holeX1, bottomCenterY, bottomZ1);

    // 底面左上角区域
    glVertex3f(bottomX, bottomCenterY, holeZ2);
    glVertex3f(bottomX, bottomCenterY, bottomZ2);
    glVertex3f(holeX1, bottomCenterY, bottomZ2);
    glVertex3f(holeX1, bottomCenterY, holeZ2);

    // 底面右下角区域
    glVertex3f(holeX2, bottomCenterY, bottomZ1);
    glVertex3f(holeX2, bottomCenterY, holeZ1);
    glVertex3f(topX, bottomCenterY, holeZ1);
    glVertex3f(topX, bottomCenterY, topZ1);

    // 底面右上角区域
    glVertex3f(holeX2, bottomCenterY, holeZ2);
    glVertex3f(holeX2, bottomCenterY, bottomZ2);
    glVertex3f(topX, bottomCenterY, topZ2);
    glVertex3f(topX, bottomCenterY, holeZ2);

    // 底面中下区域
    glVertex3f(holeX1, bottomCenterY, bottomZ1);
    glVertex3f(holeX1, bottomCenterY, holeZ1);
    glVertex3f(holeX2, bottomCenterY, holeZ1);
    glVertex3f(holeX2, bottomCenterY, bottomZ1);

    // 底面中上区域
    glVertex3f(holeX1, bottomCenterY, holeZ2);
    glVertex3f(holeX1, bottomCenterY, bottomZ2);
    glVertex3f(holeX2, bottomCenterY, bottomZ2);
    glVertex3f(holeX2, bottomCenterY, holeZ2);

    // 底面左中区域
    glVertex3f(bottomX, bottomCenterY, holeZ1);
    glVertex3f(bottomX, bottomCenterY, holeZ2);
    glVertex3f(holeX1, bottomCenterY, holeZ2);
    glVertex3f(holeX1, bottomCenterY, holeZ1);

    // 底面右中区域
    glVertex3f(holeX2, bottomCenterY, holeZ1);
    glVertex3f(holeX2, bottomCenterY, holeZ2);
    glVertex3f(topX, bottomCenterY, holeZ2);
    glVertex3f(topX, bottomCenterY, holeZ1);

    glEnd();

    // 4. 绘制顶面（8个四边形区域）
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);

    // 顶面左下角区域
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, bottomZ1);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, holeZ1);
    glVertex3f(bottomX, bottomCenterY + cylinderHeight, holeZ1);
    glVertex3f(bottomX, bottomCenterY + cylinderHeight, bottomZ1);

    // 顶面左上角区域
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, holeZ2);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, bottomZ2);
    glVertex3f(bottomX, bottomCenterY + cylinderHeight, bottomZ2);
    glVertex3f(bottomX, bottomCenterY + cylinderHeight, holeZ2);

    // 顶面右下角区域
    glVertex3f(topX, bottomCenterY + cylinderHeight, topZ1);
    glVertex3f(topX, bottomCenterY + cylinderHeight, holeZ1);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, holeZ1);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, bottomZ1);

    // 顶面右上角区域
    glVertex3f(topX, bottomCenterY + cylinderHeight, holeZ2);
    glVertex3f(topX, bottomCenterY + cylinderHeight, topZ2);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, bottomZ2);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, holeZ2);

    // 顶面中下区域
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, bottomZ1);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, holeZ1);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, holeZ1);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, bottomZ1);

    // 顶面中上区域
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, holeZ2);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, bottomZ2);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, bottomZ2);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, holeZ2);

    // 顶面左中区域
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, holeZ1);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, holeZ2);
    glVertex3f(bottomX, bottomCenterY + cylinderHeight, holeZ2);
    glVertex3f(bottomX, bottomCenterY + cylinderHeight, holeZ1);

    // 顶面右中区域
    glVertex3f(topX, bottomCenterY + cylinderHeight, holeZ1);
    glVertex3f(topX, bottomCenterY + cylinderHeight, holeZ2);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, holeZ2);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, holeZ1);

    glEnd();
}

// 绘制中空半椭圆柱体（去掉X轴正半轴，柱体高度沿Y轴正方向，内外椭圆可以不同心）
// centerX, centerY, centerZ: 外椭圆底面中心坐标
// outerRadiusX, outerRadiusZ: 外椭圆的X轴和Z轴半径
// innerCenterX, innerCenterZ: 内椭圆的X轴和Z轴中心偏移
// innerRadiusX, innerRadiusZ: 内椭圆的X轴和Z轴半径
// height: 柱体高度（沿Y轴正方向）
// slices: 圆周分段数
void drawHollowHalfEllipticCylinderYNeg(float centerX, float centerY, float centerZ,
                                        float outerRadiusX, float outerRadiusZ,
                                        float innerCenterX, float innerCenterZ,
                                        float innerRadiusX, float innerRadiusZ,
                                        float height, int slices) {
    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);

    // 绘制外表面（X轴负半轴）
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices/2; i++) {  // 只绘制X负半轴，从Z负轴到Z正轴
        float angle = M_PI/2 + M_PI * i / (slices/2);  // 角度从π/2到3π/2
        float x = outerRadiusX * cos(angle);  // cos确保X≤0
        float z = outerRadiusZ * sin(angle);

        // 计算外表面法向量
        float nx = cos(angle) / outerRadiusX;  // 对应cos
        float ny = 0.0f;
        float nz = sin(angle) / outerRadiusZ;  // 对应sin
        float len = sqrt(nx*nx + nz*nz);
        glNormal3f(nx/len, ny, nz/len);

        // 底面点（Y=0）
        glVertex3f(x, 0.0f, z);
        // 顶面点（Y=height）
        glVertex3f(x, height, z);
    }
    glEnd();

    // 绘制内表面（X轴负半轴，使用内椭圆的中心偏移）
    glBegin(GL_QUAD_STRIP);
    for (int i = slices/2; i >= 0; i--) {  // 逆序绘制内表面
        float angle = M_PI/2 + M_PI * i / (slices/2);  // 角度从3π/2到π/2
        float x = innerCenterX + innerRadiusX * cos(angle);  // 加上内椭圆的X偏移
        float z = innerCenterZ + innerRadiusZ * sin(angle);  // 加上内椭圆的Z偏移

        // 计算内表面法向量（向内，相对于内椭圆中心）
        float nx = -cos(angle) / innerRadiusX;  // 对应cos，向内
        float ny = 0.0f;
        float nz = -sin(angle) / innerRadiusZ;  // 对应sin，向内
        float len = sqrt(nx*nx + nz*nz);
        glNormal3f(nx/len, ny, nz/len);

        // 底面点（Y=0）
        glVertex3f(x, 0.0f, z);
        // 顶面点（Y=height）
        glVertex3f(x, height, z);
    }
    glEnd();

    // 绘制底面环形（Y=0的面）
    glBegin(GL_QUAD_STRIP);
    glNormal3f(0.0f, -1.0f, 0.0f);  // 向Y负方向的法向量
    for (int i = 0; i <= slices/2; i++) {
        float angle = M_PI/2 + M_PI * i / (slices/2);
        // 外椭圆点
        float outerX = outerRadiusX * cos(angle);  // cos确保X≤0
        float outerZ = outerRadiusZ * sin(angle);
        glVertex3f(outerX, 0.0f, outerZ);
        // 内椭圆点（使用偏移中心）
        float innerX = innerCenterX + innerRadiusX * cos(angle);
        float innerZ = innerCenterZ + innerRadiusZ * sin(angle);
        glVertex3f(innerX, 0.0f, innerZ);
    }
    glEnd();

    // 绘制顶面环形（Y=height的面）
    glBegin(GL_QUAD_STRIP);
    glNormal3f(0.0f, 1.0f, 0.0f);  // 向Y正方向的法向量
    for (int i = 0; i <= slices/2; i++) {
        float angle = M_PI/2 + M_PI * i / (slices/2);
        // 内椭圆点（使用偏移中心）
        float innerX = innerCenterX + innerRadiusX * cos(angle);
        float innerZ = innerCenterZ + innerRadiusZ * sin(angle);
        glVertex3f(innerX, height, innerZ);
        // 外椭圆点
        float outerX = outerRadiusX * cos(angle);  // cos确保X≤0
        float outerZ = outerRadiusZ * sin(angle);
        glVertex3f(outerX, height, outerZ);
    }
    glEnd();

    // 绘制前端面（Z正方向的切面）
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);  // 向Z正方向的法向量
    glVertex3f(0.0f, 0.0f, outerRadiusZ);
    glVertex3f(innerCenterX, 0.0f, innerCenterZ + innerRadiusZ);
    glVertex3f(innerCenterX, height, innerCenterZ + innerRadiusZ);
    glVertex3f(0.0f, height, outerRadiusZ);
    glEnd();

    // 绘制后端面（Z负方向的切面）
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);  // 向Z负方向的法向量
    glVertex3f(innerCenterX, 0.0f, innerCenterZ - innerRadiusZ);
    glVertex3f(0.0f, 0.0f, -outerRadiusZ);
    glVertex3f(0.0f, height, -outerRadiusZ);
    glVertex3f(innerCenterX, height, innerCenterZ - innerRadiusZ);
    glEnd();

    glPopMatrix();
}



// 绘制Z方向圆柱体
// centerX, centerY, centerZ: 底面圆心坐标
// radius: 底面半径
// height: 柱体高度（沿Z轴正方向）
// slices: 圆周分段数
void drawCylinderZ(float centerX, float centerY, float centerZ,
                   float radius, float height, int slices) {
    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);
    // 绘制侧面
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * M_PI * i / slices;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        // 计算侧面法向量（向外）
        float nx = cos(angle);
        float ny = sin(angle);
        float nz = 0.0f;
        glNormal3f(nx, ny, nz);
        // 底面点（Z=0）
        glVertex3f(x, y, 0.0f);
        // 顶面点（Z=height）
        glVertex3f(x, y, height);
    }
    glEnd();
    // 绘制底面（Z=0）
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, -1.0f);  // 向Z负方向的法向量
    glVertex3f(0.0f, 0.0f, 0.0f);   // 中心点
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * M_PI * i / slices;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
    // 绘制顶面（Z=height）
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, 1.0f);   // 向Z正方向的法向量
    glVertex3f(0.0f, 0.0f, height); // 中心点
    for (int i = slices; i >= 0; i--) {  // 逆时针顺序
        float angle = 2.0f * M_PI * i / slices;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex3f(x, y, height);
    }
    glEnd();
    glPopMatrix();
}

// 绘制实心立方体
// x1, y1, z1: 第一个顶角坐标（通常是最小值角）
// x2, y2, z2: 第二个顶角坐标（通常是最大值角）
void drawSolidCube(float x1, float y1, float z1, float x2, float y2, float z2) {

    // 确保坐标顺序正确（min, max）
    float xmin = (x1 < x2) ? x1 : x2;
    float xmax = (x1 < x2) ? x2 : x1;
    float ymin = (y1 < y2) ? y1 : y2;
    float ymax = (y1 < y2) ? y2 : y1;
    float zmin = (z1 < z2) ? z1 : z2;
    float zmax = (z1 < z2) ? z2 : z1;

    glBegin(GL_QUADS);

    // 前面 (Z = zmax)
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(xmin, ymin, zmax);
    glVertex3f(xmax, ymin, zmax);
    glVertex3f(xmax, ymax, zmax);
    glVertex3f(xmin, ymax, zmax);

    // 后面 (Z = zmin)
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(xmin, ymin, zmin);
    glVertex3f(xmin, ymax, zmin);
    glVertex3f(xmax, ymax, zmin);
    glVertex3f(xmax, ymin, zmin);

    // 左面 (X = xmin)
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(xmin, ymin, zmin);
    glVertex3f(xmin, ymin, zmax);
    glVertex3f(xmin, ymax, zmax);
    glVertex3f(xmin, ymax, zmin);

    // 右面 (X = xmax)
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(xmax, ymin, zmin);
    glVertex3f(xmax, ymax, zmin);
    glVertex3f(xmax, ymax, zmax);
    glVertex3f(xmax, ymin, zmax);

    // 底面 (Y = ymin)
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(xmin, ymin, zmin);
    glVertex3f(xmax, ymin, zmin);
    glVertex3f(xmax, ymin, zmax);
    glVertex3f(xmin, ymin, zmax);

    // 顶面 (Y = ymax)
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(xmin, ymax, zmin);
    glVertex3f(xmin, ymax, zmax);
    glVertex3f(xmax, ymax, zmax);
    glVertex3f(xmax, ymax, zmin);

    glEnd();
}

// 向量运算辅助函数
void vectorSubtract(float result[3], float a[3], float b[3]) {
    result[0] = a[0] - b[0];
    result[1] = a[1] - b[1];
    result[2] = a[2] - b[2];
}

void vectorAdd(float result[3], float a[3], float b[3]) {
    result[0] = a[0] + b[0];
    result[1] = a[1] + b[1];
    result[2] = a[2] + b[2];
}

void vectorCross(float result[3], float a[3], float b[3]) {
    result[0] = a[1] * b[2] - a[2] * b[1];
    result[1] = a[2] * b[0] - a[0] * b[2];
    result[2] = a[0] * b[1] - a[1] * b[0];
}

void vectorNormalize(float v[3]) {
    float length = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if (length > 0) {
        v[0] /= length;
        v[1] /= length;
        v[2] /= length;
    }
}

void drawHexahedron(float x1, float y1, float z1,  // 顶点A (起始点)
                    float x2, float y2, float z2,  // 顶点B
                    float x3, float y3, float z3,  // 顶点C
                    float x4, float y4, float z4)  // 顶点D
{
    // 定义四个顶点
    float A[3] = {x1, y1, z1};
    float B[3] = {x2, y2, z2};
    float C[3] = {x3, y3, z3};
    float D[3] = {x4, y4, z4};

    // 计算三个边向量
    float AB[3], AC[3], AD[3];
    vectorSubtract(AB, B, A);  // A到B的向量
    vectorSubtract(AC, C, A);  // A到C的向量
    vectorSubtract(AD, D, A);  // A到D的向量

    // 构造平行六面体的8个顶点
    float vertices[8][3];

    // 计算所有8个顶点
    // 顶点0: A
    vertices[0][0] = A[0]; vertices[0][1] = A[1]; vertices[0][2] = A[2];

    // 顶点1: A + AB
    vectorAdd(vertices[1], A, AB);

    // 顶点2: A + AC
    vectorAdd(vertices[2], A, AC);

    // 顶点3: A + AD
    vectorAdd(vertices[3], A, AD);

    // 顶点4: A + AB + AC
    float temp[3];
    vectorAdd(temp, AB, AC);
    vectorAdd(vertices[4], A, temp);

    // 顶点5: A + AB + AD
    vectorAdd(temp, AB, AD);
    vectorAdd(vertices[5], A, temp);

    // 顶点6: A + AC + AD
    vectorAdd(temp, AC, AD);
    vectorAdd(vertices[6], A, temp);

    // 顶点7: A + AB + AC + AD
    vectorAdd(temp, AB, AC);
    vectorAdd(temp, temp, AD);
    vectorAdd(vertices[7], A, temp);

    // 绘制六个面，每个面单独计算法向量
    glBegin(GL_QUADS);

    // 面1: 底面 (顶点0,1,4,2) - 法向量向外
    float v1[3], v2[3], normal[3];
    vectorSubtract(v1, vertices[1], vertices[0]); // 0->1
    vectorSubtract(v2, vertices[2], vertices[0]); // 0->2
    vectorCross(normal, v1, v2);
    vectorNormalize(normal);
    glNormal3fv(normal);
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[1]);
    glVertex3fv(vertices[4]);
    glVertex3fv(vertices[2]);

    // 面2: 顶面 (顶点3,6,7,5) - 法向量向外
    vectorSubtract(v1, vertices[6], vertices[3]); // 3->6
    vectorSubtract(v2, vertices[5], vertices[3]); // 3->5
    vectorCross(normal, v1, v2);
    vectorNormalize(normal);
    glNormal3fv(normal);
    glVertex3fv(vertices[3]);
    glVertex3fv(vertices[6]);
    glVertex3fv(vertices[7]);
    glVertex3fv(vertices[5]);

    // 面3: 前面 (顶点0,3,5,1) - 法向量向外
    vectorSubtract(v1, vertices[3], vertices[0]); // 0->3
    vectorSubtract(v2, vertices[1], vertices[0]); // 0->1
    vectorCross(normal, v1, v2);
    vectorNormalize(normal);
    glNormal3fv(normal);
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[3]);
    glVertex3fv(vertices[5]);
    glVertex3fv(vertices[1]);

    // 面4: 后面 (顶点2,4,7,6) - 法向量向外
    vectorSubtract(v1, vertices[4], vertices[2]); // 2->4
    vectorSubtract(v2, vertices[6], vertices[2]); // 2->6
    vectorCross(normal, v1, v2);
    vectorNormalize(normal);
    glNormal3fv(normal);
    glVertex3fv(vertices[2]);
    glVertex3fv(vertices[4]);
    glVertex3fv(vertices[7]);
    glVertex3fv(vertices[6]);

    // 面5: 左面 (顶点0,2,6,3) - 法向量向外
    vectorSubtract(v1, vertices[2], vertices[0]); // 0->2
    vectorSubtract(v2, vertices[3], vertices[0]); // 0->3
    vectorCross(normal, v1, v2);
    vectorNormalize(normal);
    glNormal3fv(normal);
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[2]);
    glVertex3fv(vertices[6]);
    glVertex3fv(vertices[3]);

    // 面6: 右面 (顶点1,5,7,4) - 法向量向外
    vectorSubtract(v1, vertices[5], vertices[1]); // 1->5
    vectorSubtract(v2, vertices[4], vertices[1]); // 1->4
    vectorCross(normal, v1, v2);
    vectorNormalize(normal);
    glNormal3fv(normal);
    glVertex3fv(vertices[1]);
    glVertex3fv(vertices[5]);
    glVertex3fv(vertices[7]);
    glVertex3fv(vertices[4]);

    glEnd();
}

// 新增：鞋面动画定时器函数
void shoeFaceTimer(int value) {
    const float shoeFaceStep = 3.0f;    // 鞋面移动步长
    const float maxOffset = 30.0f;      // 最大偏移量

    if(shoeFaceDirection) {
        // 向上移动
        shoeFaceOffsetY += shoeFaceStep;
        if(shoeFaceOffsetY >= maxOffset) {
            shoeFaceOffsetY = maxOffset;
        }
    } else {
        // 向下移动
        shoeFaceOffsetY -= shoeFaceStep;
        if(shoeFaceOffsetY <= 0.0f) {
            shoeFaceOffsetY = 0.0f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(60, shoeFaceTimer, 0); // 每60毫秒更新一次（稍慢一点）
}

// 鞋面动画定时器函数
void screwTimer(int value) {
    const float screwStep = 3.0f;
    const float maxOffset = 30.0f;

    if(screwDirection) {
        // 拔出动画 - 向下移动
        screwOffsetY -= screwStep;
        if(screwOffsetY <= -maxOffset) {
            screwOffsetY = -maxOffset;
        }
    } else {
        // 插入动画 - 向上移动
        screwOffsetY += screwStep;
        if(screwOffsetY >= 0.0f) {
            screwOffsetY = 0.0f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(50, screwTimer, 0); // 每50毫秒更新一次
}

// display回调函数
void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -300.0f);  // 后退300单位观察椭圆柱

    // 应用鼠标旋转
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);

    // 绘制坐标轴作为参考
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
        // X轴 - 红色
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-80.0f, 0.0f, 0.0f);
        glVertex3f(80.0f, 0.0f, 0.0f);

        // Y轴 - 绿色
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -10.0f, 0.0f);
        glVertex3f(0.0f, 20.0f, 0.0f);

        // Z轴 - 蓝色
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, -70.0f);
        glVertex3f(0.0f, 0.0f, 70.0f);
    glEnd();
    glEnable(GL_LIGHTING);

    // 线框图开关
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

/*
 * 鞋面
 */
    glPushMatrix();
    if (ShoeFace) {
        // =================== 绘制鞋面 (带Y轴动画) ===================
        glTranslatef(0.0f, shoeFaceOffsetY, 0.0f); // 应用Y轴偏移动画

        // 鞋面侧
        // 启用Bang.png纹理
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, bangTexture);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // 设置Bang纹理坐标生成参数
        GLfloat bangSPlane[] = {0.005f, 0.01f, 0.0f, 0.0f};  // X方向
        GLfloat bangTPlane[] = {0.01f, 0.0f, 0.005f, 0.0f};  // Y方向
        glTexGenfv(GL_S, GL_OBJECT_PLANE, bangSPlane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, bangTPlane);

        glColor3f(1.0f, 1.0f, 1.0f);  // 白色以显示纹理原色

        drawCylinderZ(0.0f, 27.0f, -55.0f, 10.0f, 10.0f, 30);
        drawCylinderZ(0.0f, 27.0f, 45.0f, 10.0f, 10.0f, 30);
        drawSolidCube(0.0f, 22.0f, -50.0f, -90.0f, 32.0f, -45.0f);
        drawSolidCube(0.0f, 22.0f, 50.0f, -90.0f, 32.0f, 45.0f);
        drawHollowHalfEllipticCylinderYNeg(-90.0f, 22.0f, 0.0f, 44.0f, 50.0f, 0.0f, 0.0f, 39.0f, 45.0f, 10.0f, 30);

        // 关闭Bang纹理
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);

        // 启用ShoeFace.png纹理
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, shoeFaceTexture);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // 设置鞋面纹理坐标生成参数
        GLfloat shoeFaceSPlane[] = {0.02f, 0.01f, 0.0f, 0.0f};  // X方向
        GLfloat shoeFaceTPlane[] = {0.0f, 0.01f, 0.02f, 0.0f};  // Z方向
        glTexGenfv(GL_S, GL_OBJECT_PLANE, shoeFaceSPlane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, shoeFaceTPlane);

        glColor3f(1.0f, 1.0f, 1.0f);  // 白色以显示纹理原色
        // 绘制鞋带
        drawHollowHalfEllipticCylinderX(-20.0f, 37.0f, 0.0f, 22.0f, 45.0f, 17.0f, 40.0f, 70.0f, 30);
        drawSolidCube(-20.0f, 9.0f, -45.0f, 50.0f, 37.0f, -40.0f);
        drawSolidCube(-20.0f, 9.0f, 45.0f, 50.0f, 37.0f, 40.0f);

        // 关闭ShoeFace纹理
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, shoeFaceTexture);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // 设置鞋面纹理坐标生成参数
        GLfloat shoeFaceSsPlane[] = {0.02f, 0.0f, 0.0f, 0.0f};  // X方向
        GLfloat shoeFaceTtPlane[] = {0.0f, 0.0f, 0.02f, 0.0f};  // Z方向
        glTexGenfv(GL_S, GL_OBJECT_PLANE, shoeFaceSsPlane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, shoeFaceTtPlane);

        glColor3f(1.0f, 1.0f, 1.0f);  // 白色以显示纹理原色
        // 绘制鞋带
        drawHollowHalfEllipticCylinderX(-20.0f, 37.0f, 0.0f, 22.0f, 45.0f, 17.0f, 40.0f, 70.0f, 30);

        // 关闭ShoeFace纹理
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);

        // 绘制魔力贴
        // 启用Magic.png纹理
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, magicTexture);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // 设置Magic纹理坐标生成参数
        GLfloat magicSPlane[] = {0.03f, 0.0f, 0.0f, 0.0f};  // X方向
        GLfloat magicTPlane[] = {0.0f, 0.0f, 0.03f, 0.0f};  // Z方向
        glTexGenfv(GL_S, GL_OBJECT_PLANE, magicSPlane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, magicTPlane);

        glColor3f(1.0f, 1.0f, 1.0f);  // 白色以显示纹理原色

        drawSolidCube(-20.0f, 8.0f, -45.0f, 50.0f, 9.0f, -35.0f);
        drawSolidCube(-20.0f, 8.0f, 45.0f, 50.0f, 9.0f, 35.0f);

        // 关闭Magic纹理
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);

/*
 * 主体
 */

    // 绘制鞋子主体
    if (baseShow) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, shoeTexture);

        // 启用自动纹理坐标生成
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);

        // 设置纹理坐标生成模式 - 基于对象坐标的线性映射
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // 设置纹理坐标生成的线性方程参数
        // S坐标 (U方向) - 基于X坐标
        GLfloat sPlane[] = {0.06f, 0.02f, 0.0f, 0.0f};  // 纹理S = 0.01*X
        glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);

        // T坐标 (V方向) - 基于Z坐标
        GLfloat tPlane[] = {0.02f, 0.0f, 0.06f, 0.0f};  // 纹理T = 0.01*Z
        glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);

        // 直接使用原有的draw函数，无需修改
        drawEllipticCylinder(-80.0f, 0.0f, 0.0f, 54.0f, 32.5f, 7.0f, 30);
        drawEllipticCylinder(20.0f, 0.0f, 0.0f, 100.0f, 50.0f, 7.0f, 30);
        drawTrapezoidalPrism(-80.0f, 0.0f, 0.0f, 65.0f, 85.0f, 54.0f, 7.0f);

        // 关闭自动纹理坐标生成
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);


        glColor3f(3, 3, 3);

        // glColor3f(0.5f, 0.5f, 0.5f);

        // 启用Basic.png纹理
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, basicTexture);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // 设置Basic纹理坐标生成参数
        GLfloat basicSPlane[] = {0.02f, 0.01f, 0.005f, 0.0f};  // S主要受X和Z影响
        GLfloat basicTPlane[] = {0.005f, 0.01f, 0.02f, 0.0f};  // T主要受Y影响
        glTexGenfv(GL_S, GL_OBJECT_PLANE, basicSPlane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, basicTPlane);

        glColor3f(1.0f, 1.0f, 1.0f);  // 白色以显示纹理原色

        drawHollowEllipticalCylinder(-72.0f, -13.0f, 0.0f, 72.0f, 26.0f, 59.5f, 0.0f, 5.5f, 5.5f, 5.0f, 30);
        drawHollowEllipticalCylinder(65.0f, -13.0f, 0.0f, 65.0f, 40.0f, -52.5f, 0.0f, 5.5f, 5.5f, 5.0f, 30);
        drawTrapezoidalPrism(-5.0f, -13.0f, 0.0f, 76.0f, 80.0f, 5.0f, 5.0f);
        drawTrapezoidalPrism(-72.0f, -13.0f, 23.0f, 6.0f, 32.0f, 72.0f, 5.0f);
        drawTrapezoidalPrism(-72.0f, -13.0f, -23.0f, 6.0f, 32.0f, 72.0f, 5.0f);
        drawSolidCube(65.0f, -13.0f, -40.0f, 0.0f, -8.0f, 40.0f);
        drawEllipticCylinder(50.0f, -17.0f, 0.0f, 25.0f, 10.0f, 17.0f, 30);
        drawEllipticCylinder(-55.0f, -17.0f, 0.0f, 25.0f, 10.0f, 17.0f, 30);
        drawSolidCube(50.0f, -17.0f, -10.0f, 25.0f, 0.0f, 10.0f);
        drawSolidCube(-55.0f, -17.0f, -10.0f, -30.0f, 0.0f, 10.0f);
        drawHollowEllipticalCylinder(-12.5f, -17.0f, 0.0f, 7.5f, 7.5f, 0.0f, 0.0f, 5.5f, 5.5f, 17.0f, 30);

        // 关闭Basic纹理
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);


        // 绘制魔力贴
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, bindTexture);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        GLfloat SPlane[] = {0.02f, 0.02f, 0.0f, 0.0f};
        GLfloat TPlane[] = {0.0f, 0.02f, 0.02f, 0.0f};
        glTexGenfv(GL_S, GL_OBJECT_PLANE, SPlane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, TPlane);

        glColor3f(1.0f, 1.0f, 1.0f);  // 白色以显示纹理原色
        drawSolidCube(-20.0f, 7.0f, -45.0f, 50.0f, 8.0f, -35.0f);
        drawSolidCube(-20.0f, 7.0f, 45.0f, 50.0f, 8.0f, 35.0f);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);

        // 保存当前变换矩阵
        glPushMatrix();
    }

    // 保存当前变换矩阵
    glPushMatrix();

    // 主体吸铁石
    glColor3f(0.0f, 0.0f, 0.0f);
    drawHollowEllipticalCylinder(-12.5f, -1.0f, 0.0f, 5.5f, 5.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 30);

    if (LosiShow) {
        // =================== 绘制螺丝钉 (带Y轴动画) ===================
        glTranslatef(0.0f, screwOffsetY, 0.0f); // 应用Y轴偏移动画

        // 启用Luosi.png纹理
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, luosiTexture);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // 设置螺丝纹理坐标生成参数（适合圆柱体）
        GLfloat luosiSPlane[] = {0.04f, 0.0f, 0.0f, 0.0f};   // X方向，较密集适合螺纹
        GLfloat luosiTPlane[] = {0.0f, 0.04f, 0.0f, 0.0f};   // Y方向，较密集适合螺纹
        glTexGenfv(GL_S, GL_OBJECT_PLANE, luosiSPlane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, luosiTPlane);

        glColor3f(1.0f, 1.0f, 1.0f);  // 白色以显示纹理原色

        // 绘制螺丝的三个部分（全部带纹理）
        drawEllipticCylinder(-12.5f, -27.0f, 0.0f, 7.5f, 7.5f, 2.0f, 30);
        drawEllipticCylinder(-12.5f, -25.0f, 0.0f, 5.5f, 5.5f, 21.0f, 30);
        drawEllipticCylinder(-12.5f, -4.0f, 0.0f, 5.5f, 5.5f, 4.0f, 30);

        // 关闭螺丝纹理
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);
    }
    // 恢复变换矩阵
    glPopMatrix();



    if (redShow) {
        // =================== 绘制红色插件 (带X轴动画) ===================
        glTranslatef(redPluginOffsetX, 0.0f, 0.0f); // 应用X轴偏移动画

        // ===== 整个红色组件都启用Under.png纹理 =====
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, underTexture);

        // 启用自动纹理坐标生成
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // 使用混合坐标映射
        GLfloat sPlane[] = {0.05f, 0.02f, 0.0f, 0.0f};  // X+Y方向
        GLfloat tPlane[] = {0.0f, 0.02f, 0.05f, 0.0f};  // Y+Z方向
        glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);

        // 设置白色以显示纹理原色
        glColor3f(0.5f, 0.5f, 0.5f);
        // 绘制所有红色组件（全部带Under.png纹理）
        drawHollowHalfEllipticCylinderY(100.0f, 0.0f, 0.0f, 40.0f, 55.0f, -80.0f, 0.0f, 100.0f, 50.0f, 11.0f, 30);
        drawHollowHalfEllipticCylinderY(100.0f, -8.0f, 0.0f, 40.0f, 55.0f, -50.0f, 0.0f, 25.0f, 10.0f, 8.0f, 30);
        drawHollowHalfEllipticCylinderY(100.0f, -13.0f, 0.0f, 40.0f, 55.0f, -35.0f, 0.0f, 65.0f, 40.0f, 5.0f, 30);
        drawHollowHalfEllipticCylinderY(100.0f, -17.0f, 0.0f, 40.0f, 55.0f, -50.0f, 0.0f, 25.0f, 10.0f, 4.0f, 30);
        drawHollowHalfEllipticCylinderY(100.0f, -27.0f, 0.0f, 40.0f, 55.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10.0f, 30);
        drawSolidCube(100.0f, 0.0f, -55.0f, 0.0f, 11.0f, -50.0f);
        drawSolidCube(100.0f, 0.0f, 55.0f, 0.0f, 11.0f, 50.0f);
        drawSolidCube(100.0f, -8.0f, -55.0f, 0.0f, 0.0f, -10.0f);
        drawSolidCube(100.0f, -8.0f, 55.0f, 0.0f, 0.0f, 10.0f);
        drawSolidCube(100.0f, -13.0f, -55.0f, 0.0f, -8.0f, -40.0f);
        drawSolidCube(100.0f, -13.0f, 55.0f, 0.0f, -8.0f, 40.0f);
        drawSolidCube(100.0f, -17.0f, -55.0f, 0.0f, -13.0f, -10.0f);
        drawSolidCube(100.0f, -17.0f, 55.0f, 0.0f, -13.0f, 10.0f);
        drawSolidCube(100.0f, -27.0f, -55.0f, 0.0f, -17.0f, 55.0f);
        drawHollowEllipticalCylinder(-12.5f, -22.0f, 0.0f, 12.5f, 20.0f, 0.0f, 0.0f, 5.5f, 5.5f, 5.0f, 30);
        drawTrapezoidalPrism(-25.0f, -22.0f, 20.0f, 14.0f, 40.0f, 25.0f, 5.0f);
        drawTrapezoidalPrism(-25.0f, -22.0f, -20.0f, 14.0f, 40.0f, 25.0f, 5.0f);
        drawSolidCube(-25.0f, -22.0f, -20.0f, -20.0f, -17.0f, 20.0f);

        // ===== 关闭Under.png纹理 =====
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);

        // 绘制鞋印（带ShoePrint.png纹理）
        // 启用ShoePrint.png纹理
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, shoePrintTexture);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // 设置鞋印纹理坐标生成参数
        GLfloat ssPlane[] = {0.05f, 0.02f, 0.0f, 0.0f};  // X+Y方向
        GLfloat ttPlane[] = {0.0f, 0.02f, 0.05f, 0.0f};  // Y+Z方向
        glTexGenfv(GL_S, GL_OBJECT_PLANE, ssPlane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, ttPlane);

        glColor3f(1.0f, 1.0f, 1.0f);  // 白色以显示纹理原色

        glPushMatrix();
        drawSolidCube(5.0f, -37.0f, -45.0f, 25.0f, -27.0f, -5.0f);
        drawSolidCube(5.0f, -37.0f, 45.0f, 25.0f, -27.0f, 5.0f);
        drawSolidCube(30.0f, -37.0f, -45.0f, 50.0f, -27.0f, -5.0f);
        drawSolidCube(30.0f, -37.0f, 45.0f, 50.0f, -27.0f, 5.0f);
        drawSolidCube(55.0f, -37.0f, -45.0f, 75.0f, -27.0f, -5.0f);
        drawSolidCube(55.0f, -37.0f, 45.0f, 75.0f, -27.0f, 5.0f);
        drawSolidCube(80.0f, -37.0f, -45.0f, 100.0f, -27.0f, -5.0f);
        drawSolidCube(80.0f, -37.0f, 45.0f, 100.0f, -27.0f, 5.0f);
        drawHollowHalfEllipticCylinderY(110.0f, -37.0f, 0.0f, 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10.0f, 30);
        glPopMatrix();

        // 关闭鞋印纹理
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);
    }

    // 恢复变换矩阵
    glPopMatrix();

    // 保存当前变换矩阵
    glPushMatrix();

    if (yellowShow) {
        // =================== 绘制黄色插件 (带X轴动画) ===================
        glTranslatef(yellowPluginOffsetX, 0.0f, 0.0f); // 应用X轴偏移动画

        // ===== 整个黄色组件都启用Under.png纹理 =====
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, underTexture);

        // 启用自动纹理坐标生成
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // 使用混合坐标映射
        GLfloat sPlane[] = {0.05f, 0.02f, 0.0f, 0.0f};  // X+Y方向
        GLfloat tPlane[] = {0.0f, 0.02f, 0.05f, 0.0f};  // Y+Z方向
        glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);

        // 设置白色以显示纹理原色
        glColor3f(0.5f, 0.5f, 0.5f);

        // 绘制所有黄色组件（全部带Under.png纹理）
        drawHollowHalfEllipticCylinderYNeg(-114.0f, 0.0f, 0.0f, 40.0f, 37.5f, 34.0f, 0.0f, 54.0f, 32.5f, 11.0f, 30);
        drawHollowHalfEllipticCylinderYNeg(-114.0f, -8.0f, 0.0f, 40.0f, 37.5f, 59.0f, 0.0f, 25.0f, 10.0f, 8.0f, 30);
        drawHollowHalfEllipticCylinderYNeg(-114.0f, -13.0f, 0.0f, 40.0f, 37.5f, 42.0f, 0.0f, 72.0f, 26.0f, 5.0f, 30);
        drawHollowHalfEllipticCylinderYNeg(-114.0f, -17.0f, 0.0f, 40.0f, 37.5f, 59.0f, 0.0f, 25.0f, 10.0f, 4.0f, 30);
        drawHollowHalfEllipticCylinderYNeg(-114.0f, -27.0f, 0.0f, 40.0f, 37.5f, 0.0f, 0.0f, 0.0f, 0.0f, 10.0f, 30);

        drawHexahedron(-80.0f, -27.0f, 32.5f, 0.0f, -27.0f, 50.0f, -114.0f, -27.0f, 37.5f, -80.0f, 11.0f, 32.5f);
        drawHexahedron(-80.0f, -27.0f, -32.5f, -114.0f, -27.0f, -37.5f, 0.0f, -27.0f, -50.0f, -80.0f, 11.0f, -32.5f);

        drawSolidCube(-114.0f, -8.0f, 37.5f, 0.0f, 0.0f, 10.0f);
        drawSolidCube(-114.0f, -8.0f, -37.5f, 0.0f, 0.0f, -10.f);
        drawSolidCube(-114.0f, -13.0f, 37.5f, 0.0f, -8.0f, 26.0f);
        drawSolidCube(-114.0f, -13.0f, -37.5f, 0.0f, -8.0f, -26.0f);
        drawSolidCube(-114.0f, -17.0f, 37.5f, 0.0f, -13.0f, 10.0f);
        drawSolidCube(-114.0f, -17.0f, -37.5f, 0.0f, -13.0f, -10.0f);
        drawSolidCube(-114.0f, -22.0f, -37.5f, -25.0f, -17.0f, 37.5f);
        drawSolidCube(-114.0f, -27.0f, 37.5f, -20.0f, -22.0f, -37.5f);
        drawSolidCube(-5.0f, -27.0f, -10.0f, 0.0f, -25.0f, 10.0f);
        drawSolidCube(-5.0f, -25.0f, -10.0f, 0.0f, -22.0f, 10.0f);
        drawSolidCube(0.0f, 11.0f, 55.0f, -34.5f, -27.0f, 50.0f);
        drawSolidCube(0.0f, 11.0f, -55.0f, -34.5f, -27.0f, -50.0f);

        drawHexahedron(0.0f, -17.0f, 37.5f, 0.0f, 0.0f, 37.5f, 0.0f, -17.0f, 55.0f, -80.0f, -17.0f, 20.0f);
        drawHexahedron(0.0f, -17.0f, -37.5f, 0.0f, 0.0f, -37.5f, -80.0f ,-17.0f, -20.0f, 0.0f, -17.0f, -55.0f);
        drawHexahedron(0.0f, -22.0f, 40.0f, 0.0f, -17.0f, 40.0f, 0.0f, -22.0f, 55.0f, -25.0f, -22.0f, 26.0f);
        drawHexahedron(0.0f, -22.0f, -40.0f, 0.0f, -17.0f, -40.0f, -25.0f, -22.0f, -26.0f, 0.0f, -22.0f, -55.0f);
        drawHexahedron(0.0f, -22.0f, 40.0f, 0.0f, -17.0f, 40.0f, 0.0f, -22.0f, 55.0f, -60.0f, -22.0f, 32.0f);
        drawHexahedron(0.0f, -22.0f, -40.0f, 0.0f, -17.0f, -40.0f, -60.0f, -22.0f, -32.0f, 0.0f, -22.0f, -55.0f);

        drawHollowEllipticalCylinder(-12.5f, -27.0f, 0.0f, 12.5f, 30.0f, 0.0f, 0.0f, 7.5f, 7.5f, 2.0f, 30);
        drawHollowEllipticalCylinder(-12.5f, -25.0f, 0.0f, 12.5f, 30.0f, 0.0f, 0.0f, 5.5f, 5.5f, 3.0f, 30);

        drawTrapezoidalPrism(-60.0f, -27.0f, 30.0f, 30.0f, 45.0f, 60.0f, 2.0f);
        drawTrapezoidalPrism(-60.0f, -27.0f, -30.0f, 30.0f, 45.0f, 60.0f, 2.0f);
        drawTrapezoidalPrism(-60.0f, -25.0f, 30.0f, 30.0f, 45.0f, 60.0f, 3.0f);
        drawTrapezoidalPrism(-60.0f, -25.0f, -30.0f, 30.0f, 45.0f, 60.0f, 3.0f);

        // ===== 关闭Under.png纹理 =====
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);

        // 绘制鞋印（带ShoePrint.png纹理）
        // 先关闭之前的Under.png纹理
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);

        // 启用ShoePrint.png纹理
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, shoePrintTexture);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // 设置鞋印纹理坐标生成参数
        GLfloat ssPlane[] = {0.05f, 0.02f, 0.0f, 0.0f};  // X+Y方向
        GLfloat ttPlane[] = {0.0f, 0.02f, 0.05f, 0.0f};  // Y+Z方向
        glTexGenfv(GL_S, GL_OBJECT_PLANE, ssPlane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, ttPlane);

        glColor3f(3.0f, 3.0f, 3.0f);  // 白色以显示纹理原色

        glPushMatrix();
        drawSolidCube(-5.0f, -37.0f, -15.0f, -25.0f, -27.0f, -50.0f);
        drawSolidCube(-5.0f, -37.0f, 15.0f, -25.0f, -27.0f, 50.0f);
        drawSolidCube(-30.0f, -37.0f, -35.0f, -50.0f, -27.0f, 35.0f);
        drawSolidCube(-55.0f, -37.0f, -35.0f, -75.0f, -27.0f, 35.0f);
        drawSolidCube(-80.0f, -37.0f, -35.0f, -100.0f, -27.0f, 35.0f);
        drawHollowHalfEllipticCylinderYNeg(-105.0f, -37.0f, 0.0f, 40.0f, 35.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10.0f, 30);
        glPopMatrix();

        // 关闭鞋印纹理
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);

    }
    // 恢复变换矩阵
    glPopMatrix();


    glutSwapBuffers();
}

// 窗口大小改变回调
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // 设置透视投影，适合观察椭圆柱体
    gluPerspective(45.0, (double)width / (double)height, 1.0, 500.0);

    glMatrixMode(GL_MODELVIEW);
}

// 鼠标按下回调
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mousePressed = 1;
            lastMouseX = x;
            lastMouseY = y;
        } else {
            mousePressed = 0;
        }
    }
}

// 鼠标移动回调
void mouseMotion(int x, int y) {
    if (mousePressed) {
        rotateY += (x - lastMouseX) * 0.5f;
        rotateX += (y - lastMouseY) * 0.5f;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}

// 键盘回调函数 (增强版本)
void keyboard(unsigned char key, int x, int y) {
    const float moveStep = 3.0f;  // 移动步长

    switch (key) {

        // 红色插件控制
        case 'r':
        case 'R':
            redShow = !redShow;
            break;
        case 'w':
        case 'W':
            redPluginOffsetX += moveStep;
            if(redPluginOffsetX > 200.0f) redPluginOffsetX = 200.0f;
            //printf("红色插件X坐标: %.1f\n", redPluginOffsetX);
            break;

        case 's':
        case 'S':
            redPluginOffsetX -= moveStep;
            if(redPluginOffsetX < 0.0f) redPluginOffsetX = 0.0f;
            //printf("红色插件X坐标: %.1f\n", redPluginOffsetX);
            break;

        // 黄色插件控制
        case 'y':
        case 'Y':
            yellowShow = !yellowShow;
            break;
        case 'e':
        case 'E':
            yellowPluginOffsetX += moveStep;
            if(yellowPluginOffsetX > 0.0f) yellowPluginOffsetX = 0.0f;
            //printf("黄色插件X坐标: %.1f\n", yellowPluginOffsetX);
            break;

        case 'd':
        case 'D':
            yellowPluginOffsetX -= moveStep;
            if(yellowPluginOffsetX < -200.0f) yellowPluginOffsetX = -200.0f;
            //printf("黄色插件X坐标: %.1f\n", yellowPluginOffsetX);
            break;

        // 螺丝钉控制
        case 'l':
        case 'L':
            LosiShow = !LosiShow;
            break;
        case 'f':
        case 'F':
            screwDirection = !screwDirection;
            printf("螺丝钉模式: %s\n", screwDirection ? "插入" : "拔出");
            break;
        // 主体控制
        case 'b':
        case 'B':
            baseShow = !baseShow;
            break;
        // 重置所有动画
        case 'c':
        case 'C':
            redPluginOffsetX = 0.0f;
            yellowPluginOffsetX = 0.0f;
            screwOffsetY = 0.0f;
            screwDirection = false;
            shoeFaceOffsetY = 0.0f;
            shoeFaceDirection = false;
            printf("所有动画已重置\n");
            break;
        case 'm':
        case 'M':
            ShoeFace = !ShoeFace;
            break;
        case 'g':
        case 'G':
            shoeFaceDirection = !shoeFaceDirection;
            printf("鞋面动画模式: %s\n", shoeFaceDirection ? "向上" : "向下");
            break;

        case 27:  // ESC键退出
            printf("程序退出\n");
            exit(0);
            break;
    }

    glutPostRedisplay();
}

// 初始化函数
void init() {
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);  // 深蓝色背景
    glEnable(GL_DEPTH_TEST);

    // 启用光照
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);

    // 设置光源
    GLfloat lightPos[] = {100.0f, 100.0f, 100.0f, 1.0f};
    GLfloat lightAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    // 设置材质
    GLfloat matAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat matDiffuse[] = {0.8f, 0.6f, 0.2f, 1.0f};
    GLfloat matSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat matShininess[] = {50.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    // *** 添加这一行：加载纹理 ***
    loadTextures();
}

// 主函数
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 750);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("3D鞋子动画 - 椭圆柱体模型");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);

    // 启动螺丝钉动画定时器
    glutTimerFunc(50, screwTimer, 0);
    glutTimerFunc(60, shoeFaceTimer, 0);  // 新增：鞋面动画

    // 打印控制说明
    printf("=== 3D鞋子动画控制说明 ===\n");
    printf("鼠标控制:\n");
    printf("- 鼠标左键拖拽: 旋转视角\n\n");
    printf("键盘动画控制:\n");
    printf("- W/w: 红色插件向前移动 \n");
    printf("- S/s: 红色插件向后移动 \n");
    printf("- E/e: 黄色插件向前移动 \n");
    printf("- D/d: 黄色插件向后移动 \n");
    printf("- F/f: 螺丝钉插入/拔出切换\n");
    printf("- G/g: 鞋面插入/拔出\n\n");
    printf("- R/r: 开启/关闭红色插件\n");
    printf("- Y/y: 开启/关闭黄色插件\n");
    printf("- L/l: 开启/关闭螺丝钉\n");
    printf("- B/b: 开启/关闭主体\n");
    printf("- M/m: 开启/关闭鞋面\n");
    printf("- ESC: 退出程序\n");
    printf("================================\n\n");

    glutMainLoop();
    return 0;
}

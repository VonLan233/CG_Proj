#include <GLUT/glut.h>
#include <cmath>
#include <iostream>
#include <vector>
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

// 窗口尺寸
const int WINDOW_WIDTH = 1600;
const int WINDOW_HEIGHT = 800;

// 相机控制变量
float cameraAngleX = 15.0f;
float cameraAngleY = 25.0f;
float cameraDistance = 20.0f;
bool mousePressed = false;
int lastMouseX = 0, lastMouseY = 0;

// 模块控制
bool showLeftModule = true;
bool showRightModule = true;
bool showTopModule = true;
float animationTime = 0.0f;

// 添加以下动画控制变量
float leftModuleOffset = 0.0f;
float rightModuleOffset = 0.0f;
float topModuleOffset = 0.0f;
float targetLeftOffset = 0.0f;
float targetRightOffset = 0.0f;
float targetTopOffset = 0.0f;
bool animateLeft = false;
bool animateRight = false;
bool animateTop = false;

// 光照参数
GLfloat lightPosition[] = {8.0f, 8.0f, 8.0f, 1.0f};
GLfloat lightAmbient[] = {0.4f, 0.4f, 0.4f, 1.0f};
GLfloat lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};

// 材质结构体
struct Material {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
};

// 定义不同的材质
Material keyboardMaterial = {
    {0.15f, 0.15f, 0.15f, 1.0f},  // 深灰色底座
    {0.25f, 0.25f, 0.25f, 1.0f},
    {0.4f, 0.4f, 0.4f, 1.0f},
    32.0f
};

Material keycapMaterial = {
    {0.8f, 0.8f, 0.8f, 1.0f},     // 浅灰色键帽
    {0.9f, 0.9f, 0.9f, 1.0f},
    {0.95f, 0.95f, 0.95f, 1.0f},
    64.0f
};

Material leftModuleMaterial = {
    {0.1f, 0.15f, 0.2f, 1.0f},    // 蓝灰色左模块
    {0.15f, 0.2f, 0.3f, 1.0f},
    {0.3f, 0.4f, 0.5f, 1.0f},
    32.0f
};

Material rightModuleMaterial = {
    {0.2f, 0.1f, 0.1f, 1.0f},     // 红灰色右模块
    {0.3f, 0.15f, 0.15f, 1.0f},
    {0.5f, 0.3f, 0.3f, 1.0f},
    32.0f
};

Material topModuleMaterial = {
    {0.1f, 0.2f, 0.15f, 1.0f},    // 绿灰色上模块
    {0.15f, 0.3f, 0.2f, 1.0f},
    {0.3f, 0.5f, 0.4f, 1.0f},
    32.0f
};

Material buttonMaterial = {
    {0.5f, 0.7f, 0.5f, 1.0f},     // 绿色按钮
    {0.6f, 0.8f, 0.6f, 1.0f},
    {0.8f, 0.9f, 0.8f, 1.0f},
    64.0f
};

Material connectorMaterial = {
    {0.6f, 0.6f, 0.1f, 1.0f},     // 金色连接器
    {0.8f, 0.8f, 0.2f, 1.0f},
    {0.9f, 0.9f, 0.4f, 1.0f},
    128.0f
};

Material shellMaterial = {
    {0.1f, 0.1f, 0.12f, 1.0f},    // 深色外壳
    {0.18f, 0.18f, 0.22f, 1.0f},
    {0.3f, 0.3f, 0.35f, 1.0f},
    64.0f
};

Material accentMaterial = {
    {0.05f, 0.15f, 0.25f, 1.0f},  // 蓝色装饰
    {0.1f, 0.3f, 0.5f, 1.0f},
    {0.2f, 0.5f, 0.8f, 1.0f},
    128.0f
};

// 设置材质
void setMaterial(const Material& mat) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat.specular);
    glMaterialf(GL_FRONT, GL_SHININESS, mat.shininess);
}

// 绘制立方体
void drawCube(float width, float height, float depth) {
    glPushMatrix();
    glScalef(width, height, depth);
    glutSolidCube(1.0);
    glPopMatrix();
}

// 绘制圆柱体
void drawCylinder(float radius, float height) {
    GLUquadric* quad = gluNewQuadric();
    glPushMatrix();
    glTranslatef(0, -height/2, 0);
    gluCylinder(quad, radius, radius, height, 20, 1);
    // 添加圆柱体的上下盖
    gluDisk(quad, 0, radius, 20, 1);
    glTranslatef(0, 0, height);
    gluDisk(quad, 0, radius, 20, 1);
    glPopMatrix();
    gluDeleteQuadric(quad);
}

// 绘制梯形（用于键盘主体的倾斜设计）
void drawTrapezoid(float bottomWidth, float topWidth, float height, float depth) {
    glBegin(GL_QUADS);
    
    float halfBottomWidth = bottomWidth / 2.0f;
    float halfTopWidth = topWidth / 2.0f;
    float halfDepth = depth / 2.0f;
    float halfHeight = height / 2.0f;
    
    // 前面 (梯形)
    glNormal3f(0, 0, 1);
    glVertex3f(-halfBottomWidth, -halfHeight, halfDepth);
    glVertex3f(halfBottomWidth, -halfHeight, halfDepth);
    glVertex3f(halfTopWidth, halfHeight, halfDepth);
    glVertex3f(-halfTopWidth, halfHeight, halfDepth);
    
    // 后面 (梯形)
    glNormal3f(0, 0, -1);
    glVertex3f(-halfBottomWidth, -halfHeight, -halfDepth);
    glVertex3f(-halfTopWidth, halfHeight, -halfDepth);
    glVertex3f(halfTopWidth, halfHeight, -halfDepth);
    glVertex3f(halfBottomWidth, -halfHeight, -halfDepth);
    
    // 左侧面 (梯形)
    glNormal3f(-1, 0, 0);
    glVertex3f(-halfBottomWidth, -halfHeight, -halfDepth);
    glVertex3f(-halfBottomWidth, -halfHeight, halfDepth);
    glVertex3f(-halfTopWidth, halfHeight, halfDepth);
    glVertex3f(-halfTopWidth, halfHeight, -halfDepth);
    
    // 右侧面 (梯形)
    glNormal3f(1, 0, 0);
    glVertex3f(halfBottomWidth, -halfHeight, -halfDepth);
    glVertex3f(halfTopWidth, halfHeight, -halfDepth);
    glVertex3f(halfTopWidth, halfHeight, halfDepth);
    glVertex3f(halfBottomWidth, -halfHeight, halfDepth);
    
    // 顶面
    glNormal3f(0, 1, 0);
    glVertex3f(-halfTopWidth, halfHeight, -halfDepth);
    glVertex3f(-halfTopWidth, halfHeight, halfDepth);
    glVertex3f(halfTopWidth, halfHeight, halfDepth);
    glVertex3f(halfTopWidth, halfHeight, -halfDepth);
    
    // 底面
    glNormal3f(0, -1, 0);
    glVertex3f(-halfBottomWidth, -halfHeight, -halfDepth);
    glVertex3f(halfBottomWidth, -halfHeight, -halfDepth);
    glVertex3f(halfBottomWidth, -halfHeight, halfDepth);
    glVertex3f(-halfBottomWidth, -halfHeight, halfDepth);
    
    glEnd();
}

// 绘制不规则四边形外壳（用于模块的特殊造型）
void drawCustomShape(float width1, float width2, float height, float depth1, float depth2) {
    glBegin(GL_QUADS);
    
    float halfHeight = height / 2.0f;
    
    // 前面 - 较窄
    glNormal3f(0, 0, 1);
    glVertex3f(-width2/2, -halfHeight, depth1/2);
    glVertex3f(width2/2, -halfHeight, depth1/2);
    glVertex3f(width2/2, halfHeight, depth1/2);
    glVertex3f(-width2/2, halfHeight, depth1/2);
    
    // 后面 - 较宽
    glNormal3f(0, 0, -1);
    glVertex3f(-width1/2, -halfHeight, -depth2/2);
    glVertex3f(-width1/2, halfHeight, -depth2/2);
    glVertex3f(width1/2, halfHeight, -depth2/2);
    glVertex3f(width1/2, -halfHeight, -depth2/2);
    
    // 左侧面
    glNormal3f(-1, 0, 0);
    glVertex3f(-width1/2, -halfHeight, -depth2/2);
    glVertex3f(-width2/2, -halfHeight, depth1/2);
    glVertex3f(-width2/2, halfHeight, depth1/2);
    glVertex3f(-width1/2, halfHeight, -depth2/2);
    
    // 右侧面
    glNormal3f(1, 0, 0);
    glVertex3f(width1/2, -halfHeight, -depth2/2);
    glVertex3f(width1/2, halfHeight, -depth2/2);
    glVertex3f(width2/2, halfHeight, depth1/2);
    glVertex3f(width2/2, -halfHeight, depth1/2);
    
    // 顶面
    glNormal3f(0, 1, 0);
    glVertex3f(-width1/2, halfHeight, -depth2/2);
    glVertex3f(-width2/2, halfHeight, depth1/2);
    glVertex3f(width2/2, halfHeight, depth1/2);
    glVertex3f(width1/2, halfHeight, -depth2/2);
    
    // 底面
    glNormal3f(0, -1, 0);
    glVertex3f(-width1/2, -halfHeight, -depth2/2);
    glVertex3f(width1/2, -halfHeight, -depth2/2);
    glVertex3f(width2/2, -halfHeight, depth1/2);
    glVertex3f(-width2/2, -halfHeight, depth1/2);
    
    glEnd();
}

// 绘制球体
void drawSphere(float radius) {
    glutSolidSphere(radius, 20, 20);
}

// 绘制连接器
void drawConnector(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    setMaterial(connectorMaterial);
    
    // 连接器主体
    drawCube(0.3f, 0.2f, 0.8f);
    
    // 连接器引脚
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glTranslatef(-0.1f + i * 0.1f, 0, 0.45f);
        drawCube(0.05f, 0.05f, 0.1f);
        glPopMatrix();
    }
    
    glPopMatrix();
}

// 绘制直角梯形主键盘外壳（后大前小）
void drawMainKeyboardShell() {
    setMaterial(shellMaterial);
    
    glPushMatrix();
    glTranslatef(0, -0.2f, 0);
    
    // 直角梯形外壳 - 后大前小
    glBegin(GL_QUADS);
    
    float backWidth = 17.0f;     // 后面宽度（较宽）
    float frontWidth = 10.0f;    // 前面宽度（较窄）
    float depth = 7.0f;          // 深度
    float height = 1.0f;         // 高度
    
    float halfBackWidth = backWidth / 2.0f;
    float halfFrontWidth = halfBackWidth;
    float halfDepth = depth / 2.0f;
    float halfHeight = height / 2.0f;
    // 底面（平面，梯形）- 这是基底
    glNormal3f(0, -1, 0);
    glVertex3f(-halfBackWidth, -halfHeight, -halfDepth);     // 后左
    glVertex3f(halfBackWidth, -halfHeight, -halfDepth);      // 后右
    glVertex3f(halfFrontWidth, -halfHeight, halfDepth);      // 前右
    glVertex3f(-halfFrontWidth, -halfHeight, halfDepth);     // 前左
    
    // 顶面（平面，梯形）
    glNormal3f(0, 1, 0);
    glVertex3f(-halfBackWidth, halfHeight, -halfDepth);      // 后左
    glVertex3f(-halfFrontWidth, halfHeight/2, halfDepth);    // 前左
    glVertex3f(halfFrontWidth, halfHeight/2, halfDepth);     // 前右
    glVertex3f(halfBackWidth, halfHeight, -halfDepth);       // 后右
    
    // 前面（较窄的矩形侧面）
    glNormal3f(0, 0, 1);
    glVertex3f(-halfFrontWidth, -halfHeight, halfDepth);
    glVertex3f(halfFrontWidth, -halfHeight, halfDepth);
    glVertex3f(halfFrontWidth, halfHeight/2, halfDepth);
    glVertex3f(-halfFrontWidth, halfHeight/2, halfDepth);

    
    
    // 后面（较宽的矩形侧面）
    glNormal3f(0, 0, -1);
    glVertex3f(-halfBackWidth, -halfHeight, -halfDepth);
    glVertex3f(-halfBackWidth, halfHeight, -halfDepth);
    glVertex3f(halfBackWidth, halfHeight, -halfDepth);
    glVertex3f(halfBackWidth, -halfHeight, -halfDepth);
    
    // 左侧面（矩形侧面）
    glNormal3f(-1, 0, 0);
    glVertex3f(-halfBackWidth, -halfHeight, -halfDepth);
    glVertex3f(-halfFrontWidth, -halfHeight, halfDepth);
    glVertex3f(-halfFrontWidth, halfHeight/2, halfDepth);
    glVertex3f(-halfBackWidth, halfHeight, -halfDepth);
    
    // 右侧面（矩形侧面）
    glNormal3f(1, 0, 0);
    glVertex3f(halfBackWidth, -halfHeight, -halfDepth);
    glVertex3f(halfBackWidth, halfHeight, -halfDepth);
    glVertex3f(halfFrontWidth, halfHeight/2, halfDepth);
    glVertex3f(halfFrontWidth, -halfHeight, halfDepth);

    // 左腕托左侧面
    glNormal3f(-1, 0, 0);
    glVertex3f(-halfBackWidth, -halfHeight, halfDepth);
    glVertex3f(-halfFrontWidth, -halfHeight, halfDepth+1.0f);
    glVertex3f(-halfFrontWidth, halfHeight/3, halfDepth+1.0f);
    glVertex3f(-halfBackWidth, halfHeight/2, halfDepth);

    // 左腕托前面
    glNormal3f(0, 0, 1);
    glVertex3f(-halfBackWidth, -halfHeight, halfDepth+1.0f);
    glVertex3f(-halfFrontWidth+4.0f, -halfHeight, halfDepth+1.0f);
    glVertex3f(-halfFrontWidth+4.0f, halfHeight/3, halfDepth+1.0f);
    glVertex3f(-halfBackWidth, halfHeight/3, halfDepth+1.0f);


    // 左腕托顶面
    glNormal3f(0, 1, 0);
    glVertex3f(-halfBackWidth, halfHeight/2, halfDepth);
    glVertex3f(-halfFrontWidth, halfHeight/3, halfDepth+1.0f);
    glVertex3f(-halfFrontWidth+4.0f, halfHeight/3, halfDepth+1.0f);
    glVertex3f(-halfBackWidth+5.0f, halfHeight/2, halfDepth);

        // 左腕托底面
    glNormal3f(0, -1, 0);
    glVertex3f(-halfBackWidth, -halfHeight, halfDepth);
    glVertex3f(-halfFrontWidth, -halfHeight, halfDepth+1.0f);
    glVertex3f(-halfFrontWidth+4.0f, -halfHeight, halfDepth+1.0f);
    glVertex3f(-halfBackWidth+5.0f, -halfHeight, halfDepth);

    // 左腕托右侧面
    glNormal3f(1, 0, 0);
    glVertex3f(-halfBackWidth+5.0f, -halfHeight, halfDepth);
    glVertex3f(-halfBackWidth+5.0f, halfHeight/2, halfDepth);
    glVertex3f(-halfFrontWidth+4.0f, halfHeight/3, halfDepth+1.0f);
    glVertex3f(-halfFrontWidth+4.0f, -halfHeight, halfDepth+1.0f);

        // 右腕托右侧面
    glNormal3f(1, 0, 0);
    glVertex3f(halfBackWidth, -halfHeight, halfDepth);
    glVertex3f(halfBackWidth, halfHeight/2, halfDepth);
    glVertex3f(halfFrontWidth, halfHeight/3, halfDepth+1.0f);
    glVertex3f(halfFrontWidth, -halfHeight, halfDepth+1.0f);

    // 右腕托前面
    glNormal3f(0, 0, 1);
    glVertex3f(halfBackWidth-4.0f, -halfHeight, halfDepth+1.0f);
    glVertex3f(halfFrontWidth, -halfHeight, halfDepth+1.0f);
    glVertex3f(halfFrontWidth, halfHeight/3, halfDepth+1.0f);
    glVertex3f(halfBackWidth-4.0f, halfHeight/3, halfDepth+1.0f);

    // 右腕托顶面
    glNormal3f(0, 1, 0);
    glVertex3f(halfBackWidth, halfHeight/2, halfDepth);
    glVertex3f(halfBackWidth-5.0f, halfHeight/2, halfDepth);
    glVertex3f(halfFrontWidth-4.0f, halfHeight/3, halfDepth+1.0f);
    glVertex3f(halfFrontWidth, halfHeight/3, halfDepth+1.0f);

    // 右腕托底面
    glNormal3f(0, -1, 0);
    glVertex3f(halfBackWidth, -halfHeight, halfDepth);
    glVertex3f(halfFrontWidth, -halfHeight, halfDepth+1.0f);
    glVertex3f(halfFrontWidth-4.0f, -halfHeight, halfDepth+1.0f);
    glVertex3f(halfBackWidth-5.0f, -halfHeight, halfDepth);

    // 右腕托左侧面（内侧）
    glNormal3f(-1, 0, 0);
    glVertex3f(halfBackWidth-5.0f, -halfHeight, halfDepth);
    glVertex3f(halfFrontWidth-4.0f, -halfHeight, halfDepth+1.0f);
    glVertex3f(halfFrontWidth-4.0f, halfHeight/3, halfDepth+1.0f);
    glVertex3f(halfBackWidth-5.0f, halfHeight/2, halfDepth);

    glEnd();
    
    glPopMatrix();
}


// 绘制左侧模块外壳（类似主键盘的直角梯形设计）
void drawLeftModuleShell() {
    if (!showLeftModule) return;
    
    glPushMatrix();
    glTranslatef(-10.0f, 0, 0);
    
    setMaterial(leftModuleMaterial);
    
    glPushMatrix();
    glTranslatef(0, -0.2f, 0);
    
    // 左侧模块的直角梯形外壳 - 后大前小（与主键盘类似）
    glBegin(GL_QUADS);
    
    float backWidth = 3.0f;      // 后面宽度（较宽）
    float frontWidth = 3.0f;     // 前面宽度（较窄）
    float depth = 7.0f;          // 深度
    float height = 1.0f;         // 高度
    
    float halfBackWidth = backWidth / 2.0f;
    float halfFrontWidth = frontWidth / 2.0f;
    float halfDepth = depth / 2.0f;
    float halfHeight = height / 2.0f;
    
    // 底面（平面，梯形）- 这是基底
    glNormal3f(0, -1, 0);
    glVertex3f(-halfBackWidth, -halfHeight, -halfDepth);     // 后左
    glVertex3f(halfBackWidth, -halfHeight, -halfDepth);      // 后右
    glVertex3f(halfFrontWidth, -halfHeight, halfDepth);      // 前右
    glVertex3f(-halfFrontWidth, -halfHeight, halfDepth);     // 前左
    
    // 顶面（平面，梯形）
    glNormal3f(0, 1, 0);
    glVertex3f(-halfBackWidth, halfHeight, -halfDepth);      // 后左
    glVertex3f(-halfFrontWidth, halfHeight/2, halfDepth);    // 前左
    glVertex3f(halfFrontWidth, halfHeight/2, halfDepth);     // 前右
    glVertex3f(halfBackWidth, halfHeight, -halfDepth);       // 后右
    
    // 前面（较窄的矩形侧面）
    glNormal3f(0, 0, 1);
    glVertex3f(-halfFrontWidth, -halfHeight, halfDepth);
    glVertex3f(halfFrontWidth, -halfHeight, halfDepth);
    glVertex3f(halfFrontWidth, halfHeight/2, halfDepth);
    glVertex3f(-halfFrontWidth, halfHeight/2, halfDepth);
    
    // 后面（较宽的矩形侧面）
    glNormal3f(0, 0, -1);
    glVertex3f(-halfBackWidth, -halfHeight, -halfDepth);
    glVertex3f(-halfBackWidth, halfHeight, -halfDepth);
    glVertex3f(halfBackWidth, halfHeight, -halfDepth);
    glVertex3f(halfBackWidth, -halfHeight, -halfDepth);
    
    // 左侧面（梯形侧面）
    glNormal3f(-1, 0, 0);
    glVertex3f(-halfBackWidth, -halfHeight, -halfDepth);
    glVertex3f(-halfFrontWidth, -halfHeight, halfDepth);
    glVertex3f(-halfFrontWidth, halfHeight/2, halfDepth);
    glVertex3f(-halfBackWidth, halfHeight, -halfDepth);
    
    // 右侧面（梯形侧面）
    glNormal3f(1, 0, 0);
    glVertex3f(halfBackWidth, -halfHeight, -halfDepth);
    glVertex3f(halfBackWidth, halfHeight, -halfDepth);
    glVertex3f(halfFrontWidth, halfHeight/2, halfDepth);
    glVertex3f(halfFrontWidth, -halfHeight, halfDepth);
    
    glEnd();
    
    // 添加轨迹球专用凹槽
    // glPushMatrix();
    // glTranslatef(0, halfHeight/2 + 0.1f, -1.0f);
    // setMaterial(accentMaterial);
    // drawCylinder(1.0f, 0.15f);  // 轨迹球凹槽
    // glPopMatrix();
    
    // 添加前端控制区域凹槽
    // glPushMatrix();
    // glTranslatef(0, halfHeight/2 + 0.05f, 1.0f);
    // setMaterial(accentMaterial);
    // drawCube(3.0f, 0.1f, 2.0f);  // 按钮控制区域
    // glPopMatrix();
    
    glPopMatrix();
    
    glPopMatrix();
}

// 绘制右侧模块外壳（类似主键盘的直角梯形设计）
void drawRightModuleShell() {
    if (!showRightModule) return;
    
    glPushMatrix();
    glTranslatef(10.25f, 0, 0);
    
    setMaterial(rightModuleMaterial);
    
    glPushMatrix();
    glTranslatef(0, -0.2f, 0);
    
    // 右侧模块的直角梯形外壳 - 后大前小（与主键盘类似）
    glBegin(GL_QUADS);
    
    float backWidth =3.5f;      // 后面宽度（较宽）
    float frontWidth = 3.5f;     // 前面宽度（较窄）
    float depth = 7.0f;          // 深度
    float height = 1.0f;         // 高度
    
    float halfBackWidth = backWidth / 2.0f;
    float halfFrontWidth = frontWidth / 2.0f;
    float halfDepth = depth / 2.0f;
    float halfHeight = height / 2.0f;
    
    // 底面（平面，梯形）- 这是基底
    glNormal3f(0, -1, 0);
    glVertex3f(-halfBackWidth, -halfHeight, -halfDepth);     // 后左
    glVertex3f(halfBackWidth, -halfHeight, -halfDepth);      // 后右
    glVertex3f(halfFrontWidth, -halfHeight, halfDepth);      // 前右
    glVertex3f(-halfFrontWidth, -halfHeight, halfDepth);     // 前左
    
    // 顶面（平面，梯形）
    glNormal3f(0, 1, 0);
    glVertex3f(-halfBackWidth, halfHeight, -halfDepth);      // 后左
    glVertex3f(-halfFrontWidth, halfHeight/2, halfDepth);    // 前左
    glVertex3f(halfFrontWidth, halfHeight/2, halfDepth);     // 前右
    glVertex3f(halfBackWidth, halfHeight, -halfDepth);       // 后右
    
    // 前面（较窄的矩形侧面）
    glNormal3f(0, 0, 1);
    glVertex3f(-halfFrontWidth, -halfHeight, halfDepth);
    glVertex3f(halfFrontWidth, -halfHeight, halfDepth);
    glVertex3f(halfFrontWidth, halfHeight/2, halfDepth);
    glVertex3f(-halfFrontWidth, halfHeight/2, halfDepth);
    
    // 后面（较宽的矩形侧面）
    glNormal3f(0, 0, -1);
    glVertex3f(-halfBackWidth, -halfHeight, -halfDepth);
    glVertex3f(-halfBackWidth, halfHeight, -halfDepth);
    glVertex3f(halfBackWidth, halfHeight, -halfDepth);
    glVertex3f(halfBackWidth, -halfHeight, -halfDepth);
    
    // 左侧面（梯形侧面）
    glNormal3f(-1, 0, 0);
    glVertex3f(-halfBackWidth, -halfHeight, -halfDepth);
    glVertex3f(-halfFrontWidth, -halfHeight, halfDepth);
    glVertex3f(-halfFrontWidth, halfHeight/2, halfDepth);
    glVertex3f(-halfBackWidth, halfHeight, -halfDepth);
    
    // 右侧面（梯形侧面）
    glNormal3f(1, 0, 0);
    glVertex3f(halfBackWidth, -halfHeight, -halfDepth);
    glVertex3f(halfBackWidth, halfHeight, -halfDepth);
    glVertex3f(halfFrontWidth, halfHeight/2, halfDepth);
    glVertex3f(halfFrontWidth, -halfHeight, halfDepth);
    
    glEnd();
    
    // 添加数字键盘专用区域凹槽
    // glPushMatrix();
    // glTranslatef(0, halfHeight/2 + 0.05f, 0);
    // setMaterial(accentMaterial);
    // drawCube(3.5f, 0.1f, 4.0f);  // 数字键盘区域平台
    // glPopMatrix();
    
    // 添加右侧功能区域标识
    glPushMatrix();
    glTranslatef(1.0f, halfHeight/2 + 0.08f, -1.5f);
    setMaterial(accentMaterial);
    drawCube(0.8f, 0.05f, 1.0f);  // 特殊功能区域
    glPopMatrix();
    
    glPopMatrix();
    
    glPopMatrix();
}

// 绘制上侧模块外壳（类似主键盘的直角梯形设计，作为主外壳的延伸）
void drawTopModuleShell() {
    if (!showTopModule) return;
    
    glPushMatrix();
    glTranslatef(0, 0, -5.0f);
    
    setMaterial(topModuleMaterial);
    
    glPushMatrix();
    glTranslatef(0, -0.2f, 0);
    
    // 上侧模块的直角梯形外壳 - 作为主外壳的延伸
    glBegin(GL_QUADS);
    
    float backWidth = 17.0f;     // 后面宽度（与主键盘前端相接）
    float frontWidth = 17.0f;     // 前面宽度（较窄）
    float depth = 3.0f;          // 深度（比主键盘浅）
    float height = 1.0f;         // 高度（与主键盘一致）
    
    float halfBackWidth = backWidth / 2.0f;
    float halfFrontWidth = frontWidth / 2.0f;
    float halfDepth = depth / 2.0f;
    float halfHeight = height / 2.0f;
    
    // 底面（平面，梯形）- 这是基底
    glNormal3f(0, -1, 0);
    glVertex3f(-halfBackWidth, -halfHeight, -halfDepth);     // 后左
    glVertex3f(halfBackWidth, -halfHeight, -halfDepth);      // 后右
    glVertex3f(halfFrontWidth, -halfHeight, halfDepth);      // 前右
    glVertex3f(-halfFrontWidth, -halfHeight, halfDepth);     // 前左
    
    // 顶面（平面，梯形）
    glNormal3f(0, 1, 0);
    glVertex3f(-halfBackWidth, halfHeight*(1+2/7.0), -halfDepth);      // 后左
    glVertex3f(-halfFrontWidth, halfHeight, halfDepth);    // 前左
    glVertex3f(halfFrontWidth, halfHeight, halfDepth);     // 前右
    glVertex3f(halfBackWidth, halfHeight*(1+2/7.0), -halfDepth);       // 后右
    
    // 前面（较窄的矩形侧面）
    glNormal3f(0, 0, 1);
    glVertex3f(-halfFrontWidth, -halfHeight, halfDepth);
    glVertex3f(halfFrontWidth, -halfHeight, halfDepth);
    glVertex3f(halfFrontWidth, halfHeight, halfDepth);
    glVertex3f(-halfFrontWidth, halfHeight, halfDepth);
    
    // 后面（较宽的矩形侧面）- 与主键盘连接处
    glNormal3f(0, 0, -1);
    glVertex3f(-halfBackWidth, -halfHeight, -halfDepth);
    glVertex3f(-halfBackWidth, halfHeight*(1+2/7.0), -halfDepth);
    glVertex3f(halfBackWidth, halfHeight*(1+2/7.0), -halfDepth);
    glVertex3f(halfBackWidth, -halfHeight, -halfDepth);
    
    // 左侧面（梯形侧面）
    glNormal3f(-1, 0, 0);
    glVertex3f(-halfBackWidth, -halfHeight, -halfDepth);
    glVertex3f(-halfFrontWidth, -halfHeight, halfDepth);
    glVertex3f(-halfFrontWidth, halfHeight, halfDepth);
    glVertex3f(-halfBackWidth, halfHeight*(1+2/7.0), -halfDepth);
    
    // 右侧面（梯形侧面）
    glNormal3f(1, 0, 0);
    glVertex3f(halfBackWidth, -halfHeight, -halfDepth);
    glVertex3f(halfBackWidth, halfHeight*(1+2/7.0), -halfDepth);
    glVertex3f(halfFrontWidth, halfHeight, halfDepth);
    glVertex3f(halfFrontWidth, -halfHeight, halfDepth);

    
    glEnd();
    glPopMatrix();
    
    glPopMatrix();
}


// 绘制棱台（用于键帽）
void drawTruncatedPyramid(float bottomWidth, float bottomDepth, float topWidth, float topDepth, float height) {
    glBegin(GL_QUADS);
    
    float halfBottomWidth = bottomWidth / 2.0f;
    float halfBottomDepth = bottomDepth / 2.0f;
    float halfTopWidth = topWidth / 2.0f;
    float halfTopDepth = topDepth / 2.0f;
    float halfHeight = height / 2.0f;
    
    // 前面
    glNormal3f(0, 0, 1);
    glVertex3f(-halfBottomWidth, -halfHeight, halfBottomDepth);
    glVertex3f(halfBottomWidth, -halfHeight, halfBottomDepth);
    glVertex3f(halfTopWidth, halfHeight, halfTopDepth);
    glVertex3f(-halfTopWidth, halfHeight, halfTopDepth);
    
    // 后面
    glNormal3f(0, 0, -1);
    glVertex3f(-halfBottomWidth, -halfHeight, -halfBottomDepth);
    glVertex3f(-halfTopWidth, halfHeight, -halfTopDepth);
    glVertex3f(halfTopWidth, halfHeight, -halfTopDepth);
    glVertex3f(halfBottomWidth, -halfHeight, -halfBottomDepth);
    
    // 左侧面
    glNormal3f(-1, 0, 0);
    glVertex3f(-halfBottomWidth, -halfHeight, -halfBottomDepth);
    glVertex3f(-halfBottomWidth, -halfHeight, halfBottomDepth);
    glVertex3f(-halfTopWidth, halfHeight, halfTopDepth);
    glVertex3f(-halfTopWidth, halfHeight, -halfTopDepth);
    
    // 右侧面
    glNormal3f(1, 0, 0);
    glVertex3f(halfBottomWidth, -halfHeight, -halfBottomDepth);
    glVertex3f(halfTopWidth, halfHeight, -halfTopDepth);
    glVertex3f(halfTopWidth, halfHeight, halfTopDepth);
    glVertex3f(halfBottomWidth, -halfHeight, halfBottomDepth);
    
    // 顶面
    glNormal3f(0, 1, 0);
    glVertex3f(-halfTopWidth, halfHeight, -halfTopDepth);
    glVertex3f(-halfTopWidth, halfHeight, halfTopDepth);
    glVertex3f(halfTopWidth, halfHeight, halfTopDepth);
    glVertex3f(halfTopWidth, halfHeight, -halfTopDepth);
    
    // 底面
    glNormal3f(0, -1, 0);
    glVertex3f(-halfBottomWidth, -halfHeight, -halfBottomDepth);
    glVertex3f(halfBottomWidth, -halfHeight, -halfBottomDepth);
    glVertex3f(halfBottomWidth, -halfHeight, halfBottomDepth);
    glVertex3f(-halfBottomWidth, -halfHeight, halfBottomDepth);
    
    glEnd();
}

// 绘制键帽（使用棱台形状））
void drawKeycap(float x, float y, float z, float width, float depth, bool pressed = false) {
    glPushMatrix();
    glTranslatef(x, y + (pressed ? -0.02f : 0), z);
    setMaterial(keycapMaterial);
    
    // 键帽主体 - 梯形设计
    glPushMatrix();
    glScalef(width * 0.85f, 0.12f, depth * 0.85f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPopMatrix();
}


void drawSpace() {
    // 绘制空格键的棱台形状（上窄下宽）
    glBegin(GL_QUADS);
    
    float spaceWidth = 6.25f * 1.0f; // 空格键宽度
    float spaceDepth = 0.7f;
    float spaceHeight = 0.12f;
    float topScale = 0.85f;
    
    float halfWidth = spaceWidth / 2.0f;
    float halfDepth = spaceDepth / 2.0f;
    float halfHeight = spaceHeight / 2.0f;
    float topHalfWidth = halfWidth * topScale;
    float topHalfDepth = halfDepth * topScale;
    
    // 前面 (梯形)
    glNormal3f(0, 0, 1);
    glVertex3f(-halfWidth, -halfHeight, halfDepth);
    glVertex3f(halfWidth, -halfHeight, halfDepth);
    glVertex3f(topHalfWidth, halfHeight, topHalfDepth);
    glVertex3f(-topHalfWidth, halfHeight, topHalfDepth);
    
    // 后面 (梯形)
    glNormal3f(0, 0, -1);
    glVertex3f(-halfWidth, -halfHeight, -halfDepth);
    glVertex3f(-topHalfWidth, halfHeight, -topHalfDepth);
    glVertex3f(topHalfWidth, halfHeight, -topHalfDepth);
    glVertex3f(halfWidth, -halfHeight, -halfDepth);
    
    // 左侧面 (梯形)
    glNormal3f(-1, 0, 0);
    glVertex3f(-halfWidth, -halfHeight, -halfDepth);
    glVertex3f(-halfWidth, -halfHeight, halfDepth);
    glVertex3f(-topHalfWidth, halfHeight, topHalfDepth);
    glVertex3f(-topHalfWidth, halfHeight, -topHalfDepth);
    
    // 右侧面 (梯形)
    glNormal3f(1, 0, 0);
    glVertex3f(halfWidth, -halfHeight, -halfDepth);
    glVertex3f(topHalfWidth, halfHeight, -topHalfDepth);
    glVertex3f(topHalfWidth, halfHeight, topHalfDepth);
    glVertex3f(halfWidth, -halfHeight, halfDepth);
    
    // 顶面 (较小的矩形)
    glNormal3f(0, 1, 0);
    glVertex3f(-topHalfWidth, halfHeight, -topHalfDepth);
    glVertex3f(-topHalfWidth, halfHeight, topHalfDepth);
    glVertex3f(topHalfWidth, halfHeight, topHalfDepth);
    glVertex3f(topHalfWidth, halfHeight, -topHalfDepth);
    
    // 底面 (较大的矩形)
    glNormal3f(0, -1, 0);
    glVertex3f(-halfWidth, -halfHeight, -halfDepth);
    glVertex3f(halfWidth, -halfHeight, -halfDepth);
    glVertex3f(halfWidth, -halfHeight, halfDepth);
    glVertex3f(-halfWidth, -halfHeight, halfDepth);
    
    glEnd();
}

// 绘制主键盘
void drawMainKeyboard() {
    glPushMatrix();
    
    // 键盘主底座 - 更厚更有立体感
    setMaterial(keyboardMaterial);
    // glTranslatef(0, -0.2f, 0);
    // glRotatef(3, 1, 0, 0);  // 轻微向后倾斜
    // drawCube(15.0f, 0.3f, 7.0f);
    
    glPopMatrix();
    
    float keySpacing = 1.0f;
    float rowSpacing = 1.0f;
    
    // 定义每行的高度（阶梯状）
    float funcKeyHeight = 0.3f;    // 功能键最低
    float numberRowHeight = 0.24f;  // 数字行
    float qwertyRowHeight = 0.20f;  // QWERTY行最高
    float asdfRowHeight = 0.18f;    // ASDF行
    float zxcvRowHeight = 0.14f;    // ZXCV行
    float spaceRowHeight = 0.10f;   // 空格键行
    
    // 第1行：ESC + F1-F12 + Delete
    drawKeycap(-8.0f, funcKeyHeight, -2.0f, 0.8f, 0.8f);  // ESC
    
    // F1-F4组
    for (int i = 0; i < 4; i++) {
        drawKeycap(-6.5f + i * keySpacing, funcKeyHeight, -2.0f, 0.8f, 0.8f);
    }
    
    // F5-F8组
    for (int i = 0; i < 4; i++) {
        drawKeycap(-2.0f + i * keySpacing, funcKeyHeight, -2.0f, 0.8f, 0.8f);
    }
    
    // F9-F12组
    for (int i = 0; i < 4; i++) {
        drawKeycap(2.5f + i * keySpacing, funcKeyHeight, -2.0f, 0.8f, 0.8f);
    }
    
    drawKeycap(7.0f, funcKeyHeight, -2.0f, 0.8f, 0.8f);   // Delete
    
    // 第2行：数字行 + Backspace
    // `~键
    drawKeycap(-8.0f, numberRowHeight, -1.0f, 0.8f, 0.8f);
    
    // 1-9, 0键
    for (int i = 0; i < 10; i++) {
        drawKeycap(-7.0f + i * keySpacing, numberRowHeight, -1.0f, 0.8f, 0.8f);
    }
    
    // -=键
    drawKeycap(3.0f, numberRowHeight, -1.0f, 0.8f, 0.8f);  // -
    drawKeycap(4.0f, numberRowHeight, -1.0f, 0.8f, 0.8f);  // =
    
    // Backspace (1.5倍宽)
    drawKeycap(5.25f, numberRowHeight, -1.0f, 1.5f, 0.8f);  // 修正位置
    
    // 第3行：Tab + QWERTY行 + \|键
    // Tab (1.5倍宽)
    drawKeycap(-7.25f, qwertyRowHeight, 0.0f, 1.5f, 0.8f);  // 修正位置
    
    // QWERTYUIOP键
    for (int i = 0; i < 10; i++) {
        drawKeycap(-6.0f + i * keySpacing, qwertyRowHeight, 0.0f, 0.8f, 0.8f);  // 修正起始位置
    }
    
    // []键
    drawKeycap(4.0f, qwertyRowHeight, 0.0f, 0.8f, 0.8f);  // [ 修正位置
    drawKeycap(5.0f, qwertyRowHeight, 0.0f, 0.8f, 0.8f);  // ] 修正位置

    // \|键 (1.5倍宽)
    drawKeycap(6.25f, qwertyRowHeight, 0.0f, 1.5f, 0.8f);  // 修正位置
    
    // 第4行：Caps + ASDF行 + Enter
    // Caps Lock (1.75倍宽)
    drawKeycap(-6.875f, asdfRowHeight, 1.0f, 1.75f, 0.8f);  // 修正位置
    
    // ASDFGHJKL键
    for (int i = 0; i < 9; i++) {
        drawKeycap(-5.5f + i * keySpacing, asdfRowHeight, 1.0f, 0.8f, 0.8f);  // 修正起始位置
    }
    
    // ;'键
    drawKeycap(3.5f, asdfRowHeight, 1.0f, 0.8f, 0.8f);   // ; 修正位置
    drawKeycap(4.5f, asdfRowHeight, 1.0f, 0.8f, 0.8f);   // ' 修正位置
    
    // Enter (2.25倍宽)
    drawKeycap(6.125f, asdfRowHeight, 1.0f, 2.25f, 0.8f);  // 修正位置
    
    // 第5行：Shift + ZXCV行 + Shift + 方向键
    // Left Shift (2.25倍宽)
    drawKeycap(-6.375f, zxcvRowHeight, 2.0f, 2.25f, 0.8f);  // 修正位置
    
    // ZXCVBNM键
    for (int i = 0; i < 7; i++) {
        drawKeycap(-4.5f + i * keySpacing, zxcvRowHeight, 2.0f, 0.8f, 0.8f);  // 修正起始位置
    }
    
    // ,.键
    drawKeycap(2.5f, zxcvRowHeight, 2.0f, 0.8f, 0.8f);    // , 修正位置
    drawKeycap(3.5f, zxcvRowHeight, 2.0f, 0.8f, 0.8f);    // . 修正位置
    drawKeycap(4.5f, zxcvRowHeight, 2.0f, 0.8f, 0.8f);    // / 修正位置
    
    // Right Shift (1.75倍宽)
    drawKeycap(5.875f, zxcvRowHeight, 2.0f, 1.75f, 0.8f);  // 修正位置
    
    // 第6行：底行
    // Ctrl (1.25倍宽)
    drawKeycap(-7.375f, spaceRowHeight, 3.0f, 1.25f, 0.8f);
    
    // Win (1.25倍宽)
    drawKeycap(-5.875f, spaceRowHeight, 3.0f, 1.25f, 0.8f);
    
    // Alt (1.25倍宽)
    drawKeycap(-4.375f, spaceRowHeight, 3.0f, 1.25f, 0.8f);
    
    // 空格键 (6.25倍宽) - 保持原来的棱台绘制方式
    glPushMatrix();
    glTranslatef(0, spaceRowHeight, 0);
    setMaterial(keycapMaterial);
    drawKeycap(-0.5f, spaceRowHeight, 3.0f, 6.75f, 0.8f); // 修正位置
    glPopMatrix();  // 添加缺失的 glPopMatrix()
    
    // Alt (1.25倍宽)
    drawKeycap(3.375f, spaceRowHeight, 3.0f, 1.25f, 0.8f);
    
    // Fn (1.25倍宽)
    drawKeycap(4.875f, spaceRowHeight, 3.0f, 1.25f, 0.8f);
    
    // 方向键组 - 修正位置使其更紧凑
    drawKeycap(6.0f, spaceRowHeight, 3.0f, 0.8f, 0.8f);   // 左
    drawKeycap(7.0f, spaceRowHeight, 3.0f, 0.8f, 0.8f);   // 下
    drawKeycap(8.0f, spaceRowHeight, 3.0f, 0.8f, 0.8f);   // 右
    
    // 上方向键（在第5行）
    drawKeycap(7.0f, zxcvRowHeight, 2.0f, 0.8f, 0.8f);    // 上方向键位置修正
    
    // 左右连接器
    // drawConnector(-7.0f, 0, 0);   // 左侧连接器
    // drawConnector(7.0f, 0, 0);    // 右侧连接器
    // drawConnector(0, 0, -2.5f);   // 上侧连接器
}

// 绘制左侧扩展模块（游戏控制器风格）- 修改为轨迹球设计
void drawLeftModule() {
    if (!showLeftModule) return;
    
    glPushMatrix();
    glTranslatef(-10.0f, 0, 0);
    
    // 模块底座 - 平整设计
    setMaterial(leftModuleMaterial);
    glTranslatef(0, -0.15f, 0);
    
    // 主底座
    // drawCube(4.0f, 0.3f, 5.0f);
    
    glPopMatrix();
    
    // 轨迹球区域（替换原来的摇杆）
    glPushMatrix();
    glTranslatef(-10.0f, 0.2f, -1.0f);
    setMaterial(buttonMaterial);
    
    // 轨迹球底座凹槽
    // drawCylinder(1.0f, 0.15f);
    
    // 轨迹球本体 - 没入一半露出一半
    glTranslatef(0, 0, 0);  // 向上移动，使球体一半露出
    Material ballMaterial = {{0.2f, 0.2f, 0.8f, 1.0f}, {0.3f, 0.3f, 0.9f, 1.0f}, {0.5f, 0.5f, 1.0f, 1.0f}, 64.0f};
    setMaterial(ballMaterial);
    drawSphere(0.7f);  // 稍大的球体
    
    glPopMatrix();
    
    // 十字键区域 - 保持水平
    setMaterial(buttonMaterial);
    glRotatef(3, 1, 0, 0);  // 轻微向后倾斜
    float dpadSize = 0.6f;
    drawKeycap(-10.0f, 0.3f, 0.5f, dpadSize, dpadSize);   // 上
    drawKeycap(-10.0f, 0.3f, 1.5f, dpadSize, dpadSize);   // 下
    drawKeycap(-10.5f, 0.3f, 1.0f, dpadSize, dpadSize);   // 左
    drawKeycap(-9.5f, 0.3f, 1.0f, dpadSize, dpadSize);    // 右
    
    // 连接器
    glRotatef(90, 0, 1, 0);  // 轻微向后倾斜
    glTranslatef(0, 0,-8.5f);
    drawConnector(0, 0, 0);
    
    glPopMatrix();
}

// 绘制右侧扩展模块（数字键盘）
void drawRightModule() {
    if (!showRightModule) return;
    
    glPushMatrix();
    glTranslatef(10.0f, 0, 0);
    
    // 模块底座
    setMaterial(rightModuleMaterial);
    // glTranslatef(0, -0.15f, 0);
    // drawCube(4.0f, 0.3f, 5.0f);
    
    glPopMatrix();
    
    // 数字键盘区域
    float keySize = 0.7f;
    float keySpacing = 0.85f;
    
    // 数字键盘布局（4x4） - 修正调用
    const char numpadLayout[4][4] = {
        {'/', '*', '-', '+'},
        {'7', '8', '9', ' '},
        {'4', '5', '6', ' '},
        {'1', '2', '3', ' '}
    };
    
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 3; col++) {
            float x = 10.0f + (col - 1) * keySpacing;
            float z = (row - 1.5f) * keySpacing;
            drawKeycap(x, (0.3f-row/25.0f), z, keySize, keySize);  // 添加depth参数
        }
    }
    
    // 大Enter键
    glPushMatrix();
    glTranslatef(11.5f, 0.2f, 0.0f);
    glRotatef(3, 1, 0, 0);  // 旋转90度
    setMaterial(keycapMaterial);
    drawCube(keySize, 0.12f, keySpacing * 2);
    glPopMatrix();
    
    // 0键（双倍宽度）
    glPushMatrix();
    glTranslatef(9.6f, 0.15f, 2.0f);
    setMaterial(keycapMaterial);
    drawCube(keySpacing * 1.5f, 0.12f, keySize);
    glPopMatrix();
    
    // 小数点键 - 修正调用
    drawKeycap(10.8f, 0.15f, 2.0f, keySize, keySize);
    
    // 连接器
    glRotatef(-90, 0, 1, 0);  // 轻微向后倾斜
    glTranslatef(0, 0,-8.4f);
    drawConnector(0, 0, 0);
    
    glPopMatrix();
}

// 绘制上侧扩展模块（草图中的顶部模块）- 使用绝对坐标
void drawTopModule() {
    if (!showTopModule) return;
    
    // === 控制按钮区域（5个按钮）===
    for (int i = 0; i < 5; i++) {
        glPushMatrix();
        glTranslatef(-1.0f + i * 0.8f, 0.5f, -4.3f);  // 绝对坐标
        setMaterial(keycapMaterial);
        glScalef(0.5f * 0.85f, 0.12f, 0.5f * 0.85f);
        glutSolidCube(1.0);
        glPopMatrix();
    }
    
    // === 中间投影仪主体圆柱 ===
    glPushMatrix();
    glTranslatef(0,  -0.5, -6.1f);  // 绝对坐标
    glRotatef(225, 1, 0, 0);  // 水平方向
    setMaterial(buttonMaterial);
    drawCylinder(0.7f, 1.0f);
    glPopMatrix();

    // === 连接器 ===
    glPushMatrix();
    // 连接器
    // glRotatef(-90, 0, 1, 0);  // 轻微向后倾斜
    glTranslatef(0, 0,-3.5f);
    drawConnector(0, 0, 0);
    glPopMatrix();
}
// 显示回调函数
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // 设置相机位置
    float camX = cameraDistance * cos(cameraAngleX * M_PI / 180.0f) * cos(cameraAngleY * M_PI / 180.0f);
    float camY = cameraDistance * sin(cameraAngleX * M_PI / 180.0f);
    float camZ = cameraDistance * cos(cameraAngleX * M_PI / 180.0f) * sin(cameraAngleY * M_PI / 180.0f);
    
    gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
    // 设置光照位置
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    
    // 绘制各个模块的外壳（每个都独立）
// 绘制各个模块的外壳（每个都独立，带动画）

    glPushMatrix();
    glColor3f(0.45f,0.25f,0.65f);
    drawMainKeyboardShell();
    glPopMatrix();

    // 绘制各个模块的外壳（每个都独立，带动画）
    glPushMatrix();
    glTranslatef(-leftModuleOffset, 0, 0);  // 左模块动画偏移
    glColor3f(0.15f,0.3f,0.8f);
    drawLeftModuleShell();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(rightModuleOffset, 0, 0);  // 右模块动画偏移
    glColor3f(0.9f,0.15f,0.15f);
    drawRightModuleShell();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, -topModuleOffset);   // 上模块动画偏移
    glColor3f(0.08f,0.12f,0.25f);
    drawTopModuleShell();
    glPopMatrix();

    // 绘制各个模块（每个都独立，带动画）
    glPushMatrix();
    glColor3f(0.2f,0.75f,0.3f);
    drawMainKeyboard();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-leftModuleOffset, 0, 0);
    glColor3f(1.0f,0.5f,0.1f);
    drawLeftModule();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(rightModuleOffset, 0, 0);
    glColor3f(1.0f,0.5f,0.1f);
    drawRightModule();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, -topModuleOffset);
    glColor3f(0.6f,0.6f,0.65f);
    drawTopModule();
    glPopMatrix();
        
    glutSwapBuffers();
}

// 窗口重置回调
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void idle() {
    animationTime += 0.01f;
    
    // 模块动画逻辑 - 平滑移动
    float speed = 0.2f;  // 动画速度
    bool needsRedraw = false;
    
    // 左模块动画
    if (animateLeft) {
        float diff = targetLeftOffset - leftModuleOffset;
        if (abs(diff) > 0.05f) {
            leftModuleOffset += diff * speed;
            needsRedraw = true;
        } else {
            leftModuleOffset = targetLeftOffset;
            animateLeft = false;
        }
    }
    
    // 右模块动画
    if (animateRight) {
        float diff = targetRightOffset - rightModuleOffset;
        if (abs(diff) > 0.05f) {
            rightModuleOffset += diff * speed;
            needsRedraw = true;
        } else {
            rightModuleOffset = targetRightOffset;
            animateRight = false;
        }
    }
    
    // 上模块动画
    if (animateTop) {
        float diff = targetTopOffset - topModuleOffset;
        if (abs(diff) > 0.05f) {
            topModuleOffset += diff * speed;
            needsRedraw = true;
        } else {
            topModuleOffset = targetTopOffset;
            animateTop = false;
        }
    }
    
    glutPostRedisplay();
}

// 鼠标回调
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mousePressed = true;
            lastMouseX = x;
            lastMouseY = y;
        } else {
            mousePressed = false;
        }
    }
}

// 鼠标移动回调
void mouseMotion(int x, int y) {
    if (mousePressed) {
        float deltaX = x - lastMouseX;
        float deltaY = y - lastMouseY;
        
        cameraAngleY += deltaX * 0.5f;
        cameraAngleX -= deltaY * 0.5f;
        
        // 限制垂直角度
        if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
        if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;
        
        lastMouseX = x;
        lastMouseY = y;
        
        glutPostRedisplay();
    }
}

// 键盘回调
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC
            exit(0);
            break;
        case '1':
            targetLeftOffset = (targetLeftOffset == 0.0f) ? 12.0f : 0.0f;
            animateLeft = true;  // 使用正确的变量名
            std::cout << "左侧模块动画: " << (targetLeftOffset == 0.0f ? "收回" : "展开") << std::endl;
            break;
        case '2':
            targetRightOffset = (targetRightOffset == 0.0f) ? 12.0f : 0.0f;
            animateRight = true;  // 使用正确的变量名
            std::cout << "右侧模块动画: " << (targetRightOffset == 0.0f ? "收回" : "展开") << std::endl;
            break;
        case '3':
            targetTopOffset = (targetTopOffset == 0.0f) ? 8.0f : 0.0f;
            animateTop = true;  // 使用正确的变量名
            std::cout << "上侧模块动画: " << (targetTopOffset == 0.0f ? "收回" : "展开") << std::endl;
            break;
        case 'r':
        case 'R':
            cameraAngleX = 15.0f;
            cameraAngleY = 25.0f;
            cameraDistance = 20.0f;
            std::cout << "视角已重置" << std::endl;
            break;
        case '+':
        case '=':
            cameraDistance -= 1.0f;
            if (cameraDistance < 8.0f) cameraDistance = 8.0f;
            break;
        case '-':
            cameraDistance += 1.0f;
            if (cameraDistance > 40.0f) cameraDistance = 40.0f;
            break;
    }
    glutPostRedisplay();
}

// 特殊键回调
void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            cameraAngleX += 5.0f;
            break;
        case GLUT_KEY_DOWN:
            cameraAngleX -= 5.0f;
            break;
        case GLUT_KEY_LEFT:
            cameraAngleY -= 5.0f;
            break;
        case GLUT_KEY_RIGHT:
            cameraAngleY += 5.0f;
            break;
    }
    
    // 限制垂直角度
    if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
    if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;
    
    glutPostRedisplay();
}

// 初始化OpenGL
void initGL() {
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    
    // 启用光照
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // 设置光照参数
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    
    // 启用颜色材质
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    // 启用法向量归一化
    glEnable(GL_NORMALIZE);
    
    // 设置背景色
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    
    // 启用平滑着色
    glShadeModel(GL_SMOOTH);
    
    // 启用背面剔除
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

// 显示使用说明
void printInstructions() {
    std::cout << "=== 模块化键盘3D模型（含外壳）===" << std::endl;
    std::cout << "完整的外壳设计，包含所有模块和外壳细节" << std::endl;
    std::cout << "操作说明:" << std::endl;
    std::cout << "- 鼠标左键拖拽: 旋转视角" << std::endl;
    std::cout << "- 方向键: 微调视角" << std::endl;
    std::cout << "- +/-键: 缩放" << std::endl;
    std::cout << "- 数字键1: 切换左侧游戏模块" << std::endl;
    std::cout << "- 数字键2: 切换右侧数字键盘模块" << std::endl;
    std::cout << "- 数字键3: 切换上侧控制模块" << std::endl;
    std::cout << "- R键: 重置视角" << std::endl;
    std::cout << "- ESC: 退出程序" << std::endl;
    std::cout << "====================================" << std::endl;
}

int main(int argc, char** argv) {
    // 初始化GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("模块化键盘3D模型 - 完整设计");
    
    // 初始化OpenGL
    initGL();
    
    // 注册回调函数
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    // 显示使用说明
    printInstructions();
    
    // 进入主循环
    glutMainLoop();
    
    return 0;
}
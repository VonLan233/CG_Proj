#include <GLUT/glut.h>
#include <cmath>
#include <iostream>
#include <vector>

// 窗口尺寸
const int WINDOW_WIDTH = 1200;
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

// 绘制主键盘外壳（完全按照草图的梯形设计）
void drawMainKeyboardShell() {
    setMaterial(shellMaterial);
    
    glPushMatrix();
    
    // 主键盘的梯形外壳 - 后高前低，完全按照草图
    glTranslatef(0, -0.3f, 0);
    
    // 主体梯形外壳 - 底部宽，顶部稍窄，后面高
    drawTrapezoid(15.0f, 13.5f, 1.2f, 6.0f);
    
    // 键盘区域的平台
    glTranslatef(0, 0.5f, 0);
    setMaterial(accentMaterial);
    drawTrapezoid(13.5f, 12.5f, 0.3f, 5.0f);
    
    glPopMatrix();
}

// 绘制左侧模块外壳（按照草图的独特轮廓）
void drawLeftModuleShell() {
    if (!showLeftModule) return;
    
    glPushMatrix();
    glTranslatef(-10.0f, 0, 0);
    
    setMaterial(leftModuleMaterial);
    
    // 草图中左模块的特殊形状 - 前窄后宽的不规则四边形
    glTranslatef(0, -0.3f, 0);
    
    // 使用不规则形状，模拟草图中的轮廓
    drawCustomShape(5.5f, 3.5f, 1.0f, 3.0f, 4.5f);
    
    // 顶部控制面板区域
    glTranslatef(0, 0.4f, 0);
    setMaterial(accentMaterial);
    drawCustomShape(4.8f, 3.0f, 0.4f, 2.5f, 4.0f);
    
    // 圆形控制区域的特殊凹槽
    glPushMatrix();
    glTranslatef(0, 0.15f, -0.5f);
    drawCylinder(1.3f, 0.1f);
    glPopMatrix();
    
    glPopMatrix();
    
    glPopMatrix();
}

// 绘制右侧模块外壳（按照草图的特殊轮廓）
void drawRightModuleShell() {
    if (!showRightModule) return;
    
    glPushMatrix();
    glTranslatef(10.0f, 0, 0);
    
    setMaterial(rightModuleMaterial);
    
    // 草图中右模块的特殊外形
    glTranslatef(0, -0.3f, 0);
    
    // 右模块的特殊形状 - 与左模块相对称但有自己的特征
    drawCustomShape(4.8f, 4.2f, 1.0f, 4.0f, 3.5f);
    
    // 数字键盘区域平台
    glTranslatef(0, 0.4f, 0);
    setMaterial(accentMaterial);
    drawCustomShape(4.2f, 3.8f, 0.4f, 3.5f, 3.0f);
    
    // 右侧特殊的垂直突起区域
    glPushMatrix();
    glTranslatef(1.5f, 0.15f, 0);
    drawCube(0.8f, 0.6f, 3.0f);
    glPopMatrix();
    
    glPopMatrix();
    
    glPopMatrix();
}

// 绘制上侧模块外壳（完全按照草图的长条形设计）
void drawTopModuleShell() {
    if (!showTopModule) return;
    
    glPushMatrix();
    glTranslatef(0, 0, -5.0f);
    
    setMaterial(topModuleMaterial);
    
    // 草图中上模块的长条形外壳 - 明显的长条形状
    glTranslatef(0, -0.25f, 0);
    
    // 主体长条形外壳
    drawCube(12.0f, 0.8f, 2.8f);
    
    // 顶部控制面板平台
    glTranslatef(0, 0.3f, 0);
    setMaterial(accentMaterial);
    drawCube(11.5f, 0.3f, 2.4f);
    
    // 三个功能区域的分隔
    setMaterial(shellMaterial);
    
    // 旋钮区域（左侧）
    glPushMatrix();
    glTranslatef(-3.5f, 0.1f, 0);
    drawCube(3.0f, 0.2f, 2.0f);
    // 大圆形凹槽
    glTranslatef(0, 0.05f, 0);
    drawCylinder(1.2f, 0.1f);
    glPopMatrix();
    
    // 按钮控制区域（中间）
    glPushMatrix();
    glTranslatef(0, 0.1f, 0);
    drawCube(4.0f, 0.2f, 2.0f);
    glPopMatrix();
    
    // 显示屏区域（右侧）
    glPushMatrix();
    glTranslatef(3.5f, 0.1f, 0);
    drawCube(3.0f, 0.2f, 2.0f);
    // 显示屏凹槽
    glTranslatef(0, 0.05f, 0);
    drawCube(2.5f, 0.08f, 1.5f);
    glPopMatrix();
    
    glPopMatrix();
    
    glPopMatrix();
}

// 绘制键帽
void drawKeycap(float x, float y, float z, float size, bool pressed = false) {
    glPushMatrix();
    glTranslatef(x, y + (pressed ? -0.02f : 0), z);
    setMaterial(keycapMaterial);
    
    // 键帽主体 - 略微倾斜的设计
    glPushMatrix();
    glScalef(size * 0.85f, 0.12f, size * 0.85f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    // 键帽顶部略小的平面
    glPushMatrix();
    glTranslatef(0, 0.08f, 0);
    glScalef(size * 0.7f, 0.02f, size * 0.7f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPopMatrix();
}

// 绘制主键盘
void drawMainKeyboard() {
    glPushMatrix();
    
    // 键盘主底座 - 更厚更有立体感
    setMaterial(keyboardMaterial);
    glTranslatef(0, -0.2f, 0);
    drawCube(14.0f, 0.4f, 5.0f);
    
    // 键盘斜面设计
    glTranslatef(0, 0.1f, -1.0f);
    glRotatef(5, 1, 0, 0);  // 轻微向后倾斜
    drawCube(13.5f, 0.2f, 4.0f);
    
    glPopMatrix();
    
    // 键盘布局 - 更真实的间距
    float keySize = 0.8f;
    float keySpacing = 1.0f;
    
    // ESC键和功能键区域
    drawKeycap(-6.5f, 0.1f, -2.2f, keySize * 0.8f); // ESC
    
    // F1-F12功能键（分组）
    for (int i = 0; i < 4; i++) {
        drawKeycap(-4.0f + i * keySpacing, 0.1f, -2.2f, keySize * 0.8f);
    }
    for (int i = 0; i < 4; i++) {
        drawKeycap(0.5f + i * keySpacing, 0.1f, -2.2f, keySize * 0.8f);
    }
    for (int i = 0; i < 4; i++) {
        drawKeycap(5.0f + i * keySpacing, 0.1f, -2.2f, keySize * 0.8f);
    }
    
    // 数字行
    for (int i = 0; i < 13; i++) {
        float x = (i - 6.0f) * keySpacing;
        drawKeycap(x, 0.1f, -1.0f, keySize);
    }
    
    // QWERTY行
    for (int i = 0; i < 13; i++) {
        float x = (i - 6.0f) * keySpacing;
        drawKeycap(x, 0.1f, 0.0f, keySize);
    }
    
    // ASDF行
    for (int i = 0; i < 12; i++) {
        float x = (i - 5.5f) * keySpacing;
        drawKeycap(x, 0.1f, 1.0f, keySize);
    }
    
    // ZXCV行
    for (int i = 0; i < 10; i++) {
        float x = (i - 4.5f) * keySpacing;
        drawKeycap(x, 0.1f, 2.0f, keySize);
    }
    
    // 空格键
    glPushMatrix();
    glTranslatef(0, 0.1f, 3.0f);
    setMaterial(keycapMaterial);
    drawCube(6.0f, 0.12f, 0.8f);
    glPopMatrix();
    
    // 左右连接器
    drawConnector(-7.0f, 0, 0);   // 左侧连接器
    drawConnector(7.0f, 0, 0);    // 右侧连接器
    drawConnector(0, 0, -2.5f);   // 上侧连接器
}

// 绘制左侧扩展模块（游戏控制器风格）
void drawLeftModule() {
    if (!showLeftModule) return;
    
    glPushMatrix();
    glTranslatef(-10.0f, 0, 0);
    
    // 模块底座 - 更有机的形状
    setMaterial(leftModuleMaterial);
    glTranslatef(0, -0.15f, 0);
    
    // 主底座
    drawCube(4.0f, 0.3f, 5.0f);
    
    // 手柄握持区域
    glTranslatef(0, -0.1f, 1.5f);
    glRotatef(-15, 1, 0, 0);
    drawCube(3.5f, 0.4f, 2.0f);
    
    glPopMatrix();
    
    // 模拟摇杆区域（草图中的大圆形）
    glPushMatrix();
    glTranslatef(-10.0f, 0.3f, -1.0f);
    setMaterial(buttonMaterial);
    
    // 摇杆底座
    drawCylinder(0.8f, 0.1f);
    
    // 摇杆本体
    glTranslatef(0, 0.15f, 0);
    drawSphere(0.4f);
    
    glPopMatrix();
    
    // 十字键区域
    setMaterial(buttonMaterial);
    float dpadSize = 0.5f;
    drawKeycap(-10.0f, 0.15f, 0.5f, dpadSize);   // 上
    drawKeycap(-10.0f, 0.15f, 1.5f, dpadSize);   // 下
    drawKeycap(-10.5f, 0.15f, 1.0f, dpadSize);   // 左
    drawKeycap(-9.5f, 0.15f, 1.0f, dpadSize);    // 右
    
    // 功能按钮（ABXY风格）
    drawKeycap(-8.5f, 0.15f, -0.5f, 0.4f);  // A
    drawKeycap(-8.5f, 0.15f, -1.5f, 0.4f);  // B
    drawKeycap(-9.0f, 0.15f, -1.0f, 0.4f);  // X
    drawKeycap(-8.0f, 0.15f, -1.0f, 0.4f);  // Y
    
    // 肩部按钮
    glPushMatrix();
    glTranslatef(-10.0f, 0.4f, -2.0f);
    setMaterial(buttonMaterial);
    drawCube(1.5f, 0.2f, 0.4f);
    glPopMatrix();
    
    // 连接器
    drawConnector(-7.0f, 0, 0);
    
    glPopMatrix();
}

// 绘制右侧扩展模块（数字键盘）
void drawRightModule() {
    if (!showRightModule) return;
    
    glPushMatrix();
    glTranslatef(10.0f, 0, 0);
    
    // 模块底座
    setMaterial(rightModuleMaterial);
    glTranslatef(0, -0.15f, 0);
    drawCube(4.0f, 0.3f, 5.0f);
    
    glPopMatrix();
    
    // 数字键盘区域
    float keySize = 0.7f;
    float keySpacing = 0.85f;
    
    // 数字键盘布局（4x4）
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
            drawKeycap(x, 0.1f, z, keySize);
        }
    }
    
    // 大Enter键
    glPushMatrix();
    glTranslatef(11.3f, 0.1f, 0.0f);
    setMaterial(keycapMaterial);
    drawCube(keySize, 0.12f, keySpacing * 2);
    glPopMatrix();
    
    // 0键（双倍宽度）
    glPushMatrix();
    glTranslatef(9.6f, 0.1f, 1.3f);
    setMaterial(keycapMaterial);
    drawCube(keySpacing * 1.5f, 0.12f, keySize);
    glPopMatrix();
    
    // 小数点键
    drawKeycap(10.8f, 0.1f, 1.3f, keySize * 0.8f);
    
    // 连接器
    drawConnector(7.0f, 0, 0);
    
    glPopMatrix();
}

// 绘制上侧扩展模块（草图中的顶部模块）
void drawTopModule() {
    if (!showTopModule) return;
    
    glPushMatrix();
    glTranslatef(0, 0, -5.0f);
    
    // 模块底座
    setMaterial(topModuleMaterial);
    glTranslatef(0, -0.15f, 0);
    drawCube(8.0f, 0.3f, 2.5f);
    
    glPopMatrix();
    
    // 大旋钮（草图中显著的圆形元素）
    glPushMatrix();
    glTranslatef(-2.0f, 0.25f, -5.0f);
    setMaterial(buttonMaterial);
    
    // 旋钮底座
    drawCylinder(0.8f, 0.2f);
    
    // 旋钮顶部
    glTranslatef(0, 0.15f, 0);
    drawCylinder(0.6f, 0.1f);
    
    // 旋钮指示器
    glPushMatrix();
    glTranslatef(0.3f, 0.05f, 0);
    drawCube(0.1f, 0.02f, 0.05f);
    glPopMatrix();
    
    glPopMatrix();
    
    // 控制按钮区域
    for (int i = 0; i < 5; i++) {
        float x = -1.0f + i * 0.8f;
        drawKeycap(x, 0.1f, -4.3f, 0.5f);
    }
    
    // 显示屏区域（模拟）
    glPushMatrix();
    glTranslatef(2.5f, 0.05f, -5.0f);
    setMaterial(keyboardMaterial);
    drawCube(2.0f, 0.05f, 1.0f);
    
    // 显示屏框架
    glTranslatef(0, 0.03f, 0);
    Material screenMaterial = {{0.0f, 0.1f, 0.0f, 1.0f}, {0.0f, 0.3f, 0.0f, 1.0f}, {0.0f, 0.5f, 0.0f, 1.0f}, 32.0f};
    setMaterial(screenMaterial);
    drawCube(1.8f, 0.02f, 0.8f);
    glPopMatrix();
    
    // 连接器
    drawConnector(0, 0, -2.5f);
    
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
    
    // 绘制各个模块的外壳
    drawMainKeyboardShell();
    drawLeftModuleShell();
    drawRightModuleShell();
    drawTopModuleShell();
    
    // 绘制各个模块
    drawMainKeyboard();
    drawLeftModule();
    drawRightModule();
    drawTopModule();
    
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

// 空闲回调（用于动画）
void idle() {
    animationTime += 0.01f;
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
            showLeftModule = !showLeftModule;
            std::cout << "左侧模块: " << (showLeftModule ? "显示" : "隐藏") << std::endl;
            break;
        case '2':
            showRightModule = !showRightModule;
            std::cout << "右侧模块: " << (showRightModule ? "显示" : "隐藏") << std::endl;
            break;
        case '3':
            showTopModule = !showTopModule;
            std::cout << "上侧模块: " << (showTopModule ? "显示" : "隐藏") << std::endl;
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
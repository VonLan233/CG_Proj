#include <GLUT/glut.h>
#include <cmath>
#include <iostream>
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

// 窗口尺寸
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;

// 相机控制变量
float cameraAngleX = 20.0f;
float cameraAngleY = 30.0f;
float cameraDistance = 12.0f;
bool mousePressed = false;
int lastMouseX = 0, lastMouseY = 0;

// 动画控制变量
float animationTime = 0.0f;
float boxLidAngle = 0.0f;          
bool animationStarted = false;
bool opening = true; // true为开启，false为关闭

// 光照参数
GLfloat lightPosition[] = {8.0f, 12.0f, 8.0f, 1.0f};
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

// 材质定义 - 确保所有材质都是不透明的
Material woodMaterial = {
    {0.3f, 0.2f, 0.1f, 1.0f},
    {0.6f, 0.4f, 0.2f, 1.0f},
    {0.4f, 0.4f, 0.4f, 1.0f},
    32.0f
};

Material metalMaterial = {
    {0.2f, 0.2f, 0.2f, 1.0f},
    {0.4f, 0.4f, 0.4f, 1.0f},
    {0.8f, 0.8f, 0.8f, 1.0f},
    64.0f
};

Material paddingMaterial = {
    {0.4f, 0.1f, 0.1f, 1.0f},
    {0.7f, 0.2f, 0.2f, 1.0f},
    {0.5f, 0.5f, 0.5f, 1.0f},
    32.0f
};

Material goldMaterial = {
    {0.6f, 0.5f, 0.1f, 1.0f},
    {0.8f, 0.7f, 0.2f, 1.0f},
    {0.9f, 0.8f, 0.3f, 1.0f},
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
    gluDisk(quad, 0, radius, 20, 1);
    glTranslatef(0, 0, height);
    gluDisk(quad, 0, radius, 20, 1);
    glPopMatrix();
    gluDeleteQuadric(quad);
}

// 绘制带纹理的木质表面
void drawWoodBoard(float width, float height, float depth) {
    glPushMatrix();
    
    // 确保不透明渲染
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // 主体
    drawCube(width, height, depth);
    
    glPopMatrix();
}

// 绘制箱子底座
void drawBoxBase() {
    // 确保使用不透明渲染
    glDisable(GL_BLEND);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    setMaterial(woodMaterial);
    
    // 底部
    glPushMatrix();
    glTranslatef(0, -1.8f, 0);
    drawWoodBoard(5.0f, 0.3f, 3.5f);
    glPopMatrix();
    
    // 前面板
    glPushMatrix();
    glTranslatef(0, -0.3f, 1.75f);
    drawWoodBoard(5.0f, 2.7f, 0.25f);
    glPopMatrix();
    
    // 后面板
    glPushMatrix();
    glTranslatef(0, -0.3f, -1.75f);
    drawWoodBoard(5.0f, 2.7f, 0.25f);
    glPopMatrix();
    
    // 左侧面板
    glPushMatrix();
    glTranslatef(-2.5f, -0.3f, 0);
    drawWoodBoard(0.25f, 2.7f, 3.5f);
    glPopMatrix();
    
    // 右侧面板
    glPushMatrix();
    glTranslatef(2.5f, -0.3f, 0);
    drawWoodBoard(0.25f, 2.7f, 3.5f);
    glPopMatrix();
}

// 绘制箱子盖子
void drawBoxLid() {
    // 确保使用不透明渲染
    glDisable(GL_BLEND);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glPushMatrix();
    
    // 盖子旋转轴位置（后侧）
    glTranslatef(0, 1.0f, -1.75f);
    glRotatef(-boxLidAngle, 1, 0, 0);  
    glTranslatef(0, 0, 1.75f);
    
    setMaterial(woodMaterial);
    
    // 盖子主体
    drawWoodBoard(5.0f, 0.25f, 3.5f);
    
    // 盖子边框装饰
    setMaterial(metalMaterial);
    
    // 前边框
    glPushMatrix();
    glTranslatef(0, 0, 1.75f);
    drawCube(5.2f, 0.08f, 0.08f);
    glPopMatrix();
    
    // 左右边框
    glPushMatrix();
    glTranslatef(-2.5f, 0, 0);
    drawCube(0.08f, 0.08f, 3.5f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(2.5f, 0, 0);
    drawCube(0.08f, 0.08f, 3.5f);
    glPopMatrix();
    
    // 后边框
    glPushMatrix();
    glTranslatef(0, 0, -1.75f);
    drawCube(5.2f, 0.08f, 0.08f);
    glPopMatrix();
    
    glPopMatrix();
}

// 绘制内部衬垫
void drawInnerPadding() {
    // 确保使用不透明渲染
    glDisable(GL_BLEND);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    setMaterial(paddingMaterial);
    
    // 底部衬垫
    glPushMatrix();
    glTranslatef(0, -1.6f, 0);
    drawCube(4.7f, 0.08f, 3.2f);
    glPopMatrix();
    
    // 侧面衬垫
    glPushMatrix();
    glTranslatef(-2.35f, -0.3f, 0);
    drawCube(0.08f, 2.4f, 3.2f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(2.35f, -0.3f, 0);
    drawCube(0.08f, 2.4f, 3.2f);
    glPopMatrix();
    
    // 前后衬垫
    glPushMatrix();
    glTranslatef(0, -0.3f, 1.6f);
    drawCube(4.7f, 2.4f, 0.08f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, -0.3f, -1.6f);
    drawCube(4.7f, 2.4f, 0.08f);
    glPopMatrix();
}

// 参考lab8.cpp的阴影矩阵计算函数
void shadowMatrix(GLfloat shadowMat[4][4], GLfloat groundplane[4], GLfloat lightpos[4]) {
    GLfloat dot;
    
    // 计算光源位置向量和地面法向量的点积
    dot = groundplane[0] * lightpos[0] + groundplane[1] * lightpos[1] + 
          groundplane[2] * lightpos[2] + groundplane[3] * lightpos[3];
    
    shadowMat[0][0] = dot - lightpos[0] * groundplane[0];
    shadowMat[1][0] = 0.0f - lightpos[0] * groundplane[1];
    shadowMat[2][0] = 0.0f - lightpos[0] * groundplane[2];
    shadowMat[3][0] = 0.0f - lightpos[0] * groundplane[3];
    
    shadowMat[0][1] = 0.0f - lightpos[1] * groundplane[0];
    shadowMat[1][1] = dot - lightpos[1] * groundplane[1];
    shadowMat[2][1] = 0.0f - lightpos[1] * groundplane[2];
    shadowMat[3][1] = 0.0f - lightpos[1] * groundplane[3];
    
    shadowMat[0][2] = 0.0f - lightpos[2] * groundplane[0];
    shadowMat[1][2] = 0.0f - lightpos[2] * groundplane[1];
    shadowMat[2][2] = dot - lightpos[2] * groundplane[2];
    shadowMat[3][2] = 0.0f - lightpos[2] * groundplane[3];
    
    shadowMat[0][3] = 0.0f - lightpos[3] * groundplane[0];
    shadowMat[1][3] = 0.0f - lightpos[3] * groundplane[1];
    shadowMat[2][3] = 0.0f - lightpos[3] * groundplane[2];
    shadowMat[3][3] = dot - lightpos[3] * groundplane[3];
}

// 计算地面平面方程
void findPlane(GLfloat plane[4], GLfloat v0[3], GLfloat v1[3], GLfloat v2[3]) {
    GLfloat vec0[3], vec1[3];
    
    // 需要两个向量来计算叉积
    vec0[0] = v1[0] - v0[0];
    vec0[1] = v1[1] - v0[1];
    vec0[2] = v1[2] - v0[2];
    
    vec1[0] = v2[0] - v0[0];
    vec1[1] = v2[1] - v0[1];
    vec1[2] = v2[2] - v0[2];
    
    // 计算叉积得到平面方程的A、B、C
    plane[0] = vec0[1] * vec1[2] - vec0[2] * vec1[1];
    plane[1] = -(vec0[0] * vec1[2] - vec0[2] * vec1[0]);
    plane[2] = vec0[0] * vec1[1] - vec0[1] * vec1[0];
    plane[3] = -(plane[0] * v0[0] + plane[1] * v0[1] + plane[2] * v0[2]);
}

// 绘制箱子组件（用于阴影）
void drawBoxForShadow() {
    // 箱子底座
    glPushMatrix();
    glTranslatef(0, -1.8f, 0);
    glScalef(5.0f, 0.3f, 3.5f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    // 前面板
    glPushMatrix();
    glTranslatef(0, -0.3f, 1.75f);
    glScalef(5.0f, 2.7f, 0.25f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    // 后面板
    glPushMatrix();
    glTranslatef(0, -0.3f, -1.75f);
    glScalef(5.0f, 2.7f, 0.25f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    // 左侧面板
    glPushMatrix();
    glTranslatef(-2.5f, -0.3f, 0);
    glScalef(0.25f, 2.7f, 3.5f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    // 右侧面板
    glPushMatrix();
    glTranslatef(2.5f, -0.3f, 0);
    glScalef(0.25f, 2.7f, 3.5f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    // 盖子
    glPushMatrix();
    glTranslatef(0, 1.0f, -1.75f);
    glRotatef(-boxLidAngle, 1, 0, 0);  
    glTranslatef(0, 0, 1.75f);
    glScalef(5.0f, 0.25f, 3.5f);
    glutSolidCube(1.0);
    glPopMatrix();
}

// 绘制投影矩阵阴影
void drawProjectiveShadows() {
    // 地面顶点定义
    static GLfloat floorVertices[4][3] = {
        {-10.0f, -2.1f, 10.0f},
        {10.0f, -2.1f, 10.0f}, 
        {10.0f, -2.1f, -10.0f},
        {-10.0f, -2.1f, -10.0f}
    };
    
    // 调整光源位置，让阴影更自然
    static GLfloat lightPosition[4] = {6.0f, 10.0f, 6.0f, 1.0f};
    
    // 地面平面方程
    static GLfloat floorPlane[4];
    static GLfloat floorShadow[4][4];
    static bool initialized = false;
    
    if (!initialized) {
        findPlane(floorPlane, floorVertices[1], floorVertices[2], floorVertices[3]);
        initialized = true;
    }
    
    // 计算阴影矩阵
    shadowMatrix(floorShadow, floorPlane, lightPosition);
    
    // 保存当前状态
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    // 禁用深度写入，但保持深度测试
    glDepthMask(GL_FALSE);
    
    // 设置混合模式
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // 禁用光照
    glDisable(GL_LIGHTING);
    
    // 设置更合适的阴影颜色 - 深灰色但不是纯黑
    glColor3f(0, 0, 0);
    
    // 稍微抬高阴影位置，避免z-fighting
    glPushMatrix();
    glTranslatef(0, 0.01f, 0);  // 轻微向上偏移
    glMultMatrixf((GLfloat*)floorShadow);
    drawBoxForShadow();
    glPopMatrix();
    
    // 恢复所有状态
    glPopAttrib();
}

// 绘制箱子装饰
void drawBoxDecorations() {
    // 确保使用不透明渲染
    glDisable(GL_BLEND);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // 前面的标志
    setMaterial(goldMaterial);
    
    glPushMatrix();
    glTranslatef(0, 0.2f, 1.88f);
    
    // 警告三角形
    glBegin(GL_TRIANGLES);
    glNormal3f(0, 0, 1);
    glVertex3f(0, 0.4f, 0);
    glVertex3f(-0.25f, -0.1f, 0);
    glVertex3f(0.25f, -0.1f, 0);
    glEnd();
    
    // 感叹号
    setMaterial({{0.8f, 0.1f, 0.1f, 1.0f}, {1.0f, 0.2f, 0.2f, 1.0f}, {0.5f, 0.5f, 0.5f, 1.0f}, 32.0f});
    glPushMatrix();
    glTranslatef(0, 0.1f, 0.005f);
    drawCube(0.04f, 0.2f, 0.015f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, -0.08f, 0.005f);
    drawCube(0.04f, 0.04f, 0.015f);
    glPopMatrix();
    
    glPopMatrix();
    
    // 金属角装饰
    setMaterial(metalMaterial);
    
    for (int i = 0; i < 4; i++) {
        float x = (i % 2 == 0) ? -2.3f : 2.3f;
        float z = (i < 2) ? 1.6f : -1.6f;
        
        glPushMatrix();
        glTranslatef(x, -1.6f, z);
        drawCube(0.2f, 0.08f, 0.2f);
        glPopMatrix();
    }
    
    // 铰链
    setMaterial(metalMaterial);
    for (int i = 0; i < 3; i++) {
        float x = -1.5f + i * 1.5f;
        glPushMatrix();
        glTranslatef(x, 1.0f, -1.75f);
        drawCylinder(0.08f, 0.3f);
        glPopMatrix();
    }
}

// 更新动画
void updateAnimation() {
    if (!animationStarted) return;
    
    animationTime += 0.8f; // 动画速度
    
    if (opening) {
        boxLidAngle += 1.2f;
        if (boxLidAngle >= 110.0f) {
            boxLidAngle = 110.0f;
            opening = false;
            animationTime = 0.0f; // 重置时间，准备关闭
        }
    } else {
        // 等待一段时间后开始关闭
        if (animationTime > 120.0f) { // 等待2秒左右
            boxLidAngle -= 1.0f;
            if (boxLidAngle <= 0.0f) {
                boxLidAngle = 0.0f;
                opening = true;
                animationTime = 0.0f; // 重置时间，准备下次开启
            }
        }
    }
}

// 显示回调函数
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    float camX = cameraDistance * cos(cameraAngleX * M_PI / 180.0f) * cos(cameraAngleY * M_PI / 180.0f);
    float camY = cameraDistance * sin(cameraAngleX * M_PI / 180.0f);
    float camZ = cameraDistance * cos(cameraAngleX * M_PI / 180.0f) * sin(cameraAngleY * M_PI / 180.0f);
    
    gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
    // 设置光源位置
    GLfloat currentLightPosition[] = {6.0f, 10.0f, 6.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, currentLightPosition);
    
    // 确保正常渲染状态
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    
    // 绘制半透明地面（参考lab8.cpp）
    glPushMatrix();
    glTranslatef(0, -2.1f, 0);
    glRotatef(-90, 1, 0, 0);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.4f, 0.4f, 0.4f, 0.9f);  // 稍微亮一点的地面，增加不透明度
    
    // 绘制地面四边形
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glVertex3f(-10.0f, -10.0f, 0);
    glVertex3f(10.0f, -10.0f, 0);
    glVertex3f(10.0f, 10.0f, 0);
    glVertex3f(-10.0f, 10.0f, 0);
    glEnd();
    
    glDisable(GL_BLEND);
    glPopMatrix();
    
    // 先绘制投影矩阵阴影（在物体下方）
    drawProjectiveShadows();
    
    // 确保恢复正常渲染状态后再绘制箱子
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glDepthMask(GL_TRUE);
    glColor3f(1.0f, 1.0f, 1.0f);  // 重置颜色为白色
    
    // 再绘制箱子组件
    drawInnerPadding();
    drawBoxBase();
    drawBoxDecorations();
    drawBoxLid();
    
    updateAnimation();
    
    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void idle() {
    glutPostRedisplay();
}

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

void mouseMotion(int x, int y) {
    if (mousePressed) {
        float deltaX = x - lastMouseX;
        float deltaY = y - lastMouseY;
        
        cameraAngleY += deltaX * 0.5f;
        cameraAngleX -= deltaY * 0.5f;
        
        if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
        if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;
        
        lastMouseX = x;
        lastMouseY = y;
        
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC
            exit(0);
            break;
        case ' ': // 空格键开始/暂停动画
            animationStarted = !animationStarted;
            if (animationStarted) {
                std::cout << "箱子开合动画开始！" << std::endl;
            } else {
                std::cout << "动画暂停" << std::endl;
            }
            break;
        case 'r':
        case 'R':
            // 重置动画状态
            boxLidAngle = 0.0f;
            animationTime = 0.0f;
            opening = true;
            animationStarted = false;
            cameraAngleX = 20.0f;
            cameraAngleY = 30.0f;
            cameraDistance = 12.0f;
            std::cout << "动画和视角已重置" << std::endl;
            break;
        case '+':
        case '=':
            cameraDistance -= 1.0f;
            if (cameraDistance < 3.0f) cameraDistance = 3.0f;
            break;
        case '-':
            cameraDistance += 1.0f;
            if (cameraDistance > 25.0f) cameraDistance = 25.0f;
            break;
    }
    glutPostRedisplay();
}

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
    
    if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
    if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;
    
    glutPostRedisplay();
}

void initGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // 参考lab8.cpp的polygon offset设置
    glPolygonOffset(-2.0f, -2.0f);
    
    // 光照参数设置
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
    
    // 材质设置
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    // 渲染设置
    glClearColor(0.15f, 0.15f, 0.2f, 1.0f);
    glShadeModel(GL_SMOOTH);
}

void printInstructions() {
    std::cout << "=== 箱子开合动画 - 透明度修复版 ===" << std::endl;
    std::cout << "一个会自动开合的音乐盒！" << std::endl;
    std::cout << "修复内容:" << std::endl;
    std::cout << "- 修复箱子透明度问题，确保完全不透明" << std::endl;
    std::cout << "- 优化渲染状态管理" << std::endl;
    std::cout << "- 改善阴影与物体的渲染分离" << std::endl;
    std::cout << "- 确保材质正确应用" << std::endl;
    std::cout << "操作说明:" << std::endl;
    std::cout << "- 空格键: 开始/暂停动画" << std::endl;
    std::cout << "- 鼠标左键拖拽: 旋转视角" << std::endl;
    std::cout << "- 方向键: 微调视角" << std::endl;
    std::cout << "- +/-键: 缩放" << std::endl;
    std::cout << "- R键: 重置动画和视角" << std::endl;
    std::cout << "- ESC: 退出程序" << std::endl;
    std::cout << "=========================" << std::endl;
    std::cout << "按空格键开始动画！" << std::endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("箱子开合动画 - 透明度修复版");
    
    initGL();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    printInstructions();
    
    glutMainLoop();
    
    return 0;
}
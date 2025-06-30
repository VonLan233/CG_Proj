#include <GLUT/glut.h>
#include <cmath>

// 窗口尺寸
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

// 绘制圆形
void drawCircle(float x, float y, float radius, int segments = 32) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // 圆心
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
    }
    glEnd();
}

// 绘制多边形
void drawPolygon(float vertices[][2], int numVertices) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < numVertices; i++) {
        glVertex2f(vertices[i][0], vertices[i][1]);
    }
    glEnd();
}

// 绘制三角形
void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    glBegin(GL_TRIANGLES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
}

// 绘制四边形
void drawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glVertex2f(x4, y4);
    glEnd();
}

// 显示回调函数
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // 绘制背景（深灰色/黑色）
    glColor3f(0.08f, 0.08f, 0.08f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();
    
    // 绘制上方的蓝色六边形区域
    glColor3f(0.2f, 0.5f, 0.8f); // 浅蓝色
    drawQuad(0.2f, 0.2f, -0.36f, 0.45f, -0.67f, 0.44f, -0.25f, 0.75f);
    
    // 绘制深蓝色三角形区域
    glColor3f(0.15f, 0.25f, 0.6f); // 深蓝色
    drawTriangle(0.2f, 0.2f, 0.25f, 0.6f,-0.25f, 0.75f);
    
    // 绘制黄色三角形（上方）
    glColor3f(0.9f, 0.8f, 0.2f); // 黄色
    drawTriangle(-0.25f, 0.75f, 0.11f, 0.79f, 0.25f, 0.6f);
    
    // 绘制左侧的米色四边形
    glColor3f(0.9f, 0.85f, 0.75f); // 米色/浅粉色
    drawQuad(-0.17f, 0.02f, -0.55f, 0.02f, -0.67f, 0.44f, -0.39f, 0.46f);
    
    // 绘制左下角的红色区域
    glColor3f(0.8f, 0.3f, 0.2f); // 红色
    float redShape[][2] = {
        {-0.39f, -0.18f},
        {0.05f, -0.05f},
        {0.25f, -0.84f},
        {-0.73f, -0.84f}
    };
    drawPolygon(redShape, 4);
    
    // 绘制中央的棕色大型四边形
    glColor3f(0.4f, 0.25f, 0.15f); // 棕色
    drawQuad(0.05f, -0.05f, -0.73f, -0.84f, 0.68f, -0.84f, 0.4f, 0.05f);
    
    // 绘制右侧的黄色长条形
    glColor3f(0.9f, 0.8f, 0.2f); // 黄色
    drawTriangle(0.25f, 0.6f, 0.54f, -0.36f, 0.83f, -0.11f);
    
    // 绘制右下角的白色三角形
    glColor3f(0.95f, 0.95f, 0.95f); // 白色
    drawQuad(0.54f, -0.36f, 0.61f, -0.6f, 0.94f, -0.32f, 0.83f, -0.11f);
    
    // 绘制左上角的棕色三角形
    glColor3f(0.5f, 0.35f, 0.25f); // 深棕色
    drawTriangle(-0.39f, 0.46f, -0.17f, 0.02f, 0.2f, 0.2f);
    
    // 绘制小白圆
    glColor3f(0.95f, 0.95f, 0.95f); // 白色
    drawCircle(-0.05f, 0.05f, 0.03f);
    
    // 添加一些额外的小细节形状
    // 蓝灰色小三角形
    // glColor3f(0.3f, 0.4f, 0.5f);
    // drawTriangle(-0.3f, 0.2f, -0.1f, 0.3f, -0.2f, 0.0f);
    
    glutSwapBuffers();
}

// 窗口重置回调
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// 初始化OpenGL
void initGL() {
    // 设置背景色为深灰色
    glClearColor(0.08f, 0.08f, 0.08f, 1.0f);
    
    // 启用反锯齿
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

int main(int argc, char** argv) {
    // 初始化GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("抽象几何艺术");
    
    // 初始化OpenGL
    initGL();
    
    // 注册回调函数
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    // 进入主循环
    glutMainLoop();
    
    return 0;
}
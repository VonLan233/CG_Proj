#include <GLUT/glut.h>
#include <cmath>
#include <vector>
#include <iostream>
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

// 全局变量
float rotation = 0.0f;
int windowWidth = 1200;
int windowHeight = 900;

// 顶点结构
struct Vertex {
    float x, y, z;
    float nx, ny, nz;  // 法向量
};

// 贝塞尔曲线计算函数
float binomialCoeff(int n, int k) {
    if (k == 0 || k == n) return 1.0f;
    float result = 1.0f;
    for (int i = 1; i <= k; i++) {
        result = result * (n - i + 1) / i;
    }
    return result;
}

// 3D贝塞尔曲线计算
Vertex calculateBezier3D(float t, const std::vector<Vertex>& points) {
    int n = points.size() - 1;
    Vertex result = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    
    for (int i = 0; i <= n; i++) {
        float coeff = binomialCoeff(n, i) * pow(1 - t, n - i) * pow(t, i);
        result.x += coeff * points[i].x;
        result.y += coeff * points[i].y;
        result.z += coeff * points[i].z;
    }
    
    return result;
}

// 2D贝塞尔曲线计算
void calculateBezier2D(float t, const std::vector<std::pair<float, float>>& points, float& x, float& y) {
    int n = points.size() - 1;
    x = 0.0f;
    y = 0.0f;
    
    for (int i = 0; i <= n; i++) {
        float coeff = binomialCoeff(n, i) * pow(1 - t, n - i) * pow(t, i);
        x += coeff * points[i].first;
        y += coeff * points[i].second;
    }
}

// 绘制花心
void drawFlowerCenter() {
    // 设置花心材质（黄色）
    GLfloat centerAmbient[] = {0.3f, 0.25f, 0.1f, 1.0f};
    GLfloat centerDiffuse[] = {1.0f, 0.8f, 0.2f, 1.0f};
    GLfloat centerSpecular[] = {0.8f, 0.8f, 0.4f, 1.0f};
    GLfloat centerShininess[] = {32.0f};
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, centerAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, centerDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, centerSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, centerShininess);
    
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    
    // 绘制花心（多层圆形）
    int segments = 20;
    float radius = 0.3f;
    
    for (int layer = 0; layer < 3; layer++) {
        float layerHeight = layer * 0.02f;
        float layerRadius = radius * (1.0f - layer * 0.1f);
        
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, layerHeight, 0.0f);
        
        for (int i = 0; i <= segments; i++) {
            float angle = (float)i / segments * 2.0f * M_PI;
            float x = layerRadius * cos(angle);
            float z = layerRadius * sin(angle);
            float y = layerHeight + 0.01f * sin(angle * 6.0f); // 细微起伏
            
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(x, y, z);
        }
        glEnd();
    }
    
    glPopMatrix();
}

// 绘制单个花瓣
void drawPetal(float rotationAngle) {
    // 设置花瓣材质（粉红色）
    GLfloat petalAmbient[] = {0.3f, 0.15f, 0.2f, 1.0f};
    GLfloat petalDiffuse[] = {0.9f, 0.4f, 0.6f, 1.0f};
    GLfloat petalSpecular[] = {0.8f, 0.6f, 0.7f, 1.0f};
    GLfloat petalShininess[] = {64.0f};
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, petalAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, petalDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, petalSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, petalShininess);
    
    glPushMatrix();
    glRotatef(rotationAngle * 180.0f / M_PI, 0.0f, 1.0f, 0.0f);
    
    // 贝塞尔曲线控制点（花瓣轮廓）
    std::vector<Vertex> controlPoints = {
        {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},      // 基部
        {0.2f, 0.3f, 0.1f, 0.0f, 1.0f, 0.0f},      // 下部弯曲
        {0.6f, 0.8f, 0.3f, 0.0f, 1.0f, 0.0f},      // 中部展开
        {0.8f, 1.2f, 0.2f, 0.0f, 1.0f, 0.0f},      // 上部
        {0.4f, 1.6f, 0.1f, 0.0f, 1.0f, 0.0f},      // 接近尖端
        {0.0f, 1.8f, 0.0f, 0.0f, 1.0f, 0.0f}       // 尖端
    };
    
    int uSegments = 15;
    int vSegments = 20;
    
    // 生成花瓣表面
    for (int u = 0; u < uSegments; u++) {
        glBegin(GL_TRIANGLE_STRIP);
        
        for (int v = 0; v <= vSegments; v++) {
            for (int du = 0; du <= 1; du++) {
                float t = (float)(u + du) / uSegments;
                float s = (float)v / vSegments - 0.5f;
                
                // 计算花瓣中心线
                Vertex centerPoint = calculateBezier3D(t, controlPoints);
                
                // 计算花瓣宽度（盛开效果）
                float width = 0.4f * sin(M_PI * t) * (1.0f - t * 0.3f);
                
                // 花瓣向外弯曲（盛开效果）
                float bloomCurve = sin(t * M_PI) * 0.3f;
                float bloomTilt = sin(t * M_PI) * 0.2f; // 花瓣向外倾斜
                
                // 计算花瓣上的点
                float localX = width * s;
                float localY = centerPoint.y;
                float localZ = centerPoint.z + bloomCurve * abs(s) + bloomTilt * t;
                
                // 计算法向量
                float normalX = localX * 0.3f;
                float normalY = 0.7f + 0.3f * sin(t * M_PI);
                float normalZ = 0.6f + 0.4f * abs(s);
                float normalLength = sqrt(normalX*normalX + normalY*normalY + normalZ*normalZ);
                normalX /= normalLength;
                normalY /= normalLength;
                normalZ /= normalLength;
                
                glNormal3f(normalX, normalY, normalZ);
                glVertex3f(localX, localY, localZ);
            }
        }
        
        glEnd();
    }
    
    glPopMatrix();
}

// 绘制花茎
void drawStem() {
    // 设置花茎材质（绿色）
    GLfloat stemAmbient[] = {0.1f, 0.2f, 0.1f, 1.0f};
    GLfloat stemDiffuse[] = {0.2f, 0.7f, 0.3f, 1.0f};
    GLfloat stemSpecular[] = {0.3f, 0.5f, 0.3f, 1.0f};
    GLfloat stemShininess[] = {16.0f};
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, stemAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, stemDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, stemSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, stemShininess);
    
    glPushMatrix();
    
    int segments = 12;
    int rings = 30;
    float height = 3.0f;
    float radius = 0.05f;
    
    for (int i = 0; i < rings; i++) {
        float y1 = -height * (float)i / rings;
        float y2 = -height * (float)(i + 1) / rings;
        float r1 = radius * (1.0f + 0.1f * sin(i * 0.5f));
        float r2 = radius * (1.0f + 0.1f * sin((i + 1) * 0.5f));
        
        glBegin(GL_TRIANGLE_STRIP);
        
        for (int j = 0; j <= segments; j++) {
            float angle = (float)j / segments * 2.0f * M_PI;
            float cosAngle = cos(angle);
            float sinAngle = sin(angle);
            
            // 上环
            glNormal3f(cosAngle, 0.0f, sinAngle);
            glVertex3f(r1 * cosAngle, y1, r1 * sinAngle);
            
            // 下环
            glNormal3f(cosAngle, 0.0f, sinAngle);
            glVertex3f(r2 * cosAngle, y2, r2 * sinAngle);
        }
        
        glEnd();
    }
    
    glPopMatrix();
}

// 绘制叶子
void drawLeaf(float rotationAngle, float yPos) {
    // 设置叶子材质（深绿色）
    GLfloat leafAmbient[] = {0.1f, 0.15f, 0.1f, 1.0f};
    GLfloat leafDiffuse[] = {0.3f, 0.6f, 0.2f, 1.0f};
    GLfloat leafSpecular[] = {0.2f, 0.4f, 0.2f, 1.0f};
    GLfloat leafShininess[] = {8.0f};
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, leafAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, leafDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, leafSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, leafShininess);
    
    glPushMatrix();
    glRotatef(rotationAngle * 180.0f / M_PI, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, yPos, 0.0f);
    
    // 叶子形状控制点
    std::vector<std::pair<float, float>> leafProfile = {
        {0.0f, 0.0f}, {0.3f, 0.2f}, {0.5f, 0.5f}, 
        {0.6f, 0.8f}, {0.4f, 1.0f}, {0.0f, 1.2f}
    };
    
    int segments = 15;
    int rings = 10;
    
    for (int i = 0; i < rings; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        
        for (int j = 0; j <= segments; j++) {
            for (int di = 0; di <= 1; di++) {
                float t = (float)(i + di) / rings;
                float s = (float)j / segments - 0.5f;
                
                float profileX, profileY;
                calculateBezier2D(t, leafProfile, profileX, profileY);
                
                float localX = profileX * s;
                float localY = profileY;
                float localZ = 0.05f * profileX * sin(s * M_PI);
                
                // 计算法向量
                float normalX = s * 0.2f;
                float normalY = 0.8f;
                float normalZ = 0.6f;
                float normalLength = sqrt(normalX*normalX + normalY*normalY + normalZ*normalZ);
                normalX /= normalLength;
                normalY /= normalLength;
                normalZ /= normalLength;
                
                glNormal3f(normalX, normalY, normalZ);
                glVertex3f(localX, localY, localZ);
            }
        }
        
        glEnd();
    }
    
    glPopMatrix();
}

// 绘制完整的花朵
void drawFlower() {
    glPushMatrix();
    
    // 绘制花心
    drawFlowerCenter();
    
    // 绘制8片花瓣（盛开状态）
    int petalCount = 8;
    for (int i = 0; i < petalCount; i++) {
        float angle = (float)i / petalCount * 2.0f * M_PI;
        drawPetal(angle);
    }
    
    // 绘制花茎
    drawStem();
    
    // 绘制叶子
    for (int i = 0; i < 3; i++) {
        float leafAngle = i * 2.0f * M_PI / 3.0f + M_PI / 6.0f;
        float leafHeight = -0.8f - i * 0.4f;
        drawLeaf(leafAngle, leafHeight);
    }
    
    glPopMatrix();
}

// 显示回调函数
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    
    // 设置摄像机
    gluLookAt(3.0f, 2.0f, 5.0f,   // 摄像机位置
              0.0f, 0.0f, 0.0f,   // 目标位置
              0.0f, 1.0f, 0.0f);  // 上方向
    
    // 应用旋转
    glRotatef(rotation, 0.0f, 1.0f, 0.0f);
    
    // 绘制花朵
    drawFlower();
    
    glutSwapBuffers();
}

// 重新调整窗口大小
void reshape(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(45.0f, (float)width / height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

// 空闲时回调（动画）
void idle() {
    rotation += 0.5f;
    if (rotation >= 360.0f) {
        rotation -= 360.0f;
    }
    glutPostRedisplay();
}

// 键盘回调
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC键
            exit(0);
            break;
        case 'r':
        case 'R':
            rotation = 0.0f;
            break;
    }
    glutPostRedisplay();
}

// 初始化OpenGL设置
void initGL() {
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    
    // 启用光照
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // 设置光源
    GLfloat lightPosition[] = {2.0f, 4.0f, 3.0f, 1.0f};
    GLfloat lightAmbient[] = {0.2f, 0.2f, 0.3f, 1.0f};
    GLfloat lightDiffuse[] = {1.0f, 0.95f, 0.8f, 1.0f};
    GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
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

// 主函数
int main(int argc, char** argv) {
    // 初始化GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("🌸 盛开的3D花朵 - GLUT版本");
    
    // 初始化OpenGL
    initGL();
    
    // 设置回调函数
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    
    // 显示使用说明
    std::cout << "==================================" << std::endl;
    std::cout << "🌸 盛开的3D花朵 - GLUT版本" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "控制说明：" << std::endl;
    std::cout << "ESC - 退出程序" << std::endl;
    std::cout << "R   - 重置旋转" << std::endl;
    std::cout << "==================================" << std::endl;
    
    // 进入主循环
    glutMainLoop();
    
    return 0;
}

/*
编译说明：

Ubuntu/Debian:
sudo apt-get install freeglut3-dev
g++ -o flower main.cpp -lglut -lGLU -lGL -lm

macOS:
brew install freeglut
g++ -o flower main.cpp -framework OpenGL -framework GLUT

Windows (MinGW):
g++ -o flower.exe main.cpp -lfreeglut -lopengl32 -lglu32

运行：
./flower
*/
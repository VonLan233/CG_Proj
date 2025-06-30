#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// ȫ�ֱ��������������ӽ�
float rotateX = -20.0f;  // ��ʼ������бһ��
float rotateY = 0.0f;
int lastMouseX, lastMouseY;
int mousePressed = 0;
bool redShow = true, yellowShow = true, LosiShow = true, baseShow = true, ShoeFace = true;

// �������Ʊ���
static float redPluginOffsetX = 150.0f;      // ��ɫ���X��ƫ��
static float yellowPluginOffsetX = -150.0f;   // ��ɫ���X��ƫ��
static float screwOffsetY = -30.0f;          // ��˿��Y��ƫ��
static bool screwDirection = true;         // ��˿��������� (true=�γ�, false=����)
static float shoeFaceOffsetY = 30.0f;         // Ь��Y��ƫ��
static bool shoeFaceDirection = true;        // Ь�淽����� (true=����, false=����)

// ����������
GLuint shoeTexture;                  // Ь������
GLuint underTexture;                 // Ь������
GLuint bindTexture;                  // ���Ӳ�������
GLuint shoePrintTexture;             // Ьӡ����
GLuint shoeFaceTexture;              // Ь������
GLuint bangTexture;                  // Bang����
GLuint basicTexture;                 // Basic����
GLuint luosiTexture;                 // ��˿����
GLuint magicTexture;                 // Magic����

// ���������غ���
static void loadTextures()
{
    glEnable(GL_TEXTURE_2D);        // ����2D����
    int width = 0, height = 0, channels = 0;

    // ����Ь������
    const char *filename = "C:/Users/LJT/Desktop/DMT2309228/Upper.png";
    stbi_uc *image = stbi_load(filename, &width, &height, &channels, 0);
    if (image) {
        printf("Ь��������سɹ�: %dx%d, channels: %d\n", width, height, channels);
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
        printf("Ь���������ʧ��: %s\n", filename);
    }

    // ����Ь������
    const char *underFilename = "C:/Users/LJT/Desktop/DMT2309228/Under.png";
    stbi_uc *underImage = stbi_load(underFilename, &width, &height, &channels, 0);
    if (underImage) {
        printf("Ь��������سɹ�: %dx%d, channels: %d\n", width, height, channels);
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
        printf("Ь���������ʧ��: %s\n", underFilename);
    }

    // �������Ӳ�������
    const char *bindFilename = "C:/Users/LJT/Desktop/DMT2309228/Bind.png";
    stbi_uc *bindImage = stbi_load(bindFilename, &width, &height, &channels, 0);
    if (bindImage) {
        printf("���Ӳ���������سɹ�: %dx%d, channels: %d\n", width, height, channels);
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
        printf("���Ӳ����������ʧ��: %s\n", bindFilename);
    }

    // ����Ьӡ����
    const char *shoePrintFilename = "C:/Users/LJT/Desktop/DMT2309228/ShoePrint.png";
    stbi_uc *shoePrintImage = stbi_load(shoePrintFilename, &width, &height, &channels, 0);
    if (shoePrintImage) {
        printf("Ьӡ������سɹ�: %dx%d, channels: %d\n", width, height, channels);
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
        printf("Ьӡ�������ʧ��: %s\n", shoePrintFilename);
    }

    // ����Ь������
    const char *shoeFaceFilename = "C:/Users/LJT/Desktop/DMT2309228/ShoeFace.png";
    stbi_uc *shoeFaceImage = stbi_load(shoeFaceFilename, &width, &height, &channels, 0);
    if (shoeFaceImage) {
        printf("Ь��������سɹ�: %dx%d, channels: %d\n", width, height, channels);
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
        printf("Ь������������\n");
    } else {
        printf("Ь���������ʧ��: %s\n", shoeFaceFilename);
    }
    const char *bangFilename = "C:/Users/LJT/Desktop/DMT2309228/Bang.png";
    stbi_uc *bangImage = stbi_load(bangFilename, &width, &height, &channels, 0);
    if (bangImage) {
        printf("Bang������سɹ�: %dx%d, channels: %d\n", width, height, channels);
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
        printf("Bang����������\n");
    } else {
        printf("Bang�������ʧ��: %s\n", bangFilename);
    }
    const char *basicFilename = "C:/Users/LJT/Desktop/DMT2309228/Basic.png";
    stbi_uc *basicImage = stbi_load(basicFilename, &width, &height, &channels, 0);
    if (basicImage) {
        printf("Basic������سɹ�: %dx%d, channels: %d\n", width, height, channels);
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
        printf("Basic����������\n");
    } else {
        printf("Basic�������ʧ��: %s\n", basicFilename);
    }
    const char *luosiFilename = "C:/Users/LJT/Desktop/DMT2309228/Luosi.png";
    stbi_uc *luosiImage = stbi_load(luosiFilename, &width, &height, &channels, 0);
    if (luosiImage) {
        printf("��˿������سɹ�: %dx%d, channels: %d\n", width, height, channels);
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
        printf("��˿����������\n");
    } else {
        printf("��˿�������ʧ��: %s\n", luosiFilename);
    }
    const char *magicFilename = "C:/Users/LJT/Desktop/DMT2309228/Magic.png";
    stbi_uc *magicImage = stbi_load(magicFilename, &width, &height, &channels, 0);
    if (magicImage) {
        printf("Magic������سɹ�: %dx%d, channels: %d\n", width, height, channels);
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
        printf("Magic����������\n");
    } else {
        printf("Magic�������ʧ��: %s\n", magicFilename);
    }
}

// �޸���Բ������ƺ����������������
void drawEllipticCylinderWithTexture(float centerX, float centerY, float centerZ,
                         float radiusX, float radiusZ, float height, int slices) {

    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);

    // ���Ʋ��棨���������꣩
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * M_PI * i / slices;
        float x = radiusX * cos(angle);
        float z = radiusZ * sin(angle);

        // ���㷨��������Բ�����棩
        float nx = cos(angle) / radiusX;
        float nz = sin(angle) / radiusZ;
        float len = sqrt(nx*nx + nz*nz);
        glNormal3f(nx/len, 0.0f, nz/len);

        // ��������
        float u = (float)i / slices;  // ˮƽ������������

        // �����
        glTexCoord2f(u, 0.0f);
        glVertex3f(x, 0.0f, z);
        // �����
        glTexCoord2f(u, 1.0f);
        glVertex3f(x, height, z);
    }
    glEnd();

    // ���Ƶ��棨���������꣩
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, -1.0f, 0.0f);  // ���µķ�����
    glTexCoord2f(0.5f, 0.5f);  // ������������
    glVertex3f(0.0f, 0.0f, 0.0f);   // ���ĵ�
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * M_PI * i / slices;
        float x = radiusX * cos(angle);
        float z = radiusZ * sin(angle);

        // Բ����������ӳ��
        float u = 0.5f + 0.5f * cos(angle);
        float v = 0.5f + 0.5f * sin(angle);
        glTexCoord2f(u, v);
        glVertex3f(x, 0.0f, z);
    }
    glEnd();

    // ���ƶ��棨���������꣩
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 1.0f, 0.0f);   // ���ϵķ�����
    glTexCoord2f(0.5f, 0.5f);  // ������������
    glVertex3f(0.0f, height, 0.0f); // ���ĵ�
    for (int i = slices; i >= 0; i--) {  // ��ʱ��˳��
        float angle = 2.0f * M_PI * i / slices;
        float x = radiusX * cos(angle);
        float z = radiusZ * sin(angle);

        // Բ����������ӳ��
        float u = 0.5f + 0.5f * cos(angle);
        float v = 0.5f + 0.5f * sin(angle);
        glTexCoord2f(u, v);
        glVertex3f(x, height, z);
    }
    glEnd();

    glPopMatrix();
}

// ����ʵ����Բ����
void drawEllipticCylinder(float centerX, float centerY, float centerZ,
                         float radiusX, float radiusZ, float height, int slices) {

    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);

    // ���Ʋ���
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * M_PI * i / slices;
        float x = radiusX * cos(angle);
        float z = radiusZ * sin(angle);

        // ���㷨��������Բ�����棩
        float nx = cos(angle) / radiusX;
        float nz = sin(angle) / radiusZ;
        float len = sqrt(nx*nx + nz*nz);
        glNormal3f(nx/len, 0.0f, nz/len);

        // �����
        glVertex3f(x, 0.0f, z);
        // �����
        glVertex3f(x, height, z);
    }
    glEnd();

    // ���Ƶ���
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, -1.0f, 0.0f);  // ���µķ�����
    glVertex3f(0.0f, 0.0f, 0.0f);   // ���ĵ�
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * M_PI * i / slices;
        float x = radiusX * cos(angle);
        float z = radiusZ * sin(angle);
        glVertex3f(x, 0.0f, z);
    }
    glEnd();

    // ���ƶ���
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 1.0f, 0.0f);   // ���ϵķ�����
    glVertex3f(0.0f, height, 0.0f); // ���ĵ�
    for (int i = slices; i >= 0; i--) {  // ��ʱ��˳��
        float angle = 2.0f * M_PI * i / slices;
        float x = radiusX * cos(angle);
        float z = radiusZ * sin(angle);
        glVertex3f(x, height, z);
    }
    glEnd();

    glPopMatrix();
}

// �������ν������壨X��Ϊ���θߣ�Y��Ϊ����ߣ�
// centerX, centerY, centerZ: �����µ��е�����
// bottomLength: �µ׳��ȣ�Z����
// topLength: �ϵ׳��ȣ�Z����
// trapHeight: ���εĸ߶ȣ�X����
// prismHeight: ����ĸ߶ȣ�Y����
void drawTrapezoidalPrism(float centerX, float centerY, float centerZ,
                         float bottomLength, float topLength,
                         float trapHeight, float prismHeight) {

    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);

    // �������εĹؼ���
    float bl = bottomLength * 0.5f;  // �µװ볤
    float tl = topLength * 0.5f;     // �ϵװ볤

    // �������ε�8������ (����4�� + ����4��)
    float bottomVerts[4][3] = {
        {0.0f,       0.0f, -bl},      // �����µ���
        {0.0f,       0.0f,  bl},      // �����µ���
        {trapHeight, 0.0f,  tl},      // �����ϵ���
        {trapHeight, 0.0f, -tl}       // �����ϵ���
    };

    float topVerts[4][3] = {
        {0.0f,       prismHeight, -bl},      // �����µ���
        {0.0f,       prismHeight,  bl},      // �����µ���
        {trapHeight, prismHeight,  tl},      // �����ϵ���
        {trapHeight, prismHeight, -tl}       // �����ϵ���
    };

    // ���Ƶ�������
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3fv(bottomVerts[0]);
    glVertex3fv(bottomVerts[1]);
    glVertex3fv(bottomVerts[2]);
    glVertex3fv(bottomVerts[3]);
    glEnd();

    // ���ƶ�������
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3fv(topVerts[0]);
    glVertex3fv(topVerts[3]);
    glVertex3fv(topVerts[2]);
    glVertex3fv(topVerts[1]);
    glEnd();

    // �����ĸ�����
    glBegin(GL_QUADS);

    // �µ��� (X=0����)
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3fv(bottomVerts[0]);
    glVertex3fv(topVerts[0]);
    glVertex3fv(topVerts[1]);
    glVertex3fv(bottomVerts[1]);

    // �ϵ��� (X=trapHeight����)
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3fv(bottomVerts[3]);
    glVertex3fv(bottomVerts[2]);
    glVertex3fv(topVerts[2]);
    glVertex3fv(topVerts[3]);

    // ����� (б�棬Z������)
    float nx_left = trapHeight;
    float nz_left = bl - tl;  // Z�������б
    float len_left = sqrt(nx_left*nx_left + nz_left*nz_left);
    glNormal3f(-nx_left/len_left, 0.0f, -nz_left/len_left);
    glVertex3fv(bottomVerts[0]);
    glVertex3fv(bottomVerts[3]);
    glVertex3fv(topVerts[3]);
    glVertex3fv(topVerts[0]);

    // �Ҳ��� (б�棬Z������)
    float nx_right = trapHeight;
    float nz_right = tl - bl;  // Z�������б
    float len_right = sqrt(nx_right*nx_right + nz_right*nz_right);
    glNormal3f(-nx_right/len_right, 0.0f, -nz_right/len_right);
    glVertex3fv(bottomVerts[1]);
    glVertex3fv(topVerts[1]);
    glVertex3fv(topVerts[2]);
    glVertex3fv(bottomVerts[2]);

    glEnd();

    glPopMatrix();
}

// ����1/4�����壨����X-Zƽ�����棬X���������1/4��
// centerX, centerY, centerZ: ��Բ����������
// radiusX, radiusY, radiusZ: ��������İ뾶
// slices: ���߷ֶ���
// stacks: γ�߷ֶ���
void drawQuarterEllipsoid(float centerX, float centerY, float centerZ,
                         float radiusX, float radiusY, float radiusZ,
                         int slices, int stacks) {

    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);

    // ������������棨Y������X������İ�Բ��
    for (int i = 0; i < stacks/2; i++) {  // ֻ�����°벿��
        float phi1 = M_PI * (-0.5 + (float)i / stacks);        // ��ǰγ��
        float phi2 = M_PI * (-0.5 + (float)(i + 1) / stacks);  // ��һ��γ��

        glBegin(GL_QUAD_STRIP);
        // ��Z���ᵽZ���ᣬ��ֻҪX������Ĳ���
        for (int j = -slices/4; j <= slices/4; j++) {  // �ȴ�-��/2����/2
            float theta = M_PI * j / (slices/2);  // ���ȣ�-��/2����/2

            // ��һ���� (��ǰγ��)
            float x1 = radiusX * cos(phi1) * cos(theta);
            float y1 = radiusY * sin(phi1);
            float z1 = radiusZ * cos(phi1) * sin(theta);

            // �ڶ����� (��һ��γ��)
            float x2 = radiusX * cos(phi2) * cos(theta);
            float y2 = radiusY * sin(phi2);
            float z2 = radiusZ * cos(phi2) * sin(theta);

            // ���㷨�������ڹ���
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

    // ����X-Zƽ������棨Y=0���棩- ����Բ
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 1.0f, 0.0f);  // ���ϵķ�����
    glVertex3f(0.0f, 0.0f, 0.0f);  // ���ĵ�

    for (int j = -slices/4; j <= slices/4; j++) {
        float theta = M_PI * j / (slices/2);  // �ȴ�-��/2����/2
        float x = radiusX * cos(theta);
        float z = radiusZ * sin(theta);
        glVertex3f(x, 0.0f, z);
    }
    glEnd();

    // ����Z����������棨X�����ᵽ���ģ�
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, -1.0f);  // ��Z������ķ�����
    glVertex3f(0.0f, 0.0f, 0.0f);  // ���ĵ�

    for (int i = 0; i <= stacks/2; i++) {
        float phi = M_PI * (-0.5 + (float)i / stacks);
        float x = radiusX * cos(phi);
        float y = radiusY * sin(phi);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    // ����Z����������棨X�����ᵽ���ģ�
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, 1.0f);  // ��Z������ķ�����
    glVertex3f(0.0f, 0.0f, 0.0f);  // ���ĵ�

    for (int i = stacks/2; i >= 0; i--) {  // ��ʱ��˳��
        float phi = M_PI * (-0.5 + (float)i / stacks);
        float x = radiusX * cos(phi);
        float y = radiusY * sin(phi);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    glPopMatrix();
}

// �����пհ���Բ���壨ȥ��Y�Ḻ���ᣬ����߶���X��������
// centerX, centerY, centerZ: ������Բ��������
// outerRadiusY, outerRadiusZ: ����Բ��Y���Z��뾶
// innerRadiusY, innerRadiusZ: ����Բ��Y���Z��뾶
// height: ����߶ȣ���X��������
// slices: Բ�ֶܷ���
void drawHollowHalfEllipticCylinderX(float centerX, float centerY, float centerZ,
                                   float outerRadiusY, float outerRadiusZ,
                                   float innerRadiusY, float innerRadiusZ,
                                   float height, int slices) {

    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);

    // ��������棨Y�������ᣩ
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices/2; i++) {  // ֻ����Y�����ᣬ��Z���ᵽZ����
        float angle = M_PI * i / (slices/2);  // �Ƕȴ�0����
        float y = outerRadiusY * sin(angle);  // sinȷ��Y��0
        float z = outerRadiusZ * cos(angle);

        // ��������淨����
        float nx = 0.0f;
        float ny = sin(angle) / outerRadiusY;  // ��Ӧsin
        float nz = cos(angle) / outerRadiusZ;  // ��Ӧcos
        float len = sqrt(ny*ny + nz*nz);

        glNormal3f(nx, ny/len, nz/len);

        // ����㣨X=0��
        glVertex3f(0.0f, y, z);
        // ����㣨X=height��
        glVertex3f(height, y, z);
    }
    glEnd();

    // �����ڱ��棨Y�������ᣩ
    glBegin(GL_QUAD_STRIP);
    for (int i = slices/2; i >= 0; i--) {  // ��������ڱ���
        float angle = M_PI * i / (slices/2);  // �ǶȴӦе�0
        float y = innerRadiusY * sin(angle);  // sinȷ��Y��0
        float z = innerRadiusZ * cos(angle);

        // �����ڱ��淨���������ڣ�
        float nx = 0.0f;
        float ny = -sin(angle) / innerRadiusY;  // ��Ӧsin������
        float nz = -cos(angle) / innerRadiusZ;  // ��Ӧcos������
        float len = sqrt(ny*ny + nz*nz);

        glNormal3f(nx, ny/len, nz/len);

        // ����㣨X=0��
        glVertex3f(0.0f, y, z);
        // ����㣨X=height��
        glVertex3f(height, y, z);
    }
    glEnd();

    // ���Ƶ��滷�Σ�X=0���棩
    glBegin(GL_QUAD_STRIP);
    glNormal3f(-1.0f, 0.0f, 0.0f);  // ��X������ķ�����
    for (int i = 0; i <= slices/2; i++) {
        float angle = M_PI * i / (slices/2);

        // ����Բ��
        float outerY = outerRadiusY * sin(angle);  // sinȷ��Y��0
        float outerZ = outerRadiusZ * cos(angle);
        glVertex3f(0.0f, outerY, outerZ);

        // ����Բ��
        float innerY = innerRadiusY * sin(angle);  // sinȷ��Y��0
        float innerZ = innerRadiusZ * cos(angle);
        glVertex3f(0.0f, innerY, innerZ);
    }
    glEnd();

    // ���ƶ��滷�Σ�X=height���棩
    glBegin(GL_QUAD_STRIP);
    glNormal3f(1.0f, 0.0f, 0.0f);  // ��X������ķ�����
    for (int i = 0; i <= slices/2; i++) {
        float angle = M_PI * i / (slices/2);

        // ����Բ��
        float innerY = innerRadiusY * sin(angle);  // sinȷ��Y��0
        float innerZ = innerRadiusZ * cos(angle);
        glVertex3f(height, innerY, innerZ);

        // ����Բ��
        float outerY = outerRadiusY * sin(angle);  // sinȷ��Y��0
        float outerZ = outerRadiusZ * cos(angle);
        glVertex3f(height, outerY, outerZ);
    }
    glEnd();

    // ��������棨Z����������棩
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);  // ��Z������ķ�����
    // ����
    glVertex3f(0.0f, 0.0f, -outerRadiusZ);
    glVertex3f(0.0f, 0.0f, -innerRadiusZ);
    glVertex3f(height, 0.0f, -innerRadiusZ);
    glVertex3f(height, 0.0f, -outerRadiusZ);
    glEnd();

    // �����Ҷ��棨Z����������棩
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);  // ��Z������ķ�����
    // ����
    glVertex3f(0.0f, 0.0f, innerRadiusZ);
    glVertex3f(0.0f, 0.0f, outerRadiusZ);
    glVertex3f(height, 0.0f, outerRadiusZ);
    glVertex3f(height, 0.0f, innerRadiusZ);
    glEnd();

    glPopMatrix();
}

// �����п���Բ����
void drawHollowEllipticalCylinder(float centerX, float centerY, float centerZ,
                                 float outerRadiusX, float outerRadiusZ,
                                 float innerCenterX, float innerCenterZ,
                                 float innerRadiusX, float innerRadiusZ,
                                 float height, int slices) {
    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);

    // �������
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * M_PI * i / slices;
        float x = outerRadiusX * cos(angle);
        float z = outerRadiusZ * sin(angle);

        // ������ڷ�����
        float nx = cos(angle) / outerRadiusX;
        float nz = sin(angle) / outerRadiusZ;
        float length = sqrt(nx * nx + nz * nz);
        nx /= length;
        nz /= length;

        glNormal3f(nx, 0.0f, nz);

        // �����
        glVertex3f(x, 0.0f, z);
        // �����
        glVertex3f(x, height, z);
    }
    glEnd();

    // �����ڱ�
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * M_PI * i / slices;
        float x = innerCenterX + innerRadiusX * cos(angle);
        float z = innerCenterZ + innerRadiusZ * sin(angle);

        // �����ڱڷ����������ڣ�
        float nx = -cos(angle) / innerRadiusX;
        float nz = -sin(angle) / innerRadiusZ;
        float length = sqrt(nx * nx + nz * nz);
        nx /= length;
        nz /= length;

        glNormal3f(nx, 0.0f, nz);

        // �����
        glVertex3f(x, 0.0f, z);
        // �����
        glVertex3f(x, height, z);
    }
    glEnd();

    // ���Ƶ��滷����
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.0f, -1.0f, 0.0f);  // ���µķ�����
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * M_PI * i / slices;

        // ����Բ��
        float outerX = outerRadiusX * cos(angle);
        float outerZ = outerRadiusZ * sin(angle);
        glVertex3f(outerX, 0.0f, outerZ);

        // ����Բ��
        float innerX = innerCenterX + innerRadiusX * cos(angle);
        float innerZ = innerCenterZ + innerRadiusZ * sin(angle);
        glVertex3f(innerX, 0.0f, innerZ);
    }
    glEnd();

    // ���ƶ��滷����
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.0f, 1.0f, 0.0f);   // ���ϵķ�����
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * M_PI * i / slices;

        // ����Բ��
        float outerX = outerRadiusX * cos(angle);
        float outerZ = outerRadiusZ * sin(angle);
        glVertex3f(outerX, height, outerZ);

        // ����Բ��
        float innerX = innerCenterX + innerRadiusX * cos(angle);
        float innerZ = innerCenterZ + innerRadiusZ * sin(angle);
        glVertex3f(innerX, height, innerZ);
    }
    glEnd();

    glPopMatrix();
}

// �����пհ���Բ���壨ȥ��X�Ḻ���ᣬ����߶���Y��������������Բ���Բ�ͬ�ģ�
// centerX, centerY, centerZ: ����Բ������������
// outerRadiusX, outerRadiusZ: ����Բ��X���Z��뾶
// innerCenterX, innerCenterZ: ����Բ��X���Z������ƫ��
// innerRadiusX, innerRadiusZ: ����Բ��X���Z��뾶
// height: ����߶ȣ���Y��������
// slices: Բ�ֶܷ���
void drawHollowHalfEllipticCylinderY(float centerX, float centerY, float centerZ,
                                   float outerRadiusX, float outerRadiusZ,
                                   float innerCenterX, float innerCenterZ,
                                   float innerRadiusX, float innerRadiusZ,
                                   float height, int slices) {
    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);

    // ��������棨X�������ᣩ
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices/2; i++) {  // ֻ����X�����ᣬ��Z���ᵽZ����
        float angle = M_PI * i / (slices/2);  // �Ƕȴ�0����
        float x = outerRadiusX * sin(angle);  // sinȷ��X��0
        float z = outerRadiusZ * cos(angle);

        // ��������淨����
        float nx = sin(angle) / outerRadiusX;  // ��Ӧsin
        float ny = 0.0f;
        float nz = cos(angle) / outerRadiusZ;  // ��Ӧcos
        float len = sqrt(nx*nx + nz*nz);
        glNormal3f(nx/len, ny, nz/len);

        // ����㣨Y=0��
        glVertex3f(x, 0.0f, z);
        // ����㣨Y=height��
        glVertex3f(x, height, z);
    }
    glEnd();

    // �����ڱ��棨X�������ᣬʹ������Բ������ƫ�ƣ�
    glBegin(GL_QUAD_STRIP);
    for (int i = slices/2; i >= 0; i--) {  // ��������ڱ���
        float angle = M_PI * i / (slices/2);  // �ǶȴӦе�0
        float x = innerCenterX + innerRadiusX * sin(angle);  // ��������Բ��Xƫ��
        float z = innerCenterZ + innerRadiusZ * cos(angle);  // ��������Բ��Zƫ��

        // �����ڱ��淨���������ڣ����������Բ���ģ�
        float nx = -sin(angle) / innerRadiusX;  // ��Ӧsin������
        float ny = 0.0f;
        float nz = -cos(angle) / innerRadiusZ;  // ��Ӧcos������
        float len = sqrt(nx*nx + nz*nz);
        glNormal3f(nx/len, ny, nz/len);

        // ����㣨Y=0��
        glVertex3f(x, 0.0f, z);
        // ����㣨Y=height��
        glVertex3f(x, height, z);
    }
    glEnd();

    // ���Ƶ��滷�Σ�Y=0���棩
    glBegin(GL_QUAD_STRIP);
    glNormal3f(0.0f, -1.0f, 0.0f);  // ��Y������ķ�����
    for (int i = 0; i <= slices/2; i++) {
        float angle = M_PI * i / (slices/2);
        // ����Բ��
        float outerX = outerRadiusX * sin(angle);  // sinȷ��X��0
        float outerZ = outerRadiusZ * cos(angle);
        glVertex3f(outerX, 0.0f, outerZ);
        // ����Բ�㣨ʹ��ƫ�����ģ�
        float innerX = innerCenterX + innerRadiusX * sin(angle);
        float innerZ = innerCenterZ + innerRadiusZ * cos(angle);
        glVertex3f(innerX, 0.0f, innerZ);
    }
    glEnd();

    // ���ƶ��滷�Σ�Y=height���棩
    glBegin(GL_QUAD_STRIP);
    glNormal3f(0.0f, 1.0f, 0.0f);  // ��Y������ķ�����
    for (int i = 0; i <= slices/2; i++) {
        float angle = M_PI * i / (slices/2);
        // ����Բ�㣨ʹ��ƫ�����ģ�
        float innerX = innerCenterX + innerRadiusX * sin(angle);
        float innerZ = innerCenterZ + innerRadiusZ * cos(angle);
        glVertex3f(innerX, height, innerZ);
        // ����Բ��
        float outerX = outerRadiusX * sin(angle);  // sinȷ��X��0
        float outerZ = outerRadiusZ * cos(angle);
        glVertex3f(outerX, height, outerZ);
    }
    glEnd();

    // ��������棨Z����������棩
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);  // ��Z������ķ�����
    glVertex3f(0.0f, 0.0f, -outerRadiusZ);
    glVertex3f(innerCenterX, 0.0f, innerCenterZ - innerRadiusZ);
    glVertex3f(innerCenterX, height, innerCenterZ - innerRadiusZ);
    glVertex3f(0.0f, height, -outerRadiusZ);
    glEnd();

    // �����Ҷ��棨Z����������棩
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);  // ��Z������ķ�����
    glVertex3f(innerCenterX, 0.0f, innerCenterZ + innerRadiusZ);
    glVertex3f(0.0f, 0.0f, outerRadiusZ);
    glVertex3f(0.0f, height, outerRadiusZ);
    glVertex3f(innerCenterX, height, innerCenterZ + innerRadiusZ);
    glEnd();

    glPopMatrix();
}

// �����п��������壨�����οף�
// bottomCenterX, bottomCenterY, bottomCenterZ: ���ε����µױ��е�����
// bottomLength: �µ׳��ȣ���Z�ᣩ
// topLength: �ϵ׳��ȣ���Z�ᣩ
// trapezoidHeight: ���εĸߣ���X��������
// cylinderHeight: ����ĸߣ���Y��������
// holeCenterX, holeCenterZ: �����ο����ĵ�X,Z����
// holeSize: �����ο׵ı߳�
void drawHollowTrapezoidCylinder(float bottomCenterX, float bottomCenterY, float bottomCenterZ,
                                float bottomLength, float topLength, float trapezoidHeight,
                                float cylinderHeight, float holeCenterX, float holeCenterZ,
                                float holeSize) {

    // �������ε��ĸ�����
    float bottomZ1 = bottomCenterZ - bottomLength / 2.0f;
    float bottomZ2 = bottomCenterZ + bottomLength / 2.0f;
    float topZ1 = bottomCenterZ - topLength / 2.0f;
    float topZ2 = bottomCenterZ + topLength / 2.0f;

    float bottomX = bottomCenterX;
    float topX = bottomCenterX + trapezoidHeight;

    // ���������ο׵��ĸ�����
    float halfSize = holeSize / 2.0f;
    float holeX1 = holeCenterX - halfSize;
    float holeX2 = holeCenterX + halfSize;
    float holeZ1 = holeCenterZ - halfSize;
    float holeZ2 = holeCenterZ + halfSize;

    // 1. ��������������ĸ������
    glBegin(GL_QUADS);

    // ǰ�棨Z�Ḻ����
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(bottomX, bottomCenterY, bottomZ1);
    glVertex3f(bottomX, bottomCenterY + cylinderHeight, bottomZ1);
    glVertex3f(topX, bottomCenterY + cylinderHeight, topZ1);
    glVertex3f(topX, bottomCenterY, topZ1);

    // ���棨Z��������
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(topX, bottomCenterY, topZ2);
    glVertex3f(topX, bottomCenterY + cylinderHeight, topZ2);
    glVertex3f(bottomX, bottomCenterY + cylinderHeight, bottomZ2);
    glVertex3f(bottomX, bottomCenterY, bottomZ2);

    // ���б��
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

    // �Ҳ�б��
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

    // 2. ���������ο׵��ĸ��ڱ���
    glBegin(GL_QUADS);

    // �����ο׵�ǰ�棨Z�Ḻ����
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(holeX1, bottomCenterY, holeZ1);
    glVertex3f(holeX2, bottomCenterY, holeZ1);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, holeZ1);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, holeZ1);

    // �����ο׵ĺ��棨Z��������
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(holeX2, bottomCenterY, holeZ2);
    glVertex3f(holeX1, bottomCenterY, holeZ2);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, holeZ2);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, holeZ2);

    // �����ο׵����棨X�Ḻ����
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(holeX1, bottomCenterY, holeZ2);
    glVertex3f(holeX1, bottomCenterY, holeZ1);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, holeZ1);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, holeZ2);

    // �����ο׵����棨X��������
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(holeX2, bottomCenterY, holeZ1);
    glVertex3f(holeX2, bottomCenterY, holeZ2);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, holeZ2);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, holeZ1);

    glEnd();

    // 3. ���Ƶ��棨8���ı�������
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);

    // �������½�����
    glVertex3f(bottomX, bottomCenterY, bottomZ1);
    glVertex3f(bottomX, bottomCenterY, holeZ1);
    glVertex3f(holeX1, bottomCenterY, holeZ1);
    glVertex3f(holeX1, bottomCenterY, bottomZ1);

    // �������Ͻ�����
    glVertex3f(bottomX, bottomCenterY, holeZ2);
    glVertex3f(bottomX, bottomCenterY, bottomZ2);
    glVertex3f(holeX1, bottomCenterY, bottomZ2);
    glVertex3f(holeX1, bottomCenterY, holeZ2);

    // �������½�����
    glVertex3f(holeX2, bottomCenterY, bottomZ1);
    glVertex3f(holeX2, bottomCenterY, holeZ1);
    glVertex3f(topX, bottomCenterY, holeZ1);
    glVertex3f(topX, bottomCenterY, topZ1);

    // �������Ͻ�����
    glVertex3f(holeX2, bottomCenterY, holeZ2);
    glVertex3f(holeX2, bottomCenterY, bottomZ2);
    glVertex3f(topX, bottomCenterY, topZ2);
    glVertex3f(topX, bottomCenterY, holeZ2);

    // ������������
    glVertex3f(holeX1, bottomCenterY, bottomZ1);
    glVertex3f(holeX1, bottomCenterY, holeZ1);
    glVertex3f(holeX2, bottomCenterY, holeZ1);
    glVertex3f(holeX2, bottomCenterY, bottomZ1);

    // ������������
    glVertex3f(holeX1, bottomCenterY, holeZ2);
    glVertex3f(holeX1, bottomCenterY, bottomZ2);
    glVertex3f(holeX2, bottomCenterY, bottomZ2);
    glVertex3f(holeX2, bottomCenterY, holeZ2);

    // ������������
    glVertex3f(bottomX, bottomCenterY, holeZ1);
    glVertex3f(bottomX, bottomCenterY, holeZ2);
    glVertex3f(holeX1, bottomCenterY, holeZ2);
    glVertex3f(holeX1, bottomCenterY, holeZ1);

    // ������������
    glVertex3f(holeX2, bottomCenterY, holeZ1);
    glVertex3f(holeX2, bottomCenterY, holeZ2);
    glVertex3f(topX, bottomCenterY, holeZ2);
    glVertex3f(topX, bottomCenterY, holeZ1);

    glEnd();

    // 4. ���ƶ��棨8���ı�������
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);

    // �������½�����
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, bottomZ1);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, holeZ1);
    glVertex3f(bottomX, bottomCenterY + cylinderHeight, holeZ1);
    glVertex3f(bottomX, bottomCenterY + cylinderHeight, bottomZ1);

    // �������Ͻ�����
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, holeZ2);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, bottomZ2);
    glVertex3f(bottomX, bottomCenterY + cylinderHeight, bottomZ2);
    glVertex3f(bottomX, bottomCenterY + cylinderHeight, holeZ2);

    // �������½�����
    glVertex3f(topX, bottomCenterY + cylinderHeight, topZ1);
    glVertex3f(topX, bottomCenterY + cylinderHeight, holeZ1);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, holeZ1);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, bottomZ1);

    // �������Ͻ�����
    glVertex3f(topX, bottomCenterY + cylinderHeight, holeZ2);
    glVertex3f(topX, bottomCenterY + cylinderHeight, topZ2);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, bottomZ2);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, holeZ2);

    // ������������
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, bottomZ1);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, holeZ1);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, holeZ1);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, bottomZ1);

    // ������������
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, holeZ2);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, bottomZ2);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, bottomZ2);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, holeZ2);

    // ������������
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, holeZ1);
    glVertex3f(holeX1, bottomCenterY + cylinderHeight, holeZ2);
    glVertex3f(bottomX, bottomCenterY + cylinderHeight, holeZ2);
    glVertex3f(bottomX, bottomCenterY + cylinderHeight, holeZ1);

    // ������������
    glVertex3f(topX, bottomCenterY + cylinderHeight, holeZ1);
    glVertex3f(topX, bottomCenterY + cylinderHeight, holeZ2);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, holeZ2);
    glVertex3f(holeX2, bottomCenterY + cylinderHeight, holeZ1);

    glEnd();
}

// �����пհ���Բ���壨ȥ��X�������ᣬ����߶���Y��������������Բ���Բ�ͬ�ģ�
// centerX, centerY, centerZ: ����Բ������������
// outerRadiusX, outerRadiusZ: ����Բ��X���Z��뾶
// innerCenterX, innerCenterZ: ����Բ��X���Z������ƫ��
// innerRadiusX, innerRadiusZ: ����Բ��X���Z��뾶
// height: ����߶ȣ���Y��������
// slices: Բ�ֶܷ���
void drawHollowHalfEllipticCylinderYNeg(float centerX, float centerY, float centerZ,
                                        float outerRadiusX, float outerRadiusZ,
                                        float innerCenterX, float innerCenterZ,
                                        float innerRadiusX, float innerRadiusZ,
                                        float height, int slices) {
    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);

    // ��������棨X�Ḻ���ᣩ
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices/2; i++) {  // ֻ����X�����ᣬ��Z���ᵽZ����
        float angle = M_PI/2 + M_PI * i / (slices/2);  // �ǶȴӦ�/2��3��/2
        float x = outerRadiusX * cos(angle);  // cosȷ��X��0
        float z = outerRadiusZ * sin(angle);

        // ��������淨����
        float nx = cos(angle) / outerRadiusX;  // ��Ӧcos
        float ny = 0.0f;
        float nz = sin(angle) / outerRadiusZ;  // ��Ӧsin
        float len = sqrt(nx*nx + nz*nz);
        glNormal3f(nx/len, ny, nz/len);

        // ����㣨Y=0��
        glVertex3f(x, 0.0f, z);
        // ����㣨Y=height��
        glVertex3f(x, height, z);
    }
    glEnd();

    // �����ڱ��棨X�Ḻ���ᣬʹ������Բ������ƫ�ƣ�
    glBegin(GL_QUAD_STRIP);
    for (int i = slices/2; i >= 0; i--) {  // ��������ڱ���
        float angle = M_PI/2 + M_PI * i / (slices/2);  // �Ƕȴ�3��/2����/2
        float x = innerCenterX + innerRadiusX * cos(angle);  // ��������Բ��Xƫ��
        float z = innerCenterZ + innerRadiusZ * sin(angle);  // ��������Բ��Zƫ��

        // �����ڱ��淨���������ڣ����������Բ���ģ�
        float nx = -cos(angle) / innerRadiusX;  // ��Ӧcos������
        float ny = 0.0f;
        float nz = -sin(angle) / innerRadiusZ;  // ��Ӧsin������
        float len = sqrt(nx*nx + nz*nz);
        glNormal3f(nx/len, ny, nz/len);

        // ����㣨Y=0��
        glVertex3f(x, 0.0f, z);
        // ����㣨Y=height��
        glVertex3f(x, height, z);
    }
    glEnd();

    // ���Ƶ��滷�Σ�Y=0���棩
    glBegin(GL_QUAD_STRIP);
    glNormal3f(0.0f, -1.0f, 0.0f);  // ��Y������ķ�����
    for (int i = 0; i <= slices/2; i++) {
        float angle = M_PI/2 + M_PI * i / (slices/2);
        // ����Բ��
        float outerX = outerRadiusX * cos(angle);  // cosȷ��X��0
        float outerZ = outerRadiusZ * sin(angle);
        glVertex3f(outerX, 0.0f, outerZ);
        // ����Բ�㣨ʹ��ƫ�����ģ�
        float innerX = innerCenterX + innerRadiusX * cos(angle);
        float innerZ = innerCenterZ + innerRadiusZ * sin(angle);
        glVertex3f(innerX, 0.0f, innerZ);
    }
    glEnd();

    // ���ƶ��滷�Σ�Y=height���棩
    glBegin(GL_QUAD_STRIP);
    glNormal3f(0.0f, 1.0f, 0.0f);  // ��Y������ķ�����
    for (int i = 0; i <= slices/2; i++) {
        float angle = M_PI/2 + M_PI * i / (slices/2);
        // ����Բ�㣨ʹ��ƫ�����ģ�
        float innerX = innerCenterX + innerRadiusX * cos(angle);
        float innerZ = innerCenterZ + innerRadiusZ * sin(angle);
        glVertex3f(innerX, height, innerZ);
        // ����Բ��
        float outerX = outerRadiusX * cos(angle);  // cosȷ��X��0
        float outerZ = outerRadiusZ * sin(angle);
        glVertex3f(outerX, height, outerZ);
    }
    glEnd();

    // ����ǰ���棨Z����������棩
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);  // ��Z������ķ�����
    glVertex3f(0.0f, 0.0f, outerRadiusZ);
    glVertex3f(innerCenterX, 0.0f, innerCenterZ + innerRadiusZ);
    glVertex3f(innerCenterX, height, innerCenterZ + innerRadiusZ);
    glVertex3f(0.0f, height, outerRadiusZ);
    glEnd();

    // ���ƺ���棨Z����������棩
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);  // ��Z������ķ�����
    glVertex3f(innerCenterX, 0.0f, innerCenterZ - innerRadiusZ);
    glVertex3f(0.0f, 0.0f, -outerRadiusZ);
    glVertex3f(0.0f, height, -outerRadiusZ);
    glVertex3f(innerCenterX, height, innerCenterZ - innerRadiusZ);
    glEnd();

    glPopMatrix();
}



// ����Z����Բ����
// centerX, centerY, centerZ: ����Բ������
// radius: ����뾶
// height: ����߶ȣ���Z��������
// slices: Բ�ֶܷ���
void drawCylinderZ(float centerX, float centerY, float centerZ,
                   float radius, float height, int slices) {
    glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);
    // ���Ʋ���
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * M_PI * i / slices;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        // ������淨���������⣩
        float nx = cos(angle);
        float ny = sin(angle);
        float nz = 0.0f;
        glNormal3f(nx, ny, nz);
        // ����㣨Z=0��
        glVertex3f(x, y, 0.0f);
        // ����㣨Z=height��
        glVertex3f(x, y, height);
    }
    glEnd();
    // ���Ƶ��棨Z=0��
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, -1.0f);  // ��Z������ķ�����
    glVertex3f(0.0f, 0.0f, 0.0f);   // ���ĵ�
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * M_PI * i / slices;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
    // ���ƶ��棨Z=height��
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, 1.0f);   // ��Z������ķ�����
    glVertex3f(0.0f, 0.0f, height); // ���ĵ�
    for (int i = slices; i >= 0; i--) {  // ��ʱ��˳��
        float angle = 2.0f * M_PI * i / slices;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex3f(x, y, height);
    }
    glEnd();
    glPopMatrix();
}

// ����ʵ��������
// x1, y1, z1: ��һ���������꣨ͨ������Сֵ�ǣ�
// x2, y2, z2: �ڶ����������꣨ͨ�������ֵ�ǣ�
void drawSolidCube(float x1, float y1, float z1, float x2, float y2, float z2) {

    // ȷ������˳����ȷ��min, max��
    float xmin = (x1 < x2) ? x1 : x2;
    float xmax = (x1 < x2) ? x2 : x1;
    float ymin = (y1 < y2) ? y1 : y2;
    float ymax = (y1 < y2) ? y2 : y1;
    float zmin = (z1 < z2) ? z1 : z2;
    float zmax = (z1 < z2) ? z2 : z1;

    glBegin(GL_QUADS);

    // ǰ�� (Z = zmax)
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(xmin, ymin, zmax);
    glVertex3f(xmax, ymin, zmax);
    glVertex3f(xmax, ymax, zmax);
    glVertex3f(xmin, ymax, zmax);

    // ���� (Z = zmin)
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(xmin, ymin, zmin);
    glVertex3f(xmin, ymax, zmin);
    glVertex3f(xmax, ymax, zmin);
    glVertex3f(xmax, ymin, zmin);

    // ���� (X = xmin)
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(xmin, ymin, zmin);
    glVertex3f(xmin, ymin, zmax);
    glVertex3f(xmin, ymax, zmax);
    glVertex3f(xmin, ymax, zmin);

    // ���� (X = xmax)
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(xmax, ymin, zmin);
    glVertex3f(xmax, ymax, zmin);
    glVertex3f(xmax, ymax, zmax);
    glVertex3f(xmax, ymin, zmax);

    // ���� (Y = ymin)
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(xmin, ymin, zmin);
    glVertex3f(xmax, ymin, zmin);
    glVertex3f(xmax, ymin, zmax);
    glVertex3f(xmin, ymin, zmax);

    // ���� (Y = ymax)
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(xmin, ymax, zmin);
    glVertex3f(xmin, ymax, zmax);
    glVertex3f(xmax, ymax, zmax);
    glVertex3f(xmax, ymax, zmin);

    glEnd();
}

// �������㸨������
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

void drawHexahedron(float x1, float y1, float z1,  // ����A (��ʼ��)
                    float x2, float y2, float z2,  // ����B
                    float x3, float y3, float z3,  // ����C
                    float x4, float y4, float z4)  // ����D
{
    // �����ĸ�����
    float A[3] = {x1, y1, z1};
    float B[3] = {x2, y2, z2};
    float C[3] = {x3, y3, z3};
    float D[3] = {x4, y4, z4};

    // ��������������
    float AB[3], AC[3], AD[3];
    vectorSubtract(AB, B, A);  // A��B������
    vectorSubtract(AC, C, A);  // A��C������
    vectorSubtract(AD, D, A);  // A��D������

    // ����ƽ���������8������
    float vertices[8][3];

    // ��������8������
    // ����0: A
    vertices[0][0] = A[0]; vertices[0][1] = A[1]; vertices[0][2] = A[2];

    // ����1: A + AB
    vectorAdd(vertices[1], A, AB);

    // ����2: A + AC
    vectorAdd(vertices[2], A, AC);

    // ����3: A + AD
    vectorAdd(vertices[3], A, AD);

    // ����4: A + AB + AC
    float temp[3];
    vectorAdd(temp, AB, AC);
    vectorAdd(vertices[4], A, temp);

    // ����5: A + AB + AD
    vectorAdd(temp, AB, AD);
    vectorAdd(vertices[5], A, temp);

    // ����6: A + AC + AD
    vectorAdd(temp, AC, AD);
    vectorAdd(vertices[6], A, temp);

    // ����7: A + AB + AC + AD
    vectorAdd(temp, AB, AC);
    vectorAdd(temp, temp, AD);
    vectorAdd(vertices[7], A, temp);

    // ���������棬ÿ���浥�����㷨����
    glBegin(GL_QUADS);

    // ��1: ���� (����0,1,4,2) - ����������
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

    // ��2: ���� (����3,6,7,5) - ����������
    vectorSubtract(v1, vertices[6], vertices[3]); // 3->6
    vectorSubtract(v2, vertices[5], vertices[3]); // 3->5
    vectorCross(normal, v1, v2);
    vectorNormalize(normal);
    glNormal3fv(normal);
    glVertex3fv(vertices[3]);
    glVertex3fv(vertices[6]);
    glVertex3fv(vertices[7]);
    glVertex3fv(vertices[5]);

    // ��3: ǰ�� (����0,3,5,1) - ����������
    vectorSubtract(v1, vertices[3], vertices[0]); // 0->3
    vectorSubtract(v2, vertices[1], vertices[0]); // 0->1
    vectorCross(normal, v1, v2);
    vectorNormalize(normal);
    glNormal3fv(normal);
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[3]);
    glVertex3fv(vertices[5]);
    glVertex3fv(vertices[1]);

    // ��4: ���� (����2,4,7,6) - ����������
    vectorSubtract(v1, vertices[4], vertices[2]); // 2->4
    vectorSubtract(v2, vertices[6], vertices[2]); // 2->6
    vectorCross(normal, v1, v2);
    vectorNormalize(normal);
    glNormal3fv(normal);
    glVertex3fv(vertices[2]);
    glVertex3fv(vertices[4]);
    glVertex3fv(vertices[7]);
    glVertex3fv(vertices[6]);

    // ��5: ���� (����0,2,6,3) - ����������
    vectorSubtract(v1, vertices[2], vertices[0]); // 0->2
    vectorSubtract(v2, vertices[3], vertices[0]); // 0->3
    vectorCross(normal, v1, v2);
    vectorNormalize(normal);
    glNormal3fv(normal);
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[2]);
    glVertex3fv(vertices[6]);
    glVertex3fv(vertices[3]);

    // ��6: ���� (����1,5,7,4) - ����������
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

// ������Ь�涯����ʱ������
void shoeFaceTimer(int value) {
    const float shoeFaceStep = 3.0f;    // Ь���ƶ�����
    const float maxOffset = 30.0f;      // ���ƫ����

    if(shoeFaceDirection) {
        // �����ƶ�
        shoeFaceOffsetY += shoeFaceStep;
        if(shoeFaceOffsetY >= maxOffset) {
            shoeFaceOffsetY = maxOffset;
        }
    } else {
        // �����ƶ�
        shoeFaceOffsetY -= shoeFaceStep;
        if(shoeFaceOffsetY <= 0.0f) {
            shoeFaceOffsetY = 0.0f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(60, shoeFaceTimer, 0); // ÿ60�������һ�Σ�����һ�㣩
}

// Ь�涯����ʱ������
void screwTimer(int value) {
    const float screwStep = 3.0f;
    const float maxOffset = 30.0f;

    if(screwDirection) {
        // �γ����� - �����ƶ�
        screwOffsetY -= screwStep;
        if(screwOffsetY <= -maxOffset) {
            screwOffsetY = -maxOffset;
        }
    } else {
        // ���붯�� - �����ƶ�
        screwOffsetY += screwStep;
        if(screwOffsetY >= 0.0f) {
            screwOffsetY = 0.0f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(50, screwTimer, 0); // ÿ50�������һ��
}

// display�ص�����
void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -300.0f);  // ����300��λ�۲���Բ��

    // Ӧ�������ת
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);

    // ������������Ϊ�ο�
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
        // X�� - ��ɫ
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-80.0f, 0.0f, 0.0f);
        glVertex3f(80.0f, 0.0f, 0.0f);

        // Y�� - ��ɫ
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -10.0f, 0.0f);
        glVertex3f(0.0f, 20.0f, 0.0f);

        // Z�� - ��ɫ
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, -70.0f);
        glVertex3f(0.0f, 0.0f, 70.0f);
    glEnd();
    glEnable(GL_LIGHTING);

    // �߿�ͼ����
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

/*
 * Ь��
 */
    glPushMatrix();
    if (ShoeFace) {
        // =================== ����Ь�� (��Y�ᶯ��) ===================
        glTranslatef(0.0f, shoeFaceOffsetY, 0.0f); // Ӧ��Y��ƫ�ƶ���

        // Ь���
        // ����Bang.png����
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, bangTexture);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // ����Bang�����������ɲ���
        GLfloat bangSPlane[] = {0.005f, 0.01f, 0.0f, 0.0f};  // X����
        GLfloat bangTPlane[] = {0.01f, 0.0f, 0.005f, 0.0f};  // Y����
        glTexGenfv(GL_S, GL_OBJECT_PLANE, bangSPlane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, bangTPlane);

        glColor3f(1.0f, 1.0f, 1.0f);  // ��ɫ����ʾ����ԭɫ

        drawCylinderZ(0.0f, 27.0f, -55.0f, 10.0f, 10.0f, 30);
        drawCylinderZ(0.0f, 27.0f, 45.0f, 10.0f, 10.0f, 30);
        drawSolidCube(0.0f, 22.0f, -50.0f, -90.0f, 32.0f, -45.0f);
        drawSolidCube(0.0f, 22.0f, 50.0f, -90.0f, 32.0f, 45.0f);
        drawHollowHalfEllipticCylinderYNeg(-90.0f, 22.0f, 0.0f, 44.0f, 50.0f, 0.0f, 0.0f, 39.0f, 45.0f, 10.0f, 30);

        // �ر�Bang����
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);

        // ����ShoeFace.png����
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, shoeFaceTexture);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // ����Ь�������������ɲ���
        GLfloat shoeFaceSPlane[] = {0.02f, 0.01f, 0.0f, 0.0f};  // X����
        GLfloat shoeFaceTPlane[] = {0.0f, 0.01f, 0.02f, 0.0f};  // Z����
        glTexGenfv(GL_S, GL_OBJECT_PLANE, shoeFaceSPlane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, shoeFaceTPlane);

        glColor3f(1.0f, 1.0f, 1.0f);  // ��ɫ����ʾ����ԭɫ
        // ����Ь��
        drawHollowHalfEllipticCylinderX(-20.0f, 37.0f, 0.0f, 22.0f, 45.0f, 17.0f, 40.0f, 70.0f, 30);
        drawSolidCube(-20.0f, 9.0f, -45.0f, 50.0f, 37.0f, -40.0f);
        drawSolidCube(-20.0f, 9.0f, 45.0f, 50.0f, 37.0f, 40.0f);

        // �ر�ShoeFace����
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, shoeFaceTexture);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // ����Ь�������������ɲ���
        GLfloat shoeFaceSsPlane[] = {0.02f, 0.0f, 0.0f, 0.0f};  // X����
        GLfloat shoeFaceTtPlane[] = {0.0f, 0.0f, 0.02f, 0.0f};  // Z����
        glTexGenfv(GL_S, GL_OBJECT_PLANE, shoeFaceSsPlane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, shoeFaceTtPlane);

        glColor3f(1.0f, 1.0f, 1.0f);  // ��ɫ����ʾ����ԭɫ
        // ����Ь��
        drawHollowHalfEllipticCylinderX(-20.0f, 37.0f, 0.0f, 22.0f, 45.0f, 17.0f, 40.0f, 70.0f, 30);

        // �ر�ShoeFace����
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);

        // ����ħ����
        // ����Magic.png����
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, magicTexture);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // ����Magic�����������ɲ���
        GLfloat magicSPlane[] = {0.03f, 0.0f, 0.0f, 0.0f};  // X����
        GLfloat magicTPlane[] = {0.0f, 0.0f, 0.03f, 0.0f};  // Z����
        glTexGenfv(GL_S, GL_OBJECT_PLANE, magicSPlane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, magicTPlane);

        glColor3f(1.0f, 1.0f, 1.0f);  // ��ɫ����ʾ����ԭɫ

        drawSolidCube(-20.0f, 8.0f, -45.0f, 50.0f, 9.0f, -35.0f);
        drawSolidCube(-20.0f, 8.0f, 45.0f, 50.0f, 9.0f, 35.0f);

        // �ر�Magic����
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);

/*
 * ����
 */

    // ����Ь������
    if (baseShow) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, shoeTexture);

        // �����Զ�������������
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);

        // ����������������ģʽ - ���ڶ������������ӳ��
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // ���������������ɵ����Է��̲���
        // S���� (U����) - ����X����
        GLfloat sPlane[] = {0.06f, 0.02f, 0.0f, 0.0f};  // ����S = 0.01*X
        glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);

        // T���� (V����) - ����Z����
        GLfloat tPlane[] = {0.02f, 0.0f, 0.06f, 0.0f};  // ����T = 0.01*Z
        glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);

        // ֱ��ʹ��ԭ�е�draw�����������޸�
        drawEllipticCylinder(-80.0f, 0.0f, 0.0f, 54.0f, 32.5f, 7.0f, 30);
        drawEllipticCylinder(20.0f, 0.0f, 0.0f, 100.0f, 50.0f, 7.0f, 30);
        drawTrapezoidalPrism(-80.0f, 0.0f, 0.0f, 65.0f, 85.0f, 54.0f, 7.0f);

        // �ر��Զ�������������
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);


        glColor3f(3, 3, 3);

        // glColor3f(0.5f, 0.5f, 0.5f);

        // ����Basic.png����
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, basicTexture);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // ����Basic�����������ɲ���
        GLfloat basicSPlane[] = {0.02f, 0.01f, 0.005f, 0.0f};  // S��Ҫ��X��ZӰ��
        GLfloat basicTPlane[] = {0.005f, 0.01f, 0.02f, 0.0f};  // T��Ҫ��YӰ��
        glTexGenfv(GL_S, GL_OBJECT_PLANE, basicSPlane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, basicTPlane);

        glColor3f(1.0f, 1.0f, 1.0f);  // ��ɫ����ʾ����ԭɫ

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

        // �ر�Basic����
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);


        // ����ħ����
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

        glColor3f(1.0f, 1.0f, 1.0f);  // ��ɫ����ʾ����ԭɫ
        drawSolidCube(-20.0f, 7.0f, -45.0f, 50.0f, 8.0f, -35.0f);
        drawSolidCube(-20.0f, 7.0f, 45.0f, 50.0f, 8.0f, 35.0f);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);

        // ���浱ǰ�任����
        glPushMatrix();
    }

    // ���浱ǰ�任����
    glPushMatrix();

    // ��������ʯ
    glColor3f(0.0f, 0.0f, 0.0f);
    drawHollowEllipticalCylinder(-12.5f, -1.0f, 0.0f, 5.5f, 5.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 30);

    if (LosiShow) {
        // =================== ������˿�� (��Y�ᶯ��) ===================
        glTranslatef(0.0f, screwOffsetY, 0.0f); // Ӧ��Y��ƫ�ƶ���

        // ����Luosi.png����
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, luosiTexture);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // ������˿�����������ɲ������ʺ�Բ���壩
        GLfloat luosiSPlane[] = {0.04f, 0.0f, 0.0f, 0.0f};   // X���򣬽��ܼ��ʺ�����
        GLfloat luosiTPlane[] = {0.0f, 0.04f, 0.0f, 0.0f};   // Y���򣬽��ܼ��ʺ�����
        glTexGenfv(GL_S, GL_OBJECT_PLANE, luosiSPlane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, luosiTPlane);

        glColor3f(1.0f, 1.0f, 1.0f);  // ��ɫ����ʾ����ԭɫ

        // ������˿���������֣�ȫ��������
        drawEllipticCylinder(-12.5f, -27.0f, 0.0f, 7.5f, 7.5f, 2.0f, 30);
        drawEllipticCylinder(-12.5f, -25.0f, 0.0f, 5.5f, 5.5f, 21.0f, 30);
        drawEllipticCylinder(-12.5f, -4.0f, 0.0f, 5.5f, 5.5f, 4.0f, 30);

        // �ر���˿����
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);
    }
    // �ָ��任����
    glPopMatrix();



    if (redShow) {
        // =================== ���ƺ�ɫ��� (��X�ᶯ��) ===================
        glTranslatef(redPluginOffsetX, 0.0f, 0.0f); // Ӧ��X��ƫ�ƶ���

        // ===== ������ɫ���������Under.png���� =====
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, underTexture);

        // �����Զ�������������
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // ʹ�û������ӳ��
        GLfloat sPlane[] = {0.05f, 0.02f, 0.0f, 0.0f};  // X+Y����
        GLfloat tPlane[] = {0.0f, 0.02f, 0.05f, 0.0f};  // Y+Z����
        glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);

        // ���ð�ɫ����ʾ����ԭɫ
        glColor3f(0.5f, 0.5f, 0.5f);
        // �������к�ɫ�����ȫ����Under.png����
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

        // ===== �ر�Under.png���� =====
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);

        // ����Ьӡ����ShoePrint.png����
        // ����ShoePrint.png����
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, shoePrintTexture);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // ����Ьӡ�����������ɲ���
        GLfloat ssPlane[] = {0.05f, 0.02f, 0.0f, 0.0f};  // X+Y����
        GLfloat ttPlane[] = {0.0f, 0.02f, 0.05f, 0.0f};  // Y+Z����
        glTexGenfv(GL_S, GL_OBJECT_PLANE, ssPlane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, ttPlane);

        glColor3f(1.0f, 1.0f, 1.0f);  // ��ɫ����ʾ����ԭɫ

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

        // �ر�Ьӡ����
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);
    }

    // �ָ��任����
    glPopMatrix();

    // ���浱ǰ�任����
    glPushMatrix();

    if (yellowShow) {
        // =================== ���ƻ�ɫ��� (��X�ᶯ��) ===================
        glTranslatef(yellowPluginOffsetX, 0.0f, 0.0f); // Ӧ��X��ƫ�ƶ���

        // ===== ������ɫ���������Under.png���� =====
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, underTexture);

        // �����Զ�������������
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // ʹ�û������ӳ��
        GLfloat sPlane[] = {0.05f, 0.02f, 0.0f, 0.0f};  // X+Y����
        GLfloat tPlane[] = {0.0f, 0.02f, 0.05f, 0.0f};  // Y+Z����
        glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);

        // ���ð�ɫ����ʾ����ԭɫ
        glColor3f(0.5f, 0.5f, 0.5f);

        // �������л�ɫ�����ȫ����Under.png����
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

        // ===== �ر�Under.png���� =====
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);

        // ����Ьӡ����ShoePrint.png����
        // �ȹر�֮ǰ��Under.png����
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);

        // ����ShoePrint.png����
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, shoePrintTexture);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

        // ����Ьӡ�����������ɲ���
        GLfloat ssPlane[] = {0.05f, 0.02f, 0.0f, 0.0f};  // X+Y����
        GLfloat ttPlane[] = {0.0f, 0.02f, 0.05f, 0.0f};  // Y+Z����
        glTexGenfv(GL_S, GL_OBJECT_PLANE, ssPlane);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, ttPlane);

        glColor3f(3.0f, 3.0f, 3.0f);  // ��ɫ����ʾ����ԭɫ

        glPushMatrix();
        drawSolidCube(-5.0f, -37.0f, -15.0f, -25.0f, -27.0f, -50.0f);
        drawSolidCube(-5.0f, -37.0f, 15.0f, -25.0f, -27.0f, 50.0f);
        drawSolidCube(-30.0f, -37.0f, -35.0f, -50.0f, -27.0f, 35.0f);
        drawSolidCube(-55.0f, -37.0f, -35.0f, -75.0f, -27.0f, 35.0f);
        drawSolidCube(-80.0f, -37.0f, -35.0f, -100.0f, -27.0f, 35.0f);
        drawHollowHalfEllipticCylinderYNeg(-105.0f, -37.0f, 0.0f, 40.0f, 35.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10.0f, 30);
        glPopMatrix();

        // �ر�Ьӡ����
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);

    }
    // �ָ��任����
    glPopMatrix();


    glutSwapBuffers();
}

// ���ڴ�С�ı�ص�
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // ����͸��ͶӰ���ʺϹ۲���Բ����
    gluPerspective(45.0, (double)width / (double)height, 1.0, 500.0);

    glMatrixMode(GL_MODELVIEW);
}

// ��갴�»ص�
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

// ����ƶ��ص�
void mouseMotion(int x, int y) {
    if (mousePressed) {
        rotateY += (x - lastMouseX) * 0.5f;
        rotateX += (y - lastMouseY) * 0.5f;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}

// ���̻ص����� (��ǿ�汾)
void keyboard(unsigned char key, int x, int y) {
    const float moveStep = 3.0f;  // �ƶ�����

    switch (key) {

        // ��ɫ�������
        case 'r':
        case 'R':
            redShow = !redShow;
            break;
        case 'w':
        case 'W':
            redPluginOffsetX += moveStep;
            if(redPluginOffsetX > 200.0f) redPluginOffsetX = 200.0f;
            //printf("��ɫ���X����: %.1f\n", redPluginOffsetX);
            break;

        case 's':
        case 'S':
            redPluginOffsetX -= moveStep;
            if(redPluginOffsetX < 0.0f) redPluginOffsetX = 0.0f;
            //printf("��ɫ���X����: %.1f\n", redPluginOffsetX);
            break;

        // ��ɫ�������
        case 'y':
        case 'Y':
            yellowShow = !yellowShow;
            break;
        case 'e':
        case 'E':
            yellowPluginOffsetX += moveStep;
            if(yellowPluginOffsetX > 0.0f) yellowPluginOffsetX = 0.0f;
            //printf("��ɫ���X����: %.1f\n", yellowPluginOffsetX);
            break;

        case 'd':
        case 'D':
            yellowPluginOffsetX -= moveStep;
            if(yellowPluginOffsetX < -200.0f) yellowPluginOffsetX = -200.0f;
            //printf("��ɫ���X����: %.1f\n", yellowPluginOffsetX);
            break;

        // ��˿������
        case 'l':
        case 'L':
            LosiShow = !LosiShow;
            break;
        case 'f':
        case 'F':
            screwDirection = !screwDirection;
            printf("��˿��ģʽ: %s\n", screwDirection ? "����" : "�γ�");
            break;
        // �������
        case 'b':
        case 'B':
            baseShow = !baseShow;
            break;
        // �������ж���
        case 'c':
        case 'C':
            redPluginOffsetX = 0.0f;
            yellowPluginOffsetX = 0.0f;
            screwOffsetY = 0.0f;
            screwDirection = false;
            shoeFaceOffsetY = 0.0f;
            shoeFaceDirection = false;
            printf("���ж���������\n");
            break;
        case 'm':
        case 'M':
            ShoeFace = !ShoeFace;
            break;
        case 'g':
        case 'G':
            shoeFaceDirection = !shoeFaceDirection;
            printf("Ь�涯��ģʽ: %s\n", shoeFaceDirection ? "����" : "����");
            break;

        case 27:  // ESC���˳�
            printf("�����˳�\n");
            exit(0);
            break;
    }

    glutPostRedisplay();
}

// ��ʼ������
void init() {
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);  // ����ɫ����
    glEnable(GL_DEPTH_TEST);

    // ���ù���
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);

    // ���ù�Դ
    GLfloat lightPos[] = {100.0f, 100.0f, 100.0f, 1.0f};
    GLfloat lightAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    // ���ò���
    GLfloat matAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat matDiffuse[] = {0.8f, 0.6f, 0.2f, 1.0f};
    GLfloat matSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat matShininess[] = {50.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    // *** �����һ�У��������� ***
    loadTextures();
}

// ������
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 750);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("3DЬ�Ӷ��� - ��Բ����ģ��");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);

    // ������˿��������ʱ��
    glutTimerFunc(50, screwTimer, 0);
    glutTimerFunc(60, shoeFaceTimer, 0);  // ������Ь�涯��

    // ��ӡ����˵��
    printf("=== 3DЬ�Ӷ�������˵�� ===\n");
    printf("������:\n");
    printf("- ��������ק: ��ת�ӽ�\n\n");
    printf("���̶�������:\n");
    printf("- W/w: ��ɫ�����ǰ�ƶ� \n");
    printf("- S/s: ��ɫ�������ƶ� \n");
    printf("- E/e: ��ɫ�����ǰ�ƶ� \n");
    printf("- D/d: ��ɫ�������ƶ� \n");
    printf("- F/f: ��˿������/�γ��л�\n");
    printf("- G/g: Ь�����/�γ�\n\n");
    printf("- R/r: ����/�رպ�ɫ���\n");
    printf("- Y/y: ����/�رջ�ɫ���\n");
    printf("- L/l: ����/�ر���˿��\n");
    printf("- B/b: ����/�ر�����\n");
    printf("- M/m: ����/�ر�Ь��\n");
    printf("- ESC: �˳�����\n");
    printf("================================\n\n");

    glutMainLoop();
    return 0;
}

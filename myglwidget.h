#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#define NUM_LS 5

#include <QWidget>
#include <QOpenGLWidget>
#include <QObject>
#include <QKeyEvent>
#include <QVector3D>
#include <QOpenGLFunctions_4_3_Core>
#include <QDebug>
#include <QOpenGLShaderProgram>
#include <QElapsedTimer>

#include "model.h"
#include "skybox.h"

class MyGLWidget : public QOpenGLWidget, private QOpenGLFunctions_4_3_Core
{

    Q_OBJECT
public:
    //QVector3D v_CameraPos = new QVector3D(0,0,0);
    QVector3D v_CameraPos = {0,0,0};
    int v_fov;
    int v_angle;
    bool v_perspective;
    int v_rotA;
    int v_rotB;
    int v_rotC;
    double v_far;
    double v_near;
    bool v_animate;
    bool v_camera;

    Model *inner_mod;
    Model *middle_mod;
    Model *outer_mod;
    Model *kugel;
    Model *lightSource1;
    Model *lightSource2;
    Model *lightSource3;
    Model *lightSource4;
    Model *lightSource5;

    float angleup;
    float angleside;

    QVector3D light1_color = QVector3D(1.0,1.0,1.0);
    QVector3D light2_color = QVector3D(1.0,0.0,0.0);
    QVector3D light3_color = QVector3D(0.0,1.0,0.0);
    QVector3D light4_color = QVector3D(0.0,0.0,1.0);
    QVector3D light5_color = QVector3D(0.0,1.0,1.0);

    double timer_rotate = 0;
    struct LightSource {
        alignas(16) QVector3D position;
        alignas(16) QVector3D color;
        float ka;
        float kd;
        float ks;
        float constant;
        float linear;
        float quadratic;
     };
    LightSource ls[NUM_LS];

    QElapsedTimer *animate_timer;

    Skybox *skybox;

    GLuint m_fbo;
    GLuint m_ibo;
    GLuint m_tex;
    GLuint m_tex_fbo;
    GLuint m_vbo;
    GLuint m_vao;
    GLuint uboLights;
    QOpenGLShaderProgram *m_prog;
    QOpenGLShaderProgram *m_prog2;
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();
    void keyPressEvent(QKeyEvent *event);
    void paintGL();
    void resizeGL(int w, int h);
    void initializeGL();
public slots:
    void setFOV(int value);
    void setAngle(int value);
    void setProjectionMode(bool value);
    void setNear(double value);
    void setFar(double value);
    void setRotationA(int value);
    void setRotationB(int value);
    void setRotationC(int value);
    void setAnimate(bool value);
    void setCamera(bool value);

signals:
    void fovChanged(int value);
    void angleChanged(int value);
    void projectionModeToggled();
    void nearChanged(double value);
    void farChanged(double value);
    void rotationAChanged(int value);
    void rotationBChanged(int value);
    void rotationCChanged(int value);
    void farOVFL(double value);
    void nearOVFL(double value);
    void animateChanged(bool value);
    void cameraChanged(bool value);


};

#endif // MYGLWIDGET_H

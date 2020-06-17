#include "myglwidget.h"
#include "skybox.h"
#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QDebug>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <array>
#include <QMatrix4x4>
#include <cmath>

MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
setFocusPolicy(Qt::StrongFocus);
}

MyGLWidget::~MyGLWidget(){
makeCurrent();

//glDeleteTextures(1, &m_tex);
//glDeleteBuffers(1, &m_vbo);
//glDeleteVertexArrays(1, &m_vao);
//glDeleteBuffers(1, &m_ibo);
//delete m_prog;
inner_mod->finiGL();
middle_mod->finiGL();
outer_mod->finiGL();


doneCurrent();
}


void MyGLWidget::paintGL(){
    // RENDERING // erase old pixels
    glClear(GL_COLOR_BUFFER_BIT);

    glClear(GL_DEPTH_BUFFER_BIT);
    // bind Resources

    QMatrix4x4 projection;
    projection.perspective(v_fov,((float) this->width()) / ((float) this->height()),v_near,v_far);


    QMatrix4x4 outer_ring_rotation;
    QMatrix4x4 middle_ring_rotation;
    QMatrix4x4 inner_ring_rotation;
    QMatrix4x4 kugel_animation;

    QMatrix4x4 lightRotation1;
    QVector4D light_temp1;
    QVector3D light1;

    QMatrix4x4 lightRotation2;
    QVector4D light_temp2;
    QVector3D light2;

    QMatrix4x4 lightRotation3;
    QVector4D light_temp3;
    QVector3D light3;

    QMatrix4x4 lightRotation4;
    QVector4D light_temp4;
    QVector3D light4;

    QMatrix4x4 lightRotation5;
    QVector4D light_temp5;
    QVector3D light5;


    //Set amb,diff,spec

    for(int i = 0; i < NUM_LS;i++){
        ls[i].ka = ((float)v_rotA) / 180.f;
        ls[i].kd = ((float)v_rotB) / 180.f;
        ls[i].ks = ((float)v_rotC) / 180.f;
    }



    if(v_animate == false){
    //QMatrix4x4 outer_ring_rotation;
    outer_ring_rotation.rotate(v_rotA,QVector3D(1.0,0.0,0.0));

    //QMatrix4x4 middle_ring_rotation;
    middle_ring_rotation.rotate(v_rotB,QVector3D(0.0,1.0,0.0));
    middle_ring_rotation = outer_ring_rotation * middle_ring_rotation;

    //QMatrix4x4 inner_ring_rotation;
    inner_ring_rotation.rotate(v_rotC,QVector3D(1.0,0.0,0.0));
    inner_ring_rotation = middle_ring_rotation * inner_ring_rotation;

    //QMatrix4x4 kugel_animation;
    kugel_animation.rotate(v_angle, QVector3D(0,0,1));              //bewegt sich ringförmig
    kugel_animation.translate(QVector3D(0.0, 10.0, 2.0));           //liegt auf gimbal
    kugel_animation = middle_ring_rotation * kugel_animation;       //bleibt relativ zu gimbal
    kugel_animation.rotate(v_angle, QVector3D(0,-1,0));             //dreht sich beim rollen

    lightRotation1.rotate(v_angle,QVector3D(1.0,0.0,0.0));
    lightRotation1.translate(QVector3D(0.0, 20.0, 0.0));
    light_temp1 =  QVector4D(1.0,1.0,1.0,1.0);
    light_temp1 = light_temp1 * lightRotation1;
    light1= QVector3D(light_temp1.x(),light_temp1.y(),light_temp1.z());
    ls[0].position = light1;

    lightRotation2.rotate(v_angle,QVector3D(0.0,0.0,1.0));
    lightRotation2.translate(QVector3D(20.0, 0.0, 0.0));
    light_temp2 =  QVector4D(1.0,1.0,1.0,1.0);
    light_temp2 = light_temp2 * lightRotation2;
    light2= QVector3D(light_temp2.x(),light_temp2.y(),light_temp2.z());
    ls[1].position = light2;

    lightRotation3.rotate(v_angle,QVector3D(0.5,0.0,0.5));
    lightRotation3.translate(QVector3D(15.0, 15.0, 0.4));
    light_temp3 =  QVector4D(1.0,1.0,1.0,1.0);
    light_temp3 = light_temp3 * lightRotation3;
    light3= QVector3D(light_temp3.x(),light_temp3.y(),light_temp3.z());
    ls[2].position = light3;

    lightRotation4.rotate(v_angle,QVector3D(0.5,0.0,0.5));
    lightRotation4.translate(QVector3D(5.0, 20.0, 0.0));
    light_temp4 =  QVector4D(1.0,1.0,1.0,1.0);
    light_temp4 = light_temp4 * lightRotation4;
    light4= QVector3D(light_temp4.x(),light_temp4.y(),light_temp4.z());
    ls[3].position = light4;

    lightRotation5.rotate(v_angle,QVector3D(0.5,0.0,0.5));
    lightRotation5.translate(QVector3D(0.0, 0.0, 0.0));
    light_temp5 =  QVector4D(1.0,1.0,1.0,1.0);
    light_temp5 = light_temp5 * lightRotation5;
    light5= QVector3D(light_temp5.x(),light_temp5.y(),light_temp5.z());
    ls[4].position = light5;


    } else {
    timer_rotate = timer_rotate + 0.5;
    //QMatrix4x4 outer_ring_rotation;
    outer_ring_rotation.rotate(timer_rotate/0.2,QVector3D(1.0,0.0,0.0));

    //QMatrix4x4 middle_ring_rotation;
    middle_ring_rotation.rotate(timer_rotate,QVector3D(0.0,1.0,0.0));
    middle_ring_rotation = outer_ring_rotation * middle_ring_rotation;

    //QMatrix4x4 inner_ring_rotation;
    inner_ring_rotation.rotate(timer_rotate/2,QVector3D(1.0,0.0,0.0));
    inner_ring_rotation = middle_ring_rotation * inner_ring_rotation;

    lightRotation1.rotate(timer_rotate,QVector3D(1.0,0.0,0.0));
    lightRotation1.translate(QVector3D(0.0, 20.0, 0.0));
    light_temp1 =  QVector4D(1.0,1.0,1.0,1.0);
    light_temp1 = light_temp1 * lightRotation1;
    light1= QVector3D(light_temp1.x(),light_temp1.y(),light_temp1.z());
    ls[0].position = light1;

    lightRotation2.rotate(timer_rotate,QVector3D(0.0,0.0,1.0));
    lightRotation2.translate(QVector3D(20.0, 0.0, 0.0));
    light_temp2 =  QVector4D(1.0,1.0,1.0,1.0);
    light_temp2 = light_temp2 * lightRotation2;
    light2= QVector3D(light_temp2.x(),light_temp2.y(),light_temp2.z());
    ls[1].position = light2;

    lightRotation3.rotate(timer_rotate/0.2,QVector3D(0.5,0.0,0.5));
    lightRotation3.translate(QVector3D(15.0, 15.0, 0.4));
    light_temp3 =  QVector4D(1.0,1.0,1.0,1.0);
    light_temp3 = light_temp3 * lightRotation3;
    light3= QVector3D(light_temp3.x(),light_temp3.y(),light_temp3.z());
    ls[2].position = light3;

    lightRotation4.rotate(timer_rotate/2,QVector3D(0.5,0.0,0.5));
    lightRotation4.translate(QVector3D(5.0, 20.0, 0.0));
    light_temp4 =  QVector4D(1.0,1.0,1.0,1.0);
    light_temp4 = light_temp4 * lightRotation4;
    light4= QVector3D(light_temp4.x(),light_temp4.y(),light_temp4.z());
    ls[3].position = light4;

    lightRotation5.rotate(timer_rotate,QVector3D(0.5,0.0,0.5));
    lightRotation5.translate(QVector3D(0.0, 0.0, 0.0));
    light_temp5 =  QVector4D(1.0,1.0,1.0,1.0);
    light_temp5 = light_temp5 * lightRotation5;
    light5= QVector3D(light_temp5.x(),light_temp5.y(),light_temp5.z());
    ls[4].position = light5;

    //QMatrix4x4 kugel_animation;
    kugel_animation.rotate(timer_rotate, QVector3D(0,0,1));              //bewegt sich ringförmig
    kugel_animation.translate(QVector3D(0.0, 10.0, 2.0));           //liegt auf gimbal
    kugel_animation = middle_ring_rotation * kugel_animation;       //bleibt relativ zu gimbal
    kugel_animation.rotate(v_angle, QVector3D(0,-1,0));
    }

    //Einheitsmatrix für Debug
    QMatrix4x4 test(
               1.0,0.0,0.0,0.0,
               0.0,1.0,0.0,0.0,
               0.0,0.0,1.0,0.0,
               0.0,0.0,0.0,1.0);

    QMatrix4x4 view;
    if(v_camera == false){
    view.lookAt(
        QVector3D(v_CameraPos.x(),v_CameraPos.y(),v_CameraPos.z() + 10.0),  //eye
        QVector3D(v_CameraPos.x(),0.0,0.0),   //center
        QVector3D(0.0,1.0,0.0)    //upvector
    );
    view.rotate(angleup,QVector3D(1.0,0.0,0.0));
    view.rotate(angleside,QVector3D(0.0,1.0,0.0));
    } else {
        view = inner_ring_rotation;
    }

    glBindBuffer(GL_UNIFORM_BUFFER, uboLights); //Bind Buer
    // Calculate something
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboLights);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ls), ls); // Übergebe LS an den Shader


    skybox->render(projection,view);

    inner_mod->m_prog->bind();
    skybox->getCubeMap(inner_mod);
    inner_mod->m_prog->setUniformValue(1,QVector4D(1.0,0.0,0.0,1.0));
    inner_mod->drawElements(v_fov,v_far,v_near,((float) this->width()) / ((float) this->height()),1.75,inner_ring_rotation,view,light1,QVector3D(0.05,0.05,0.05),QVector3D(0.05,0.05,0.05),QVector3D(0.7,0.7,0.7),0.078125,(((float)v_angle)*100.f)/360.f);

    middle_mod->m_prog->bind();
    middle_mod->m_prog->setUniformValue(1,QVector4D(0.0,1.0,0.0,1.0));
    middle_mod->drawElements(v_fov,v_far,v_near,((float) this->width()) / ((float) this->height()),2.1,middle_ring_rotation,view,light1,QVector3D(0.19225,0.19225,0.19225),QVector3D(0.50754,0.50754,0.50754),QVector3D(0.508273,0.508273,0.508273),0.4,(((float)v_angle)*100.f)/360.f);

    outer_mod->m_prog->bind();
    outer_mod->m_prog->setUniformValue(1,QVector4D(0.0,0.0,1.0,1.0));
    outer_mod->drawElements(v_fov,v_far,v_near,((float) this->width()) / ((float) this->height()),2.5,outer_ring_rotation,view,light1,QVector3D(0.0215,0.1745,0.0215),QVector3D(0.07568,0.633,0.07568),QVector3D(0.633,0.727811,0.633),0.6,(((float)v_angle)*100.f)/360.f);

    kugel->m_prog->bind();
    kugel->m_prog->setUniformValue(1,QVector4D(1.0,1.0,1.0,1.0));
    kugel->drawElements(v_fov,v_far,v_near,((float) this->width()) / ((float) this->height()),0.2,kugel_animation,view,light1,QVector3D(0.0,0.0,0.0),QVector3D(0.5,0.0,0.0),QVector3D(0.7,0.6,0.6),0.25,(((float)v_angle)*100.f)/360.f);

    lightSource1->m_prog->bind();
    lightSource1->m_prog->setUniformValue(20,light1_color);
    lightSource1->drawElements(v_fov,v_far,v_near,((float) this->width()) / ((float) this->height()),0.2,lightRotation1,view,light1,QVector3D(1.0,1.0,1.0),QVector3D(1.0,1.0,1.0),QVector3D(1.0,1.0,1.0),1,(((float)v_angle)*100.f)/360.f);

    lightSource2->m_prog->bind();
    lightSource2->m_prog->setUniformValue(20,light2_color);
    lightSource2->drawElements(v_fov,v_far,v_near,((float) this->width()) / ((float) this->height()),0.2,lightRotation2,view,light2,QVector3D(1.0,1.0,1.0),QVector3D(1.0,1.0,1.0),QVector3D(1.0,1.0,1.0),1,(((float)v_angle)*100.f)/360.f);

    lightSource3->m_prog->bind();
    lightSource3->m_prog->setUniformValue(20,light3_color);
    lightSource3->drawElements(v_fov,v_far,v_near,((float) this->width()) / ((float) this->height()),0.2,lightRotation3,view,light3,QVector3D(1.0,1.0,1.0),QVector3D(1.0,1.0,1.0),QVector3D(1.0,1.0,1.0),1,(((float)v_angle)*100.f)/360.f);

    lightSource4->m_prog->bind();
    lightSource4->m_prog->setUniformValue(20,light4_color);
    lightSource4->drawElements(v_fov,v_far,v_near,((float) this->width()) / ((float) this->height()),0.2,lightRotation4,view,light4,QVector3D(1.0,1.0,1.0),QVector3D(1.0,1.0,1.0),QVector3D(1.0,1.0,1.0),1,(((float)v_angle)*100.f)/360.f);

    lightSource5->m_prog->bind();
    lightSource5->m_prog->setUniformValue(20,light5_color);
    lightSource5->drawElements(v_fov,v_far,v_near,((float) this->width()) / ((float) this->height()),0.2,lightRotation5,view,light5,QVector3D(1.0,1.0,1.0),QVector3D(1.0,1.0,1.0),QVector3D(1.0,1.0,1.0),1,(((float)v_angle)*100.f)/360.f);
    /*QMatrix4x4 rotMat(
                cos(v_rotC),0,sin(v_rotC),0,
                0,1,0,0,
                -sin(v_rotC),0,cos(v_rotC),0,
                0,0,0,1
    );
    m_prog2->setUniformValue(2, rotMat);
    float alphaf = (float)v_rotA / 10;
    float offs = ((float)v_rotB / 5) -1;
    // write to uniform location 0
    m_prog2->setUniformValue(0, alphaf);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    m_prog->setUniformValue(7, 0);


    glBindVertexArray(m_vao);
    m_prog->bind();
    m_prog->setUniformValue(1, offs);
    // starting at vertex 0, render 3 vertices (=> 1 triangle)
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

    m_prog2->bind();

    // Render the second triangle
    void* const offset = reinterpret_cast<void* const>(sizeof(GLuint)*3);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, offset);

    glBindVertexArray(0);

    // request next frame to be drawn
    */
    update();
}

void MyGLWidget::resizeGL(int w, int h){

}

void MyGLWidget::initializeGL(){

    animate_timer = new QElapsedTimer;

    bool success = initializeOpenGLFunctions();
    Q_ASSERT(success); Q_UNUSED(success);

    glClearColor(0.3, 0.3, 0.3, 1.0);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    ls[0].position = QVector3D(0.0,0.0,0.0);
    ls[0].color = light1_color;
    ls[0].ka = 1.0;
    ls[0].kd = 1.0;
    ls[0].ks = 1.0;
    ls[0].constant = 1.0;
    ls[0].linear = 0.14;
    ls[0].quadratic = 0.07;

    ls[1].position = QVector3D(0.0,0.0,0.0);
    ls[1].color = light2_color;
    ls[1].ka = 1.0;
    ls[1].kd = 1.0;
    ls[1].ks = 1.0;
    ls[1].constant = 1.0;
    ls[1].linear = 0.14;
    ls[1].quadratic = 0.07;

    ls[2].position = QVector3D(0.0,0.0,0.0);
    ls[2].color = light3_color;
    ls[2].ka = 1.0;
    ls[2].kd = 1.0;
    ls[2].ks = 1.0;
    ls[2].constant = 1.0;
    ls[2].linear = 0.14;
    ls[2].quadratic = 0.07;

    ls[3].position = QVector3D(0.0,0.0,0.0);
    ls[3].color = light4_color;
    ls[3].ka = 1.0;
    ls[3].kd = 1.0;
    ls[3].ks = 1.0;
    ls[3].constant = 1.0;
    ls[3].linear = 0.14;
    ls[3].quadratic = 0.07;

    ls[4].position = QVector3D(0.0,0.0,0.0);
    ls[4].color = light5_color;
    ls[4].ka = 1.0;
    ls[4].kd = 1.0;
    ls[4].ks = 1.0;
    ls[4].constant = 1.0;
    ls[4].linear = 0.14;
    ls[4].quadratic = 0.07;

    inner_mod = new Model();
    inner_mod->initGL(":/gimbal.obj",":/pk3.vert",":/pk3.frag");

    middle_mod = new Model();
    middle_mod->initGL(":/gimbal.obj",":/pk3.vert",":/pk3.frag");

    outer_mod = new Model();
    outer_mod->initGL(":/gimbal.obj",":/pk3.vert",":/pk3.frag");

    kugel = new Model();
    kugel->initGL(":/sphere.obj",":/pk3.vert",":/pk3.frag");

    lightSource1 = new Model();
    lightSource1->initGL(":/sphere.obj",":/light.vert",":/light.frag");

    lightSource2 = new Model();
    lightSource2->initGL(":/sphere.obj",":/light.vert",":/light.frag");

    lightSource3 = new Model();
    lightSource3->initGL(":/sphere.obj",":/light.vert",":/light.frag");

    lightSource4 = new Model();
    lightSource4->initGL(":/sphere.obj",":/light.vert",":/light.frag");

    lightSource5 = new Model();
    lightSource5->initGL(":/sphere.obj",":/light.vert",":/light.frag");

    skybox = new Skybox();


    glGenBuffers(1, &uboLights);
    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
    glBufferData(GL_UNIFORM_BUFFER, 5 * sizeof (LightSource), nullptr, GL_STATIC_DRAW ); // Set Buer size, 64 Byte for each LS
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboLights);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    v_rotA = 0;
    v_rotB = 0;
    v_rotC = 0;
    v_fov = 45;

    v_near = 0.1;
    v_far = 99.9;
    v_angle = 0;

    angleup = 0;
    angleside = 0;

    v_CameraPos = QVector3D(0.0,0.0,0.0);
    /*struct Vertex {
        GLfloat position[2];
        GLfloat color[3];
        GLfloat t_position[2];
    };

    Vertex vert[4];
    vert[0].position[0] = 0.0;
    vert[0].position[1] = 0.5;
    vert[1].position[0] = -0.5;
    vert[1].position[1] = -0.5;
    vert[2].position[0] = 0.5;
    vert[2].position[1] = -0.5;
    vert[3].position[0] = 1.0;
    vert[3].position[1] = 0.5;
    vert[0].color[0] = 1.0;
    vert[0].color[1] = 0.0;
    vert[0].color[2] = 0.0;
    vert[1].color[0] = 0.0;
    vert[1].color[1] = 1.0;
    vert[1].color[2] = 0.0;
    vert[2].color[0] = 0.0;
    vert[2].color[1] = 0.0;
    vert[2].color[2] = 1.0;
    vert[3].color[0] = 0.0;
    vert[3].color[1] = 1.0;
    vert[3].color[2] = 0.0;
    vert[0].t_position[0] = 0.5;
    vert[0].t_position[1] = 0.75;
    vert[1].t_position[0] = 0.25;
    vert[1].t_position[1] = 0.25;
    vert[2].t_position[0] = 0.75;
    vert[2].t_position[1] = 0.25;
    vert[3].t_position[0] = 0.75;
    vert[3].t_position[1] = 0.25;


    // create buer object
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    // copy data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);


    glGenVertexArrays(1, &m_vao);
    // Start recording buer and attribute metadata
    glBindVertexArray(m_vao);
    // Associate earlier Vertex Buer with this VAO
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    // Enable and specify vertex attribute 0 (2 oats )
    // void* cast is necessary for legacy reasons

    // dene helper for osetof that does the void* cast
    #define OFS(s, a) reinterpret_cast<void* const>(offsetof(s, a))

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFS(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFS(Vertex, color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFS(Vertex, t_position));

    #undef OFS
    // Stop recording
    glBindVertexArray(0);

    glBindVertexArray(m_vao);
    // construct and ll IBO with data
    GLuint data[] = { 0,1,2,
                      0,2,3};
    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);


    QImage texImg;
    texImg.load(":/sample_texture.jpg");
    Q_ASSERT(!texImg.isNull());

    // Create texture object
    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    // ll with pixel data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                 texImg.width(), texImg.height(),
                 0, GL_BGRA, GL_UNSIGNED_BYTE, texImg.bits());
    // set ltering (interpolation) options
    // without these commands, _sampling will return black_
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // INITIALISIERUNG // bind texture we want to modify
    glBindTexture(GL_TEXTURE_2D, m_tex);

    // set wrap mode to "clamp to edge"
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // set wrap mode to "repeat"
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);



    m_prog = new QOpenGLShaderProgram();
    // addShader() compiles and attaches shader stages for linking
    m_prog->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/pk3.vert");
    m_prog->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/pk3.frag");
    // now link attached stages
    m_prog->link();
    // abort program if any of these steps failed
    Q_ASSERT(m_prog->isLinked());


    m_prog2 = new QOpenGLShaderProgram();
    // addShader() compiles and attaches shader stages for linking
    m_prog2->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/sample.vert");
    m_prog2->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/sample2.frag");
    // now link attached stages
    m_prog2->link();
    // abort program if any of these steps failed
    Q_ASSERT(m_prog2->isLinked());
    */

}

void MyGLWidget::setCamera(bool value){
v_camera = value;
}

void MyGLWidget::setAnimate(bool value){
    v_animate = value;
}

void MyGLWidget::setFOV(int value){
    v_fov = value;
}
void MyGLWidget::setAngle(int value){
    v_angle = value;
}
void MyGLWidget::setProjectionMode(bool value){
    v_perspective = value;
}
void MyGLWidget::setNear(double value){
    double diff = 0;
    bool diffb = false;
    if(v_far >= value){
        diff = v_far - value;
        if(diff < 2.0){
            diffb = true;
        } else {
            diffb = false;
        }
    } else {
        diffb = true;
    }
    if(!diffb){
        v_near = value;
    }
    else{
        emit nearChanged(v_near);
    }
}
void MyGLWidget::setFar(double value){
    double diff = 0;
    bool diffb = false;
    if(value >= v_near){
        diff = value - v_near;
        if(diff < 2.0){
            diffb = true;
        } else {
            diffb = false;
        }
    } else {
        diffb = true;
    }
    if(!diffb){
        v_far = value;
    }
    else{
        emit farChanged(v_far);
    }
}
void MyGLWidget::setRotationA(int value){
    v_rotA = value;
}
void MyGLWidget::setRotationB(int value){
    v_rotB = value;
}
void MyGLWidget::setRotationC(int value){
    v_rotC = value;
}


void MyGLWidget::keyPressEvent(QKeyEvent *event) {

    if (event->key() == Qt::Key_Escape) {
        // do something
        qInfo() << "test";
    }
    else if (event->key() == Qt::Key_W) {
        v_CameraPos.setZ(v_CameraPos.z() - 0.2);
        qInfo() << "Vector: " << v_CameraPos.x() << "," << v_CameraPos.y() << "," << v_CameraPos.z();
    }
    else if (event->key() == Qt::Key_S) {
        v_CameraPos.setZ(v_CameraPos.z() + 0.2);
        qInfo() << "Vector: " << v_CameraPos.x() << "," << v_CameraPos.y() << "," << v_CameraPos.z();
    }
    else if (event->key() == Qt::Key_A) {
        v_CameraPos.setX(v_CameraPos.x() - 0.2);
        qInfo() << "Vector: " << v_CameraPos.x() << "," << v_CameraPos.y() << "," << v_CameraPos.z();
    }
    else if (event->key() == Qt::Key_D) {
        v_CameraPos.setX(v_CameraPos.x() + 0.2);
        qInfo() << "Vector: " << v_CameraPos.x() << "," << v_CameraPos.y() << "," << v_CameraPos.z();
    }
    else if (event->key() == Qt::Key_Up) {
        angleup -= 2;
        qInfo() << "Vector: " << v_CameraPos.x() << "," << v_CameraPos.y() << "," << v_CameraPos.z();
    }
    else if (event->key() == Qt::Key_Down) {
        angleup += 2;
        qInfo() << "Vector: " << v_CameraPos.x() << "," << v_CameraPos.y() << "," << v_CameraPos.z();
    }
    else if (event->key() == Qt::Key_Left) {
        angleside -= 2;
        qInfo() << "Vector: " << v_CameraPos.x() << "," << v_CameraPos.y() << "," << v_CameraPos.z();
    }
    else if (event->key() == Qt::Key_Right) {
        angleside += 2;
        qInfo() << "Vector: " << v_CameraPos.x() << "," << v_CameraPos.y() << "," << v_CameraPos.z();
    }
    else {
        QOpenGLWidget::keyPressEvent(event);
    }
}

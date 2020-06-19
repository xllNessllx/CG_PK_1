#ifndef MODEL_H
#define MODEL_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QString>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>

//#include "common.h"

//class Model : OpenGLFunctions
class Model : QOpenGLFunctions_4_3_Core
{
public:
    Model();

    void initGL(const QString &filename,QString shader_name_vert,QString shader_name_frag);
    void drawElements(float fov,double v_far,double v_near,float aspectRatio,double scale,QMatrix4x4 rotation,QMatrix4x4 view,QVector3D light_pos,QVector3D amb_mat,QVector3D diff_mat, QVector3D spec_mat, float shine_mat,float shininess);
    void finiGL();
    QOpenGLShaderProgram *m_prog;

private:
    GLuint m_tex;

    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ibo;
    QOpenGLVertexArrayObject m_vao;

    unsigned m_iboCount = 0;
};

#endif // MODEL_H

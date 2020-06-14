#include <vector>

#include "model.h"
#include "modelloader/modelloader.h"
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>


#include <QDebug>

Model::Model()
    : m_vbo(QOpenGLBuffer::VertexBuffer),
      m_ibo(QOpenGLBuffer::IndexBuffer)
{}

void Model::initGL(const QString &filename,QString shader_name_vert,QString shader_name_frag) {
    initializeOpenGLFunctions();

    m_vbo.create();
    m_ibo.create();
    m_vao.create();

    m_vao.bind();
    m_vbo.bind();
    m_ibo.bind();

    ModelLoader ml;
    ml.loadObjectFromFile(filename);
    Q_ASSERT(ml.hasScene());

    unsigned vboLength = ml.lengthOfVBO();
    m_iboCount = ml.lengthOfIndexArray();

    // There's two ways to accomplish the buffer uploads;
    // the pointer mapping one appears more complex but
    // saves one needless allocation each
#define BUFFER_UPLOAD_COPY 0
#if BUFFER_UPLOAD_COPY == 1
    std::vector<GLfloat> vboData;
    std::vector<GLuint> iboData;

    vboData.resize(vboLength);
    iboData.resize(m_iboCount);

    ml.genVBO(vboData.data());
    ml.genIndexArray(iboData.data());

    m_vbo.allocate(vboData.data(), vboData.size()*sizeof(GLfloat));
    m_ibo.allocate(iboData.data(), iboData.size()*sizeof(GLuint));
#else
    m_vbo.allocate(vboLength*sizeof(GLfloat));
    m_ibo.allocate(m_iboCount*sizeof(GLuint));

    GLvoid *vboPtr = m_vbo.map(QOpenGLBuffer::WriteOnly);
    Q_ASSERT(vboPtr);

    ml.genVBO(static_cast<GLfloat*>(vboPtr));
    m_vbo.unmap();

    GLvoid *iboPtr = m_ibo.map(QOpenGLBuffer::WriteOnly);
    Q_ASSERT(iboPtr);

    ml.genIndexArray(static_cast<GLuint*>(iboPtr));
    m_ibo.unmap();
#endif

    size_t stride = (ML_VERTPOS_COUNT+ML_NORMAL_COUNT+ML_TEXCOOR_COUNT)*sizeof(GLfloat);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(sizeof(GLfloat)*0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(sizeof(GLfloat)*3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(sizeof(GLfloat)*6));

    m_vao.release();



    //mein Code

    //Textur
    QImage texImg;
    texImg.load(":/gimbal_wood.jpg");
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

    // set wrap mode to "clamp to edge"
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // set wrap mode to "repeat"
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Shader
    m_prog = new QOpenGLShaderProgram();
    // addShader() compiles and attaches shader stages for linking
    m_prog->addShaderFromSourceFile(QOpenGLShader::Vertex, shader_name_vert);
    m_prog->addShaderFromSourceFile(QOpenGLShader::Fragment, shader_name_frag);
    // now link attached stages
    m_prog->link();
    // abort program if any of these steps failed
    Q_ASSERT(m_prog->isLinked());

}

void Model::drawElements(float fov,double v_far,double v_near,float aspectRatio,double scale,QMatrix4x4 rotation,QMatrix4x4 view,QVector3D light_pos,QVector3D amb_mat,QVector3D diff_mat, QVector3D spec_mat,float shine_mat) {

    //Mein Code
    //Shader Program
    m_prog->bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tex);

    m_prog->setUniformValue(7, 0);

    m_prog->bind();


    //MVP
    QMatrix4x4 model;
    //QMatrix4x4 view;
    QMatrix4x4 projection;


    model.scale(QVector3D(scale,scale,scale));
    model = model * rotation;

    //view.lookAt(
    //    QVector3D(0.0,0.0,10.0),  //eye
    //    QVector3D(0.0,0.0,0.0),   //center
    //    QVector3D(0.0,1.0,0.0)    //upvector
    //);

    //projection.perspective(fov,aspectRatio,v_near,v_far);
    projection.perspective(fov,aspectRatio,v_near,v_far);



    QMatrix4x4 mvp = projection * view * model;



    //Einheitsmatrix für Debug

    QMatrix4x4 test(
               1.0,0.0,0.0,0.0,
               0.0,1.0,0.0,0.0,
               0.0,0.0,1.0,0.0,
               0.0,0.0,0.0,1.0);



    //qInfo() << mvp;
    //qInfo() << aspectRatio;

    /*
    QMatrix4x4 test2(
       2.34812,-0.168407,0,         0,
      0.164197,   2.40833  ,       0   ,      0,
             0  ,       0  , 1.0002 , -0.20002,
             0     ,    0    ,    0   ,      1
            );
    */
    m_prog->setUniformValue(0, mvp);

    float smallmvp[] = {
            model.row(0).x(), model.row(0).y(), model.row(0).z(),
            model.row(1).x(), model.row(1).y(), model.row(1).z(),
            model.row(2).x(), model.row(2).y(), model.row(2).z()
        };

    QMatrix3x3 model_mvp(smallmvp);

    m_prog->setUniformValue(0, mvp);
    m_prog->setUniformValue(2,model_mvp);
   // m_prog->setUniformValue(2,model);
    m_prog->setUniformValue(3,model.inverted().transposed());
    QVector3D camp = QVector3D(0.0f, 0.0f, 10.0f);
    m_prog->setUniformValue(8,camp);
    //QVector3D light = QVector3D(0.0f, 0.0f, 2.0f);
    m_prog->setUniformValue(9,light_pos);
    m_prog->setUniformValue(10,1.0f);
    m_prog->setUniformValue(11,1.0f);
    m_prog->setUniformValue(12,1.0f);
    m_prog->setUniformValue(13,100.0f);

    //Material
    m_prog->setUniformValue(14,amb_mat);
    m_prog->setUniformValue(15,diff_mat);
    m_prog->setUniformValue(16,spec_mat);
    m_prog->setUniformValue(17,shine_mat);


    m_vao.bind();

    glDrawElements(GL_TRIANGLES, m_iboCount, GL_UNSIGNED_INT, nullptr);

    m_vao.release();
    m_prog->release();
}

void Model::finiGL() {
    m_vao.destroy();
    m_ibo.destroy();
    m_vbo.destroy();
}

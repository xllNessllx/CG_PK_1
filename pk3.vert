
#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 normalmap;
layout(location = 2) in vec2 uPosition;

layout(location = 0) uniform mat4x4 mvp_matrix;
layout(location = 2) uniform mat3x3 uModelMatrix;
layout(location = 3) uniform mat4x4 uModelMatrix_inverse_t;

layout(location = 1) out vec2 v_Position;
layout(location = 2) out vec3 vNormal;
layout(location = 3) out vec3 vFragPos;





void main() {

    v_Position = uPosition.st;
    //vNormal = normalmap;
    vNormal = normalize(transpose(inverse(uModelMatrix)) * normalmap);
    //vNormal = normalize(vec3(uModelMatrix_inverse_t * vec4(aNormal, 1.0)));



    vFragPos = aPosition * uModelMatrix;
    //vFragPos = vec3(vec4(aPosition, 1.0) * uModelMatrix);
    // pass color to fragment shader

    // dene vertex position in space.
    // must be 4-dimensional, so supplement Z and W values.
    vec4 vertHom = vec4(aPosition, 1.0);
    //vec4 pos = vec4(aPosition, 1.0);
    gl_Position = mvp_matrix * vertHom;

}


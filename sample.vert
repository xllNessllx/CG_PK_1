#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 uPosition;
layout(location = 2) uniform mat4x4 urotmat;

layout(location = 0) out vec3 vColor;
layout(location = 1) out vec2 v_Position;

void main() {

    // pass color to fragment shader
    vColor = aColor;
    v_Position = uPosition;

    // dene vertex position in space.
    // must be 4-dimensional, so supplement Z and W values.
    vec4 pos = vec4(aPosition, 0.0, 1.0);

    gl_Position = urotmat * pos;

}

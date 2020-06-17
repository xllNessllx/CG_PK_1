
#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 20) uniform vec3 col;

layout(location = 0) out vec4 fragColor;

void main() {

//fragColor = vec4(1.0,1.0,1.0,1.0);
fragColor = vec4(col,1.0);
}

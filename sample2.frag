#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 v_Position;

layout(location = 0) uniform float uAlpha;

// output written to the screen is always location 0
layout(location = 0) out vec4 fragColor;

void main() {

    // write white color to every fragment
    //vec4 fragColor = texture(tex0, pos);
    fragColor = vec4(vColor,uAlpha);
}

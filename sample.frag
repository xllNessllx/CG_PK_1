#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 v_Position;

layout(location = 1) uniform float offset;
layout(location = 7) uniform sampler2D tex0;

// output written to the screen is always location 0
layout(location = 0) out vec4 fragColor;

void main() {

    vec2 pos = {v_Position.x + offset, v_Position.y + offset};
    // write white color to every fragment
    fragColor = texture(tex0, pos);
    //fragColor = vec4(0.0,0.0,offset,1.0);
}

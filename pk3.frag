
#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

#define NUM_LS 3

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight { // base alignment // aligned oset // total bytes
    vec3 position; // 16 0 16

    vec3 color; // 16 16 32

    float ambient; // 4 32 36
    float diffuse; // 4 36 40
    float specular; // 4 40 44

    float constant; // 4 44 48
    float linear; // 4 48 52 
    float quadratic; // 4 52 56
};


//layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 v_Position;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec3 vFragPos;

//layout(location = 1) uniform float offset;
layout(location = 1) uniform vec4 ringfarbe;
layout(location = 7) uniform sampler2D tex0;
layout (location = 8) uniform vec3 uViewPosition;
layout (location = 9) uniform vec3 uLightPosition;
layout (location = 10) uniform float uKa;
layout (location = 11) uniform float uKd;
layout (location = 12) uniform float uKs;
layout (location = 13) uniform float uShininess;
layout (location = 14) uniform Material uValues;

layout (std140) uniform lightBlock {
    // Create a UBO with layout std140 and Title lightBlock
    PointLight lights[3];
};


// output written to the screen is always location 0
layout(location = 0) out vec4 fragColor;

float angleBetween(vec3 vector1, vec3 vector2) {
    float dotProduct = dot(vector1, vector2);
    float lengthProduct = length(vector1) * length(vector2);
    return acos( dotProduct / lengthProduct );
}

vec3 calcPhongLight(PointLight light,vec3 viewDirection, vec3 lightDirection) {

    /* Calculate Phong-Illumination here */


    //Debug:
    //float alphaC = 1.0;
    //float alphaL = 0.14;
    //float alphaQ = 0.07;

    // Set the color of the light source
    vec3 lightColor = light.color;

    // Calculate the reflection angle
    vec3 R = reflect(-lightDirection, normalize(vNormal));

    // Calculate the specular factor
    //float spec = pow(max(dot(vNormal, R), 0.0), uShininess);
    float spec = pow(max(dot(vNormal, R), 0.0), uValues.shininess);

    // Calculate the angles theta and alpha
    float theta = angleBetween(lightDirection, normalize(vNormal));

    // Calculate the cos of theta and alpha
    float cos_theta = cos(theta);

    // If cos theta or cos alpha are negative set them to zero
    cos_theta = cos_theta < 0.0 ? 0.0 : cos_theta;

    // Calculate the values ambient, diffuse and specular
    vec3 iAmbient = light.ambient * uValues.ambient;
    vec3 iDiffuse = light.diffuse * cos_theta * uValues.diffuse;
    vec3 iSpecular = light.specular * spec * uValues.specular;

    // Calculate the distance between the this fragment and the light source
    float dist_ls = distance(light.position, vFragPos);

    // Calculate the attenuation value for the fragment
    float attenuation = 1.0 / (light.constant + light.linear * dist_ls + light.quadratic * pow(dist_ls, 2.0));

    // Return the light intensity vector
    return (iAmbient + iDiffuse + iSpecular) * attenuation * light.color;

}

void main() {

    // write white color to every fragment
    //fragColor = texture(tex0, pos);
    //fragColor = vec4(0.0,1.0,1.0,1.0);

    //fragColor = texture(tex0, v_Position) * ringfarbe;

    // Calculate the view and light direction
        vec3 viewDirection = normalize(uViewPosition - vFragPos);
        //vec3 lightDirection = normalize(uLightPosition - vFragPos);

        // Calculate the intensity vector
        vec3 result = vec3(0.0,0.0,0.0);

        for (int i = 0;i < 3;i++){
            vec3 lightDirection = normalize(lights[i].position - vFragPos);

            result += calcPhongLight(lights[i],viewDirection,lightDirection);
        }
        // Calculate the fragment color
        //fragColor = vec4((solidColor * lightIntensity), 1.0);
        //fragColor = texture(tex0, v_Position) * ringfarbe;
        vec4 texel = texture(tex0, v_Position);
        fragColor = vec4((texel.rgb * result), 1.0);

}

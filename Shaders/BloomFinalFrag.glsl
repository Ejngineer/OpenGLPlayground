#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrTexture;
uniform sampler2D scene;
//uniform bool hdr;
uniform float exposure;

void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdrTexture, TexCoords).rgb;
    vec3 BloomColor = texture(scene, TexCoords).rgb;

    hdrColor += BloomColor;
    // reinhard
    //vec3 result = hdrColor / (hdrColor + vec3(1.0));
    // exposure
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}


#version 460 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 Normal;
out vec2 UV;

void main(){
    Normal= normalize(mat3(transpose(inverse(uModel))) * aNormal);
    UV = aUV;
    gl_Position = uProjection * uView * uModel * vec4(aPosition,1.0);
}

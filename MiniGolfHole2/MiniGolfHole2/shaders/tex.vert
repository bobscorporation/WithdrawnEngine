#version 150

in vec3 pos;
in vec2 tex;
in vec3 normal;

uniform mat4 M;
uniform mat3 M_n;
uniform mat4 P;

out vec2 texCoord;

void main(void) {
    texCoord = tex;
    gl_Position = P * (M * vec4(pos, 1.0));
}

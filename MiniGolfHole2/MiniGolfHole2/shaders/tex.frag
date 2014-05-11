#version 150

in vec2 texCoord;

uniform sampler2D tex0;

out vec4 frag_color;

void main(void) {
    vec4 c = vec4(texture(tex0, texCoord));

    frag_color = c;
}

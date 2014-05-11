#version 150

in vec3 L;
in vec3 N;
in vec3 V;
in vec3 H;
in vec4 frag_color;
in vec2 texCoord;

uniform sampler2D tex0;

out vec4 end_color;

const float s = 250.0;
const vec4 L_s = vec4(1.0);
const vec4 L_d = vec4(1.0);
const vec4 K_s = vec4(1.0);

void main() {
    float phi = acos(dot(N,H));
    float LdotN = dot(L,N);

    if (LdotN > 0.0) {
        vec4 diff = frag_color * L_d * LdotN;
        vec4 spec = K_s * L_s * pow(cos(phi), s);
        end_color = clamp(diff + spec, 0.0, 1.0);
    } else {
        end_color = vec4(0,0.0,0,1);
    }
	//end_color = vec4(N,1.0);
}

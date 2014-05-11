#version 150

in vec3 L;
in vec3 N;
in vec3 V;
in vec3 H;
in vec3 ACTUALN;
in vec4 frag_color;

out vec4 end_color;

const float s = 32.0;
const vec4 L_s = vec4(1.0);
const vec4 L_d = vec4(1.0);
const vec4 K_s = vec4(1.0);

in vec3 vertex_light_position;
in vec3 vertex_normal;

void main() {
    float diffuse_value = max(dot(vertex_normal, vertex_light_position), 0.0);
	end_color = vec4(1,1,1,1) * diffuse_value;
	end_color = vec4(vertex_normal,1);
}

#version 150

//These variables are constant for all vertices
uniform mat4 M; //modelview matrix
uniform mat4 P; //projection matrix
uniform mat3 M_n; //normal matrix
uniform float time; //time variable
uniform vec4 color; //color variable

uniform vec3 light_dir;
uniform vec3 light_source;
uniform vec3 light_color;
float NdotL;

//input variables from host
in vec3 pos; //vertex position
in vec3 norm; //vertex normal

//variables to be passed to the fragment shader
out vec4 frag_color;

void main() {
	vec3 normal = normalize(M_n * norm);

	NdotL = max(dot(normal, normalize(light_dir)), 0.0);

    gl_Position = P * (M * vec4(pos, 1.0));
	
	frag_color = color;
}

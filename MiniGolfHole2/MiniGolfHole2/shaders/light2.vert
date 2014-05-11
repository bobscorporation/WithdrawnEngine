#version 150

//These variables are constant for all vertices
uniform mat4 M; //modelview matrix
uniform mat4 P; //projection matrix
uniform mat4 M_n; //normal matrix
uniform vec3 L_p; //light position
uniform vec3 E; //view position
uniform float time; //time variable

//input variables from host
in vec3 pos; //vertex position
in vec3 norm; //vertex normal
in vec4 color; //vertex color

//variables to be passed to the fragment shader
out vec3 L;
out vec3 N;
out vec3 V;
out vec3 H;
out vec3 ACTUALN;
out vec4 frag_color;



out vec3 vertex_light_position;
out vec3 vertex_normal;

void main() {
    vertex_normal = normalize(M_n * vec4(norm,1.0)).xyz;
	vertex_light_position = normalize(L_p.xyz);
	gl_FrontColor = vec4(1,1,1,1);
	gl_Position = P * M * vec4(pos,1.0);
}

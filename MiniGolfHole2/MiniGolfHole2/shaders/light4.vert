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
out vec4 frag_color;

void main() {
    vec4 posT = M * vec4(pos,1.0);
    
    L = normalize(M * vec4(L_p, 1.0)).xyz;
    V = (M * vec4(E,1.0)).xyz;
    
    N = normalize(M_n * vec4(norm,1.0)).xyz;
    L = normalize(L - posT.xyz);
    V = normalize(V - posT.xyz);
    H = normalize(L + V);
    
    //determine vertex color based on position and time
    frag_color = vec4(1.0);//color;
    
    gl_Position = P * posT;
}

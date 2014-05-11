#version 150

//These variables are constant for all vertices
uniform mat4 M; //modelview matrix
uniform mat4 P;
uniform mat4 C;
uniform mat4 M_n; //normal matrix
uniform vec3 E; //view position
uniform float time; //time variable
uniform vec3 L_p;//light variable

//input variables from host(in similar to attributes)
in vec4 pos; //vertex position
in vec3 color;
in vec3 norm;
in vec3 objVert;
//texture
in vec2 vTexCoords;
 
//variables to be passed to the fragment shader
out vec4 frag_color;
out vec3 N;
out vec3 L;
out vec3 V;
out vec3 H;
//texture
out vec2 texCoord;

void main() {
    texCoord = vTexCoords;

    gl_Position = P * (M*vec4(pos.x, pos.y, pos.z, 1.0));
    vec4 coor;
    coor = vec4(1,1,1,1);//color.x, color.y, color.z, 1.0);
    V = vec3(0,1,0);
    N = normalize(M_n * vec4(norm,1.0)).xyz;
    L = normalize(L_p.xyz-(M*pos).xyz);
    H= normalize(E+V);

   frag_color = clamp(coor, 0.0, 1.0);
  
}
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
const vec4 L_d = vec4(0.6);
const vec4 K_s = vec4(1.0);

void main() {
    float phi = acos(dot(N,H));
    float LdotN = dot(L,N);
    
    if (LdotN > 0.0) {
        vec4 diff = frag_color * L_d * LdotN;
        vec4 spec = K_s * L_s * pow(cos(phi), s);
        end_color = clamp(diff + spec, 0.0, 1.0);
		//end_color = vec4(0,1,0,1);
    } else {
        end_color = vec4(0.0,0.0,0.0,1.0);
    }
	//end_color = frag_color;
	//end_color = clamp(vec4(ACTUALN.x,ACTUALN.y,ACTUALN.z,1.0),0.2,1.0);

	if (frag_color.g == 1.0)
		end_color = vec4(ACTUALN,1.0);
	else
		end_color = frag_color;
}

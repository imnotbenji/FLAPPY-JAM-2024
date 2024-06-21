@vs
void main()
{
	vec4 vertices[4] = vec4[4](
        vec4(-1, -1, 0, 1),
        vec4( 1, -1, 0, 1),
        vec4(-1,  1, 0, 1),
        vec4( 1,  1, 0, 1)
    );

	gl_Position = vertices[gl_VertexID];
}

@fs
// based on https://www.shadertoy.com/view/DlSBzK
out vec4 color;

uniform float scroll;
uniform float time;
uniform vec2 resolution;

float hash(float p)
{
    p = fract(p * .1031);
    p *= p + 33.33;
    p *= p + p;
    return fract(p);
}

float blinkLevel(int size, float frameNum){
    if(size == 1){
        return floor(mod(frameNum, 2.));
    }else if(size == 2){
        float num = ceil(mod(frameNum, 4.));
        if(num > 2.){
            return 4. - num;
        }else{
            return num;
        }
    }else{
        return 0.;
    }
}

vec3 blinkColor(int id){
    if(id == 0){
        return vec3(.0,0.,1.);
    }else if(id == 1){
        return vec3(.7,.2,1.);
    }else if(id == 2){
        return vec3(0.,.8,1.);
    }else if(id == 3){
        return vec3(.4,0.,1.0);
    }else if(id == 4){
        return vec3(0.3,0.,1.);
    }else if(id == 5){
        return vec3(0.,.8,1.);
    }else if(id == 6){
        return vec3(1.,1.,1.);
    }
    return vec3(0.);
}


vec4 blink(vec2 p){
    vec2 pixelUnit = vec2(1.) / resolution.xy;
    vec2 whRatio = vec2(resolution.x / resolution.y,1.);
    vec4 l = vec4(0.);
    float whAvg = (resolution.x + resolution.y) / resolution.y * 256.;
    for (float i = 0.; i < 64.; i += 1.) {
        float z = hash(i*.1);
        l.rgb = blinkColor(int(hash(i*.4) * 7.));
        float offset = hash(i*.5) * 3.;
        int size = int(3.*z*z);
        int depth = (3 - size) * 100;
        float level = blinkLevel(size, time / 0.2 + offset) + 1.;
        float invDepth = 1./float(depth);
        vec2 pos = fract(vec2(hash(i*.2), hash(i*.3)) + 100.*vec2(0, scroll)/resolution.xy*invDepth);
        pos -= mod(pos,pixelUnit) + pixelUnit / 2.;
        vec2 delta = (p - pos) * whRatio;
        float range = abs(delta.x) + abs(delta.y);

        float inRange = (level - 2.) / whAvg;
        float outRange = level / whAvg;

        l.a = step(inRange, range) * step(range, outRange);

        if(l.a > 0.0){
            return vec4(mix(vec3(1.0),l.rgb, -scroll/25600.0), -scroll/25600.0);
        }
    }

    vec4 bg = mix(vec4(0.3,0.0,1.0,1.0), vec4(0.0), -scroll/25600.0);

    return bg;
}

void main()
{
    vec2 p = gl_FragCoord.xy/resolution.xy;

    color = blink(p);
    color.a = 1.0;
}

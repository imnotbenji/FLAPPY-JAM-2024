@vs
out vec2 uv;

void main()
{
	vec4 vertices[4] = vec4[4](
        vec4(-1, -1, 0, 1),
        vec4( 1, -1, 0, 1),
        vec4(-1,  1, 0, 1),
        vec4( 1,  1, 0, 1)
    );

	vec2 uv_coord[4] = vec2[4](
        vec2(0, 0),
        vec2(1, 0),
        vec2(0, 1),
        vec2(1, 1)
    );

	uv = uv_coord[gl_VertexID];
	gl_Position = vertices[gl_VertexID];
}

@fs
in  vec2 uv;
out vec4 color;

uniform bool paused;
uniform sampler2D tex;

const vec3 luminance = vec3(0.2126729, 0.7151522, 0.0721750);

void main()
{
    vec3 texel = texture(tex, uv).rgb;
    
    vec3 rgb = texel;

    if (paused)
    {
        rgb   = vec3(dot(rgb, luminance));
        rgb.r = texel.g;
    }

    color = vec4(rgb, 1.0);
}

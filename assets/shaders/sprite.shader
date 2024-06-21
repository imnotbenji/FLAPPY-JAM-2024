@vs
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec4 a_tint;
layout(location = 3) in vec4 a_fill;

out vec2 uv;
out vec4 tint;
out vec4 fill;

uniform mat4 matrix;

void main()
{
    uv   = a_uv;
    tint = a_tint;
    fill = a_fill;

    gl_Position = matrix * vec4(a_pos, 1.0);
}

@fs
in vec2 uv;
in vec4 tint;
in vec4 fill;

out vec4 color;

uniform sampler2D tex;

void main()
{
    vec4 texel = texture(tex, uv);

    vec4 a = texel * tint;
    vec4 b = fill;

    float blend = b.a;
    float alpha = 0.0;

    if (tint.a == 0.0) alpha = b.a;
    else alpha = a.a;

    color = vec4(mix(a, b, blend).rgb, alpha);
}


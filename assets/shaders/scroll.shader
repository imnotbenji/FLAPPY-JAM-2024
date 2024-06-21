@vs
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_uv;

out vec2 uv;

uniform float scroll;
uniform float height;
uniform mat4 matrix;

void main()
{
    uv = a_uv + vec2(0, scroll / height);

    gl_Position = matrix * vec4(a_pos, 1.0);
}

@fs
in vec2 uv;

out vec4 color;

uniform sampler2D tex;

void main()
{
    color = texture(tex, uv);
}


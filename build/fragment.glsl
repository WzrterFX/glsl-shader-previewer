#version 330 core

out vec4 texel;

uniform vec2 resolution;
uniform vec2 mouse;
uniform float time;

float random(in vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

mat2 rotate(float angle)
{
    return mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
}

float lava(in vec2 coord, in float direction)
{
    float len;

    for(int i = 0; i < 3; i++)
    {
        float noi = sin(floor(random(coord) * 0.9));
        len = length(coord) * length(coord) / 14.0;

        coord.x = coord.x - cos(coord.y + sin(len)) + cos(time / 9.0);
        coord.y = coord.y + cos(coord.y + cos(len)) + sin(time / 12.0);

        coord *= rotate(direction * time / 3.14);
    }

    return len;
}

void main()
{
    vec2 coord = 12.0 * (gl_FragCoord.xy - resolution.xy / 2.0) / min(resolution.x, resolution.y);

    float distortionFactor = length(mouse - gl_FragCoord.xy) / length(resolution);

    float n = random(gl_FragCoord.xy / 200.0);

    float circle = sin(length(coord) * 1.5);
    float circleMask = 1.0 - smoothstep(0.1, 0.2, circle);
    float circleMask2 = smoothstep(0.1, 0.1, circle);

    float lava1 = lava(coord, -1.0) * circleMask;
    float lava2 = lava(coord + 2.0, 1.0) * circleMask2;
    float fx = (lava1 + lava2) * (1.0 + distortionFactor * 0.5);

    texel = vec4(cos((fx - 0.09) + 0.79), (cos(fx - 0.02)) - 0.2, sin(cos(fx - 0.02)) + 0.1, 1.0);
}
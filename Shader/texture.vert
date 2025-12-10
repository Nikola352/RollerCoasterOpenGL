#version 330 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inTex;

out vec2 chTex;

uniform float uX;
uniform float uY;

uniform float uRotSin;
uniform float uRotCos;

uniform float uAspect;

void main()
{
    vec2 correctedPos = vec2(inPos.x * uAspect, inPos.y);

    mat2 rotation = mat2(uRotCos, uRotSin, -uRotSin, uRotCos);
    vec2 rotatedPos = rotation * correctedPos;

    vec2 actualPos = vec2(rotatedPos.x / uAspect, rotatedPos.y);

    gl_Position = vec4(actualPos.x + uX, actualPos.y + uY , 0.0, 1.0);

    chTex = inTex;
}
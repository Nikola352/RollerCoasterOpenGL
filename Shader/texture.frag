#version 330 core

in vec2 chTex;
out vec4 outCol;

uniform sampler2D uTex;
uniform sampler2D uOverlayTex;

uniform bool uHasOverlay;

void main()
{
    if(!uHasOverlay) outCol = texture(uTex, chTex);
    else {
        vec4 overlayCol = texture(uOverlayTex, chTex);
        if (overlayCol.a == 0.0f) {
            outCol = texture(uTex, chTex);
        } else {
            outCol = overlayCol;
        }
    }
} 
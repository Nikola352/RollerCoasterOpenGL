#include "../../Header/Game/Track.h"
#include <GL/glew.h>
#include <cmath>
#include <iostream>

float getTrackHeightAt(float x) {
    if (x < -0.5f) {
        return -0.5f;
    }
    // return -0.5f * cbrt(cos(12 * x + 6));
    return -0.5f * cos(12 * x + 6);
}

float getTrackSlopeAt(float x) {
    if (x < -0.5f) {
        return 0;
    }
    // float cosVal = cos(12 * x + 6);
    float sinVal = sin(12 * x + 6);
    return 6 * sinVal;

    // if (fabs(cosVal) < 1e-6) {
    //     return (sinVal > 0) ? 10000.0f : -10000.0f;
    // }

    // return 2.0f * sinVal * pow(cbrt(cosVal), -2.0f);
}

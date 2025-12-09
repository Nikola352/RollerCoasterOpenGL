#include "../../Header/Game/Track.h"
#include <GL/glew.h>
#include <cmath>
#include <iostream>

float Track::getHeightAt(float x) const {
    if (x < -0.5f) {
        return -0.5f;
    }
    return -0.5f * cbrt(cos(12 * x + 6));
}

float Track::getSlopeAt(float x) const {
    if (x < -0.5f) {
        return 0;
    }
    float cosVal = cos(12 * x + 6);
    float sinVal = sin(12 * x + 6);

    if (fabs(cosVal) < 1e-4) {
        return (sinVal > 0) ? 100000.0f : -100000.0f;
    }

    return 2.0f * sin(12 * x + 6) * pow(cbrt(cos(12*x+6)), -2.0f);
}

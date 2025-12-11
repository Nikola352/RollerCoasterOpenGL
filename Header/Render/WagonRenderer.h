#pragma once
#include <vector>
#include "../Game/Wagon.h"

class WagonRenderer {
private:
    unsigned int shader;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int texture;

    void setupBuffers();

public:
    WagonRenderer(unsigned int textureShader);

    ~WagonRenderer();

    void init();

    void render(const Wagon& wagon) const;

    void render(const std::vector<Wagon>& wagons) const;
};
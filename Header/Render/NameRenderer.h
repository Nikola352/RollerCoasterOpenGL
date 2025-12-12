#pragma once

class NameRenderer {
private:
    unsigned int shader;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int texture;

    void setupBuffers();

public:
    NameRenderer(unsigned int textureShader);

    ~NameRenderer();

    void init();

    void render() const;
};
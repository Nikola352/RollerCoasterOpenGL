#pragma once
#include "../Game/Person.h"
#include <vector>

class PersonRenderer {
private:
    unsigned int shader;
    
    unsigned int VAO;
    unsigned int VBO;

    unsigned int normalTexture;
    unsigned int sickTexture;
    unsigned int seatbeltTexture;

    void setupBuffers();
    void loadTextures();
public:
    PersonRenderer(unsigned int textureShader);

    ~PersonRenderer();

    void init();

    void render(const Person& person) const;

    void render(const std::vector<Person*>& persons) const;
};
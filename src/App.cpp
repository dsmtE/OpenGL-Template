#include "App.hpp"

#include <iostream>
#include <string>

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "helpers/RootDir.hpp"

App::App(): App(2.0f) {
}

App::App(float viewSize) : _previousTime(0.0), _imageAngle(0.0f), _viewSize(viewSize) {

    // tips found here for absolut ressources path: https://shot511.github.io/2018-05-29-how-to-setup-opengl-project-with-cmake/
    const std::string imagePath = std::string(ROOT_DIR) + "res/poulpile.jpg";

    LoadImage(imagePath);
}

void App::LoadImage(const std::string& imagePath) {
    // Generate texture
    glGenTextures(1, &_textureId);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // load and generate the texture
    int width, height, nrChannels;
    
    unsigned char *data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void App::Update() {

    double currentTime = glfwGetTime();
    double elapsedTime = currentTime - _previousTime;
    _previousTime = currentTime;
    
    // update imageAngle (use elapsedTime to update without being dependent on the frame rate)
    _imageAngle = fmod(_imageAngle + 10.0f * (float)elapsedTime, 360.0f);

    Render();
}

void App::Render() {
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    // render exemple quad
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glVertex2f(0.5f, 0.5f);
        glVertex2f(-0.5f, 0.5f);
    glEnd();

    const float imageAngleRad = glm::radians(_imageAngle);
    //Render the texture on the screen
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glm::vec2 upperLeft = glm::rotate(glm::vec2(-0.3f, -0.3f), imageAngleRad);
        glTexCoord2d(0,0); glVertex2f(upperLeft.x, upperLeft.y);

        glm::vec2 upperRight = glm::rotate(glm::vec2(0.3f, -0.3f), imageAngleRad);
        glTexCoord2d(1,0); glVertex2f(upperRight.x, upperRight.y);

        glm::vec2 bottomRight =  glm::rotate(glm::vec2(0.3f, 0.3f), imageAngleRad);
        glTexCoord2d(1,1); glVertex2f(bottomRight.x, bottomRight.y);

        glm::vec2 bottomLeft =  glm::rotate(glm::vec2(-0.3f, 0.3f), imageAngleRad);
        glTexCoord2d(0,1); glVertex2f(bottomLeft.x, bottomLeft.y);
    glEnd();
    glDisable(GL_TEXTURE_2D);

}

void App::key_callback(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/) {
}

void App::mouse_button_callback(int /*button*/, int /*action*/, int /*mods*/) {
}

void App::scroll_callback(double /*xoffset*/, double /*yoffset*/) {
}

void App::cursor_position_callback(double /*xpos*/, double /*ypos*/) {
}

void App::size_callback(int width, int height) {
    _width  = width;
    _height = height;

    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, _width, _height);

    const float aspectRatio = _width / (float) _height;

    // Change the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if( aspectRatio > 1) {
        glOrtho(-_viewSize / 2.0f * aspectRatio, _viewSize / 2.0f * aspectRatio, -_viewSize / 2.0f, _viewSize / 2.0f, -1.0f, 1.0f);
    } else {
        glOrtho(-_viewSize / 2.0f, _viewSize / 2.0f, -_viewSize / 2.0f / aspectRatio, _viewSize / 2.0f / aspectRatio, -1.0f, 1.0f);
    }
}

glm::vec2 App::rotateVec2(const glm::vec2& vec, const glm::vec2& center, const float& angle) {
    return glm::rotate(vec-center,  glm::radians(angle))+center;
}
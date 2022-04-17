#include "App.hpp"
#include "GLFW/glfw3.h"

App::App() {
}

void App::render()
{
    glClearColor(0.643f, 0.909f, 0.878f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
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
    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, width, height);
}

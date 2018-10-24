#include "PlanetScene.hpp"

#include <atlas/utils/Application.hpp>
#include <atlas/utils/WindowSettings.hpp>
#include <atlas/gl/ErrorCheck.hpp>

int main()
{
    using atlas::utils::WindowSettings;
    using atlas::utils::ContextVersion;
    using atlas::utils::Application;
    using atlas::utils::ScenePointer;
    using namespace lab5;

    atlas::gl::setGLErrorSeverity(
        ATLAS_GL_ERROR_SEVERITY_HIGH | ATLAS_GL_ERROR_SEVERITY_MEDIUM);

    WindowSettings settings;
    settings.contextVersion = { 3, 3 };
    settings.isForwardCompat = true;
    settings.isMaximized = true;

    Application::getInstance().createWindow(settings);
    Application::getInstance().addScene(ScenePointer(new PlanetScene));
    Application::getInstance().runApplication();

    return 0;

}
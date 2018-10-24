#include "PlanetScene.hpp"

#include <atlas/utils/GUI.hpp>
#include <atlas/gl/GL.hpp>

namespace lab5
{
    PlanetScene::PlanetScene() :
        mPlay(false),
        mAnimCounter(60.0f),
        mEarth("earth.jpg"),
        mSun("sun.jpg")
    {
        mSun.setPosition({ 0.0f, 0.0f, 0.0f });
        mEarth.setPosition({ 25.0f, 0.0f, 25.0f });
    }

    void PlanetScene::updateScene(double time)
    {
        using atlas::core::Time;
        
        ModellingScene::updateScene(time);
        if (mPlay && mAnimCounter.isFPS(mTime))
        {
            mEarth.updateGeometry(mTime);
        }
    }

    void PlanetScene::renderScene()
    {
        using atlas::utils::Gui;
        
        Gui::getInstance().newFrame();
        const float grey = 92.0f / 255.0f;
        glClearColor(grey, grey, grey, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mProjection = glm::perspective(
            glm::radians(mCamera.getCameraFOV()),
            (float)mWidth / mHeight, 1.0f, 100000000.0f);
        mView = mCamera.getCameraMatrix();

        mGrid.renderGeometry(mProjection, mView);
        mEarth.renderGeometry(mProjection, mView);
        mSun.renderGeometry(mProjection, mView);

        // Global HUD
        ImGui::SetNextWindowSize(ImVec2(350, 140), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Global HUD");
        if (ImGui::Button("Reset Camera"))
        {
            mCamera.resetCamera();
        }

        if (mPlay)
        {
            if (ImGui::Button("Pause"))
            {
                mPlay = !mPlay;
            }
        }
        else
        {
            if (ImGui::Button("Play"))
            {
                mPlay = !mPlay;
            }
        }

        if (ImGui::Button("Reset"))
        {
            mEarth.setPosition({ 25.0f, 0.0f, 25.0f });
            mEarth.resetGeometry();
            mPlay = false;
            mTime.currentTime = 0.0f;
            mTime.totalTime = 0.0f;
        }

        ImGui::Text("Application average %.3f ms/frame (%.1FPS)",
            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Render();
    }
}
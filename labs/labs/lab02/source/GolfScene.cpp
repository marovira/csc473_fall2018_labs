#include "GolfScene.hpp"

#include <atlas/utils/GUI.hpp>
#include <atlas/gl/GL.hpp>
#include <atlas/core/Log.hpp>

namespace lab2
{
    GolfScene::GolfScene() :
        mPlay(false),
        mFPSOption(0),
        mFPS(60.0f),
        mCounter(mFPS)
    { }

    void GolfScene::updateScene(double time)
    {
        using atlas::core::Time;
        
        ModellingScene::updateScene(time);
        if (mPlay && mCounter.isFPS(mTime))
        {
            const float delta = 1.0f / mFPS;
            mAnimTime.currentTime += delta;
            mAnimTime.deltaTime = delta;
            mAnimTime.totalTime = mAnimTime.currentTime;

            mBall.updateGeometry(mAnimTime);
        }
    }

    void GolfScene::renderScene()
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
        mBall.renderGeometry(mProjection, mView);

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
            mBall.resetGeometry();
            mAnimTime.currentTime = 0.0f;
            mAnimTime.totalTime = 0.0f;
            mPlay = false;
        }

        ImGui::Text("Application average %.3f ms/frame (%.1FPS)",
            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        std::vector<const char*> options = { "60 FPS", "30 FPS", "20 FPS",
        "10 FPS", "5 FPS", "1 FPS" };
        ImGui::Combo("Choose FPS: ", &mFPSOption, options.data(),
            ((int)options.size()));
        ImGui::End();

        switch (mFPSOption)
        {
        case 0:
            mFPS = 60.0f;
            break;

        case 1:
            mFPS = 30.0f;
            break;

        case 2:
            mFPS = 20.0f;
            break;

        case 3:
            mFPS = 10.0f;
            break;

        case 4:
            mFPS = 5.0f;
            break;

        case 5:
            mFPS = 1.0f;
            break;

        default:
            break;
        }

        mCounter.setFPS(mFPS);

        mBall.drawGui();
        ImGui::Render();
    }
}
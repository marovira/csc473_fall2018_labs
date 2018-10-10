#include "TrackScene.hpp"

#include <atlas/utils/GUI.hpp>
#include <atlas/gl/GL.hpp>
#include <atlas/core/Log.hpp>
#include <atlas/math/Math.hpp>

namespace lab3
{
    TrackScene::TrackScene() :
        mPlay(false),
        mFPS(60.0f),
        mAnimLength(10.0f),
        mSpline(int(mAnimLength * mFPS)),
        mCounter(mFPS)
    { }

    void TrackScene::updateScene(double time)
    {
        using atlas::core::Time;
        
        ModellingScene::updateScene(time);
        if (mPlay && mCounter.isFPS(mTime))
        {
            const float delta = 1.0f / mFPS;
            mAnimTime.currentTime += delta;
            mAnimTime.deltaTime = delta;
            mAnimTime.totalTime = mAnimTime.currentTime;

            mSpline.updateGeometry(mAnimTime);

            if (mSpline.doneInterpolation())
            {
                mPlay = false;
                return;
            }

        }

        auto point = mSpline.getPosition();
        auto mat = glm::translate(atlas::math::Matrix4(1.0f), point);

        mBall.transformGeometry(mat);
    }

    void TrackScene::renderScene()
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
        mSpline.renderGeometry(mProjection, mView);

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
            mSpline.resetGeometry();
            mAnimTime.currentTime = 0.0f;
            mAnimTime.totalTime = 0.0f;
            mPlay = false;
        }

        ImGui::Text("Application average %.3f ms/frame (%.1FPS)",
            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        mSpline.drawGui();
        ImGui::Render();
    }
}
#include "CameraScene.hpp"

#include <atlas/gl/GL.hpp>
#include <atlas/utils/GUI.hpp>

namespace lab4
{
    CameraScene::CameraScene() :
        mCameraMode(0)
    { }

    void CameraScene::mousePressEvent(int button, int action, int modifiers,
        double xPos, double yPos)
    {
        using atlas::tools::MayaMovements;
        atlas::utils::Gui::getInstance().mousePressed(button, action, modifiers);

        if (action == GLFW_PRESS)
        {
            atlas::math::Point2 point(xPos, yPos);

            if (button == GLFW_MOUSE_BUTTON_LEFT &&
                modifiers == GLFW_MOD_ALT)
            {
                if (mCameraMode == 0)
                {
                    mCamera.setMovement(MayaMovements::Tumble);
                    mCamera.mouseDown(point);
                }
                else
                {
                    mQuatCamera.setMovement(MayaMovements::Tumble);
                    mQuatCamera.mouseDown(point);
                }
            }
            else if (button == GLFW_MOUSE_BUTTON_MIDDLE &&
                modifiers == GLFW_MOD_ALT)
            {
                if (mCameraMode == 0)
                {
                    mCamera.setMovement(MayaMovements::Track);
                    mCamera.mouseDown(point);
                }
                else
                {
                    mQuatCamera.setMovement(MayaMovements::Track);
                    mQuatCamera.mouseDown(point);
                }
            }
            else if (button == GLFW_MOUSE_BUTTON_RIGHT &&
                modifiers == GLFW_MOD_ALT)
            {
                if (mCameraMode == 0)
                {
                    mCamera.setMovement(MayaMovements::Dolly);
                    mCamera.mouseDown(point);
                }
                else
                {
                    mQuatCamera.setMovement(MayaMovements::Dolly);
                    mQuatCamera.mouseDown(point);
                }
            }
        }
        else
        {
            if (mCameraMode == 0)
            {
                mCamera.mouseUp();
            }
            else
            {
                mQuatCamera.mouseUp();
            }
        }
    }

    void CameraScene::mouseMoveEvent(double xPos, double yPos)
    {
        atlas::utils::Gui::getInstance().mouseMoved(xPos, yPos);
        if (mCameraMode == 0)
        {
            mCamera.mouseMove(atlas::math::Point2(xPos, yPos));
        }
        else
        {
            mQuatCamera.mouseMove(atlas::math::Point2(xPos, yPos));
        }
    }

    void CameraScene::mouseScrollEvent(double xOffset, double yOffset)
    {
        atlas::utils::Gui::getInstance().mouseScroll(xOffset, yOffset);

        if (mCameraMode == 0)
        {
            mCamera.mouseScroll(atlas::math::Point2(xOffset, yOffset));
        }
        else
        {
            mQuatCamera.mouseScroll(atlas::math::Point2(xOffset, yOffset));
        }
    }

    void CameraScene::renderScene()
    {
        using atlas::utils::Gui;

        Gui::getInstance().newFrame();
        const float grey = 92.0f / 255.0f;
        glClearColor(grey, grey, grey, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mProjection = glm::perspective(
            glm::radians(mCamera.getCameraFOV()),
            (float)mWidth / mHeight, 1.0f, 100000000.0f);

        mView = (mCameraMode == 0) ? mCamera.getCameraMatrix() :
            mQuatCamera.getCameraMatrix();


        mGrid.renderGeometry(mProjection, mView);
        mDragon.renderGeometry(mProjection, mView);

        ImGui::SetNextWindowSize(ImVec2(350, 140), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Global HUD");
        if (ImGui::Button("Reset Camera"))
        {
            mCamera.resetCamera();
            mQuatCamera.resetCamera();
        }


        std::vector<const char*> options = { "Maya Camera", "Quaternion Camera" };
        ImGui::Combo("Camera mode: ", &mCameraMode, options.data(),
            ((int)options.size()));

        ImGui::Text("Application average %.3f ms/frame (%.1FPS)",
            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        ImGui::Render();
    }
}
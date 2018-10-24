#pragma once

#include "Dragon.hpp"
#include "QuaternionCamera.hpp"

#include <atlas/tools/ModellingScene.hpp>
#include <atlas/tools/MayaCamera.hpp>
#include <atlas/tools/Grid.hpp>

namespace lab4
{
    class CameraScene : public atlas::tools::ModellingScene
    {
    public:
        CameraScene();

        void mousePressEvent(int button, int action, int modifiers,
            double xPos, double yPos) override;
        void mouseMoveEvent(double xPos, double yPos) override;
        void mouseScrollEvent(double xOffset, double yOffset) override;

        void renderScene() override;

    private:
        int mCameraMode;

        Dragon mDragon;
        QuaternionCamera mQuatCamera;
    };
}
#pragma once

#include "GolfBall.hpp"

#include <atlas/tools/ModellingScene.hpp>
#include <atlas/utils/FPSCounter.hpp>

namespace lab2
{
    class GolfScene : public atlas::tools::ModellingScene
    {
    public:
        GolfScene();

        void updateScene(double time) override;
        void renderScene() override;

    private:
        bool mPlay;
        int mFPSOption;
        float mFPS;

        atlas::core::Time<float> mAnimTime;
        atlas::utils::FPSCounter mCounter;

        GolfBall mBall;
    };
}


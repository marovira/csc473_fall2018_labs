#pragma once

#include "TrackBall.hpp"
#include "Spline.hpp"

#include <atlas/tools/ModellingScene.hpp>
#include <atlas/utils/FPSCounter.hpp>

namespace lab3
{
    class TrackScene : public atlas::tools::ModellingScene
    {
    public:
        TrackScene();

        void updateScene(double time) override;
        void renderScene() override;

    private:
        bool mPlay;
        float mFPS;
        float mAnimLength;

        atlas::core::Time<float> mAnimTime;
        atlas::utils::FPSCounter mCounter;

        TrackBall mBall;
        Spline mSpline;
    };
}


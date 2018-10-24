#pragma once

#include "Planet.hpp"

#include <atlas/tools/ModellingScene.hpp>
#include <atlas/utils/FPSCounter.hpp>

namespace lab1
{
    class PlanetScene : public atlas::tools::ModellingScene
    {
    public:
        PlanetScene();

        void updateScene(double time) override;
        void renderScene() override;

    private:
        bool mPlay;
        atlas::utils::FPSCounter mAnimCounter;
        atlas::utils::FPSCounter mRenderCounter;
        Planet mPlanet;
    };
}


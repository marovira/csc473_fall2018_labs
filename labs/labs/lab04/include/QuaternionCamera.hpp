#pragma once

#include <atlas/tools/MayaCamera.hpp>

namespace lab4
{
    class QuaternionCamera : public atlas::tools::MayaCamera
    {
    public:
        QuaternionCamera();

        void setMovement(atlas::tools::MayaMovements movement);

        void mouseDown(atlas::math::Point2 const& point) override;
        void mouseMove(atlas::math::Point2 const& point) override;
        void mouseUp() override;
        void resetCamera() override;


        atlas::math::Matrix4 getCameraMatrix() const override;

    private:
        atlas::math::Point mPosition;
        atlas::math::Point mTarget;
        atlas::math::Vector mUp;
        atlas::math::Point2 mLastPos;
        atlas::math::Quaternion mQuat;

        float mFov;
        atlas::tools::MayaMovements mMovement;
    };

}
#include "QuaternionCamera.hpp"

namespace lab4
{
    QuaternionCamera::QuaternionCamera() :
        mPosition(30.0f),
        mTarget(0.0f),
        mUp(0, 1, 0),
        mLastPos(0.0f),
        mQuat(),
        mFov(45.0f)
    { }

    void QuaternionCamera::setMovement(atlas::tools::MayaMovements movement)
    {
        mMovement = movement;
    }

    void QuaternionCamera::mouseDown(atlas::math::Point2 const& point)
    {
        mLastPos = point;
    }

    void QuaternionCamera::mouseMove(atlas::math::Point2 const& point)
    {
        using atlas::math::Quaternion;

        auto delta = point - mLastPos;

        switch (mMovement)
        {
        case atlas::tools::MayaMovements::Dolly:
        {
            delta *= -0.5f;
            auto view = glm::normalize(mPosition - mTarget);

            mPosition += (view * delta.x);
            mTarget += (view * delta.x);
            break;
        }
        case atlas::tools::MayaMovements::Track:
        {
            delta *= 0.05f;

            auto view = glm::normalize(mPosition - mTarget);
            auto right = glm::normalize(glm::cross(mUp, view));
            auto up = glm::cross(view, right);

            right *= -delta.x;
            up *= delta.y;

            mPosition += (right + up);
            mTarget += (right + up);
            break;
        }
        case atlas::tools::MayaMovements::Tumble:
        {
            delta *= 0.005f;
            // TODO: Fill in the rotation logic using quaternions.
            break;
        }

        case atlas::tools::MayaMovements::None:
            break;
        }

        mLastPos = point;
    }

    void QuaternionCamera::mouseUp()
    {
        mMovement = atlas::tools::MayaMovements::None;
    }

    void QuaternionCamera::resetCamera()
    {
        using atlas::math::Point;
        using atlas::math::Vector;
        using atlas::math::Quaternion;

        mPosition = Point(30.0f);
        mTarget = Point(0.0f);
        mUp = Vector(0, 1, 0);
        mQuat = Quaternion();
        mFov = 45.0f;
    }

    atlas::math::Matrix4 QuaternionCamera::getCameraMatrix() const
    {
        auto rotate = glm::mat4_cast(mQuat);
        return glm::lookAt(mPosition, mTarget, mUp) * rotate;
    }


}

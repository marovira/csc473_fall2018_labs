#pragma once

#include <atlas/utils/Geometry.hpp>
#include <atlas/gl/Buffer.hpp>
#include <atlas/gl/VertexArrayObject.hpp>
#include <atlas/gl/Texture.hpp>

namespace lab2
{
    class GolfBall : public atlas::utils::Geometry
    {
    public:
        GolfBall();

        void updateGeometry(atlas::core::Time<> const& t) override;
        void drawGui() override;
        void renderGeometry(atlas::math::Matrix4 const& projection,
            atlas::math::Matrix4 const& view) override;

        void resetGeometry() override;

    private:
        void eulerIntegrator(atlas::core::Time<> const& t);
        void implicitEulerIntegrator(atlas::core::Time<> const& t);
        void verletIntegrator(atlas::core::Time<> const& t);

        atlas::gl::Buffer mVertexBuffer;
        atlas::gl::Buffer mIndexBuffer;
        atlas::gl::VertexArrayObject mVao;

        atlas::math::Vector mTruePosition;
        atlas::math::Vector mOffset;
        atlas::math::Vector mApproxPosition;
        atlas::math::Vector mApproxVelocity;
        atlas::math::Vector mApproxOldPosition;
        atlas::math::Vector mForce;


        float mMass;
        int mIntegrator;

        GLsizei mIndexCount;
    };
}
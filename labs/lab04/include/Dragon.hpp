#pragma once

#include <atlas/utils/Geometry.hpp>
#include <atlas/gl/VertexArrayObject.hpp>
#include <atlas/gl/Buffer.hpp>

namespace lab4
{
    class Dragon : public atlas::utils::Geometry
    {
    public:
        Dragon();

        void renderGeometry(atlas::math::Matrix4 const& projection,
            atlas::math::Matrix4 const& view) override;

    private:
        atlas::gl::VertexArrayObject mVao;
        atlas::gl::Buffer mDataBuffer;
        atlas::gl::Buffer mIndexBuffer;
        GLsizei mIndices;
    };
}
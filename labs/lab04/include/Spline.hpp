#pragma once

#include <atlas/utils/Geometry.hpp>
#include <atlas/gl/Buffer.hpp>
#include <atlas/gl/VertexArrayObject.hpp>

namespace lab3
{
    class Spline : public atlas::utils::Geometry
    {
    public:
        Spline(int totalFrames);

        void updateGeometry(atlas::core::Time<> const& t) override;
        void renderGeometry(atlas::math::Matrix4 const& projection,
            atlas::math::Matrix4 const& view) override;
        void drawGui() override;

        void resetGeometry();
        
        atlas::math::Point getPosition() const;
        bool doneInterpolation() const;

    private:
        atlas::math::Point interpolateOnSpline() const;

        atlas::math::Point evaluateSpline(float t) const;
        void generateArcLengthTable();
        int tableLookUp(float distance) const;
        float chooseEpsilon() const;

        atlas::math::Matrix4 mBasisMatrix;
        std::vector<atlas::math::Point> mControlPoints;

        std::vector<float> mTable;

        atlas::math::Point mSplinePosition;

        atlas::gl::VertexArrayObject mControlVao;
        atlas::gl::VertexArrayObject mSplineVao;
        atlas::gl::Buffer mControlBuffer;
        atlas::gl::Buffer mSplineBuffer;

        int mResolution;
        int mTotalFrames;
        int mCurrentFrame;

        bool mShowControlPoints;
        bool mShowCage;
        bool mShowSplinePoints;
        bool mShowSpline;
        bool mIsInterpolationDone;
    };
}
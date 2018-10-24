#include "Spline.hpp"

#include "Paths.hpp"
#include "LayoutLocations.glsl"

#include <atlas/utils/Mesh.hpp>
#include <atlas/core/STB.hpp>
#include <atlas/core/Float.hpp>
#include <atlas/utils/GUI.hpp>
#include <atlas/core/Macros.hpp>

namespace lab3
{
    Spline::Spline(int totalFrames) :
        mControlBuffer(GL_ARRAY_BUFFER),
        mSplineBuffer(GL_ARRAY_BUFFER),
        mResolution(500),
        mTotalFrames(totalFrames),
        mCurrentFrame(0),
        mShowSplinePoints(false),
        mShowControlPoints(true),
        mShowCage(false),
        mShowSpline(true),
        mIsInterpolationDone(false)
    {
        namespace math = atlas::math;
        namespace gl = atlas::gl;
        using atlas::math::Point;

        mBasisMatrix = math::Matrix4(
            1.0f, 0.0f, 0.0f, 0.0f,
            -3.0f, 3.0f, 0.0f, 0.0f,
            3.0f, -6.0f, 3.0f, 0.0f,
            -1.0f, 3.0f, -3.0f, 1.0f);

        mControlPoints = std::vector<Point>
        {
            { -20, 5, 0 },
            { -19, 5, -15 },
            { 12.7f, -5, -1.4f },
            { 20, 8.2f, 4.4f }
        };

        std::vector<Point> splinePoints;

        float scale = 1.0f / mResolution;
        for (int res = 0; res < mResolution; ++res)
        {
            auto pt = evaluateSpline(scale * res);
            splinePoints.push_back(pt);
        }

        generateArcLengthTable();

        mControlVao.bindVertexArray();
        mControlBuffer.bindBuffer();
        mControlBuffer.bufferData(gl::size<Point>(mControlPoints.size()),
            mControlPoints.data(), GL_STATIC_DRAW);
        mControlBuffer.vertexAttribPointer(VERTICES_LAYOUT_LOCATION, 3, GL_FLOAT,
            GL_FALSE, 0, gl::bufferOffset<float>(0));
        mControlVao.enableVertexAttribArray(VERTICES_LAYOUT_LOCATION);

        mControlBuffer.unBindBuffer();
        mControlVao.unBindVertexArray();

        mSplineVao.bindVertexArray();
        mSplineBuffer.bindBuffer();
        mSplineBuffer.bufferData(gl::size<Point>(splinePoints.size()),
            splinePoints.data(), GL_STATIC_DRAW);
        mSplineBuffer.vertexAttribPointer(VERTICES_LAYOUT_LOCATION, 3, GL_FLOAT,
            GL_FALSE, 0, gl::bufferOffset<float>(0));
        mSplineVao.enableVertexAttribArray(VERTICES_LAYOUT_LOCATION);
        mSplineBuffer.unBindBuffer();
        mSplineVao.unBindVertexArray();

        std::vector<gl::ShaderUnit> shaders
        {
            {std::string(ShaderDirectory) + "Spline.vs.glsl", GL_VERTEX_SHADER},
            {std::string(ShaderDirectory) + "Spline.fs.glsl", GL_FRAGMENT_SHADER}
        };

        mShaders.emplace_back(shaders);
        mShaders[0].setShaderIncludeDir(ShaderDirectory);
        mShaders[0].compileShaders();
        mShaders[0].linkShaders();

        auto var = mShaders[0].getUniformVariable("model");
        mUniforms.insert(UniformKey("model", var));
        var = mShaders[0].getUniformVariable("projection");
        mUniforms.insert(UniformKey("projection", var));
        var = mShaders[0].getUniformVariable("view");
        mUniforms.insert(UniformKey("view", var));
        var = mShaders[0].getUniformVariable("colour");
        mUniforms.insert(UniformKey("colour", var));

        mSplinePosition = interpolateOnSpline();

        mShaders[0].disableShaders();
    }

    void Spline::updateGeometry(atlas::core::Time<> const& t)
    {
        UNUSED(t);

        mSplinePosition = interpolateOnSpline();
        mCurrentFrame++;
        if (mCurrentFrame == mTotalFrames)
        {
            mIsInterpolationDone = true;
            return;
        }
    }

    void Spline::renderGeometry(atlas::math::Matrix4 const& projection,
        atlas::math::Matrix4 const& view)
    {
        namespace math = atlas::math;

        mShaders[0].hotReloadShaders();
        if (!mShaders[0].shaderProgramValid())
        {
            return;
        }

        mShaders[0].enableShaders();
        mControlVao.bindVertexArray();

        glUniformMatrix4fv(mUniforms["projection"], 1, GL_FALSE,
            &projection[0][0]);
        glUniformMatrix4fv(mUniforms["view"], 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(mUniforms["model"], 1, GL_FALSE, &mModel[0][0]);

        // Render the control points first.
        glUniform3f(mUniforms["colour"], 1, 0, 0);

        if (mShowControlPoints)
        {
            glPointSize(5.0f);
            glDrawArrays(GL_POINTS, 0, GLsizei(mControlPoints.size()));
            glPointSize(1.0f);
        }

        if (mShowCage)
        {
            glDrawArrays(GL_LINE_STRIP, 0, GLsizei(mControlPoints.size()));
        }

        mControlVao.unBindVertexArray();

        mSplineVao.bindVertexArray();

        // Now draw the splines.
        glUniform3f(mUniforms["colour"], 0, 1, 0);
        
        if (mShowSpline)
        {
            glDrawArrays(GL_LINE_STRIP, 0, mResolution);
        }
        if (mShowSplinePoints)
        {
            glPointSize(8.0f);
            glDrawArrays(GL_POINTS, 0, mResolution);
            glPointSize(1.0f);
        }

        mSplineVao.unBindVertexArray();
        mShaders[0].disableShaders();
    }

    void Spline::drawGui()
    {
        ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_FirstUseEver);

        ImGui::Begin("Spline Controls");
        ImGui::Checkbox("Show Control Points", &mShowControlPoints);
        ImGui::Checkbox("Show Cage", &mShowCage);
        ImGui::Checkbox("Show Spline", &mShowSpline);
        ImGui::Checkbox("Show Spline Points", &mShowSplinePoints);
        ImGui::End();
    }

    void Spline::resetGeometry()
    {
        mCurrentFrame = 0;
        mIsInterpolationDone = false;
        mSplinePosition = interpolateOnSpline();
    }

    atlas::math::Point Spline::getPosition() const
    {
        return mSplinePosition;
    }

    bool Spline::doneInterpolation() const
    {
        return mIsInterpolationDone;
    }

    atlas::math::Point Spline::interpolateOnSpline() const
    {
        float totalDistance = mTable[mTable.size() - 1];
        float step = totalDistance / mTotalFrames;
        float currDistance = step * mCurrentFrame;

        int index = tableLookUp(currDistance);
        float t = (1.0f / mResolution) * (index % mResolution);
        return evaluateSpline(t);
    }

    atlas::math::Point Spline::evaluateSpline(float t) const
    {
        // TODO by students.
        using atlas::math::Vector4;
        using atlas::math::Point;

        Vector4 xControls =
        {
            mControlPoints[0].x, mControlPoints[1].x,
            mControlPoints[2].x, mControlPoints[3].x
        };

        Vector4 yControls = 
        {
            mControlPoints[0].y, mControlPoints[1].y,
            mControlPoints[2].y, mControlPoints[3].y
        };

        Vector4 zControls
        {
            mControlPoints[0].z, mControlPoints[1].z,
            mControlPoints[2].z, mControlPoints[3].z
        };

        Vector4 xCoeff = xControls * mBasisMatrix;
        Vector4 yCoeff = yControls * mBasisMatrix;
        Vector4 zCoeff = zControls * mBasisMatrix;

        float xcr, ycr, zcr;
        xcr = xCoeff[0] + t * xCoeff[1] + t * t * xCoeff[2] + t * t * t * xCoeff[3];
        ycr = yCoeff[0] + t * yCoeff[1] + t * t * yCoeff[2] + t * t * t * yCoeff[3];
        zcr = zCoeff[0] + t * zCoeff[1] + t * t * zCoeff[2] + t * t * t * zCoeff[3];

        return Point(xcr, ycr, zcr);
    }

    void Spline::generateArcLengthTable()
    {
        using atlas::math::Point;

        if (!mTable.empty())
        {
            mTable.clear();
        }

        // TODO by students.
        float scale = 1.0f / mResolution;
        mTable.push_back(0.0f);

        for (int i = 1; i < mResolution + 1; ++i)
        {
            Point p0 = evaluateSpline((i - 1) * scale);
            Point p1 = evaluateSpline(i * scale);

            Point dist = p0 - p1;
            mTable.push_back(mTable[i - 1] + glm::length(dist));
        }
    }

    int Spline::tableLookUp(float distance) const
    {
        float epsilon = chooseEpsilon();
        for (std::size_t i = 0; i < mTable.size(); ++i)
        {
            if (glm::abs(mTable[i] - distance) < epsilon)
            {
                return static_cast<int>(i);
            }
        }

        return -1;
    }

    float Spline::chooseEpsilon() const
    {
        float epsilon = 0.0f;
        float diff;

        for (std::size_t i = 0; i < mTable.size() - 1; ++i)
        {
            diff = glm::abs(mTable[i] - mTable[i + 1]);
                if (diff > epsilon)
                {
                    epsilon = diff;
                }
        }

        return epsilon;
    }

}
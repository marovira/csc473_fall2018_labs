#include "Dragon.hpp"
#include "Paths.hpp"
#include "LayoutLocations.glsl"

#include <atlas/utils/Mesh.hpp>

namespace lab4
{
    Dragon::Dragon() :
        mDataBuffer(GL_ARRAY_BUFFER),
        mIndexBuffer(GL_ELEMENT_ARRAY_BUFFER),
        mIndices(0)
    {
        using atlas::utils::Mesh;
        namespace gl = atlas::gl;
        namespace math = atlas::math;

        Mesh dragon;
        std::string path{ DataDirectory };
        path = path + "dragon.obj";
        Mesh::fromFile(path, dragon);

        mIndices = static_cast<GLsizei>(dragon.indices().size());

        std::vector<float> data;
        for (std::size_t i = 0; i < dragon.vertices().size(); ++i)
        {
            data.push_back(dragon.vertices()[i].x);
            data.push_back(dragon.vertices()[i].y);
            data.push_back(dragon.vertices()[i].z);

            data.push_back(dragon.normals()[i].x);
            data.push_back(dragon.normals()[i].y);
            data.push_back(dragon.normals()[i].z);
        }

        mVao.bindVertexArray();
        mDataBuffer.bindBuffer();
        mDataBuffer.bufferData(gl::size<float>(data.size()), data.data(), 
            GL_STATIC_DRAW);
        mDataBuffer.vertexAttribPointer(VERTICES_LAYOUT_LOCATION, 3, GL_FLOAT,
            GL_FALSE, gl::stride<float>(6), gl::bufferOffset<float>(0));
        mDataBuffer.vertexAttribPointer(NORMALS_LAYOUT_LOCATION, 3, GL_FLOAT,
            GL_FALSE, gl::stride<float>(6), gl::bufferOffset<float>(3));

        mVao.enableVertexAttribArray(VERTICES_LAYOUT_LOCATION);
        mVao.enableVertexAttribArray(NORMALS_LAYOUT_LOCATION);

        mIndexBuffer.bindBuffer();
        mIndexBuffer.bufferData(gl::size<GLuint>(dragon.indices().size()),
            dragon.indices().data(), GL_STATIC_DRAW);

        mIndexBuffer.unBindBuffer();
        mDataBuffer.unBindBuffer();
        mVao.unBindVertexArray();

        std::vector<gl::ShaderUnit> shaders
        {
            {std::string(ShaderDirectory) + "Dragon.vs.glsl", GL_VERTEX_SHADER},
            {std::string(ShaderDirectory) + "Dragon.fs.glsl", GL_FRAGMENT_SHADER}
        };

        mShaders.emplace_back(shaders);
        mShaders[0].setShaderIncludeDir(ShaderDirectory);
        mShaders[0].compileShaders();
        mShaders[0].linkShaders();

        auto var = mShaders[0].getUniformVariable("projection");
        mUniforms.insert(UniformKey("projection", var));
        var = mShaders[0].getUniformVariable("view");
        mUniforms.insert(UniformKey("view", var));
        var = mShaders[0].getUniformVariable("model");
        mUniforms.insert(UniformKey("model", var));

        mShaders[0].disableShaders();
    }

    void Dragon::renderGeometry(atlas::math::Matrix4 const& projection,
        atlas::math::Matrix4 const& view)
    {
        namespace math = atlas::math;

        mShaders[0].hotReloadShaders();
        if (!mShaders[0].shaderProgramValid())
        {
            return;
        }

        mShaders[0].enableShaders();

        mVao.bindVertexArray();
        mIndexBuffer.bindBuffer();

        glUniformMatrix4fv(mUniforms["projection"], 1, GL_FALSE, 
            &projection[0][0]);
        glUniformMatrix4fv(mUniforms["view"], 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(mUniforms["model"], 1, GL_FALSE, &mModel[0][0]);

        glDrawElements(GL_TRIANGLES, mIndices, GL_UNSIGNED_INT, 0);

        mIndexBuffer.unBindBuffer();
        mVao.unBindVertexArray();
        mShaders[0].disableShaders();
    }
}
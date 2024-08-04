#include "OGL3D/Graphics/OGraphicsEngine.h"
#include "OGL3D/Graphics/OVertexArrayObject.h"
#include "OGL3D/Graphics/OUniformBuffer.h"
#include "OGL3D/Graphics/OShaderProgram.h"
#include "glad/glad.h"
#include "assert.h"
#include "stdexcept"

OVertexArrayObjectPtr OGraphicsEngine::createVertexArrayObject(const OVertexBufferDesc& data)
{
	return std::make_shared<OVertexArrayObject>(data);
}

OUniformBufferPtr OGraphicsEngine::createUniformBuffer(const OUniformBufferDesc& desc)
{
	return std::make_shared<OUniformBuffer>(desc);
}

OShaderProgramPtr OGraphicsEngine::createShaderProgram(const OShaderProgramDesc& desc)
{
	return std::make_shared<OShaderProgram>(desc);
}

void OGraphicsEngine::clear(const OVec4& color)
{
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

void OGraphicsEngine::setViewport(const ORect& size)
{
	glViewport(size.left, size.top, size.width, size.height);
}

void OGraphicsEngine::setVertexArrayObject(const OVertexArrayObjectPtr& vao)
{
	glBindVertexArray(vao->getId());
}

void OGraphicsEngine::setUniformBuffer(const OUniformBufferPtr& buffer, ui32 slot)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, buffer->getId());
}

void OGraphicsEngine::setShaderProgram(const OShaderProgramPtr& program)
{
	glUseProgram(program->getId());
}

void OGraphicsEngine::drawTriangles(const OTriangleType& triangleType, ui32 vertexCount, ui32 offset)
{
	auto glTriType = GL_TRIANGLES;

	switch (triangleType)
	{
		case TriangleList: { glTriType = GL_TRIANGLES; break; }
		case TriangleStrip: { glTriType = GL_TRIANGLE_STRIP; break; }
	}
	glDrawArrays(glTriType, offset, vertexCount);
}


#include "OGL3D/Graphics/OVertexArrayObject.h"
#include <glad/glad.h>


OVertexArrayObject::OVertexArrayObject(const OVertexBufferDesc& vbDesc)
{
	if (!vbDesc.listSize) OGL3D_ERROR("OVertexArrayObject | listSize is NULL");
	if (!vbDesc.vertexSize) OGL3D_ERROR("OVertexArrayObject | vertexSize is NULL");
	if (!vbDesc.verticesList) OGL3D_ERROR("OVertexArrayObject | verticesList is NULL");

	glGenVertexArrays(1, &m_vertexArrayObjectId);
	glBindVertexArray(m_vertexArrayObjectId); // Create and bind array

	glGenBuffers(1,&m_vertexBufferId); // Generate a buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId); // bind ID
	glBufferData(GL_ARRAY_BUFFER, vbDesc.vertexSize* vbDesc.listSize, vbDesc.verticesList,GL_STATIC_DRAW); // buffer input data

	for (ui32 i = 0; i < vbDesc.attributeListSize; i++)
	{
		glVertexAttribPointer(
			i,
			vbDesc.attributeList[i].numElements,
			GL_FLOAT,
			GL_FALSE,
			vbDesc.vertexSize,
			(void*)((i == 0)?0: vbDesc.attributeList[i-1].numElements*sizeof(f32))
		);
		glEnableVertexAttribArray(i);
	}

	glBindVertexArray(0); // Signal done

	m_vertexBufferData = vbDesc;

}

OVertexArrayObject::OVertexArrayObject(const OVertexBufferDesc& vbDesc, const OIndexBufferDesc& ibDesc):OVertexArrayObject(vbDesc)
{
	if (!ibDesc.listSize) OGL3D_ERROR("OVertexArrayObject | listSize is NULL");
	if (!ibDesc.indicesList) OGL3D_ERROR("OVertexArrayObject | indicesList is NULL");

	glBindVertexArray(m_vertexArrayObjectId); // Create and bind array

	glGenBuffers(1, &m_elementBufferId); // Generate a buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferId); // bind ID
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibDesc.listSize, ibDesc.indicesList, GL_STATIC_DRAW); // buffer input data

	glBindVertexArray(0); // Signal done

}

OVertexArrayObject::~OVertexArrayObject()
{
	glDeleteBuffers(1, &m_elementBufferId);
	glDeleteBuffers(1, &m_vertexBufferId);
	glDeleteVertexArrays(1, &m_vertexArrayObjectId);
}

ui32 OVertexArrayObject::getId()
{
	return m_vertexArrayObjectId;
}

ui32 OVertexArrayObject::getVertexBufferSize()
{
	return m_vertexBufferData.listSize;
}

ui32 OVertexArrayObject::getVertexSize()
{
	return m_vertexBufferData.vertexSize;
}

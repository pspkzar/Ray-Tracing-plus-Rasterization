#include <curitiba/render/optix/optixBufferLib.h>
#include <curitiba.h>

using namespace curitiba::render::optixRender;

OptixBufferLib::OptixBufferLib() {
}


void
OptixBufferLib::setContext(optix::Context &c) {

	m_Context = c;
}


optix::Buffer & 
OptixBufferLib::getBuffer(int glBufferID, unsigned int size) {

	// check to see if a buffer has already been created
	// if not then create it

	if (!m_Buffer.count(glBufferID)) {

		m_Buffer[glBufferID] = m_Context->createBufferFromGLBO(RT_BUFFER_INPUT,glBufferID);
		m_Buffer[glBufferID]->setFormat(RT_FORMAT_FLOAT4);
		m_Buffer[glBufferID]->setSize(size);
	}
	return m_Buffer[glBufferID];
}


optix::Buffer & 
OptixBufferLib::getIndexBuffer(int glBufferID, unsigned int size) {

	// check to see if a buffer has already been created
	// if not then create it

	if (!m_IndexBuffer.count(glBufferID)) {

		m_IndexBuffer[glBufferID] = m_Context->createBufferFromGLBO(RT_BUFFER_INPUT,glBufferID);
		m_IndexBuffer[glBufferID]->setFormat(RT_FORMAT_UNSIGNED_INT);
		m_IndexBuffer[glBufferID]->setSize(size);
	}
	return m_IndexBuffer[glBufferID];
}

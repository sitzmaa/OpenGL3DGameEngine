#include <OGL3D/Game/OGame.h>
#include <OGL3D/Window/OWindow.h>
#include <OGL3D/Graphics/OVertexArrayObject.h>
#include <OGL3D/Graphics/OGraphicsEngine.h>
#include <OGL3D/Graphics/OShaderProgram.h>
#include <OGL3D/Graphics/OUniformBuffer.h>

struct UniformData
{
	f32 scale;
};


OGame::OGame()
{
	m_graphicsEngine = std::make_unique<OGraphicsEngine>();
	m_display = std::make_unique<OWindow>();

	m_display->makeCurrentContext();

	m_graphicsEngine->setViewport(m_display->getInnerSize());
}

OGame::~OGame()
{
}

void OGame::onCreate()
{
	const f32 polygonVertices[] = {
		-0.5f,-0.5f,0.0f,
		1,0,0,
		-0.5f,0.5f,0.0f,
		0,1,0,
		0.5f,-0.5f,0.0f,
		0,0,1,		
		0.5f,0.5f,0.0f,
		1,1,0

	};
	OVertexAttribute attribsList[] = {
		3, // position
		3 // color
	};
	m_polygonVAO = m_graphicsEngine->createVertexArrayObject({
		(void*)polygonVertices,
		sizeof(f32)*(3+3),
		4,

		attribsList,
		2
		});

	m_uniform = m_graphicsEngine->createUniformBuffer({
		sizeof(UniformData)
		});

	m_shader = m_graphicsEngine->createShaderProgram(
		{
			L"Assets/Shaders/BasicShader.vert",
			L"Assets/Shaders/BasicShader.frag"
		});
	m_shader->setUniformBufferSlot("UniformData", 0);
}

void OGame::onUpdate()
{
	// computing delta time
	auto currentTime = std::chrono::system_clock::now();
	auto elapsedSeconds = std::chrono::duration<double>();
	if (m_previousTime.time_since_epoch().count())
		elapsedSeconds = currentTime - m_previousTime;
	
	m_previousTime = currentTime;

	auto deltaTime = (f32)elapsedSeconds.count();

	m_scale += 0.707f * deltaTime;
	auto currentScale = abs(sin(m_scale));

	UniformData data = { currentScale };
	m_uniform->setData(&data);



	m_graphicsEngine->clear(OVec4(0, 0, 0, 1));

	m_graphicsEngine->setVertexArrayObject(m_polygonVAO);
	m_graphicsEngine->setUniformBuffer(m_uniform, 0);
	m_graphicsEngine->setShaderProgram(m_shader);
	m_graphicsEngine->drawTriangles(TriangleStrip, m_polygonVAO->getVertexBufferSize(), 0);
	m_display->present(false);
}

void OGame::onQuit()
{
}


void OGame::quit()
{
	m_isRunning = false;
}
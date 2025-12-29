#include "pch.h"
#include "Rendering/OpenGLRenderer.h"

#include "Rendering/OpenGLWindow.h"
#include "Rendering/OpenGLErrorCheck.h"

#include "GL/glew.h" // glew.h must always be the first openGL header to be included (before glfw3.h)
#include "GLFW/glfw3.h"

#include "glm/ext/matrix_clip_space.hpp"

static constexpr int DEFAULT_WINDOW_WIDTH = 2560;
static constexpr int DEFAULT_WINDOW_HEIGHT = 1440;

static constexpr float NEAR_PLANE = 0.01f;
static constexpr float FAR_PLANE = 1000.0f;

static constexpr float FOV = 90.0f;

OpenGLRenderer::OpenGLRenderer()
	:m_projMatrix{ 1.0f }, m_appVAO(0)
{
}

OpenGLRenderer::~OpenGLRenderer()
{
}

/// <summary>
/// Initialize GLEW and GLFW, set GL capabilities and create the OpenGL window
/// </summary>
bool OpenGLRenderer::Init()
{
	// GLFW
	if (!glfwInit())
	{
		PRINT_RED("GLFW failed to initialize");
		return false;
	}

	TheOpenGLWindow::Get()->Init(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "Curve3D Window", false);

	// GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		PRINT_RED("GLEW failed to initialize");
		return false;
	}

	//PRINT_TRACE("Vendor is {0}", glGetString(GL_VENDOR));
	//PRINT_TRACE("Renderer is {0}", glGetString(GL_RENDERER));
	//PRINT_TRACE("Version is {0}", glGetString(GL_VERSION));

	// Depth Buffer
	glCall(glEnable(GL_DEPTH_TEST));
	glCall(glDepthFunc(GL_LEQUAL));

	// Back face Culling
	glCall(glEnable(GL_CULL_FACE));
	glCall(glFrontFace(GL_CCW));
	glCall(glCullFace(GL_BACK));

	// Multi sampling
	glCall(glEnable(GL_MULTISAMPLE));

	// Build applications projection matrix
	m_projMatrix = glm::perspective(glm::radians(FOV), TheOpenGLWindow::Get()->GetAspectRatio(), NEAR_PLANE, FAR_PLANE);

	// Build applications VAO
	glCall(glGenVertexArrays(1, &m_appVAO));
	glCall(glBindVertexArray(m_appVAO));

	// Create the apps Frame buffers
	//m_pSceneMSAAFrameBuffer = std::make_unique<OpenGLFramebuffer>(true);

	// FRAMEBUFFER STUFF

	return true;
}

/// <summary>
/// Delete OpenGL VAO and terminate GLFW
/// </summary>
/// <returns></returns>
bool OpenGLRenderer::Clean()
{
	if (m_appVAO)
		glDeleteVertexArrays(1, &m_appVAO);

	glfwTerminate();

	return true;
}

/// <summary>
/// Prepare the application to render a frame
/// Clear rendering buffers, poll GLFW events
/// </summary>
void OpenGLRenderer::StartOfFrame() const
{
	// Clear the screen buffers
	glCall(glClear(GL_DEPTH_BUFFER_BIT)); 
	glCall(glfwPollEvents());
}

/// <summary>
/// Finalize the applications rendering of a frame
/// </summary>
void OpenGLRenderer::EndOfFrame() const
{
	glCall(glfwSwapBuffers(TheOpenGLWindow::Get()->GetGLFWWindow()));
}

/// <summary>
/// Draws any bound texture and mesh buffers with the bound shader
/// </summary>
void OpenGLRenderer::Draw(size_t indicesCount) const
{
	glCall(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indicesCount), GL_UNSIGNED_INT, NULL));
}

/// <summary>
/// Draws using the cubemap shader
/// </summary>
void OpenGLRenderer::DrawCubemap(size_t vertexCount) const
{
	// Disables writing to the depth buffer
	glCall(glDepthFunc(GL_LEQUAL));
	glCall(glDrawArrays(GL_TRIANGLES, NULL, static_cast<GLsizei>(vertexCount)));
	glCall(glDepthFunc(GL_LESS));
}

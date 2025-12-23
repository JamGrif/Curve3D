#pragma once

#include <glm/mat4x4.hpp>

/// <summary>
/// Interface for rendering buffers that are bound to the OpenGL context
/// </summary>
class OpenGLRenderer
{
public:

	bool				Init();
	bool				Clean();

	const glm::mat4&	GetProjectionMatrix() { return m_projMatrix; }

	void				StartOfFrame() const;
	void				EndOfFrame() const;

	void				Draw(size_t indicesCount) const;
	void				DrawCubemap(size_t vertexCount) const;

	static OpenGLRenderer* Get() // Get instance
	{
		static OpenGLRenderer* s_pInstance = new OpenGLRenderer;
		return s_pInstance;
	}

private:
	OpenGLRenderer();
	~OpenGLRenderer();

	glm::mat4			m_projMatrix;

	OpenGLIndex			m_appVAO;

};
typedef OpenGLRenderer TheOpenGLRenderer;

#pragma once

/// <summary>
/// Base class for the resource loader
/// The loader sets the resources initial parameters
/// Mesh - Texture - Cubemmap - Shader
/// </summary>
struct IResourceLoader 
{
	std::string file;

	virtual ~IResourceLoader() = 0;

};




/// <summary>
/// Base class interface for a Resource object
/// Mesh - Texture - Cubemap - Shader
/// </summary>
class IResource
{
public:
	IResource();
	virtual ~IResource();

	virtual void		Bind() = 0;
	virtual void		Unbind() = 0;

	virtual void		Parse(IResourceLoader* resourceLoader) = 0;
	virtual void		Parse(const std::string& filepath) = 0;
	virtual void		Parse(const std::string& firstFilepath, const std::string& secondFilepath) = 0;
	virtual void		Create() = 0;

	OpenGLIndex			GetOpenGLID() { return m_OpenGLResourceID; }
	bool				GetCreated() { return m_bIsCreated; }

	const ResourceFile&	GetResourceID() { return m_resourceID; }
	const std::string&	GetFilepath() { return m_resourceFilepath; }

protected:

	// Internal OpenGL index for resource
	OpenGLIndex			m_OpenGLResourceID;

	bool				m_bIsCreated;

	// ID used by the ResourceManager to retrieve this object
	ResourceFile			m_resourceID;

	std::string			m_resourceFilepath;
};

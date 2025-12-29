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

	virtual bool		Parse(IResourceLoader* resourceLoader) = 0;
	virtual bool		Create() = 0;

	OpenGLIndex			GetOpenGLID() { return m_OpenGLResourceID; }
	bool				GetCreated() { return m_bIsCreated; }

	const ResourceFile&	GetResourceID() { return m_resourceFile; }
	const std::string&	GetFilepath() { return m_resourceFilepath; }

protected:

	// ID used by the ResourceManager to retrieve this object
	ResourceFile			m_resourceFile;

	// Internal OpenGL index for resource
	OpenGLIndex			m_OpenGLResourceID;

	bool				m_bIsCreated;

	std::string			m_resourceFilepath;
};

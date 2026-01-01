#pragma once

#include "Rendering/Resource/IResource.h"

/// <summary>
/// Tells a Texture object which slot in the shader to use
/// Numbers correspond to index in the shader file
/// </summary>
enum TextureType
{
	UNSET					= -1,
	DIFFUSE					= 0,
	SPECULAR				= 1,
	NORMAL					= 2,
	HEIGHT					= 3,
	EMISSION				= 4,
};



struct TextureLoader :
	public IResourceLoader
{
	TextureType textureType;
};

/// <summary>
/// Encapsulates an OpenGL texture buffer to act as a Texture
/// </summary>
class Texture :
	public IResource
{
public:
	Texture();
	~Texture();

private:

	virtual void	Bind() override;
	virtual void	Unbind() override;

	virtual bool	Parse(IResourceLoader* resourceLoader) override;
	virtual bool	Create() override;

private:

	// Texture data
	int32_t			m_textureWidth, m_textureHeight, m_textureBPP;

	// sampler2D slot used in shader
	TextureType		m_textureType;

	// Temp storage between parse and create
	unsigned char*	m_pTempBuffer;

	// Ensures only the TextureManager can call functions of a texture object
	template<typename Texture>
	friend class ResourceManager;
};

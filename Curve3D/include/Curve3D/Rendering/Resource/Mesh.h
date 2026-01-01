#pragma once

#include "Rendering/Resource/IResource.h"

struct Vertex;

struct MeshLoader :
	public IResourceLoader
{
};

/// <summary>
/// Encapsulates an OpenGL VBO and EBO buffer to act as a mesh
/// </summary>
class Mesh :
	public IResource
{
public:
	Mesh();
	~Mesh();

	size_t			GetIndicesCount() const { return m_meshIndices.size(); }

 private:

	 virtual void	Bind() override;
	 virtual void	Unbind() override;

	 virtual bool	Parse(IResourceLoader* resourceLoader) override;

	 virtual bool	Create() override;

private:

	uint32_t		m_meshEBO;

	typedef std::vector<Vertex> MeshVertices;
	MeshVertices	m_meshVertices;

	typedef std::vector<uint32_t> MeshIndices;
	MeshIndices		m_meshIndices;

	template<typename Mesh>
	friend class ResourceManager;
};

#pragma once

// Classes that are managed through the ResourceManager
#include "Rendering/Resource/Mesh.h"
#include "Rendering/Resource/Texture.h"
#include "Rendering/Resource/Cubemap.h"
#include "Rendering/Resource/Shader.h"

/// <summary>
/// Encapsulates and abstracts the creation, deletion and usage of a Resource object providing a consistent interface
/// Mesh, Texture, Cubemap and Shader objects can only be used through this class
/// </summary>
template <typename T>
class ResourceManager
{
public:

	bool AddResource(IResourceLoader* resourceLoader)
	{
		// Check if resource with ID already exists
		if (m_resourcePool.find(resourceLoader->file) != m_resourcePool.end())
			return false;

		// Create and parse the resource file
		std::shared_ptr<T> pResource = std::make_shared<T>();
		pResource->Parse(resourceLoader);

		m_resourcePool.insert({ resourceLoader->file, pResource });

		return true;
	}

	/// <summary>
	/// 1 / 2 of resource creation
	/// Adds a new resource and parses its data file
	/// </summary>
	bool AddResource(const ResourceFile& resourceID)
	{
		// Check if resource with ID already exists
		if (m_resourcePool.find(resourceID) != m_resourcePool.end())
			return false;

		// Create and parse the resource file
		std::shared_ptr<T> pResource = std::make_shared<T>();
		pResource->Parse(resourceID);

		m_resourcePool.insert({ resourceID, pResource });

		return true;
	}

	/// <summary>
	/// 1 / 2 of resource creation
	/// Adds a new resource and parses its data file
	/// </summary>
	bool AddResource(const ResourceFile& resourceID, const std::string& vertexPath, const std::string& fragmentPath)
	{
		// Check if resource with ID already exists
		if (m_resourcePool.find(resourceID) != m_resourcePool.end())
			return false;

		// Create and parse the resource file
		std::shared_ptr<T> pResource = std::make_shared<T>();
		pResource->Parse(vertexPath, fragmentPath);

		m_resourcePool.insert({ resourceID, pResource });

		return true;
	}

	/// <summary>
	/// 2 / 2 of resource creation
	/// Creates all resources of a specific type that have been successfully parsed
	/// </summary>
	void CreateAllResources()
	{
		for (const auto& [ResourceFile, resourceObject] : m_resourcePool)
		{
			// If resource isn't already created, create it
			if (!resourceObject->GetCreated())
				resourceObject->Create();
		}
	}

	/// <summary>
	/// Bind resource at resourceID to the OpenGL context
	/// </summary>
	void BindResourceAtID(const ResourceFile& resourceID)
	{
		if (m_resourcePool.count(resourceID))
		{
			m_resourcePool.at(resourceID)->Bind();
		}
	}

	/// <summary>
	/// Unbind resource at resourceID from the OpenGL context
	/// </summary> 
	void UnbindResourceAtID(const ResourceFile& resourceID)
	{
		if (m_resourcePool.count(resourceID))
		{
			m_resourcePool.at(resourceID)->Unbind();
		}
	}

	/// <summary>
	/// Return the resource at resourceID allowing public functions to be called on that resource
	/// </summary>
	std::shared_ptr<T> GetResourceAtID(const ResourceFile& resourceID)
	{
		if (m_resourcePool.count(resourceID))
		{
			return m_resourcePool.at(resourceID);
		}
		return {};
	}

	/// <summary>
	/// Deletes all resources of a specific type
	/// </summary>
	void ClearAllResources()
	{
		m_resourcePool.clear();
	}

	static ResourceManager<T>* Get() // Get instance
	{
		static ResourceManager<T>* s_pInstance = new ResourceManager<T>;
		return s_pInstance;
	}
private:

	std::unordered_map<ResourceFile, std::shared_ptr<T>> m_resourcePool;

	ResourceManager() {}
	~ResourceManager() {}
	ResourceManager(const ResourceManager&) = delete;
};

typedef ResourceManager<Mesh> MeshManager;
typedef ResourceManager<Texture> TextureManager;
typedef ResourceManager<Cubemap> CubemapManager;
typedef ResourceManager<Shader> ShaderManager;

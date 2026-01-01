#pragma once

// Classes managed through the ResourceManager
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
	typedef int ResourceID;

	/// <summary>
	/// 1 / 2 of resource creation
	/// Adds a new resource and parses its data file
	/// </summary>
	int AddResource(IResourceLoader* resourceLoader)
	{
		// If resource at file already exists, return its index in resource pool
		if (m_createdResources.find(resourceLoader->file) != m_createdResources.end())
			return m_createdResources.at(resourceLoader->file);

		// Create and parse the resource
		std::shared_ptr<T> pResource = std::make_shared<T>();
		if (!pResource->Parse(resourceLoader))
		{
			PRINT_RED("{0}", pResource->GetError());
			return UNSET_RESOURCE_ID;
		}

		// ID of new resource is the number of currently created resources, plus one as UNSET_RESOURCE_ID uses slot 0
		ResourceID newID = static_cast<ResourceID>(m_createdResources.size()) + 1;

		m_createdResources.insert({ resourceLoader->file, newID });
		m_resourcePool.insert({ newID, pResource });

		PRINT_GREEN("Parse() {0} with ID {1}", resourceLoader->file, newID);
		return newID;
	}

	/// <summary>
	/// 2 / 2 of resource creation
	/// Creates all resources of a specific type that have been successfully parsed
	/// </summary>
	void CreateAllResources()
	{
		for (const auto& [ResourceID, resourceObject] : m_resourcePool)
		{
			// If resource isn't already created, create it
			if (!resourceObject->GetCreated())
			{
				if (resourceObject->Create())
					PRINT_GREEN("Create() of ID {0}", ResourceID);
				else
					PRINT_RED("{0}", resourceObject->GetError());
			}
		}
	}

	/// <summary>
	/// Bind resource at resourceID to the OpenGL context
	/// </summary>
	void BindResourceAtID(ResourceID resourceID)
	{
		if (m_resourcePool.count(resourceID))
			m_resourcePool.at(resourceID)->Bind();
	}

	/// <summary>
	/// Unbind resource at resourceID from the OpenGL context
	/// </summary> 
	void UnbindResourceAtID(ResourceID resourceID)
	{
		if (m_resourcePool.count(resourceID))
			m_resourcePool.at(resourceID)->Unbind();
	}

	/// <summary>
	/// Return the resource at resourceID allowing public functions to be called on that resource
	/// </summary>
	std::shared_ptr<T> GetResourceAtID(ResourceID resourceID)
	{
		if (m_resourcePool.count(resourceID))
			return m_resourcePool.at(resourceID);

		return {};
	}

	/// <summary>
	/// If you know the filename of a resource, get ID from resource pool
	/// </summary>
	ResourceID GetResourceIDFromFile(const std::string& file)
	{
		if (m_createdResources.count(file))
			return m_createdResources.at(file);

		return UNSET_RESOURCE_ID;
	}

	/// <summary>
	/// Deletes all resources of a specific type
	/// </summary>
	void ClearAllResources()
	{
		m_resourcePool.clear();
		m_createdResources.clear();
	}

	static ResourceManager<T>* Get() 
	{
		static ResourceManager<T>* s_pInstance = new ResourceManager<T>;
		return s_pInstance;
	}

	void SetErrorResourceID(ResourceID newResourceID) { m_errorResourceID = newResourceID; }
	ResourceID GetErrorResourceID() { return m_errorResourceID; }

private:

	std::unordered_map<ResourceID, std::shared_ptr<T>> m_resourcePool;
	std::unordered_map<ResourceFile, ResourceID> m_createdResources;

	// The ID of the resource to use when a resource fails to be parsed / created
	ResourceID m_errorResourceID = UNSET_RESOURCE_ID;

	ResourceManager() {}
	~ResourceManager() {}
	ResourceManager(const ResourceManager&) = delete;

};
typedef ResourceManager<Mesh> MeshManager;
typedef ResourceManager<Texture> TextureManager;
typedef ResourceManager<Cubemap> CubemapManager;
typedef ResourceManager<Shader> ShaderManager;

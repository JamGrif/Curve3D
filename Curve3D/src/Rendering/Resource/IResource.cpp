#include "pch.h"
#include "Rendering/Resource/IResource.h"

IResourceLoader::~IResourceLoader() {}

IResource::IResource()
	:m_OpenGLResourceID(-1), m_bIsCreated(false), m_resourceID(UNSET_RESOURCE_ID)
{
}

IResource::~IResource()
{
}

#include <Resource/Resource.h>

Resource::Resource(const wchar_t* full_path, ResourceManager* manager): m_full_path(full_path), m_manager(manager)
{
}


Resource::~Resource()
{
}

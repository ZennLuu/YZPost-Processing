#pragma once
#include <Prerequisites.h>
#include <string>

class Resource
{
public:
	Resource(const wchar_t* full_path, ResourceManager* manager);
	virtual ~Resource();
protected:
	std::wstring m_full_path;
	ResourceManager* m_manager = nullptr;
};


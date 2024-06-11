#pragma once
#include <filesystem>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>
#include <sstream>

class ShaderInclude : public ID3DInclude
{
public:
	ShaderInclude(const std::filesystem::path& shaderPath): m_shaderPath(shaderPath)
	{

	}
	~ShaderInclude()
	{
		
	}

	virtual HRESULT Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName,
		LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes)
	{
		auto includePath = m_shaderPath;
		includePath = includePath.parent_path();
		includePath += "/";
		includePath += pFileName;

		std::ifstream shaderStream(includePath);
		if (shaderStream.is_open())
		{
			std::stringstream sstr;
			sstr << shaderStream.rdbuf();
			auto str = sstr.str();

			char* shaderCode = new char[str.size() + 1];
			memcpy(shaderCode, str.c_str(), str.size() + 1);

			shaderStream.close();

			*ppData = shaderCode;
			*pBytes = str.size();

			return S_OK;
		}
		return S_FALSE;
	}
	virtual HRESULT Close(LPCVOID pData)
	{
		delete pData;
		return S_OK;
	}
private:
	std::filesystem::path m_shaderPath;
};
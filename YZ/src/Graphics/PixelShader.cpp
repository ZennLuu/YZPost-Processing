#include <Graphics/PixelShader.h>
#include <Graphics/RenderSystem.h>

#include <Graphics/ShaderInclude.h>

#include <d3dcompiler.h>
#include <exception>

PixelShader::PixelShader(const wchar_t* full_path, const char* entryPoint, RenderSystem* system) : m_system(system)
{
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
	ShaderInclude shaderInclude(full_path);

	D3DCompileFromFile(full_path, nullptr, &shaderInclude,
		entryPoint, "ps_5_0", 0, 0, &blob, &errorBlob);

	if (errorBlob)
		DX3DWarning("Pixel Shader " << full_path << " compiled with errors: \n" << (char*)errorBlob->GetBufferPointer());
	if (!blob)
		DX3DError("Pixel Shader " << full_path << " not created successfully.");
	if (FAILED(m_system->m_d3d_device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_ps)))
		DX3DError("Pixel Shader " << full_path << " not created successfully.");
}	
#include <Graphics/VertexShader.h>
#include <Graphics/RenderSystem.h>

#include <Graphics/ShaderInclude.h>

#include <d3dcompiler.h>
#include <exception>

VertexShader::VertexShader(const wchar_t* full_path, const char* entryPoint, RenderSystem* system) : m_system(system)
{
	Microsoft::WRL::ComPtr<ID3DBlob>  blob;
	Microsoft::WRL::ComPtr<ID3DBlob>  errorBlob;
	ShaderInclude shaderInclude(full_path);

	D3DCompileFromFile(full_path, nullptr, &shaderInclude,
		entryPoint, "vs_5_0", 0, 0, &blob, &errorBlob);

	if (errorBlob)
		DX3DWarning("Vertex Shader " << full_path << " compiled with errors: \n" << (char*)errorBlob->GetBufferPointer());
	if (!blob)
		DX3DError("Vertex Shader " << full_path << " not created successfully.");
	if (FAILED(m_system->m_d3d_device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_vs)))
		DX3DError("Vertex Shader " << full_path << " not created successfully.");
}
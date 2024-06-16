#pragma once
#include <Entity/Component.h>
#include <Math/Rect.h>



class PostProcessComponent : public Component
{
public:
	enum class Type
	{
		None = 0, 
		HDR
	};
public:
	PostProcessComponent();
	PostProcessComponent(Type type);
	virtual ~PostProcessComponent();

	void setShader(const wchar_t* file_path);
	void setData(void* data, ui32 size);

	void resize(const Rect& window);
protected:
	virtual void onCreateInternal();
private:
	TexturePtr m_render_target;
	TexturePtr m_depth_stencil;

	VertexBufferPtr m_meshVb;
	IndexBufferPtr m_meshIb;
	ConstantBufferPtr m_cb;

	VertexShaderPtr m_vertexShader;
	PixelShaderPtr m_pixelShader;

	Type m_type = Type::None;

	friend class GraphicsEngine;
};


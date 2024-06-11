#pragma once
#include <Entity/Component.h>
#include <Math/Rect.h>

class TextComponent : public Component
{
public:
	TextComponent();
	virtual ~TextComponent();

	void setFont(const FontPtr& font);
	const FontPtr& getFont();

	void setText(const wchar_t* text);
	const wchar_t* getText();

	Rect getBounds();
protected:
	virtual void onCreateInternal();
private:
	std::wstring m_text = L"";
	FontPtr m_font;
};
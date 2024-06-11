#pragma once
#include <Resource/Resource.h>

class Font : public Resource
{
public:
	Font(const wchar_t* fullPath, ResourceManager* manager);
	const Font2DPtr& getFont();
private:
	Font2DPtr m_font;
};



class FontUtility
{
public:
	bool createFont(const wchar_t* fontName, f32 fontSize, const wchar_t* outputPath);
};
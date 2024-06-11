#include <Resource/Font.h>
#include <Resource/ResourceManager.h>
#include <Game/Game.h>
#include <Graphics/GraphicsEngine.h>
#include <Graphics/RenderSystem.h>
#include <sstream>
#include <regex>
#include <filesystem>

Font::Font(const wchar_t* fullPath, ResourceManager* manager) : Resource(fullPath, manager)
{
	m_font = manager->getGame()->getGraphicsEngine()->getRenderSystem()->createFont(fullPath);
}

const Font2DPtr& Font::getFont()
{
	return m_font;
}

bool FontUtility::createFont(const wchar_t* fontName, f32 fontSize, const wchar_t* outputPath)
{
	std::wstringstream p;
	std::wstring s = std::regex_replace(fontName, std::wregex(L"\\s+"), L"");
	p << outputPath << s << ".font";

	if (!std::filesystem::exists(p.str()))
	{
		std::wstringstream command;
		command << "DX3D\\Vendor\\DirectXTK\\bin\\MakeSpriteFont ";
		command << "\"" << fontName << "\" ";
		command << p.str() << " ";
		command << "/FontSize:" << fontSize;

		if (_wsystem(command.str().c_str()) != 0)
			DX3DError("Creation of the font " << fontName << " failed");

		return true;
	}

	return true;
}
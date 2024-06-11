#pragma once
#include <Entity/Component.h>
#include <Math/Rect.h>

class ImageComponent : public Component
{
public:
	ImageComponent();
	virtual ~ImageComponent();

	void setImage(const TexturePtr& image);
	const TexturePtr& getImage();

	void setSize(const Rect& size);
	const Rect& getSize();

protected:
	virtual void onCreateInternal();
private:

	TexturePtr m_image;
	Rect m_size;
};


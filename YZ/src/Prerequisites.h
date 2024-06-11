#pragma once
#include <memory>
#include <iostream>
#include <stdexcept>

class SwapChain;
class DeviceContext;
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class VertexShader;
class PixelShader;
class Texture2D;
class Font2D;
class RenderSystem;
class GraphicsEngine;
class Resource;
class ResourceManager;
class Texture;
class TextureManager;
class Mesh;
class MeshManager;
class Material;
class Font;
class FrameBuffer;
class Window;
class Game;
class Display;
class InputSystem;
class PhysicsEngine;

class Component;
class TransformComponent;
class MeshComponent;
class CameraComponent;
class LightComponent;
class TerrainComponent;
class WaterComponent;
class FogComponent;
class TextComponent;
class ImageComponent;
class PlayerControllerComponent;
class PostProcessComponent;

class Entity;
class World;

typedef std::shared_ptr<SwapChain> SwapChainPtr;
typedef std::shared_ptr<DeviceContext> DeviceContextPtr;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;
typedef std::shared_ptr<ConstantBuffer> ConstantBufferPtr;
typedef std::shared_ptr<VertexShader> VertexShaderPtr;
typedef std::shared_ptr<PixelShader> PixelShaderPtr;
typedef std::shared_ptr<Texture2D> Texture2DPtr;
typedef std::shared_ptr<Font2D> Font2DPtr;
typedef std::shared_ptr<Resource> ResourcePtr;
typedef std::shared_ptr<Texture> TexturePtr;
typedef std::shared_ptr<Mesh> MeshPtr;
typedef std::shared_ptr<Material> MaterialPtr;
typedef std::shared_ptr<Font> FontPtr;

typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;

typedef unsigned int ui32;
typedef int           i32;
typedef float         f32;
typedef double        d64;

enum class Key
{
	A = 0,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	_0,
	_1,
	_2,
	_3,
	_4,
	_5,
	_6,
	_7,
	_8,
	_9,
	Escape,
	Shift,
	Space,
	Enter,
	LeftMouseButton,
	MiddleMouseButton,
	RightMouseButton
};

enum class CameraType
{
	Orthogonal = 0,
	Perspective
};

enum class CullMode
{
	None = 0,
	Front,
	Back
};


#define DX3DError(message)\
{\
std::wclog << "DX3D Error: " << message << std::endl;\
throw std::runtime_error("");\
}

#define DX3DWarning(message)\
std::wclog << "DX3D Warning: " << message << std::endl;

#define DX3DInfo(message)\
std::wclog << "DX3D Info: " << message << std::endl;
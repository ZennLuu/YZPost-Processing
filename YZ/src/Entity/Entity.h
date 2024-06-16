#pragma once
#include <Prerequisites.h>
#include <map>

class Entity
{
public:
	Entity();
	virtual ~Entity();
	void release();

	World* getWorld();
	InputSystem* getInputSystem();
	TransformComponent* getTransform();

	template <typename T>
	T* createComponent()
 	{
		static_assert(std::is_base_of<Component, T>::value, "T must derive from Component class");
		auto c = getComponent<T>();
		if (!c)
		{
			auto id = typeid(T).hash_code();
			c = new T();
			createComponentInternal(c, id);
			return c;
		}
		return nullptr;
	}

	template <typename T>
	T* getComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "T must derive from Component class");
		auto id = typeid(T).hash_code();
		return (T*)getComponentInternal(id);
	}

protected:
	virtual void onCreate() {}
	virtual void onUpdate(f32 deltaTime) {}
	virtual void onCollision(Component* body1, Component* body2) {}
private:
	void createComponentInternal(Component* component, size_t id);
	Component* getComponentInternal(size_t id);
	void removeComponent(size_t id);
protected:
	size_t m_typeId = 0;
	World* m_world = nullptr;

	TransformComponent* m_transform = nullptr;
	std::map<size_t, std::unique_ptr<Component>> m_components;
	friend class World;
	friend class Component;
	friend class PhysicsEngine;
};
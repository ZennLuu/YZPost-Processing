#pragma once
#include <Prerequisites.h>
#include <Entity/Component.h>
#include <Math/Vector3D.h>
#include <vector>

class MeshComponent: public Component
{
public:
	MeshComponent();
	virtual ~MeshComponent();

	void setMesh(const MeshPtr& mesh);
	const MeshPtr& getMesh();

	void addMaterial(const MaterialPtr& material);
	void removeMaterial(ui32 index);

	const std::vector<MaterialPtr> & getMaterials();

	void setGravity(f32 g);
	f32 getGravity();

protected:
	virtual void onCreateInternal();
private:
	MeshPtr m_mesh;
	std::vector<MaterialPtr> m_materials;
	
	f32 m_gravity = 9.8;
};


#include <curitiba/geometry/quad.h>

#include <curitiba/math/vec3.h>
#include <curitiba/math/simpletransform.h>
#include <curitiba/geometry/mesh.h>
#include <curitiba/render/vertexdata.h>
#include <curitiba/material/materialgroup.h>
#include <curitiba.h>

using namespace curitiba::geometry;
using namespace curitiba::math;
using namespace curitiba::render;
using namespace curitiba::material;

Quad::Quad(void) : 
	SceneObject()
{
	Mesh *renderable = (Mesh *)RESOURCEMANAGER->createRenderable("Mesh");//new Mesh;
	std::vector<VertexData::Attr> *vertices = new std::vector<vec4>(4);
	std::vector<VertexData::Attr> *textureCoords = new std::vector<vec4>(4);
	std::vector<VertexData::Attr> *normals = new std::vector<vec4>(4);

	for (int i = 0; i < 4; ++i)
		normals->at(i).set(0.0f, 0.0f, 0.0f);

	vertices->at (0).set (-1.0f, -1.0f, -1.0f);
	vertices->at (1).set (1.0f, -1.0f, -1.0f);
	vertices->at (2).set (1.0f, 1.0f, -1.0f);
	vertices->at (3).set (-1.0f, 1.0f, -1.0f);

	textureCoords->at (0).set (0.0f, 0.0f, 0.0f);
	textureCoords->at (1).set (1.0f, 0.0f, 0.0f);
	textureCoords->at (2).set (1.0f, 1.0f, 0.0f);
	textureCoords->at (3).set (0.0f, 1.0f, 0.0f);

	VertexData &vertexData = renderable->getVertexData();

	vertexData.setDataFor (VertexData::getAttribIndex("position"), vertices);
	vertexData.setDataFor (VertexData::getAttribIndex("texCoord0"), textureCoords);
	vertexData.setDataFor (VertexData::getAttribIndex("normal"), normals);

	MaterialGroup *aMaterialGroup = new MaterialGroup();
	

	std::vector<unsigned int> *indices = new std::vector<unsigned int>(6);

	m_Transform = new SimpleTransform;

	indices->at (0) = 0;
	indices->at (1) = 1;
	indices->at (2) = 3;
	indices->at (3) = 1;
	indices->at (4) = 2;
	indices->at (5) = 3;

//	aMaterialGroup->setMaterialId (0);
	aMaterialGroup->setIndexList (indices);
	aMaterialGroup->setParent (renderable);
	aMaterialGroup->setMaterialName("__Quad");

	renderable->addMaterialGroup (aMaterialGroup);
	setRenderable (renderable);
}

Quad::~Quad(void)
{
	delete m_Renderable;
}

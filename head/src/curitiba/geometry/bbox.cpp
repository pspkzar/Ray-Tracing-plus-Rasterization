#include <curitiba/geometry/bbox.h>

#include <curitiba/math/vec3.h>
#include <curitiba/render/vertexdata.h>
#include <curitiba/material/materialgroup.h>

using namespace curitiba::geometry;
using namespace curitiba::math;
using namespace curitiba::render;
using namespace curitiba::material;


const std::string BBox::FloatParamNames[] = {"size"};

BBox::BBox(void) : Primitive(),
	m_Floats(COUNT_FLOATPARAMS)
{
	float n = 1.0f;

	std::vector<VertexData::Attr> *vertices = new std::vector<vec4>(8);
	std::vector<VertexData::Attr> *normals = new std::vector<vec4>(8);

	//BOTTOM
	vertices->at (0).set(-n, -n,  n);
	vertices->at (1).set( n, -n,  n);
	vertices->at (2).set( n, -n, -n);
	vertices->at (3).set(-n, -n, -n);

	//TOP
	vertices->at (4).set(-n,  n,  n);
	vertices->at (5).set( n,  n,  n);
	vertices->at (6).set( n,  n, -n);
	vertices->at (7).set(-n,  n, -n);

	VertexData &vertexData = getVertexData();
	vertexData.setDataFor (VertexData::getAttribIndex("position"), vertices);
	vertexData.setDataFor (VertexData::getAttribIndex("normal"), normals);

	//FRONT
	std::vector<unsigned int> *indices = new std::vector<unsigned int>(4);
	indices->at (0) = 0;		
	indices->at (1) = 1;
	indices->at (2) = 5;
	indices->at (3) = 4;

	MaterialGroup *aMaterialGroup = new MaterialGroup();
	aMaterialGroup->setIndexList (indices);
	aMaterialGroup->setParent (this);
	aMaterialGroup->setMaterialName("__Emission Blue");
	addMaterialGroup (aMaterialGroup);

	//LEFT
	indices = new std::vector<unsigned int>(4);
	indices->at (0) = 0;		
	indices->at (1) = 4;
	indices->at (2) = 7;
	indices->at (3) = 3;

	aMaterialGroup = new MaterialGroup();
	aMaterialGroup->setIndexList (indices);
	aMaterialGroup->setParent (this);
	aMaterialGroup->setMaterialName("__Emission Cyan");
	addMaterialGroup (aMaterialGroup);

	//BACK
	indices = new std::vector<unsigned int>(4);
	indices->at (0)= 2;		
	indices->at (1)= 3;
	indices->at (2)= 7;
	indices->at (3)= 6;

	aMaterialGroup = new MaterialGroup();
	aMaterialGroup->setIndexList (indices);
	aMaterialGroup->setParent (this);
	aMaterialGroup->setMaterialName("__Emission Yellow");
	addMaterialGroup (aMaterialGroup);

	//RIGHT
	indices = new std::vector<unsigned int>(4);
	indices->at (0)= 1;		
	indices->at (1)= 2;
	indices->at (2)= 6;
	indices->at (3)= 5;

	aMaterialGroup = new MaterialGroup();
	aMaterialGroup->setIndexList (indices);
	aMaterialGroup->setParent (this);
	aMaterialGroup->setMaterialName("__Emission Red");
	addMaterialGroup (aMaterialGroup);

	//TOP
	indices = new std::vector<unsigned int>(4);
	indices->at (0)= 4;		
	indices->at (1)= 5;
	indices->at (2)= 6;
	indices->at (3)= 7;

	aMaterialGroup = new MaterialGroup();
	aMaterialGroup->setIndexList (indices);
	aMaterialGroup->setParent (this);
	aMaterialGroup->setMaterialName("__Emission Green");
	addMaterialGroup (aMaterialGroup);

	//BOTTOM
	indices = new std::vector<unsigned int>(4);
	indices->at (0)= 0;		
	indices->at (1)= 1;
	indices->at (2)= 2;
	indices->at (3)= 3;

	aMaterialGroup = new MaterialGroup();
	aMaterialGroup->setIndexList (indices);
	aMaterialGroup->setParent (this);
	aMaterialGroup->setMaterialName("__Emission Purple");
	addMaterialGroup (aMaterialGroup);
}


BBox::~BBox(void)
{

}


void 
BBox::build()
{
}


const std::string &
BBox::getParamfName(unsigned int i) 
{
	if (i < BBox::COUNT_FLOATPARAMS)
		return BBox::FloatParamNames[i];
	else
		return Primitive::NoParam;
}


float 
BBox::getParamf(unsigned int param)
{
	assert(param < BBox::COUNT_FLOATPARAMS);

	if (param < BBox::COUNT_FLOATPARAMS)
		return(m_Floats[param]);
	else
		return (0.0f);
}


void
BBox::setParam(unsigned int param, float value)
{
	assert(param < BBox::COUNT_FLOATPARAMS);

	if (param < BBox::COUNT_FLOATPARAMS)
		m_Floats[param] = value;
}


unsigned int
BBox::translate(const std::string &name) 
{
	assert("name is not a primitive param");
	return (0);
}
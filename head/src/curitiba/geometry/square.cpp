#include <curitiba/geometry/square.h>

#include <curitiba/math/vec3.h>
#include <curitiba/render/vertexdata.h>
#include <curitiba/material/materialgroup.h>

using namespace curitiba::geometry;
using namespace curitiba::render;
using namespace curitiba::material;
using namespace curitiba::math;


const std::string Square::FloatParamNames[] = {""};

Square::Square(void) : Primitive(),
	m_Floats(COUNT_FLOATPARAMS)
{
	float n = 1.0f;

	std::vector<VertexData::Attr> *vertices = new std::vector<vec4>(4);
	std::vector<VertexData::Attr> *tangent = new std::vector<vec4>(4);
	std::vector<VertexData::Attr> *textureCoords = new std::vector<vec4>(4);
	std::vector<VertexData::Attr> *normals = new std::vector<vec4>(4);	

	//BOTTOM
	vertices->at (Square::TOP_LEFT).set		(-n, 0.0f,  n);
	vertices->at (Square::TOP_RIGHT).set		( n, 0.0f,  n);
	vertices->at (Square::BOTTOM_RIGHT).set	( n, 0.0f,  -n);
	vertices->at (Square::BOTTOM_LEFT).set	(-n, 0.0f,  -n);

	tangent->at (Square::TOP_LEFT).set		(0.0f,  0.0f, -1.0f);
	tangent->at (Square::TOP_RIGHT).set		(0.0f,  0.0f, -1.0f);
	tangent->at (Square::BOTTOM_RIGHT).set	(0.0f,  0.0f, -1.0f);
	tangent->at (Square::BOTTOM_LEFT).set	(0.0f,  0.0f, -1.0f);

	textureCoords->at (Square::TOP_LEFT).set		(0.0f, n,    0.0f);
	textureCoords->at (Square::TOP_RIGHT).set	(n,    n,    0.0f);
	textureCoords->at (Square::BOTTOM_RIGHT).set	(n,    0.0f, 0.0f);
	textureCoords->at (Square::BOTTOM_LEFT).set	(0.0f, 0.0f, 0.0f);

	normals->at (Square::TOP_LEFT).set		( 0.0f, 1.0f, 0.0f);
	normals->at (Square::TOP_RIGHT).set		( 0.0f, 1.0f, 0.0f);
	normals->at (Square::BOTTOM_RIGHT).set	( 0.0f, 1.0f, 0.0f);	
	normals->at (Square::BOTTOM_LEFT).set	( 0.0f, 1.0f, 0.0f); 	

	VertexData &vertexData = getVertexData();

	vertexData.setDataFor (VertexData::getAttribIndex("position"), vertices);
	vertexData.setDataFor (VertexData::getAttribIndex("texCoord0"), textureCoords);
	vertexData.setDataFor (VertexData::getAttribIndex("tangent"), tangent);
	vertexData.setDataFor (VertexData::getAttribIndex("normal"), normals);


	MaterialGroup *aMaterialGroup = new MaterialGroup();
	
	std::vector<unsigned int> *indices = new std::vector<unsigned int>(6);

	//BOTTOM
	indices->at (0)= Square::BOTTOM_LEFT;
	indices->at (1)= Square::TOP_LEFT;		
	indices->at (2)= Square::TOP_RIGHT;

	indices->at (3)= Square::BOTTOM_RIGHT;
	indices->at (4)= Square::BOTTOM_LEFT;
	indices->at (5)= Square::TOP_RIGHT;

	aMaterialGroup->setIndexList (indices);
	aMaterialGroup->setParent (this);
	aMaterialGroup->setMaterialName("__Light Grey");

	addMaterialGroup (aMaterialGroup);
}


Square::~Square(void)
{

}


void 
Square::build()
{
}


const std::string &
Square::getParamfName(unsigned int i) 
{
	if (i < Square::COUNT_FLOATPARAMS)
		return Square::FloatParamNames[i];
	else
		return Primitive::NoParam;
}


float 
Square::getParamf(unsigned int param)
{
	assert(param < Square::COUNT_FLOATPARAMS);

	if (param < Square::COUNT_FLOATPARAMS)
		return(m_Floats[param]);
	else
		return (0.0f);
}


void
Square::setParam(unsigned int param, float value)
{
	assert(param < Square::COUNT_FLOATPARAMS);

	if (param < Square::COUNT_FLOATPARAMS)
		m_Floats[param] = value;
}


unsigned int
Square::translate(const std::string &name) 
{
	assert("name is not a primitive param");
	return (0);
}
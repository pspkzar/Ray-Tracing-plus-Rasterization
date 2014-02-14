#include <curitiba/render/opengl/glstate.h>
#include <curitiba/config.h>

#include <GL/glew.h>

using namespace curitiba::render;

//
//Constructor & Destructor
//
GlState::GlState(): IState()   {

}


GlState::~GlState() {

}

//
// Set Full States
//
void
GlState::set() {

	std::map<unsigned int, int>::iterator iterInt;
	iterInt = m_MapIntProps.begin();

	for ( ; iterInt != m_MapIntProps.end(); ++iterInt) {
	
		switch(iterInt->first) {
			case ORDER: break;
		}
	}


	std::map<unsigned int, bool>::iterator iterBool;
	iterBool = m_MapEnableProps.begin();
	for ( ; iterBool != m_MapEnableProps.end(); ++iterBool) {
	
		switch(iterBool->first) {
			case FOG:	
						if (iterBool->second)
							glEnable(GL_FOG);
						else
							glDisable(GL_FOG);
					break;
			case ALPHA_TEST: 
						if (iterBool->second)
							glEnable(GL_ALPHA_TEST);
						else
							glDisable(GL_ALPHA_TEST);
					break;
			case BLEND: 
						if (iterBool->second)
							glEnable(GL_BLEND);
						else
							glDisable(GL_BLEND);
					break;
			case DEPTH_TEST:
						if (iterBool->second)
							glEnable(GL_DEPTH_TEST);
						else
							glDisable(GL_DEPTH_TEST);
					break;
			case DEPTH_MASK:
						glDepthMask(iterBool->second);
						break;
			case CULL_FACE: 
						if (iterBool->second)
							glEnable(GL_CULL_FACE);
						else
							glDisable(GL_CULL_FACE);
					break;
			case COLOR_MASK: 
					break;
		}
	}

	std::map<unsigned int, float>::iterator iterFloat;
	iterFloat = m_MapFloatProps.begin();
	for ( ; iterFloat != m_MapFloatProps.end(); ++iterFloat) {
	
		switch(iterFloat->first) {
#if CURITIBA_CORE_OPENGL  == 0
		case FOG_START: 
				glFogf(GL_FOG_START, iterFloat->second);
				break;
			case FOG_END: 
				glFogf(GL_FOG_END, iterFloat->second);
				break;
			case FOG_DENSITY: 
				glFogf(GL_FOG_DENSITY, iterFloat->second);
				break;
#endif
			case ALPHA_VALUE: 
				// it is the responsability of someone else to ensure that
				//both fields are defined
				glAlphaFunc(translate((Func)m_MapEnumProps[ALPHA_FUNC]),iterFloat->second);
				break;
		}
	}

	std::map<unsigned int, vec4>::iterator iterVec4;
	iterVec4 = m_MapFloat4Props.begin();
	for ( ; iterVec4 != m_MapFloat4Props.end(); ++iterVec4) {
	
		switch(iterVec4->first) {
			case FOG_COLOR: 
				glFogfv(GL_FOG_COLOR, toFloatPtr(iterVec4->second));
				break;
			case BLEND_COLOR: 
				glBlendColor(iterVec4->second.x,
					 iterVec4->second.y,
					  iterVec4->second.z,
					  iterVec4->second.w);;
				break;
		}
	}

	std::map<unsigned int, BoolArray>::iterator iterBool4;
	iterBool4 = m_MapBool4Props.begin();
	for ( ; iterBool4 != m_MapBool4Props.end(); ++iterBool4) {
	
		switch(iterBool4->first) {
			case COLOR_MASK:
				glColorMask(iterBool4->second.values[0],
					iterBool4->second.values[1],
					iterBool4->second.values[2],
					iterBool4->second.values[3]);
				break;
		}
	}

	iterInt = m_MapEnumProps.begin();
	for ( ; iterInt != m_MapEnumProps.end(); ++iterInt) {
	
		switch(iterInt->first) {
			case FOG_MODE: 
				glFogi(GL_FOG_MODE, translate((FogMode)iterInt->second));
				break;
			case FOG_COORD_SRC: 
				glFogi(GL_FOG_COORD_SRC,translate((FogCoordSrc)iterInt->second));
				break;
			case DEPTH_FUNC: 
				glDepthFunc(translate((Func)iterInt->second));
				break;
			case CULL_TYPE: 
				glCullFace(translate((CullType)iterInt->second));
				break;
			case ORDER_TYPE: 
				break;
			case BLEND_SRC: 
			case BLEND_DST:
				int s,d;
				if (!m_MapEnumProps.count(BLEND_SRC))
					s = GL_ONE;
				else
					s =  translate((BlendFunc)m_MapEnumProps[BLEND_SRC]);
				if (!m_MapEnumProps.count(BLEND_DST))
					d = GL_ZERO;
				else
					d =  translate((BlendFunc)m_MapEnumProps[BLEND_DST]);
				
				glBlendFunc(s,d);
				break;
			//case BLEND_DST: 
			//	break;
			case BLEND_EQUATION: 
				glBlendEquation( translate((BlendEquation)iterInt->second));
				break;
			// done for the float props
			//case ALPHA_FUNC: 
			//	break;
		}
	}


	
/*
#ifdef COMPATIBILITY_FLAG_OPENGL
	// FOG
	if (m_EnableProps[FOG]) {
		glEnable(GL_FOG);
		glFogfv(GL_FOG_COLOR, toFloatPtr(m_Float4Props[FOG_COLOR]));
		glFogi(GL_FOG_MODE, translate((FogMode)m_EnumProps[FOG_MODE]));
		glFogf(GL_FOG_DENSITY, m_FloatProps[FOG_DENSITY]);
		glFogf(GL_FOG_START, m_FloatProps[FOG_START]);
		glFogf(GL_FOG_END, m_FloatProps[FOG_END]);
		glFogi(GL_FOG_COORD_SRC,translate((FogCoordSrc)m_EnumProps[FOG_COORD_SRC]));
	}
	else
		glDisable(GL_FOG);

	// ALPHA TESTING
	if (m_EnableProps[ALPHA_TEST]) {
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(translate((Func)m_EnumProps[ALPHA_FUNC]),m_FloatProps[ALPHA_VALUE]);
	}
	else
		glDisable(GL_ALPHA_TEST);
#endif

	// BLEND
	if (m_EnableProps[BLEND]) {
		glEnable(GL_BLEND);
		glBlendColor( m_Float4Props[BLEND_COLOR].x,
					  m_Float4Props[BLEND_COLOR].y,
					  m_Float4Props[BLEND_COLOR].z,
					  m_Float4Props[BLEND_COLOR].w);
		glBlendEquation( translate((BlendEquation)m_EnumProps[BLEND_EQUATION]));
		glBlendFunc( translate((BlendFunc)m_EnumProps[BLEND_SRC]),
					 translate((BlendFunc)m_EnumProps[BLEND_DST]));
	}
	else
		glDisable(GL_BLEND);

	// CULL_FACE
	if (m_EnableProps[CULL_FACE]) {
		glEnable(GL_CULL_FACE);
		glCullFace(translate((CullType)m_EnumProps[CULL_TYPE]));
	}
	else
		glDisable(GL_CULL_FACE);

	// DEPTH TEST
	if (m_EnableProps[DEPTH_TEST]) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(translate((Func)m_EnumProps[DEPTH_FUNC]));
	}
	else
		glDisable(GL_DEPTH_TEST);

	// COLOR_MASK
	if (m_EnableProps[COLOR_MASK])
		glColorMask(m_Bool4Props[COLOR_MASK_B4].values[0],
					m_Bool4Props[COLOR_MASK_B4].values[1],
					m_Bool4Props[COLOR_MASK_B4].values[2],
					m_Bool4Props[COLOR_MASK_B4].values[3]);
	else
		glColorMask(true,true,true,true);
*/


}


void 
GlState::setDiff(IState *def, IState *aState) {

	GlState s;
	GlState *d = (GlState *)def;
	GlState *a = (GlState *)aState;

	std::map<unsigned int, int>::iterator iterInt;
	iterInt = m_MapIntProps.begin();
	for ( ; iterInt != m_MapIntProps.end(); ++iterInt) {
	
		if (a->m_MapIntProps.count(iterInt->first)) {
			if (a->m_MapIntProps[iterInt->first] != iterInt->second) {
				// add property to new state
				s.m_MapIntProps[iterInt->first] = a->m_MapIntProps[iterInt->first];
				// change current state accordingly
				m_MapIntProps[iterInt->first] = a->m_MapIntProps[iterInt->first];
			}
		}

		else if (d->m_MapIntProps.count(iterInt->first) && d->m_MapIntProps[iterInt->first] != iterInt->second) {

			s.m_MapIntProps[iterInt->first] = d->m_MapIntProps[iterInt->first];
			m_MapIntProps[iterInt->first] = d->m_MapIntProps[iterInt->first];
		}
	}

	std::map<unsigned int, bool>::iterator iterBool;
	iterBool = m_MapEnableProps.begin();
	for ( ; iterBool != m_MapEnableProps.end(); ++iterBool) {
	
		if (a->m_MapEnableProps.count(iterBool->first)) {
			if (a->m_MapEnableProps[iterBool->first] != iterBool->second) {
				// add property to new state
				s.m_MapEnableProps[iterBool->first] = a->m_MapEnableProps[iterBool->first];
				// change current state accordingly
				m_MapEnableProps[iterBool->first] = a->m_MapEnableProps[iterBool->first];
			}
		}

		else if (d->m_MapEnableProps.count(iterBool->first) && d->m_MapEnableProps[iterBool->first] != iterBool->second) {

			s.m_MapEnableProps[iterBool->first] = d->m_MapEnableProps[iterBool->first];
			m_MapEnableProps[iterBool->first] = d->m_MapEnableProps[iterBool->first];
		}
	}

	std::map<unsigned int, float>::iterator iterFloat;
	iterFloat = m_MapFloatProps.begin();
	for ( ; iterFloat != m_MapFloatProps.end(); ++iterFloat) {

		if (a->m_MapFloatProps.count(iterFloat->first)) {
			if (a->m_MapFloatProps[iterFloat->first] != iterFloat->second) {
				// add property to new state
				s.m_MapFloatProps[iterFloat->first] = a->m_MapFloatProps[iterFloat->first];
				// change current state accordingly
				m_MapFloatProps[iterFloat->first] = a->m_MapFloatProps[iterFloat->first];
			}
		}

		else if (d->m_MapFloatProps.count(iterFloat->first) && d->m_MapFloatProps[iterFloat->first] != iterFloat->second) {

			s.m_MapFloatProps[iterFloat->first] = d->m_MapFloatProps[iterFloat->first];
			m_MapFloatProps[iterFloat->first] = d->m_MapFloatProps[iterFloat->first];
		}
	}

	std::map<unsigned int, vec4>::iterator iterVec4;
	iterVec4 = m_MapFloat4Props.begin();
	for ( ; iterVec4 != m_MapFloat4Props.end(); ++iterVec4) {

		if (a->m_MapFloat4Props.count(iterVec4->first)) {
			if (a->m_MapFloat4Props[iterVec4->first] != iterVec4->second) {
				// add property to new state
				s.m_MapFloat4Props[iterVec4->first] = a->m_MapFloat4Props[iterVec4->first];
				// change current state accordingly
				m_MapFloat4Props[iterVec4->first] = a->m_MapFloat4Props[iterVec4->first];
			}
		}

		else if (d->m_MapFloat4Props.count(iterVec4->first) && d->m_MapFloat4Props[iterVec4->first] != iterVec4->second) {

			s.m_MapFloat4Props[iterVec4->first] = d->m_MapFloat4Props[iterVec4->first];
			m_MapFloat4Props[iterVec4->first] = d->m_MapFloat4Props[iterVec4->first];
		}
	}

	std::map<unsigned int, BoolArray>::iterator iterBool4;
	iterBool4 = m_MapBool4Props.begin();
	for ( ; iterBool4 != m_MapBool4Props.end(); ++iterBool4) {

		if (a->m_MapBool4Props.count(iterBool4->first)) {
			if (a->m_MapBool4Props[iterBool4->first] != iterBool4->second) {
				// add property to new state
				s.m_MapBool4Props[iterBool4->first] = a->m_MapBool4Props[iterBool4->first];
				// change current state accordingly
				m_MapBool4Props[iterBool4->first] = a->m_MapBool4Props[iterBool4->first];
			}
		}

		else if (d->m_MapBool4Props.count(iterBool4->first) && d->m_MapBool4Props[iterBool4->first] != iterBool4->second) {

			s.m_MapBool4Props[iterBool4->first] = d->m_MapBool4Props[iterBool4->first];
			m_MapBool4Props[iterBool4->first] = d->m_MapBool4Props[iterBool4->first];
		}
	}

	iterInt = m_MapEnumProps.begin();
	for ( ; iterInt != m_MapEnumProps.end(); ++iterInt) {
	
		if (a->m_MapEnumProps.count(iterInt->first)) {
			if (a->m_MapEnumProps[iterInt->first] != iterInt->second) {
				// add property to new state
				s.m_MapEnumProps[iterInt->first] = a->m_MapEnumProps[iterInt->first];
				// change current state accordingly
				m_MapEnumProps[iterInt->first] = a->m_MapEnumProps[iterInt->first];
			}
		}

		else if (d->m_MapEnumProps.count(iterInt->first) && d->m_MapEnumProps[iterInt->first] != iterInt->second) {

			s.m_MapEnumProps[iterInt->first] = d->m_MapEnumProps[iterInt->first];
			m_MapEnumProps[iterInt->first] = d->m_MapEnumProps[iterInt->first];
		}
	}

	s.set();


	//GlState *a = (GlState *)aState;

//#ifdef COMPATIBILITY_FLAG_OPENGL
//	// FOG
//	if (m_EnableProps[FOG] != a->m_EnableProps[FOG]) {
//
//		m_EnableProps[FOG] = a->m_EnableProps[FOG];
//	}
//	if (m_EnableProps[FOG]) {
//		glEnable(GL_FOG);
//
//		setProp(FOG_COLOR,a->getProp4f(FOG_COLOR));
//		glFogfv(GL_FOG_COLOR, toFloatPtr(a->m_Float4Props[FOG_COLOR]));
//
//		m_EnumProps[FOG_MODE] = a->m_EnumProps[FOG_MODE];
//		glFogi(GL_FOG_MODE, translate((FogMode)m_EnumProps[FOG_MODE]));
//
//		m_FloatProps[FOG_DENSITY] = a->m_FloatProps[FOG_DENSITY];
//		glFogf(GL_FOG_DENSITY, m_FloatProps[FOG_DENSITY]);
//
//		m_FloatProps[FOG_START] = a->m_FloatProps[FOG_START];
//		glFogf(GL_FOG_START, m_FloatProps[FOG_START]);
//
//		m_FloatProps[FOG_END] = a->m_FloatProps[FOG_END];
//		glFogf(GL_FOG_END, m_FloatProps[FOG_END]);
//
//		m_EnumProps[FOG_COORD_SRC] = a->m_EnumProps[FOG_COORD_SRC];
//		glFogi(GL_FOG_COORD_SRC, translate((FogCoordSrc)m_EnumProps[FOG_COORD_SRC]));
//	}
//	else
//		glDisable(GL_FOG);
//
//
//	
//	if (m_EnableProps[ALPHA_TEST] != a->m_EnableProps[ALPHA_TEST]) {
//		m_EnableProps[ALPHA_TEST] = a->m_EnableProps[ALPHA_TEST];
//	}
//	if (m_EnableProps[ALPHA_TEST]) {
//		glEnable(GL_ALPHA_TEST);
//		m_FloatProps[ALPHA_VALUE] = a->m_FloatProps[ALPHA_VALUE];
//		m_EnumProps[ALPHA_FUNC] = a->m_EnumProps[ALPHA_FUNC];
//		glAlphaFunc(translate((Func)m_EnumProps[ALPHA_FUNC]),m_FloatProps[ALPHA_VALUE]);
//	}
//	else
//		glDisable(GL_ALPHA_TEST);
//	
//
//#endif


	//if (m_EnableProps[BLEND] != a->m_EnableProps[BLEND]) {

	//	m_EnableProps[BLEND] = a->m_EnableProps[BLEND];
	//}
	//if (m_EnableProps[BLEND]) {
	//	glEnable(GL_BLEND);
	//	setProp(BLEND_COLOR,a->getProp4f(BLEND_COLOR));
	//	glBlendColor( m_Float4Props[BLEND_COLOR].x,
	//					  m_Float4Props[BLEND_COLOR].y,
	//					  m_Float4Props[BLEND_COLOR].z,
	//					  m_Float4Props[BLEND_COLOR].w);
	//	m_EnumProps[BLEND_EQUATION] = a->m_EnumProps[BLEND_EQUATION];
	//	glBlendEquation( translate((BlendEquation)m_EnumProps[BLEND_EQUATION]));
	//	m_EnumProps[BLEND_SRC] = a->m_EnumProps[BLEND_SRC];
	//	m_EnumProps[BLEND_DST] = a->m_EnumProps[BLEND_DST];
	//	glBlendFunc( translate((BlendFunc)m_EnumProps[BLEND_SRC]),
	//					 translate((BlendFunc)m_EnumProps[BLEND_DST]));
	//}
	//else
	//	glDisable(GL_BLEND);
	//
	//if (m_EnableProps[CULL_FACE] != a->m_EnableProps[CULL_FACE]) {
	//	m_EnableProps[CULL_FACE] = a->m_EnableProps[CULL_FACE];
	//}
	//if (m_EnableProps[CULL_FACE]) {
	//	glEnable(GL_CULL_FACE);
	//	m_EnumProps[CULL_TYPE] = a->m_EnumProps[CULL_TYPE];
	//	glCullFace(translate((CullType)m_EnumProps[CULL_TYPE]));
	//}
	//else
	//	glDisable(GL_CULL_FACE);

	//if (m_EnableProps[DEPTH_TEST] != a->m_EnableProps[DEPTH_TEST]) {
	//	m_EnableProps[DEPTH_TEST] = a->m_EnableProps[DEPTH_TEST];
	//}
	//if (m_EnableProps[DEPTH_TEST]) {
	//	glEnable(GL_DEPTH_TEST);
	//	m_EnumProps[DEPTH_FUNC] = a->m_EnumProps[DEPTH_FUNC];
	//	glDepthFunc(translate((Func)m_EnumProps[DEPTH_FUNC]));
	//}
	//else
	//	glDisable(GL_DEPTH_TEST);
	//

	//if (m_EnableProps[COLOR_MASK] != a->m_EnableProps[COLOR_MASK]) {
	//	m_EnableProps[COLOR_MASK] = a->m_EnableProps[COLOR_MASK];
	//}
	//if (m_EnableProps[COLOR_MASK]) {
	//	m_Bool4Props[COLOR_MASK_B4].set(a->getProp4b(COLOR_MASK_B4));
	//	glColorMask(m_Bool4Props[COLOR_MASK_B4].values[0],
	//				m_Bool4Props[COLOR_MASK_B4].values[1],
	//				m_Bool4Props[COLOR_MASK_B4].values[2],
	//				m_Bool4Props[COLOR_MASK_B4].values[3]);
	//}
}


//
// Get
//


//void 
//GlState::getCurrent() 
//{
//	GLint value; 
//	float values[4];
//
//#ifdef COMPATIBILITY_FLAG_OPENGL
//	// FOG - FOG_COORD_SRC is missing
//	glGetBooleanv(GL_FOG,(GLboolean *)&m_EnableProps[FOG]);
//
//
//	glGetFloatv(GL_FOG_COLOR,values);
//	m_Float4Props[FOG_COLOR].set(values);
//
//	glGetIntegerv(GL_FOG_MODE,&value);
//	m_EnumProps[FOG_MODE] = translateFogMode(value);
//	glGetFloatv(GL_FOG_DENSITY,&m_FloatProps[FOG_DENSITY]);
//	glGetFloatv(GL_FOG_START,&m_FloatProps[FOG_START]);
//	glGetFloatv(GL_FOG_END,&m_FloatProps[FOG_END]);
//	
//	// ALPHA TEST
//	m_EnableProps[ALPHA_TEST] = (GL_TRUE == glIsEnabled(GL_ALPHA_TEST));
//	glGetIntegerv(GL_ALPHA_TEST_FUNC, &value);
//	m_EnumProps[ALPHA_FUNC] = translateAlphaFunc(value);
//	glGetFloatv(GL_ALPHA_TEST_REF,&m_FloatProps[ALPHA_VALUE]);
//
//#endif
//
//	// BLEND
//	glGetBooleanv(GL_BLEND,(GLboolean *)&m_EnableProps[BLEND]);
//
//	glGetFloatv(GL_BLEND_COLOR,values);
//	m_Float4Props[BLEND_COLOR].set(values);
//
//	glGetIntegerv(GL_BLEND_EQUATION,&value);
//	m_EnumProps[BLEND_EQUATION] = translateBlendEquation(value);
//	glGetIntegerv(GL_BLEND_SRC, &value);
//	m_EnumProps[BLEND_SRC] = translateBlendFunc(value);
//	glGetIntegerv(GL_BLEND_DST,&value);
//	m_EnumProps[BLEND_DST] = translateBlendFunc(value);
//
//	// CULL FACE
//	glGetBooleanv(GL_CULL_FACE,(GLboolean *)&m_EnableProps[CULL_FACE]);
//	glGetIntegerv(GL_CULL_FACE_MODE,&value);
//	m_EnumProps[CULL_TYPE] = translateCullType(value);
//
//	// DEPTH
//	glGetBooleanv(GL_DEPTH_TEST, (GLboolean *)&m_EnableProps[DEPTH_TEST]);
//	glGetIntegerv(GL_DEPTH_FUNC, &value);
//	m_EnumProps[DEPTH_FUNC] = translateFunc(value);
//
//	// COLOR MASK
//	GLboolean b[4];
//	glGetBooleanv(GL_COLOR_WRITEMASK, b);
//
//	m_Bool4Props[COLOR_MASK_B4].set(b[0]!=0,b[1]!=0,b[2]!=0,b[3]!=0);
//	m_EnableProps[COLOR_MASK] = b[0] && b[1] && b[2] && b[3];
//}


// 
// Aux Functions
//


unsigned int 
GlState::translate(Func aFunc) {

	unsigned int res;

	switch(aFunc){
	case NEVER:
		res = GL_NEVER;
		break;
	case ALWAYS:
		res = GL_ALWAYS;
		break;
	case LESS:
		res = GL_LESS;
		break;
	case LEQUAL:
		res = GL_LEQUAL;
		break;
	case EQUAL:
		res = GL_EQUAL;
		break;
	case GEQUAL:
		res = GL_GEQUAL;
		break;
	case GREATER:
		res = GL_GREATER;
		break;
	case NOT_EQUAL:
		res = GL_NOTEQUAL;
		break;
	}
	return res;

}

IState::Func 
GlState::translateFunc(GLint aValue) {

	Func res;

	switch(aValue){
	case GL_NEVER:
		res = NEVER;
		break;
	case GL_ALWAYS:
		res = ALWAYS;
		break;
	case GL_LESS:
		res = LESS;
		break;
	case GL_LEQUAL:
		res = LEQUAL;
		break;
	case GL_EQUAL:
		res = EQUAL;
		break;
	case GL_GEQUAL:
		res = GEQUAL;
		break;
	case GL_GREATER:
		res = GREATER;
		break;
	case GL_NOTEQUAL:
		res = NOT_EQUAL;
		break;
	}
	return res;

}


unsigned int
GlState::translate(CullType aProp) {

	unsigned int res;

	switch(aProp){
	case FRONT_FACE:
		res = GL_FRONT;
		break;
	case BACK_FACE:
		res = GL_BACK;
		break;
	case FRONT_AND_BACK_FACE:
		res = GL_FRONT_AND_BACK;
		break;
	}
	return res;
}


IState::CullType
GlState::translateCullType(GLint aProp) {

	CullType res;

	switch(aProp){
	case GL_FRONT:
		res = FRONT_FACE;
		break;
	case GL_BACK:
		res = BACK_FACE;
		break;
	case GL_FRONT_AND_BACK:
		res = FRONT_AND_BACK_FACE;
		break;
	}
	return res;
}


unsigned int
GlState::translate(BlendFunc aProp) {

	unsigned int res;

	switch(aProp){
	case ZERO:
		res = GL_ZERO;
		break;
	case ONE:
		res = GL_ONE;
		break;
	case SRC_COLOR:
		res = GL_SRC_COLOR;
		break;
	case ONE_MINUS_SRC_COLOR:
		res = GL_ONE_MINUS_SRC_COLOR;
		break;
	case DST_COLOR:
		res = GL_DST_COLOR;
		break;
	case ONE_MINUS_DST_COLOR:
		res = GL_ONE_MINUS_DST_COLOR;
		break;
	case SRC_ALPHA:
		res = GL_SRC_ALPHA;
		break;
	case ONE_MINUS_SRC_ALPHA:
		res = GL_ONE_MINUS_SRC_ALPHA;
		break;
	case DST_ALPHA:
		res = GL_DST_ALPHA;
		break;
	case ONE_MINUS_DST_ALPHA:
		res = GL_ONE_MINUS_DST_ALPHA;
		break;
	case SRC_ALPHA_SATURATE:
		res = GL_SRC_ALPHA_SATURATE;
		break;
	case CONSTANT_COLOR:
		res = GL_CONSTANT_COLOR;
		break;
	case ONE_MINUS_CONSTANT_COLOR:
		res = GL_ONE_MINUS_CONSTANT_COLOR;
		break;
	case CONSTANT_ALPHA:
		res = GL_CONSTANT_ALPHA;
		break;
	case ONE_MINUS_CONSTANT_ALPHA:
		res = GL_ONE_MINUS_CONSTANT_ALPHA;
		break;
	}
	return res;
}


IState::BlendFunc
GlState::translateBlendFunc(GLint aProp) {

	BlendFunc res;

	switch(aProp){
	case GL_ZERO:
		res = ZERO;
		break;
	case GL_ONE:
		res = ONE;
		break;
	case GL_SRC_COLOR:
		res = SRC_COLOR;
		break;
	case GL_ONE_MINUS_SRC_COLOR:
		res = ONE_MINUS_SRC_COLOR;
		break;
	case GL_DST_COLOR :
		res = DST_COLOR;
		break;
	case GL_ONE_MINUS_DST_COLOR:
		res = ONE_MINUS_DST_COLOR;
		break;
	case GL_SRC_ALPHA:
		res = SRC_ALPHA;
		break;
	case GL_ONE_MINUS_SRC_ALPHA:
		res = ONE_MINUS_SRC_ALPHA;
		break;
	case GL_DST_ALPHA:
		res = DST_ALPHA;
		break;
	case GL_ONE_MINUS_DST_ALPHA:
		res = ONE_MINUS_DST_ALPHA;
		break;
	case GL_SRC_ALPHA_SATURATE:
		res = SRC_ALPHA_SATURATE;
		break;
	case GL_CONSTANT_COLOR:
		res = CONSTANT_COLOR;
		break;
	case GL_ONE_MINUS_CONSTANT_COLOR:
		res = ONE_MINUS_CONSTANT_COLOR;
		break;
	case GL_CONSTANT_ALPHA:
		res = CONSTANT_ALPHA;
		break;
	case GL_ONE_MINUS_CONSTANT_ALPHA:
		res = ONE_MINUS_CONSTANT_ALPHA;
		break;
	}
	return res;
}


unsigned int
GlState::translate(BlendEquation aProp) {

	unsigned int res;

	switch(aProp){
	case ADD:
		res = GL_FUNC_ADD;
		break;
	case SUBTRACT:
		res = GL_FUNC_SUBTRACT;
		break;
	case REVERSE_SUBTRACT:
		res = GL_FUNC_REVERSE_SUBTRACT;
		break;
	case MIN:
		res = GL_MIN;
		break;
	case MAX:
		res = GL_MAX;
		break;
	}
	return res;
}


IState::BlendEquation
GlState::translateBlendEquation(GLint aProp) {

	 BlendEquation res;

	switch(aProp){
	case GL_FUNC_ADD:
		res = ADD;
		break;
	case GL_FUNC_SUBTRACT:
		res = SUBTRACT;
		break;
	case GL_FUNC_REVERSE_SUBTRACT:
		res = REVERSE_SUBTRACT;
		break;
	case GL_MIN:
		res = MIN;
		break;
	case GL_MAX:
		res = MAX;
		break;
	}
	return res;
}


unsigned int
GlState::translate(FogMode aProp)  {

	unsigned int res;

	switch(aProp){
	case LINEAR:
		res = GL_LINEAR;
		break;
	case EXP:
		res = GL_EXP;
		break;
	case EXP2:
		res = GL_EXP2;
		break;
	}
	return res;
}


IState::FogMode
GlState::translateFogMode(GLint aProp)  {

	FogMode res;

	switch(aProp){
	case GL_LINEAR:
		res = LINEAR;
		break;
	case GL_EXP:
		res = EXP;
		break;
	case GL_EXP2:
		res = EXP2;
		break;
	}
	return res;
}


unsigned int
GlState::translate(FogCoordSrc aProp) {

	unsigned int res;

	switch(aProp){
	case FOG_COORD:
		res = GL_FOG_COORD;
		break;
	case FRAGMENT_DEPTH:
		res = GL_FRAGMENT_DEPTH;
		break;
	}
	return res;
}


IState::FogCoordSrc 
GlState::translateFogCoordSrc(GLint aProp) {

	FogCoordSrc  res;

	switch(aProp){
	case GL_FOG_COORD:
		res = FOG_COORD;
		break;
	case GL_FRAGMENT_DEPTH:
		res = FRAGMENT_DEPTH;
		break;
	}
	return res;
}


float *
GlState::toFloatPtr(vec4& v) {

	float *res = (float *)malloc(sizeof(float) * 4);

	res[0] = v.x;
	res[1] = v.y;
	res[2] = v.z;
	res[3] = v.w;

	return res;
}


bool 
GlState::difColor(vec4& a, vec4& b) {

	if (a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w) {
		a.x = b.x;
		a.y = b.y;
		a.z = b.z;
		a.w = b.w;
		return true;
	}
	else
		return false;
}

bool 
GlState::difBoolVector(bool* a, bool* b) {

	if (a[0] != b[0] || a[1] != b[1] || a[2] != b[2] || a[3] != b[3]) {
		a[0] = b[0];
		a[1] = b[1];
		a[2] = b[2];
		a[3] = b[3];
		return true;
	}
	else
		return false;
}


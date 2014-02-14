
#include <curitiba/render/opengl/glprogram.h>
#include <curitiba/render/vertexdata.h>

#include <curitiba/system/textfile.h>

using namespace curitiba::render;
using namespace curitiba::system;

// STATIC METHOD

void GlProgram::FixedFunction() {

	glUseProgram(0);
}


// CONSTRUCTORS

GlProgram::GlProgram() : 
	m_File(SHADER_COUNT,""),
	m_Source(SHADER_COUNT,""),
	m_ID(SHADER_COUNT,0),
	m_Compiled(SHADER_COUNT,false),
	m_NumUniforms (0), 
	m_MaxLength (0),
	m_VCompiled (false), 
	m_FCompiled (false),
	m_GCompiled (false),
	m_PLinked (false),
	m_ShowGlobalUniforms (false),
	m_Name("default")
{
	init();
}

GlProgram::GlProgram (const std::string &vf, const std::string &gf, const std::string &ff) :
	m_File(SHADER_COUNT,""),
	m_Source(SHADER_COUNT,""),
	m_ID(SHADER_COUNT,0),
	m_Compiled(SHADER_COUNT,false),
	m_NumUniforms (0),
	m_MaxLength (0),
	m_PLinked(false),
	m_ShowGlobalUniforms (false),
	m_Name("default")
{
	init();

	bool allCompiled = true;
	for (unsigned int i = 0; i < SHADER_COUNT; ++i) {
		if (true == setShaderFile((IProgram::SHADER_TYPE)i,m_File[i])) {
			m_Compiled[i] = compileShader((IProgram::SHADER_TYPE)i);
			allCompiled = allCompiled && m_Compiled[i];
		}
	}

	if (allCompiled) 
		m_PLinked = linkProgram();
}

GlProgram::~GlProgram()
{
	if (m_P != 0)
		glDeleteProgram(m_P);

	for (int i = 0; i < SHADER_COUNT; ++i) {
	
		if (m_ID[i] != 0)
			glDeleteShader(m_ID[i]);
	}

}

bool
GlProgram::areCompiled(){
	
	bool res = true;

	for (int i = 0; i < SHADER_COUNT; ++i) {
		if (m_File[i] != "") 
			res = res && m_Compiled[i];
	}
	return(res);
}


bool 
GlProgram::isCompiled(IProgram::SHADER_TYPE type){
	
	return m_Compiled[type];
}


bool 
GlProgram::isLinked() {

	return(m_PLinked);
}


void 
GlProgram::setName(const std::string &name)
{
	m_Name = name;
}


const std::string &
GlProgram::getName() 
{
	return(m_Name);
}


bool
GlProgram::loadShader (IProgram::SHADER_TYPE type, const std::string &filename)
{
	if (true == setShaderFile(type,filename)) {
		m_Compiled[type] = compileShader(type);
		return m_Compiled[type];
	}
	else
		return false;
}


const std::string &
GlProgram::getShaderFile(SHADER_TYPE type) 
{
	return m_File[type];
}


bool 
GlProgram::setValueOfUniform (const std::string &name, float *values)
{
	int i;

	i = findUniform (name);
 	
	if (-1 == i) {
		return false;
	}
	m_Uniforms[i].setValues (values);
	setValueOfUniform(i);

	return true;
}


bool 
GlProgram::setValueOfUniform (const std::string &name, int *values)
{
	int i;

	i = findUniform (name);
 	
	if (-1 == i) {
		return false;
	}
	m_Uniforms[i].setValues (values);
	setValueOfUniform(i);

	return true;
}


// PRIVATE METHODS


void 
GlProgram::init() {

	m_ID[IProgram::VERTEX_SHADER] = glCreateShader(GL_VERTEX_SHADER);
	m_ID[IProgram::GEOMETRY_SHADER] = glCreateShader(GL_GEOMETRY_SHADER);
	m_ID[IProgram::FRAGMENT_SHADER] = glCreateShader(GL_FRAGMENT_SHADER);

	m_P = glCreateProgram();

	for (int i = 0; i < SHADER_COUNT; ++i) {
		glAttachShader (m_P, m_ID[i]);
		m_Compiled[i] = false;
	}
	m_PLinked = false;
}

// INSTANCE METHODS

bool
GlProgram::setShaderFile (IProgram::SHADER_TYPE type, const std::string &filename)
{
	m_Compiled[type] = false;
	m_PLinked = false;

	if (m_File[type] != "" && m_File[type] == filename){
		return true;
	}
	m_Source[type] = curitiba::system::textFileRead(filename);
	
	if (m_Source[type] != "") { // if read successfuly

		// copy filename
		m_File[type] = filename;

		// set shader source
		const char * vv = m_Source[type].c_str();
		
		glShaderSource (m_ID[type], 1, &vv, NULL);
		return true;
	} 
	else 
		return false;
	
}


bool
GlProgram::reloadShaderFile (IProgram::SHADER_TYPE type)
{

	m_Compiled[type] = false;
	m_PLinked = false;
	m_Source[type] = textFileRead (m_File[type]);
	if (m_Source[type] != "") { // if read successfuly

		// set shader source
		const char * ff = (char *)m_Source[type].c_str();

		glShaderSource (m_ID[type], 1, &ff, NULL);
		return true;
	} 
	else 
		return false;
}


bool 
GlProgram::reload (void) 
{
	for (int i = 0; i < SHADER_COUNT; ++i) {
		reloadShaderFile((IProgram::SHADER_TYPE)i);
		m_Compiled[i] = compileShader((IProgram::SHADER_TYPE)i);
	}
	
	if (areCompiled()) {
		m_PLinked = linkProgram();
	}
	
	return m_PLinked;
}


int 
GlProgram::programValidate() {

	int v;

	glGetProgramiv(m_P,GL_VALIDATE_STATUS,&v);

	return(v);
}


bool
GlProgram::compileShader (IProgram::SHADER_TYPE type)
{
	int r;

	glDetachShader(m_P, m_ID[type]);
	glDeleteProgram(m_P);
	m_P = glCreateProgram();
	glCompileShader (m_ID[type]);
		
	glGetShaderiv (m_ID[type], GL_COMPILE_STATUS, &r);
	m_Compiled[type] = (1 == r);

	m_PLinked = false;

	return (m_Compiled[type]);
}


bool 
GlProgram::linkProgram() 
{
	int r;

	if (!areCompiled()) {
		return false;
	}

	unsigned int index;
	for (index = 0; index < VertexData::MaxAttribs; index++) {
		glBindAttribLocation(m_P, index , VertexData::Syntax[index].c_str());
	}

	for (int i = 0; i < SHADER_COUNT; ++i) {
		if (m_Compiled[i])
			glAttachShader(m_P, m_ID[i]);
		else
			glDetachShader(m_P,m_ID[i]);
	}
	glLinkProgram (m_P);
	glUseProgram (m_P);

	glGetProgramiv (m_P, GL_LINK_STATUS, &r);
	m_PLinked = (1 == r);

	glGetProgramiv (m_P, GL_ACTIVE_UNIFORMS, &m_NumUniforms);
	glGetProgramiv (m_P, GL_ACTIVE_UNIFORM_MAX_LENGTH, &m_MaxLength);

	setUniforms();

	glUseProgram(0);

	return (m_PLinked);
}


int
GlProgram::getNumberOfUniforms() 
{
	if (true == m_PLinked) {
		return (m_NumUniforms);
	} else {
		return (-1);
	}
}

int
GlProgram::getAttributeLocation (const std::string &name)
{
	return glGetAttribLocation (m_P, name.c_str());
}


void 
GlProgram::useProgram (void) 
{

	if (true == m_PLinked) {
		glUseProgram (m_P);
	} else {
		glUseProgram (0);
	}
}


void 
GlProgram::showGlobalUniforms (void)
{
	m_ShowGlobalUniforms = !m_ShowGlobalUniforms;
}

bool 
GlProgram::prepare (void) 
{
	useProgram();
	return true;
}

bool
GlProgram::restore (void)
{
	glUseProgram (0);
	return true;
}

int 
GlProgram::findUniform (const std::string &name)
{
	GlUniform uni;
	int i = 0;
	bool found (false);

	std::vector<GlUniform>::iterator it;
	for (it = m_Uniforms.begin(); it != m_Uniforms.end() && !found; it++) {
		//uni = *it;
		if ((*it).getName() == name) {
			found = true;
		} else {
			i++;
		}
	}

	if (true == found) {
		return (i);
	} else {
		return (-1);
	}
}


const GlUniform& 
GlProgram::getUniform(const std::string &name) {

	int i = findUniform (name);
	if (-1 == i) {
		i = 0;
	}
	return (m_Uniforms[i]);
}




void 
GlProgram::setValueOfUniform (int i) {

	GlUniform uni;
	uni = m_Uniforms[i];

	switch(uni.getType()) {
		case GL_FLOAT:
			glUniform1f (uni.getLoc(),uni.getValues()[0]);break;
		case GL_FLOAT_VEC2:
			glUniform2fv (uni.getLoc(),1,uni.getValues());break;
		case GL_FLOAT_VEC3:
			glUniform3fv (uni.getLoc(),1,uni.getValues());break;
		case GL_FLOAT_VEC4:
			glUniform4fv (uni.getLoc(),1,uni.getValues());break;
		case GL_INT:
		case GL_BOOL:
		case GL_SAMPLER_2D:
		case GL_SAMPLER_2D_SHADOW:
		case GL_SAMPLER_1D:
		case GL_SAMPLER_3D:
		case GL_SAMPLER_CUBE:
			glUniform1i(uni.getLoc(),(int)uni.getValues()[0]);break;

		case GL_INT_VEC2: case GL_BOOL_VEC2:
			glUniform2i(uni.getLoc(),(int)uni.getValues()[0],(int)uni.getValues()[1]);break;
		case GL_INT_VEC3:case GL_BOOL_VEC3:
			glUniform3i(uni.getLoc(),(int)uni.getValues()[0],(int)uni.getValues()[1],(int)uni.getValues()[2]);break;
		case GL_INT_VEC4:case GL_BOOL_VEC4:
			glUniform4i(uni.getLoc(),(int)uni.getValues()[0],(int)uni.getValues()[1],(int)uni.getValues()[2],(int)uni.getValues()[3]);break;

		case GL_FLOAT_MAT2:
			glUniformMatrix2fv(uni.getLoc(),1,false,uni.getValues());break;
		case GL_FLOAT_MAT3:
			glUniformMatrix3fv(uni.getLoc(),1,false,uni.getValues());break;
		case GL_FLOAT_MAT4:
			glUniformMatrix4fv(uni.getLoc(),1,false,uni.getValues());break;
	}
}



void 
GlProgram::setUniforms() {

	int i,index,len,size;
	
	unsigned int type;
	char *name = new char [m_MaxLength + 1]; 
	GlUniform uni;

	// set all types = NOT_USED
	std::vector<GlUniform>::iterator it;
	for(it = m_Uniforms.begin(); it != m_Uniforms.end(); it++) {
		it->setType(GlUniform::NOT_USED);
	}

	// add new uniforms and reset types for previous uniforms
	
	for (i = 0; i < m_NumUniforms; i++) {

		glGetActiveUniform (m_P, i, m_MaxLength, &len, &size, &type, name);
		std::string n (name);


		index = findUniform (n);
		if (-1 != index) {
			m_Uniforms[index].setType (type);
			m_Uniforms[index].setLoc (i);
		}
		else {
			uni.reset();
            std::string ProgName (name); 
			uni.setName (ProgName);
			uni.setType (type);
			uni.setLoc (i);
			m_Uniforms.push_back (uni);
		}

		if (size > 1) {

			for (int i = 0; i < size; i++) {
				std::stringstream s;

				s << n.c_str() << "[" << i << "]";
                                				
                std::string Location = s.str();                                 

				index = findUniform (Location);
				
				int loc;

				loc = glGetUniformLocation(m_P, s.str().c_str());

				if (-1 != index) {
					m_Uniforms[index].setType (type);
					m_Uniforms[index].setLoc (loc);
				}
				else {
					uni.reset();
                    std::string ProgName (s.str());
                    uni.setName (ProgName);
					uni.setType (type);
					uni.setLoc (loc);
					m_Uniforms.push_back (uni);
				}
			}
		}

	}

	// delete all uniforms where type is NOT_USED
	for(it = m_Uniforms.begin(), i = 0; it != m_Uniforms.end(); i++ ) {
		if (it->getType() == GlUniform::NOT_USED) {
			it = m_Uniforms.erase(it);
		} else {
			++it;
		}
	}
	m_NumUniforms = m_Uniforms.size();
	for (int i = 0; i < m_NumUniforms; i++) {
		setValueOfUniform (i);
	}
}

void 
GlProgram::updateUniforms() {

	glUseProgram (m_P);
	for (int i = 0; i < m_NumUniforms; i++) {
		glGetUniformfv (m_P, m_Uniforms[i].getLoc(), m_Uniforms[i].getValues());
	}

	glUseProgram(0);
}

const GlUniform& 
GlProgram::getUniform (int i) {

	if (i < m_NumUniforms) {
		return (m_Uniforms[i]);
	} else {
		return (m_Uniforms[0]);
	}
}


std::string  
GlProgram::getShaderInfoLog(SHADER_TYPE type) 
{
//	GLuint shader;
    int infologLength = 0;
    int charsWritten  = 0;
	std::string res;
	char *infoLog;

	switch(type) {
	
		case IProgram::VERTEX_SHADER:
				//shader = m_V;
				res = "Vertex Shader: OK";
			break;

		case IProgram::FRAGMENT_SHADER:
				//shader = m_F;
				res = "Fragment Shader: OK";
			break;

		case IProgram::GEOMETRY_SHADER:
				//shader = m_G;
				res = "Geometry Shader: OK";
			break;
	}
	glGetShaderiv (m_ID[type], GL_INFO_LOG_LENGTH, &infologLength);

    if (infologLength > 1) {
        infoLog = new char[infologLength]; //(char *)malloc(infologLength); /***MARK***/ //MEMORY LEACK!!!
        glGetShaderInfoLog (m_ID[type], infologLength, &charsWritten, infoLog);	
		res.assign(infoLog);
		free(infoLog);
	} 
    return (res);
}




char* 
GlProgram::getProgramInfoLog() 
{

    int infologLength = 0;
    int charsWritten  = 0;
	std::string ok = "Program: OK";
    char *infoLog;

	glGetProgramiv (m_P, GL_INFO_LOG_LENGTH, &infologLength);

    if (infologLength > 1) {
        infoLog = new char[infologLength]; //(char *)malloc(infologLength); /***MARK***/ //MEMORY LEACK!!!
        glGetProgramInfoLog (m_P, infologLength, &charsWritten, infoLog);
		return (infoLog);
	} else {
		infoLog = new char[ok.size()];
		strcpy(infoLog,(char *)ok.c_str());
	}
	return(infoLog);
}


int 
GlProgram::getNumberOfUserUniforms()
{
	int count = 0;

	if (true == m_PLinked) {
		for (int i = 0; i < m_NumUniforms; i++) {
			if (m_Uniforms[i].getName().substr(0,3) != "gl_" ) {
				count++;
			}
		}
	}
	return (count);
}


bool 
GlProgram::getPropertyb(int query) {

	int res;
	glGetProgramiv(m_P, query, &res);
	return (res != 0);
}


int 
GlProgram::getPropertyi(int query) {

	int res;
	glGetProgramiv(m_P, query, &res);
	return (res);
}
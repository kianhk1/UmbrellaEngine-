#include "Shader.h"

std::map<string, Shader*> ShaderManager::shaders;
Shader::Shader(const char* vs_path, const char* fs_path) {
	path_Vertex_Shader = vs_path;
	path_Fragment_Shader = fs_path;
}

unsigned int Shader::compileshader(unsigned int type, const char* path) {

	unsigned int id = glCreateShader(type);
	std::string source = Read_File(path);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	int success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	return id;
}

unsigned int Shader::createshader() {


	program = glCreateProgram();
	vs = compileshader(GL_VERTEX_SHADER, path_Vertex_Shader);
	fs = compileshader(GL_FRAGMENT_SHADER, path_Fragment_Shader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	glValidateProgram(program);
	glDeleteShader(vs);
	glDeleteShader(fs);

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	return program;
}

unsigned int Shader::GetProgram() {
	return(program);
}

void Shader::set_mat4(glm::mat4 setmat4, const GLchar* name) {
	//Logger::WARN(to_string(program));
	int loc = glGetUniformLocation(program, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(setmat4));
}

void Shader::set_vec3(glm::vec3 setvec3, const GLchar* name) {
	int loc = glGetUniformLocation(program, name);
	glUniform3fv(loc, 1, glm::value_ptr(setvec3));
}

void Shader::set_vec3(int x, int y, int z, const GLchar* name) {
	int loc = glGetUniformLocation(program, name);
	glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(x, y, z)));
}

void Shader::set_float(float setfloat, const GLchar* name) {
	int loc = glGetUniformLocation(program, name);
	glUniform1f(loc, setfloat);
}

void Shader::set_int(int setint, const GLchar* name) {
	int loc = glGetUniformLocation(program, name);
	//if (name == "texture") {
	//	Logger::WARN(loc == -1 ? "un" : "ss");
	//}
	glUniform1i(loc, setint);
}

string Shader::Read_File(const char* path) {
	ifstream file(path + string(".glsl"));
	stringstream buf;
	buf << file.rdbuf();
	string content = buf.str();
	if (content.size() >= 3 &&
		(unsigned char)content[0] == 0xEF &&
		(unsigned char)content[1] == 0xBB &&
		(unsigned char)content[2] == 0xBF) {
		content = content.substr(3);
	}
	return content;
}

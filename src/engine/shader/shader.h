#ifndef _ENGINE_SHADER_SHADER_H_
#define _ENGINE_SHADER_SHADER_H_

#include <GL/glew.h>
#include <GL/gl.h>

namespace engine {
namespace shader {

const char *type_to_string(GLenum type);

class Shader {
public:
	Shader(GLenum type, const char *source);
	~Shader();

	GLuint id;
	GLenum type;
};

} //namespace shader
} //namespace engine

#endif //_ENGINE_SHADER_SHADER_H_

#include <sb6.h>
#include <math.h>
#include <assert.h>

GLuint makeShaderProgram()
{
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint program;

	// Use gl_VertexID to index into a statically defined vert array in
	// the shader itself.
	static const GLchar vertexShaderSource[] = \
	"#version 430 core\n"
	"\
	void main(void) \
	{ \
	const vec4 vertices[3] = vec4[3](	vec4( 0.25,  -0.25, 0.5, 1.0), \
										vec4(-0.25,  -0.25, 0.5, 1.0), \
										vec4( 0.25,   0.25, 0.5, 1.0)); \
	gl_Position = vertices[gl_VertexID]; \
	}";
	
	static const GLchar fragmentShaderSource[] = \
	"#version 430 core\n"
	"\
	out vec4 color; \
	void main() \
	{ \
		color = vec4(0.0, 0.0, 1.0, 1.0); \
	}";

	const GLchar* vertexShaderSourceArray[1] = { vertexShaderSource };
	const GLchar* fragmentShaderSourceArray[1] = { fragmentShaderSource };

	// Compile vs
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, vertexShaderSourceArray, NULL);
	glCompileShader(vertexShader);

	// Compile fs
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, fragmentShaderSourceArray, NULL);
	glCompileShader(fragmentShader);

	// Compile/link shader program
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

class Ch2PointApp : public sb6::application
{
protected:
	GLuint program_;
	GLuint vertexArrayObject_;

public:
	void startup()
	{
		program_ = makeShaderProgram();
		assert(program_ && "invalid program");
		glGenVertexArrays(1, &vertexArrayObject_);
	}

	void shutdown()
	{
		glDeleteVertexArrays(1, &vertexArrayObject_);
		glDeleteProgram(program_);
	}

	void render(double currentTime)
	{
		static GLfloat bgColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, bgColor);

		glUseProgram(program_);
		glBindVertexArray(vertexArrayObject_);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
};

DECLARE_MAIN(Ch2PointApp);
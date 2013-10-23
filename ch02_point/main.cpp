#include <sb6.h>
#include <math.h>
#include <assert.h>

GLuint makeShaderProgram()
{
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint program;

	// Shader source strings.

	// Note that #version directive needs to be followed by an "\n" Linefeed/LF character
	// Trying to escape it with '\' in the string source may fail depending on
	// line endings (e.g. might use a CR+LF ('\r\n') on windows).

	static const GLchar vertexShaderSource[] = \
	"#version 430 core\n"	// see note above
	"\
	void main() \
	{ \
		gl_Position = vec4(0.0, 0.0, 0.5, 1.0); \
	}";
	
	static const GLchar fragmentShaderSource[] = \
	"#version 430 core\n"
	"\
	out vec4 color; \
	void main() \
	{ \
		color = vec4(0.0, 0.0, 1.0, 1.0); \
	}";

	// We actually pass in arrays of source
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

	// Delete shaders, program doesnt need them anymore to function.
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
		
		// Need to create VAO (which sends vs inputs) for binding, 
		// even though we dont make use of it in this example.
		glGenVertexArrays(1, &vertexArrayObject_);
	}

	void shutdown()
	{
		// Remove VAO and shader program

		// Unbinding currently bound VAO reverts its binding to
		// zero and the default vertex array becomes current
		glDeleteVertexArrays(1, &vertexArrayObject_);
		glDeleteProgram(program_);
	}

	void render(double currentTime)
	{
		// Background colour that changes over time
		static GLfloat color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		color[0] = (float)sin(currentTime) * 0.5f + 0.5f;	// r in [0,1]
		color[1] = (float)cos(currentTime) * 0.5f + 0.5f;	// g in [0,1]
		glClearBufferfv(GL_COLOR, 0, color);

		// Use the shader program.
		glUseProgram(program_);

		// Bind the VAO to the pipeline. 
		glBindVertexArray(vertexArrayObject_);

		// Make points a bit bigger, so easier to see a single point in this example.
		// OGL guarantees up to 64.0f point size supported.
		// Lets makes it fluctuate over time
		glPointSize(30.0f + 20.0f * sinf(currentTime));

		// Draw one point, with no specified verts
		// This works because the vert is hardcoded in the vertex shader itself.
		glDrawArrays(GL_POINTS, 0, 1);
	}
};

DECLARE_MAIN(Ch2PointApp);
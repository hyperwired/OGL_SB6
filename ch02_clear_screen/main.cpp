#include <sb6.h>
#include <math.h>

class Ch2ClearApp : public sb6::application
{
public:
	void render(double currentTime)
	{
		static const GLfloat blue[] = {0.0f, 0.0f, 1.0f, 1.0f};
		const GLfloat* clearColor = blue; 

		// for 5 seconds solid blue, next 5 seconds change color between r/g
		if (static_cast<int>(currentTime) % 10 < 5)
		{
			static GLfloat color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			color[0] = (float)sin(currentTime) * 0.5f + 0.5f;	// r in [0,1]
			color[1] = (float)cos(currentTime) * 0.5f + 0.5f;	// g in [0,1]
			clearColor = color;
		}
		glClearBufferfv(GL_COLOR, 0, clearColor);
	}
};

DECLARE_MAIN(Ch2ClearApp);

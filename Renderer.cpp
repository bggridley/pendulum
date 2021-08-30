#include "Renderer.h"
#include "math.h"
#include <iostream>

float Renderer::scale;
Renderer::Color Renderer::currentColor;

void Renderer::drawCircle(float x, float y, float radius, float count) {

	int segments = 360;

	glBegin(GL_LINES);
	//glVertex2f(radius, x);
	for (int i = 0; i < segments; i++) {
		float xx = cos(i * 3.14 / 180); // 1 by default
		float yy = sin(i * 3.14 / 180);

		float xxx = cos((i + 1) * 3.14 / 180); // 1 by default
		float yyy = sin((i + 1) * 3.14 / 180);

		//x + (xx * (width / scale / 2)), y + ((yy * height / scale / 2)
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2f(x + (xx * (radius)), y + (yy * (radius)));

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2f(x + (xxx * (radius)), y + (yyy * (radius)));

		//dstd::cout << y << std::endl;
	}

	//(radius, x);



	glEnd();

	count++;

	//std::cout << count << std::endl;

	if (count < 7) {
		drawCircle(x, y + radius, radius / 2, count);
		drawCircle(x, y - radius, radius / 2, count);
		drawCircle(x + radius, y, radius / 2, count);
		drawCircle(x - radius, y, radius / 2, count);
	}
}



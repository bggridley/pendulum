#include "DoublePendulum.h"



DoublePendulum::DoublePendulum(float x, float y) {
	this->startX = x;
	this->startY = y;

	//this->theta1 = angle1;
	//this->theta2 = angle2;

	//this->length1 = length1;
	//this->length2 = length2;
}

void DoublePendulum::randomize() {
	pointArray.clear();


	//std::random_device 
	std::default_random_engine generator(rd()); // rd() provides a random seed
	//std::cout << "seed" << rd() << std::endl;
	std::uniform_real_distribution<float> angle_d(2.69, 4.69);
	std::uniform_real_distribution<float> gravity_d(9.81, 9.81 * 3);
	std::uniform_int_distribution<int> length_d(800, 1000); // total needs to be 1850
	std::uniform_int_distribution<int> linewidth_d(15, 40);
	std::uniform_real_distribution<float> mass1_d(200, 300);
	std::uniform_real_distribution<float> mass2_d(10, 155);

	theta1 = angle_d(generator);
	theta2 = angle_d(generator);
	stheta1 = theta1;
	stheta2 = theta2;

	length1 = length_d(generator);
	length2 = 1850 - length1;

	g = gravity_d(generator);

	linewidth = linewidth_d(generator);
	mass1 = mass1_d(generator);
	mass2 = mass2_d(generator);
}

DoublePendulum::~DoublePendulum() {
	std::cout << "is this even called?" << std::endl;
	// nothing bruh
}

void DoublePendulum::update(float delta, bool push) {
	float num1 = -g * (2 * mass1 + mass2) * sin(theta1);
	float num2 = -mass2 * g * sin(theta1 - 2 * theta2);
	float num3 = -2 * sin(theta1 - theta2) * mass2;
	float num4 = theta2_v * theta2_v * length2 + theta1_v * theta1_v * length1 * cos(theta1 - theta2);
	float den = length1 * (2 * mass1 + mass2 - mass2 * cos(2 * theta1 - 2 * theta2));
	float a1_a = (num1 + num2 + num3 * num4) / den;

	num1 = 2 * sin(theta1 - theta2);
	num2 = (theta1_v * theta1_v * length1 * (mass1 + mass2));
	num3 = g * (mass1 + mass2) * cos(theta1);
	num4 = theta2_v * theta2_v * length2 * mass2 * cos(theta1 - theta2);
	den = length2 * (2 * mass1 + mass2 - mass2 * cos(2 * theta1 - 2 * theta2));
	float a2_a = (num1 * (num2 + num3 + num4)) / den;

	pa = a2_a;

	theta1_v += a1_a * delta;
	theta2_v += a2_a * delta;
	theta1 += theta1_v;
	theta2 += theta2_v;

	float c1[] = { abs(pa * 30), abs(pv * 30), 1 - abs(pa) * 30, 1.0f };
	float c2[] = { .0f, .0f, .0f, 1.0f };

	float x = length1 * sin(theta1);
	float y = length1 * cos(theta1);

	float x2 = length2 * sin(theta2);
	float y2 = length2 * cos(theta2);

	if (px != 0.0f && py != 0.0f) {



		float perpX = (startY + y + y2 - py);
		float perpY = -(startX + x + x2 - px);


		float length = sqrt(perpX * perpX + perpY * perpY);

		perpX /= length;
		perpY /= length;

		//std::cout << "perp:" << perpX << std::endl;
		if (push) {
			pointArray.push_back(startX + x + x2 - perpX * linewidth / 2);
			pointArray.push_back(startY + y + y2 - perpY * linewidth / 2);
			pointArray.push_back(((((startX + x + x2) * 1.5) - px / 2)) + perpX * linewidth / 2);
			pointArray.push_back(((((startY + y + y2) * 1.5) - py / 2)) + perpY * linewidth / 2);
			pointArray.push_back(c1[0]);
			pointArray.push_back(c1[1]);
			pointArray.push_back(c1[2]);
		}
	}
	else {
		
	}

	px = startX + x + x2;
	py = startY + y + y2;

	pv = theta2_v;
}

void DoublePendulum::render(float alpha) {
	glBegin(GL_TRIANGLE_STRIP);

	for (int i = 0; i < pointArray.size(); i += 7) {



		float c[] = { pointArray[i + 4], pointArray[i + 5], pointArray[i + 6] };
		//if (pointArray.size() > 20) {
		//	renderer.drawBezier(pointArray[i - 15], pointArray[i - 14], pointArray[i - 10], pointArray[i - 9], pointArray[i - 5], pointArray[i - 4], pointArray[i], pointArray[i + 1], c);
		//}
		//float cc[] = { pointArray[i + 2], pointArray[i + 3], pointArray[i + 4] };
		//renderer.drawLine(pointArray[i - 5], pointArray[i - 4], pointArray[i], pointArray[i + 1], c, cc);

		//glPointSize(10.0f);
		///renderer.drawPoint(pointArray[i], pointArray[i + 1], c[0], c[1], c[2]);
	//	renderer.drawPoint(pointArray[i + 2], pointArray[i + 3], c[0], c[1], c[2]);
		float a = .95f;
		if (pointArray.size() - i <= 180 * 7) {
			a = .95f / (180.0f * 7.0f / float(pointArray.size() - i));
			//std::cout << a << std::endl;
		}
		//::cout << pointArray[i] << "," << pointArray[i + 1] << std::endl;
		glColor4f(c[0], c[1], c[2], a);
		glVertex2f(pointArray[i], pointArray[i + 1]);
		glColor4f(c[0], c[1], c[2], a);
		glVertex2f(pointArray[i + 2], pointArray[i + 3]);
	}



	glEnd();
}
#pragma once
#include <vector>
#include <glew.h>
#include <iostream>
#include <random>

class DoublePendulum {
public:
	DoublePendulum(float x, float y);
	~DoublePendulum();
	void randomize();
	void update(float delta, bool push);
	void render(float alpha);
	float theta1, theta2, startX, startY, length1, length2;
	float stheta1, stheta2;
	float px = 0.0f;
	float py = 0.0f;
	float pa = 0.0f;
	float pv = 0.0f;
	float theta1_v = 0.0f; // angular velocity
	float theta2_v = 0.0f;
	float mass1 = 200;
	float mass2 = 10;
	float g = 19.81f;
	float linewidth = 20;
	std::vector<float> pointArray;

	std::random_device rd;
	
};
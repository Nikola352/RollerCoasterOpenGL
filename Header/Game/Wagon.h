#pragma once

class Wagon {
private:
	float xPos;
	float yPos;
	float rotTan;
public:
	Wagon(float x, float y);

	float getXPos() const;

	float getYPos() const;

	float getRotationTangent() const;

	float getRotationAngle() const;

	void setXPos(float x);

	void setYPos(float y);

	void setRotationTangent(float tan);
};
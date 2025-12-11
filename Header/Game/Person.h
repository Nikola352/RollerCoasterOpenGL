#pragma once

class Person {
private:
	float xPos;
	float yPos;
	float rotTan;

	bool isSick;
	bool hasSeatbelt;

public:
	Person();
	Person(float x, float y);

	float getXPos() const;
	float getYPos() const;
	float getRotationTangent() const;
	float getRotationAngle() const;
	bool getIsSick() const;
	bool getHasSeatbelt() const;

	void setXPos(float x);
	void setYPos(float y);
	void setRotationTangent(float tan);
	void setIsSick(bool sick);
	void setHasSeatbelt(bool seatbelt);
};
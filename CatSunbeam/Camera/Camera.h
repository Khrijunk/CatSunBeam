#ifndef CAMERA_H
#define CAMERA_H

#include "../Includes.h"

class Camera
{

public:
	Camera(LPDIRECT3DDEVICE9 d3ddev);
	void SetCamera();
	void AddToXPos(float amount);
	void AddToYPos(float amount);
	void AddToZPos(float amount);
	void ChangeDirection(DIMOUSESTATE mousestate);
	float GetAngle();

private:
	LPDIRECT3DDEVICE9 d3ddev;
	

	float newYAngle;
	float xPosition;
	float zPosition;
	float yPosition;
	float xDirection;
	float yDirection;
	float zDirection;
	float angle;
	float yAngle;
	float speed;
};

#endif
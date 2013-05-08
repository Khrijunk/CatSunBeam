#include "Camera.h"

Camera::Camera(LPDIRECT3DDEVICE9 device)
{
	d3ddev = device;

	newYAngle = 0.0f;
	xPosition = 0.0f;
	zPosition = -15.0f;
	yPosition = 0.0f;
	xDirection = 0.0f;
	yDirection = 0.0f;
	zDirection = 0.0f;
	angle = 0.0f;
	yAngle = 90.0f;
	speed = 4.0f;
}

void Camera::SetCamera()
{
	D3DXMATRIX	 matView;

   D3DXMatrixLookAtLH(&matView,
						&D3DXVECTOR3(xPosition, yPosition, zPosition),
						&D3DXVECTOR3(xDirection, yDirection, zDirection),
						&D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	d3ddev->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProjection;

	D3DXMatrixPerspectiveFovLH(&matProjection, D3DXToRadian(45), (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, 1.0f, 100.0f);
								   
	d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);
}

void Camera::AddToXPos(float amount) { xPosition += amount; }
void Camera::AddToYPos(float amount) { yPosition += amount; }
void Camera::AddToZPos(float amount) { zPosition += amount; }

float Camera::GetAngle() { return angle; }

void Camera::ChangeDirection(DIMOUSESTATE mousestate)
{
	newYAngle = 0.0f;
	angle += mousestate.lX/speed;

			newYAngle = yAngle + mousestate.lY/4;
			if(newYAngle < 130 &&  newYAngle > 50)
				yAngle += mousestate.lY/speed;
			yDirection = 15*cos(D3DXToRadian(yAngle));
		//}

		//determining the direction of x and z
		xDirection = xPosition + 15*sin(D3DXToRadian(angle));
		zDirection = zPosition + 15*cos(D3DXToRadian(angle));
}
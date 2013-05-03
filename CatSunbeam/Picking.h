#ifndef PICKING_H
#define PICKING_H

#include "Includes.h"

class Picking
{
public:
	Picking(LPDIRECT3DDEVICE9 device);
	Ray CalcPickingRay(int x, int y);
	bool PickingTest(Ray* ray, BoundingSphere* sphere);

private:
	void TransformRay(Ray* ray, D3DXMATRIX* T);

	LPDIRECT3DDEVICE9 d3ddev;

};

#endif
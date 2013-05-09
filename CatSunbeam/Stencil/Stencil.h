#ifndef STENCIL_H
#define STENCIL_H

#include "../includes.h"
#include "../AnimatedModel.h"
#include "../Model/Model.h"
#include "../Helper.h"


class Stencil
{
public:
	Stencil(LPDIRECT3DDEVICE9 device);
	void RenderReflection(AnimatedModel* model, Model* plane, Helper* helper);
	void RenderShadow(AnimatedModel* model, D3DXVECTOR3 plane, D3DXVECTOR3 lightDir, Helper* helper);

private:
	LPDIRECT3DDEVICE9 d3ddev;
};

#endif

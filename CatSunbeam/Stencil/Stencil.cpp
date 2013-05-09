#include "Stencil.h"

Stencil::Stencil(LPDIRECT3DDEVICE9 device)
{
	d3ddev = device;
}


void Stencil::RenderReflection(AnimatedModel* model, Model* plane, Helper* helper)
{
	//start up the stencil buffer
	d3ddev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	d3ddev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	d3ddev->SetRenderState(D3DRS_STENCILREF, 0X1);
	d3ddev->SetRenderState(D3DRS_STENCILMASK, 0Xffffffff);
	d3ddev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	d3ddev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	d3ddev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	d3ddev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	
	//d3ddev->SetRenderState(D3DRS_STENCILFAIL,D3DSTENCILOP_INCRSAT);
	//d3ddev->SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_INCRSAT);
	//d3ddev->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_ALWAYS);
	

	//disable writes to the depth and back buffers
	d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
	d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//draw the floor to the stencil buffer
	plane->Render(helper->GetTime(), 0);

	d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, true);
	//d3ddev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	//d3ddev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	d3ddev->SetRenderState(D3DRS_STENCILZFAIL,D3DSTENCILOP_KEEP);
	d3ddev->SetRenderState(D3DRS_STENCILFAIL,D3DSTENCILOP_KEEP);
	d3ddev->SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_KEEP);
	d3ddev->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_LESSEQUAL);
	d3ddev->SetRenderState(D3DRS_STENCILREF,0);

	//position reflectance
	D3DXMATRIX W, T, R;
	D3DXPLANE mirror(0.0f, 1.0f, 0.0f, 0.0f);
	D3DXMatrixReflect(&R, &mirror);

	D3DXMatrixTranslation(&T, model->getPosition().x, model->getPosition().y, model->getPosition().z);

	//W = model->getCatRotation() * (T * R);
	W = T * R;

	d3ddev->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
	d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	d3ddev->SetTransform(D3DTS_WORLD, &W);
	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	model->Render(helper->GetTime(), true);

	//comment this line out to make a cool effect
	d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	d3ddev->SetRenderState(D3DRS_STENCILENABLE, false);
	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


void Stencil::RenderShadow(AnimatedModel* model, D3DXVECTOR3 plane, D3DXVECTOR3 lightDir, Helper* helper)
{
	d3ddev->SetRenderState(D3DRS_STENCILENABLE, true);
	d3ddev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	d3ddev->SetRenderState(D3DRS_STENCILREF, 0X0);
	d3ddev->SetRenderState(D3DRS_STENCILMASK, 0Xffffffff);
	d3ddev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	d3ddev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	d3ddev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	d3ddev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);

	D3DXVECTOR4 lightDirection(lightDir);
	D3DXPLANE groundPlane(plane);

	D3DXMATRIX S;
	D3DXMatrixShadow(&S, &lightDirection, &groundPlane);

	D3DXMATRIX T;
	D3DXMatrixTranslation(&T, model->getPosition().x, model->getPosition().y, model->getPosition().z);

	D3DXMATRIX W = T * S;

	d3ddev->SetTransform(D3DTS_WORLD, &W);

	d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	D3DMATERIAL9 mtrl;

	ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	mtrl.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	mtrl.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	mtrl.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	mtrl.Power = 0.0f;

	d3ddev->SetRenderState(D3DRS_ZENABLE, FALSE);
	d3ddev->SetMaterial(&mtrl);
	d3ddev->SetTexture(0,0);
	model->Render(helper->GetTime(), false);

	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);

	d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	d3ddev->SetRenderState(D3DRS_STENCILENABLE, false);
}

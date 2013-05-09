#pragma once

#ifndef MESH_H
#define MESH_H

#include "Includes.h"
#include "MeshAllocation.h"

class AnimatedModel
{
  public:
    LPD3DXMESH *_mesh;    // define the mesh pointer
    D3DMATERIAL9* material;    // define the material object
    LPDIRECT3DTEXTURE9* texture;    // a pointer to a texture
    DWORD numMaterials;    // stores the number of materials in the mesh
    LPD3DXFRAME TopFrame;    // the top-most frame of the model
    LPD3DXANIMATIONCONTROLLER AnimationController;    // an object that handles animation
    D3DXMATRIX* FinalMatrices;    // a buffer for the matrices in their final state
    int MaxFrames;    // the highest number of frames a mesh container can be affected by
	D3DXVECTOR3 getPosition();

    AnimatedModel(LPDIRECT3D9 graphics, LPDIRECT3DDEVICE9 device, LPCSTR filePath);
    ~AnimatedModel();
    LPDIRECT3D9 d3d; // the pointer to our Direct3D interface
	LPDIRECT3DDEVICE9 d3ddev; // the pointer to the device class
    void draw_mesh(CUSTOM_FRAME* pFrame);
	void draw_shadow_mesh(CUSTOM_FRAME* pFrame);
    void update_mesh_containers(CUSTOM_FRAME* pFrame, double long time);
    void update_frames(CUSTOM_FRAME* pFrame, D3DXMATRIX* pParentMatrix);
    void link_frames(CUSTOM_FRAME* pFrame);
    void Render(float deltaTime, int modelReference);
	void Render(float deltaTime, bool reflection);
	D3DXVECTOR3 pos;
};

#endif // !Mesh_H
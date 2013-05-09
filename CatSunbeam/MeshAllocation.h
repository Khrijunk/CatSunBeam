#pragma once

#ifndef MESHALLOCATOR_H
#define MESHALLOCATOR_H

#include "Includes.h"


struct CUSTOM_FRAME : public D3DXFRAME
{
  D3DXMATRIX CombTransformationMatrix;    // transform data multiplied by all parents' data
};

struct CUSTOM_MESHCONTAINER : public D3DXMESHCONTAINER
{
  D3DXMATRIX** ppFrameMatrices;    // an array of matrices to store current animation data
  LPD3DXMESH pFinalMesh;    // a duplicate of the mesh, representing it's current pose
  LPDIRECT3DTEXTURE9* pTextures;    // an array of applicable textures
};

class MeshAllocation : public ID3DXAllocateHierarchy
{
public:
  D3DXMATRIX* FinalMatrices;    // a buffer for the matrices in their final state
  int MaxFrames;    // the highest number of frames a mesh container can be affected by
  LPDIRECT3DDEVICE9 d3ddev;

  virtual __declspec(nothrow) HRESULT __stdcall CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame)
  {
    // Create and initialize the custom frame struct
    CUSTOM_FRAME* pFrame = new CUSTOM_FRAME;
    *ppNewFrame = pFrame;
    ZeroMemory(pFrame, sizeof(CUSTOM_FRAME));
    // Copy the name
    if(Name)
    {
      UINT len = strlen(Name) + 1;
      pFrame->Name = new char[len];
      memcpy(pFrame->Name, Name, len);
    }
    return S_OK;
  };

  virtual __declspec(nothrow) HRESULT __stdcall CreateMeshContainer(LPCSTR Name,
    CONST D3DXMESHDATA* pMeshData,
    CONST D3DXMATERIAL* pMaterials,
    CONST D3DXEFFECTINSTANCE* pEffectInstances,
    DWORD NumMaterials,
    CONST DWORD* pAdjacency,
    LPD3DXSKININFO pSkinInfo,
    LPD3DXMESHCONTAINER* ppNewMeshContainer)
  {
    // Create and initialize the custom mesh container
    struct CUSTOM_MESHCONTAINER *pMeshContainer = new CUSTOM_MESHCONTAINER;
    *ppNewMeshContainer = pMeshContainer;
    ZeroMemory(pMeshContainer, sizeof(CUSTOM_MESHCONTAINER));
    // Check to make sure its a normal mesh
    if(pMeshData->Type != D3DXMESHTYPE_MESH)
    {
      DestroyMeshContainer(pMeshContainer);
      return E_FAIL;
    }
    // Copy the name
    if(Name)
    {
      UINT len = strlen(Name) + 1;
      pMeshContainer->Name = new char[len];
      memcpy(pMeshContainer->Name, Name, len);
    }
    // Copy the mesh data
    pMeshContainer->MeshData.Type = pMeshData->Type;
    pMeshContainer->MeshData.pMesh = pMeshData->pMesh;
    pMeshContainer->MeshData.pMesh->AddRef();
    // Copy the materials
    pMeshContainer->pMaterials = new D3DXMATERIAL[NumMaterials];
    for(DWORD i = 0; i < NumMaterials; i++)
    {
      pMeshContainer->pMaterials[i] = pMaterials[i];
      pMeshContainer->pMaterials[i].MatD3D.Ambient = pMaterials[i].MatD3D.Diffuse;
    }
    // Don't bother with the effect instance
    pMeshContainer->pEffects = NULL;
    // Copy the number of materials
    pMeshContainer->NumMaterials = max(1, NumMaterials);
    // Copy the adjacency data
    pMeshContainer->pAdjacency = new DWORD[pMeshData->pMesh->GetNumFaces() * 3];
    memcpy(pMeshContainer->pAdjacency,
      pAdjacency,
      sizeof(DWORD) * pMeshData->pMesh->GetNumFaces() * 3);
    // Only do these steps if there is skin info
    if(pSkinInfo)
    {
      // Copy the skin data
      pMeshContainer->pSkinInfo = pSkinInfo;
      pSkinInfo->AddRef();

      // Allocate memory for the pointers to the frame matrices
      pMeshContainer->ppFrameMatrices = new D3DXMATRIX*[pSkinInfo->GetNumBones()];
      for(DWORD i = 0; i < pSkinInfo->GetNumBones(); i++)
      {
        pMeshContainer->ppFrameMatrices[i] = NULL;
      }
    }
    // Create a duplicate of the mesh
    pMeshContainer->MeshData.pMesh->CloneMesh(D3DXMESH_MANAGED,
      NULL,
      d3ddev,
      &pMeshContainer->pFinalMesh);
    // Load the textures for each material (if any)
    pMeshContainer->pTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
    for(DWORD i = 0; i < NumMaterials; i++)
    {
      pMeshContainer->pTextures[i] = 0;
      if(pMaterials[i].pTextureFilename)
      {
        D3DXCreateTextureFromFileA(d3ddev,
          pMeshContainer->pMaterials[i].pTextureFilename,
          &pMeshContainer->pTextures[i]);
      }
    }
    // Find the maximum number of frames
    if(pMeshContainer->pSkinInfo)
      MaxFrames = max(MaxFrames, (int)pMeshContainer->pSkinInfo->GetNumBones());
    return S_OK;
  };

  virtual __declspec(nothrow) HRESULT __stdcall DestroyFrame(LPD3DXFRAME pFrameToFree)
  {
    // free the name and the frame
    if(pFrameToFree->Name)
    {
      delete[](pFrameToFree->Name);
      pFrameToFree->Name = NULL;
    }
    if(pFrameToFree)
    { 
      delete(pFrameToFree);
      pFrameToFree=NULL;
    }
    return S_OK;
  };

  virtual __declspec(nothrow) HRESULT __stdcall DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
  {
    // cast the pointer to a CUSTOM_MESHCONTAINER
    CUSTOM_MESHCONTAINER* pMeshContainer = (CUSTOM_MESHCONTAINER*)pMeshContainerToFree;
    // free or release all the allocated memory
    if(pMeshContainer->Name)
    {
      delete[](pMeshContainer->Name);
      pMeshContainer->Name = NULL;
    }
    if(pMeshContainer->MeshData.pMesh)
    {
      pMeshContainer->MeshData.pMesh->Release();
      pMeshContainer->MeshData.pMesh = NULL;
    }
    if(pMeshContainer->MeshData.pMesh)
    {
      (pMeshContainer->pFinalMesh)->Release();
      pMeshContainer->pFinalMesh = NULL;
    }
    if(pMeshContainer->pMaterials)
    {
      delete[](pMeshContainer->pMaterials);
      pMeshContainer->pMaterials = NULL;
    }
    if(pMeshContainer->pAdjacency)
    {
      delete[](pMeshContainer->pAdjacency);
      pMeshContainer->pAdjacency = NULL;
    }
    if(pMeshContainer->pSkinInfo)
    {
      (pMeshContainer->pSkinInfo)->Release();
      pMeshContainer->pSkinInfo = NULL;
    }
    if(pMeshContainer->ppFrameMatrices)
    {
      delete[](pMeshContainer->ppFrameMatrices);
      pMeshContainer->ppFrameMatrices = NULL;
    }
    for(UINT i = 0; i < pMeshContainer->NumMaterials; i++)
      if(pMeshContainer->pSkinInfo)
      {
        (pMeshContainer->pSkinInfo)->Release();
        pMeshContainer->pSkinInfo = NULL;
      }
      if(pMeshContainer)
      {
        delete(pMeshContainer);
        pMeshContainer=NULL;
      }
      return S_OK;
  };
};

#endif // !MeshAllocator_H
#include "AnimatedModel.h"

AnimatedModel::AnimatedModel(LPDIRECT3D9 graphics, LPDIRECT3DDEVICE9 device, LPCSTR filePath)
{
  d3d = graphics;
  d3ddev = device;
  pos = D3DXVECTOR3(0.0f, 0.1f, 5.0f);

  MeshAllocation MemAllocator;    // create a mesh allocation class

  MemAllocator.d3ddev = device;

    // load the mesh and call the allocation functions
     HRESULT result = D3DXLoadMeshHierarchyFromX(filePath,
                               D3DXMESH_MANAGED,
                               d3ddev,
                               &MemAllocator,
                               NULL,
                               &TopFrame,
                               &AnimationController);

    FinalMatrices = MemAllocator.FinalMatrices;
    MaxFrames = MemAllocator.MaxFrames;

    // allocate memory for the FinalMatrices array
    FinalMatrices = new D3DXMATRIX[MaxFrames];
    ZeroMemory(FinalMatrices, sizeof(D3DXMATRIX) * MaxFrames);

    //_mesh = new Mesh();

    // set up the ppFrameMatrices pointers
    link_frames((CUSTOM_FRAME*)TopFrame);
}

AnimatedModel::~AnimatedModel()
{
}

void AnimatedModel::Render(float deltaTime, int modelReference)
{
  static float index = 0.0f; index+=0.03f;    // an ever-increasing float value
    //D3DXMATRIX matRotateY;    // a matrix to store the rotation for each triangle
  D3DXMATRIX transform;
  D3DXMatrixTranslation(&transform, pos.x, pos.y, pos.z);
    //D3DXMatrixRotationY(&matRotateY, index);    // the rotation matrix
    d3ddev->SetTransform(D3DTS_WORLD, &transform);    // set the world transform
    // if the mesh is animated...
    if(AnimationController)
    {
        static DWORD Time;// = GetTickCount();
        // move the animation forward by the elapsed time
        AnimationController->AdvanceTime((GetTickCount() - Time) * 75.0f, NULL);
      //AnimationController->AdvanceTime(deltaTime * 3.0f, NULL);
        // reset Time for the next time through
        Time = GetTickCount();
    }
    // update each combined matrix
    update_frames((CUSTOM_FRAME*)TopFrame, NULL);
    // update the mesh containers
    update_mesh_containers((CUSTOM_FRAME*)TopFrame, GetTickCount());
    // render each mesh container
    draw_mesh((CUSTOM_FRAME*)TopFrame);
}

void AnimatedModel::Render(float deltaTime, bool reflection)
{
	static float index = 0.0f; index+=0.03f;    // an ever-increasing float value
    //D3DXMATRIX matRotateY;    // a matrix to store the rotation for each triangle
    // if the mesh is animated...
    if(AnimationController)
    {
        static DWORD Time;// = GetTickCount();
        // move the animation forward by the elapsed time
        AnimationController->AdvanceTime((GetTickCount() - Time) * 75.0f, NULL);
      //AnimationController->AdvanceTime(deltaTime * 3.0f, NULL);
        // reset Time for the next time through
        Time = GetTickCount();
    }
    // update each combined matrix
    update_frames((CUSTOM_FRAME*)TopFrame, NULL);
    // update the mesh containers
    update_mesh_containers((CUSTOM_FRAME*)TopFrame, GetTickCount());
    // render each mesh container
	if(reflection)
		draw_mesh((CUSTOM_FRAME*)TopFrame);
	else
		draw_shadow_mesh((CUSTOM_FRAME*)TopFrame);
}

void AnimatedModel::draw_mesh(CUSTOM_FRAME* pFrame)
{
    // cast the pFrame's mesh container pointer to a CUSTOM_MESHCONTAINER*
    CUSTOM_MESHCONTAINER* pMeshContainer = (CUSTOM_MESHCONTAINER*)pFrame->pMeshContainer;
    if(pMeshContainer)
    {
        // for each material...
        for(UINT i = 0; i < pMeshContainer->NumMaterials; i++)
        {
            // set the material
            d3ddev->SetMaterial(&pMeshContainer->pMaterials[i].MatD3D);
            // set the texture
            d3ddev->SetTexture(0, pMeshContainer->pTextures[i]);
            // draw the subset!
            pMeshContainer->pFinalMesh->DrawSubset(i);
        }
    }
    // run for all siblings
    if(pFrame->pFrameSibling)
        draw_mesh((CUSTOM_FRAME*)pFrame->pFrameSibling);
    // run for the first child (which will then run all other children)
    if(pFrame->pFrameFirstChild)
        draw_mesh((CUSTOM_FRAME*)pFrame->pFrameFirstChild);
};

void AnimatedModel::draw_shadow_mesh(CUSTOM_FRAME* pFrame)
{
    // cast the pFrame's mesh container pointer to a CUSTOM_MESHCONTAINER*
    CUSTOM_MESHCONTAINER* pMeshContainer = (CUSTOM_MESHCONTAINER*)pFrame->pMeshContainer;
    if(pMeshContainer)
    {
        // for each material...
        for(UINT i = 0; i < pMeshContainer->NumMaterials; i++)
        {
            // draw the subset!
            pMeshContainer->pFinalMesh->DrawSubset(i);
        }
    }
    // run for all siblings
    if(pFrame->pFrameSibling)
        draw_shadow_mesh((CUSTOM_FRAME*)pFrame->pFrameSibling);
    // run for the first child (which will then run all other children)
    if(pFrame->pFrameFirstChild)
        draw_shadow_mesh((CUSTOM_FRAME*)pFrame->pFrameFirstChild);
};

void AnimatedModel::update_mesh_containers(CUSTOM_FRAME* pFrame, double long time)
{
    // cast the pFrame's mesh container pointer to a CUSTOM_MESHCONTAINER*
    CUSTOM_MESHCONTAINER* pMeshContainer = (CUSTOM_MESHCONTAINER*)pFrame->pMeshContainer;
    if(pMeshContainer && pMeshContainer->pSkinInfo)
    {
        UINT NumFrames = pMeshContainer->pSkinInfo->GetNumBones();    // find how many frames
        // for each frame in the mesh container...
        for(UINT i = 0; i < NumFrames; i++)
        {
            // set FinalMatrices to that frame's offset matrix
            FinalMatrices[i] = *pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
            // multiply that by the animated frame matrix
            FinalMatrices[i] *= *pMeshContainer->ppFrameMatrices[i];
        }
        void* pSrc = NULL;    // a void pointer for the original mesh
        void* pDst = NULL;    // a void pointer for the modified mesh
        // lock the two meshes
        pMeshContainer->MeshData.pMesh->LockVertexBuffer(NULL, &pSrc);
        pMeshContainer->pFinalMesh->LockVertexBuffer(NULL, &pDst);
        // store the animated mesh into FinalMesh
        pMeshContainer->pSkinInfo->UpdateSkinnedMesh(FinalMatrices, NULL, pSrc, pDst);
        // unlock the two meshes
        pMeshContainer->pFinalMesh->UnlockVertexBuffer();
        pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
    }
    // run for all siblings
    if(pFrame->pFrameSibling)
        update_mesh_containers((CUSTOM_FRAME*)pFrame->pFrameSibling, time);
    // run for the first child (which will then run all other children)
    if(pFrame->pFrameFirstChild)
        update_mesh_containers((CUSTOM_FRAME*)pFrame->pFrameFirstChild, time);
};

void AnimatedModel::update_frames(CUSTOM_FRAME* pFrame, D3DXMATRIX* pParentMatrix)
{
    // combine the frame's matrix with the parent's matrix, if any
    if(pParentMatrix)
        pFrame->CombTransformationMatrix = pFrame->TransformationMatrix * *pParentMatrix;
    else
        pFrame->CombTransformationMatrix = pFrame->TransformationMatrix;
    // run for all siblings
    if(pFrame->pFrameSibling)
        update_frames((CUSTOM_FRAME*)pFrame->pFrameSibling, pParentMatrix);
    // run for the first child (which will then run all other children)
    if(pFrame->pFrameFirstChild)
        update_frames((CUSTOM_FRAME*)pFrame->pFrameFirstChild,
            &pFrame->CombTransformationMatrix);
};

void AnimatedModel::link_frames(CUSTOM_FRAME* pFrame)
{
    // cast the pFrame's mesh container pointer to a CUSTOM_MESHCONTAINER*
    CUSTOM_MESHCONTAINER* pMeshContainer = (CUSTOM_MESHCONTAINER*)pFrame->pMeshContainer;
    // if there is a mesh container and if it has skin info...
    if(pMeshContainer && pMeshContainer->pSkinInfo)
    {
        // loop through each frame in the mesh container
        for(UINT i = 0; i < pMeshContainer->pSkinInfo->GetNumBones(); i++)
        {
            CUSTOM_FRAME* pTempFrame;    // a temporary frame pointer
            // find each frame by name
            pTempFrame = (CUSTOM_FRAME*)D3DXFrameFind(TopFrame,
                                                  pMeshContainer->pSkinInfo->GetBoneName(i));
            // and set up a pointer to it
            pMeshContainer->ppFrameMatrices[i] = &pTempFrame->CombTransformationMatrix;
        }
    }
    // run for all siblings
    if(pFrame->pFrameSibling)
        link_frames((CUSTOM_FRAME*)pFrame->pFrameSibling);
    // run for the first child (which will then run all other children)
    if(pFrame->pFrameFirstChild)
        link_frames((CUSTOM_FRAME*)pFrame->pFrameFirstChild);
};

D3DXVECTOR3 AnimatedModel::getPosition()
{
	return pos;
}
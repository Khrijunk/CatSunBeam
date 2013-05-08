#include "Model.h"

Model::Model(LPDIRECT3D9 d3d, LPDIRECT3DDEVICE9 d3ddev, LPCSTR filePath)
{
  // Variables
  _d3d = d3d;
  _d3ddev = d3ddev;
  _mesh = 0;
  //_materials = 0;
  //_textures = 0;
  HRESULT result = 0; // successful load check
  ID3DXBuffer* adjBuffer = 0;
  ID3DXBuffer* materialBuffer = 0; // material buffer
  DWORD materialCount = 0; // number of materials

  // Attempt to load the model file onto video memory
  result = D3DXLoadMeshFromX(filePath, D3DXMESH_MANAGED, d3ddev, &adjBuffer, &materialBuffer, 0, &materialCount, &_mesh);

  // If the loading failed
  if (FAILED(result)) { MessageBox(0, "Could not load the model file onto video memory.", 0, 0); }

  // Attempt to extract the materials
  if (materialBuffer != 0 && materialCount != 0)
  {
    D3DXMATERIAL *materials = (D3DXMATERIAL*)materialBuffer->GetBufferPointer();
    for (int i = 0; i < (int)materialCount; i++)
    {
      // MatD3D.Ambient is not initialized when loaded, initializing it now
      materials[i].MatD3D.Ambient = materials[i].MatD3D.Diffuse;

      // saving the ith material to memory
      _materials.push_back(&materials[i].MatD3D);

      // handle texture of the material if it contains one
      if (materials[i].pTextureFilename != 0)
      {
        IDirect3DTexture9 *texture = 0;
        D3DXCreateTextureFromFile(d3ddev, materials[i].pTextureFilename, &texture);

        // saving the texture to memory
        _textures.push_back(texture);
      }
      else
      {
        // saving the texture to a default of 0
        _textures.push_back(0);
      }
    }
  }

  // we extracted the materials and textures to memory, delete the material buffer
  materialBuffer->Release();
}

bool Model::Render(float deltaTime)
{
  if (_d3ddev)
  {

    // update before render
    static float y = 0.0f;
    D3DXMATRIX scaling;
    D3DXMATRIX yRotation;
    D3DXMatrixRotationY(&yRotation, y);
    //D3DXMatrixScaling(&scaling, .05f, .05f, .05f);
    D3DXMatrixScaling(&scaling, 5.0f, 5.0f, 5.0f);
    y += deltaTime;
    if (y < 6.28f) { y = 0.0f; }
    D3DMATRIX world = scaling;
    _d3ddev->SetTransform(D3DTS_WORLD, &world);
    // render
    for (int i = 0; i < (int)_materials.size(); i++)
    {
      _d3ddev->SetMaterial(_materials[i]);
      _d3ddev->SetTexture(0, _textures[i]);
      _mesh->DrawSubset(i);
    }
    CalculateNormals();
  }
  return true;
}

void Model::CalculateNormals()
{
  if (!(_mesh->GetFVF() & D3DFVF_NORMAL))
  {
    ID3DXMesh *temp = 0;
    _mesh->CloneMeshFVF(D3DXMESH_MANAGED, _mesh->GetFVF() | D3DFVF_NORMAL, _d3ddev, &temp);
    D3DXComputeNormals(temp, 0);
    _mesh->Release();
    _mesh = temp;
  }
}

Model::~Model()
{
  _materials.~vector();
  _textures.~vector();
  _mesh->Release();
}

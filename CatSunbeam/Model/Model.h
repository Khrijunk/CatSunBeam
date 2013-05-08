#ifndef MODEL_H
#define MODEL_H

#include "../Includes.h"

class Model
{
  public:
    Model(LPDIRECT3D9 d3d, LPDIRECT3DDEVICE9 d3ddev, LPCSTR filePath);
    bool Render(float deltaTime, int modelReference);
    void CalculateNormals();
    ~Model();
  private:
    LPDIRECT3D9 _d3d;
    LPDIRECT3DDEVICE9 _d3ddev;
    ID3DXMesh *_mesh;
    std::vector<D3DMATERIAL9*> _materials;
    std::vector<IDirect3DTexture9*> _textures;
	ID3DXBuffer* materialBuffer; // material buffer
};

#endif
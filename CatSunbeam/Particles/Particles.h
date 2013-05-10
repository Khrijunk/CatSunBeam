#ifndef _PARTICLES_H_
#define _PARTICLES_H_
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>

// include the Direct3D Library files
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// the PARTICLE class
class Particles
{
public:
	Particles();
	Particles(D3DXVECTOR3,D3DXVECTOR3,D3DXVECTOR3,D3DXVECTOR3,D3DXVECTOR3);

    void render_particle(LPDIRECT3DDEVICE9, int);
    void set_particle(float camx, float camy, float camz, LPDIRECT3DDEVICE9);
    void run_particle(float seconds);
    void reset_particle();
	void intBuffers(LPDIRECT3DDEVICE9);
	void intFlame(LPDIRECT3DDEVICE9);
	void run_particles(LPDIRECT3DDEVICE9, int);
	void initBuffer(LPDIRECT3DVERTEXBUFFER9 v_buffer);
    bool active;

protected:
    D3DXVECTOR3 position;
    D3DXVECTOR3 velocity;
    D3DXVECTOR3 acceleration;
    float radius;
    float lifespan;
    float life;
    D3DXMATRIX matRotateX;
    D3DXMATRIX matRotateY;
};
#endif
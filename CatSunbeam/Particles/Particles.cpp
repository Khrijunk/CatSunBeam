#include "Particles.h"

LPDIRECT3DVERTEXBUFFER9 t_buffer = NULL;    // the pointer to the particle's vertex buffer
LPDIRECT3DVERTEXBUFFER9 g_buffer = NULL;    // the pointer to the grid's vertex buffer
float camx, camy, camz;    // camera position
// PARTICLE particle;
static const int PARTICLECOUNT = 50000;
Particles particle[PARTICLECOUNT]; 

struct CUSTOMVERTEX1 {FLOAT X, Y, Z; DWORD COLOR; FLOAT U, V;};
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
struct GRIDVERTEX {D3DXVECTOR3 position; DWORD color;};
#define GRIDFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

// texture declarations
LPDIRECT3DTEXTURE9 texture;

// function prototypes
void run_particles(LPDIRECT3DDEVICE9);
float random_number(float low, float high);

Particles::Particles(){
	reset_particle();

}

void Particles::initBuffer(LPDIRECT3DVERTEXBUFFER9 v_buffer)
{
	t_buffer = v_buffer;
}

void Particles::intBuffers(LPDIRECT3DDEVICE9 d3ddev){
	D3DXCreateTextureFromFile(d3ddev,"dust.png",&texture);

    struct CUSTOMVERTEX1 t_vert[] =
    {
        {-.05f, .05f, 0.0f, D3DCOLOR_XRGB(255, 255, 255), 1, 0,},
        {-0.05f, -.05f, 0.0f, D3DCOLOR_XRGB(255, 255, 255), 0, 0,},
        {.05f, .05f, 0.0f, D3DCOLOR_XRGB(255, 255, 255), 1, 1,},
        {.05f, -.05f, 0.0f, D3DCOLOR_XRGB(255, 255, 255), 0, 1,},
    };

    // create a vertex buffer interface called t_buffer
    d3ddev->CreateVertexBuffer(4*sizeof(CUSTOMVERTEX1),
                               0,
                               CUSTOMFVF,
                               D3DPOOL_MANAGED,
                               &t_buffer,
                               NULL);

    VOID* pVoid;    // a void pointer

    // lock t_buffer and load the vertices into it
    t_buffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, t_vert, sizeof(t_vert));
    t_buffer->Unlock();
}

// this is the function that positions, rotates, scales and renders the particle
void Particles::set_particle(float camx, float camy, float camz, LPDIRECT3DDEVICE9 d3ddev)
{
    // Before setting the world transform, do the intense mathematics!
    // a. Calculate the Differences
    static float difx, dify, difz;
    difx = camx - position.x;
    dify = camy - position.y;
    difz = camz - position.z;

    // b. Calculate the Distances
    static float FlatDist, TotalDist;
    FlatDist = sqrt(difx * difx + difz * difz);
    TotalDist = sqrt(FlatDist * FlatDist + dify * dify);

    // c. Y Rotation
    D3DXMatrixIdentity(&matRotateY);
    matRotateY._11 = matRotateY._33 = difz / FlatDist;    // cosY
    matRotateY._31 = difx / FlatDist;    // sinY
    matRotateY._13 = -matRotateY._31;    // -sinY

    // d. X Rotation
    D3DXMatrixIdentity(&matRotateX);
    matRotateX._22 = matRotateX._33 = FlatDist / TotalDist;    // cosX
    matRotateX._32 = dify / TotalDist;    // sinX
    matRotateX._23 = -matRotateX._32;    // -sinX

    // e. Tranlation
    static D3DXMATRIX matTranslate;
    D3DXMatrixTranslation(&matTranslate, position.x, position.y, position.z);

    // f. Scaling
    static D3DXMATRIX matScale;
    D3DXMatrixIdentity(&matScale);
    matScale._11 = matScale._22 = matScale._33 = radius*.5;


    // Now build the world matrix and set it
    d3ddev->SetTransform(D3DTS_WORLD, &(matScale * matRotateX* matRotateY* matTranslate));

    return;
}

// this function renders the particle
void Particles::render_particle(LPDIRECT3DDEVICE9 d3ddev)
{
    //d3ddev->SetFVF(CUSTOMFVF);

    d3ddev->SetStreamSource(0, t_buffer, 0, sizeof(CUSTOMVERTEX1));

	d3ddev->SetTexture(0, texture);

    d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);

    return;
}

// this function updates the particle
void Particles::run_particle(float seconds)
{
    // handle lifespan
    life += seconds;
    if(life > lifespan)
    {
        reset_particle();
        return;
    }

    velocity += acceleration * seconds;
    position += velocity * seconds;

    return;
}

// this function sets the particle back to its original state
void Particles::reset_particle()
{
    active = false;
    position.x = 0.0f;
    position.y = 4.0f;
    position.z = 0.0f;
    velocity.x = random_number(-2.0f, 2.0f);
    velocity.y = -.25f;
    velocity.z = random_number(-2.0f, 2.0f);
    acceleration.x = 0.0f;
    acceleration.y = .05f;
    acceleration.z = 0.0f;
    radius = .5f;
    lifespan = 5.0f;
    life = 0.0f;

    return;
}

 

// this function goes through and runs each active particle
void Particles::run_particles(LPDIRECT3DDEVICE9 d3ddev)
{
    // calculate elapsed time
    static DWORD starting_point = GetTickCount(), time;
    time = GetTickCount() - starting_point;
    starting_point = GetTickCount();

    // turn on alpha blending
    //d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

    // run and render all active particles
    //d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    for(int index = 0; index < PARTICLECOUNT; index++)
    {
        if(particle[index].active)
        {
            particle[index].run_particle(time / 1000.0f);
            particle[index].set_particle(camx, camy, camz,d3ddev);
            particle[index].render_particle(d3ddev);
        }
    }
   // d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

    for(int index = 0; index < time / 10.0f; index++)
    {
        for(int index = 0; index < PARTICLECOUNT; index++)
        {
            if(!particle[index].active)    // if the particle is inactive...
            {
                particle[index].active = true;    // ...activate it
                break;    // then stop
           }
        }
    }

    return;
}



// this function generates random float values
float random_number(float low, float high)
{
    return low + ((float)((float)rand() / (float)RAND_MAX) * (float)((high) - (low)));
}

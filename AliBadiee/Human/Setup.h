#include "Device_Draw.h"
#include "Body_Control.h"
#include "cube.h"


//
// Functions
//
bool Setup();

void Cleanup();

void setLight();

void renderShip();

void renderScene();

void renderScene();

void renderHuman();

void renderMirror();

void renderColorText();

bool Display(float timeDelta);

void displayConditions(float timeDelta);

void renderShadow();

//
// variables
//

// window width and height
const int Width = 1200;
const int Height = 800;

// light's settings
static float light_x = 2.0f, light_y = 2.0f, light_z = 2.0f;
static string light_color = "white";
static bool lightEnable = true;

// color for text
auto text_color = &d3d::RED_MTRL;

// variables for keeping the position of the camera
static float pos_x = 0.0f;
static float pos_y = 0.0f;
static float pos_z = -10.0f;

// variables for keeping the position of the target
static float target_x = 0.0f;
static float target_y = -4.0f;
static float target_z = 0.0f;

// FVF
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

bool Setup() {
    //
    // Setup default DirectX function
    //

    // draw the parts of human
    human.createIndexObjects();
    human.drawHat();
    human.drawHead();
    human.drawEyes();
    human.drawNose();
    human.drawEars();
    human.drawMouth();
    human.drawBody();
    human.drawHand();
    human.drawLeg();
    human.drawFoot();
    human.drawFingers();

    // create floor, walls and mirror
    world.createVertexBuffer();
    world.createFloor();
    world.createWall();
    world.createOuterWall();
    world.createMirror();
    world.createBaseFloor();
    world.createBaseSky();
    world.createTexture();
    world.createDoor(0, "create");
    world.createDoor2();
    world.createDoor3();
    world.createRoof();
    world.createCeil();
    world.createWindow();
    world.createSpace();
    world.createMount();

    // create font
    world.createFont();

    // create accessories
    accessory.createShip();

    // set sampler state
    Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
    Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

    // set and enable light
    setLight();

    return true;
}

void Cleanup() {
    //
    // Cleanup default DirectX function
    //

    d3d::Release<IDirect3DVertexBuffer9 *>(VB);

    for (int i = 0; i < human_object_count; i++)
        d3d::Release<ID3DXMesh *>(human_objects[i]);

    d3d::Release<ID3DXMesh *>(ship_mesh);

    for (int i = 0; i < ship_textures.size(); i++)
        d3d::Release<IDirect3DTexture9 *>(ship_textures[i]);
}

bool Display(float timeDelta) {
    //
    // Display default DirectX function
    //

    if (Device) {
        //
        // conditions
        //
        displayConditions(timeDelta);

        /////////////////////////////////////

        //
        // Look
        //
        D3DXVECTOR3 position(pos_x, pos_y, pos_z);
        D3DXVECTOR3 target(target_x, target_y, target_z);
        D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

        D3DXMATRIX V;
        D3DXMatrixLookAtLH(&V, &position, &target, &up);
        Device->SetTransform(D3DTS_VIEW, &V);

        /////////////////////////////////////

        //
        // Draw the scene
        //
        Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0xff000000, 1.0f, 0L);
        Device->BeginScene();

        // draw world objects
        renderScene();

        // ship
        renderShip();

        // render coloring for text
        renderColorText();

        // render mirror
        renderMirror();

        // render shadow
        renderShadow();

        // human
        renderHuman();

        Device->EndScene();
        Device->Present(0, 0, 0, 0);
    }
    return true;
}

void setLight() {
    //
    // Function for setting light
    //

    // Set up a directional light.
    D3DXVECTOR3 lightDir(light_x, light_y, light_z);
    D3DXCOLOR color;

    // change light color
    if (light_color == "white")
        color = d3d::WHITE;

    else
        color = d3d::YELLOW;

    D3DLIGHT9 light = d3d::InitDirectionalLight(&lightDir, &color);

    // Set and Enable the light.
    Device->SetLight(0, &light);
    Device->LightEnable(0, true);

    // Set lighting related render states.
    Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
    Device->SetRenderState(D3DRS_SPECULARENABLE, false);

    // Set the projection matrix.
    D3DXMATRIX proj;
    D3DXMatrixPerspectiveFovLH(
            &proj,
            D3DX_PI * 0.25f, // 45 - degree
            (float) Width / (float) Height,
            1.0f,
            1000.0f);

    Device->SetTransform(D3DTS_PROJECTION, &proj);
}

void displayConditions(float timeDelta) {
    //
    // Function to add conditions for Display
    //

    //
    // keys
    //
    if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
        target_x -= 10.0f * timeDelta;

    if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
        target_x += 10.0f * timeDelta;

    if (::GetAsyncKeyState(VK_UP) & 0x8000f)
        target_y += 10.0f * timeDelta;

    if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
        target_y -= 10.0f * timeDelta;

    if (::GetAsyncKeyState(VK_LBUTTON) & 0x8000f)
        target_z -= 10.0f * timeDelta;

    if (::GetAsyncKeyState(VK_RBUTTON) & 0x8000f)
        target_z += 10.0f * timeDelta;

    if (GetKeyState(VK_SHIFT) & 0x8000f) // x
        if (::GetAsyncKeyState(0x58) & 0x8000f)
            pos_x += 20.0f * timeDelta;

    if (GetKeyState(VK_SHIFT) != 1) // x
        if (::GetAsyncKeyState(0x58) & 0x8000f)
            pos_x -= 20.0f * timeDelta;

    if (GetKeyState(VK_SHIFT) & 0x8000f) // y
        if (::GetAsyncKeyState(0x59) & 0x8000f)
            pos_y += 20.0f * timeDelta;

    if (GetKeyState(VK_SHIFT) != 1) // y
        if (::GetAsyncKeyState(0x59) & 0x8000f)
            if (pos_y > -6.8 - 10.0f * timeDelta)
                pos_y -= 20.0f * timeDelta;

    if (GetKeyState(VK_SHIFT) & 0x8000f) // z
        if (::GetAsyncKeyState(0x5a) & 0x8000f)
            pos_z += 20.0f * timeDelta;

    if (GetKeyState(VK_SHIFT) != 1) // z
        if (::GetAsyncKeyState(0x5a) & 0x8000f)
            pos_z -= 20.0f * timeDelta;

    if (::GetAsyncKeyState(0x57) & 0x8000f) { // w
        // walk back if last move is different from w
        if (last_move == "s") {
            if (walk_degree_forward_backward > 0) {
                for (int x = 0; x < 1000000; x++) {
                    if (x % 100 == 0) {
                        walk_degree_forward_backward -= speed / 2.0f;
                        walkForBack(-walk_degree_forward_backward, "w");
                    }

                    if (walk_degree_forward_backward > 0)
                        break;
                }

            } else {
                for (int x = 0; x < 1000000; x++) {
                    if (x % 100 == 0) {
                        walk_degree_forward_backward += speed / 2.0f;
                        walkForBack(-walk_degree_forward_backward, "w");
                    }

                    if (walk_degree_forward_backward < 0)
                        break;
                }
            }

            Sleep(100);
            last_move = "w";

        } else {
            last_move = "w";

            if (positive)
                walk_degree_forward_backward += speed;

            else
                walk_degree_forward_backward -= speed;

            walkForBack(walk_degree_forward_backward, "w");

            if (walk_degree_forward_backward >= 3.14f / 4)
                positive = false;

            else if (walk_degree_forward_backward <= -3.14f / 4)
                positive = true;
        }
    }

    if (::GetAsyncKeyState(0x53) & 0x8000f) { // s
        // walk back if last move is different from s
        if (last_move == "w") {
            if (walk_degree_forward_backward > 0) {
                for (int x = 0; x < 1000000; x++) {
                    if (x % 100 == 0) {
                        walk_degree_forward_backward -= speed / 2.0f;
                        walkForBack(walk_degree_forward_backward, "s");
                    }

                    if (walk_degree_forward_backward > 0)
                        break;
                }
            } else {
                for (int x = 0; x < 1000000; x++) {
                    if (x % 100 == 0) {
                        walk_degree_forward_backward += speed / 2.0f;
                        walkForBack(walk_degree_forward_backward, "s");
                    }

                    if (walk_degree_forward_backward < 0)
                        break;
                }
            }

            Sleep(100);
            last_move = "s";

        } else {
            last_move = "s";

            if (positive)
                walk_degree_forward_backward += speed / 2.0f;

            else
                walk_degree_forward_backward -= speed / 2.0f;

            walkForBack(-walk_degree_forward_backward, "s");

            if (walk_degree_forward_backward >= 3.14f / 6)
                positive = false;

            else if (walk_degree_forward_backward <= -3.14f / 6)
                positive = true;
        }

    }

    if (::GetAsyncKeyState(0x51) & 0x8000f) { // q
        walkUpDown("q");
    }

    if (::GetAsyncKeyState(0x45) & 0x8000f) { // e
        walkUpDown("e");
    }

    if (::GetAsyncKeyState(0x41) & 0x8000f) { // a
        walkLeftRight("a");
    }

    if (::GetAsyncKeyState(0x44) & 0x8000f) { // d
        walkLeftRight("d");
    }

    if (::GetAsyncKeyState(0x54) & 0x8000f) { // t
        walkTo();
    }

    if (GetKeyState(VK_SHIFT) & 0x8000f) // i
        if (::GetAsyncKeyState(0x49) & 0x8000f) {
            light_x += 5.0f * timeDelta;
            setLight();
        }

    if (GetKeyState(VK_SHIFT) != 1) // i
        if (::GetAsyncKeyState(0x49) & 0x8000f) {
            light_x -= 5.0f * timeDelta;
            setLight();
        }

    if (GetKeyState(VK_SHIFT) & 0x8000f) // o
        if (::GetAsyncKeyState(0x4f) & 0x8000f) {
            light_y += 5.0f * timeDelta;
            setLight();
        }

    if (GetKeyState(VK_SHIFT) != 1) // o
        if (::GetAsyncKeyState(0x4f) & 0x8000f) {
            light_y -= 5.0f * timeDelta;
            setLight();
        }

    if (GetKeyState(VK_SHIFT) & 0x8000f) // p
        if (::GetAsyncKeyState(0x50) & 0x8000f) {
            light_z += 5.0f * timeDelta;
            setLight();
        }

    if (GetKeyState(VK_SHIFT) != 1) // p
        if (::GetAsyncKeyState(0x50) & 0x8000f) {
            light_z -= 5.0f * timeDelta;
            setLight();
        }

    if (GetKeyState(VK_SHIFT) & 0x8000f) // l
        if (::GetAsyncKeyState(0x4c) & 0x8000f) {
            lightEnable = false;
            setLight();
        }

    if (GetKeyState(VK_SHIFT) != 1) // l
        if (::GetAsyncKeyState(0x4c) & 0x8000f) {
            lightEnable = true;
            setLight();
        }

    if (GetKeyState(VK_SHIFT) & 0x8000f) // k
        if (::GetAsyncKeyState(0x4b) & 0x8000f) {
            light_color = "yellow";
            setLight();
        }

    if (GetKeyState(VK_SHIFT) != 1) // k
        if (::GetAsyncKeyState(0x4b) & 0x8000f) {
            light_color = "white";
            setLight();
        }

    if (::GetAsyncKeyState(0x4D) & 0x8000f) { // m
        walk_to_direction[0] = 20.0f;
        walk_to_direction[1] = 0.0f;
        walk_to_direction[2] = 10.0f;

        walkTo();

        pos_x = -20.0f;
        pos_y = 0.0f;
        pos_z = -20.0f;

        target_x = -20.0f;
        target_y = 0.0f;
        target_z = 0.0f;
    }

    //
    // text coloring
    //
    static float y = 0.0f;
    y += timeDelta;
    cout << y << endl;

    if (y >= 0 && y < 2) {
        text_color = &d3d::BLUE_MTRL;
        renderColorText();

    } else if (y >= 2 && y < 4) {
        text_color = &d3d::RED_MTRL;
        renderColorText();

    } else if (y >= 4 && y < 6) {
        text_color = &d3d::WHITE_MTRL;
        renderColorText();

    } else {
        y = 0;
    }
}

void renderScene() {
    //
    // Function to render scene
    //

    D3DXMATRIX I;
    D3DXMatrixIdentity(&I);
    Device->SetTransform(D3DTS_WORLD, &I);

    Device->SetStreamSource(0, VB, 0, sizeof(Vertex));
    Device->SetFVF(Vertex::FVF);

    Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    /////////////////////////////////////

    // draw the base floor
    Device->SetMaterial(&world.base_floor_material);
    Device->SetTexture(0, world.base_floor_texture);
    Device->DrawPrimitive(D3DPT_TRIANGLELIST, any_cast<int>(vertex_indexes["base_floor"]),
                          any_cast<int>(vertex_length["base_floor"]) / 3);

    // draw the base sky
    Device->SetMaterial(&world.base_sky_material);
    Device->SetTexture(0, world.base_sky_texture);
    Device->DrawPrimitive(D3DPT_TRIANGLELIST, any_cast<int>(vertex_indexes["base_sky"]),
                          any_cast<int>(vertex_length["base_sky"]) / 3);

    // draw the floor
    Device->SetMaterial(&world.house_floor_material);
    Device->SetTexture(0, world.house_floor_texture);
    Device->DrawPrimitive(D3DPT_TRIANGLELIST, any_cast<int>(vertex_indexes["house_floor"]),
                          any_cast<int>(vertex_length["house_floor"]) / 3);

    // draw the walls
    Device->SetMaterial(&world.house_wall_material);
    Device->SetTexture(0, world.house_wall_texture);
    Device->DrawPrimitive(D3DPT_TRIANGLELIST, any_cast<int>(vertex_indexes["house_wall"]),
                          any_cast<int>(vertex_length["house_wall"]) / 3);

    // draw the outer walls
    Device->SetMaterial(&world.outer_wall_material);
    Device->SetTexture(0, world.outer_wall_texture);
    Device->DrawPrimitive(D3DPT_TRIANGLELIST, any_cast<int>(vertex_indexes["outer_wall"]),
                          any_cast<int>(vertex_length["outer_wall"]) / 3);

    // draw the door
    Device->SetMaterial(&world.house_door_material);
    Device->SetTexture(0, world.house_door_texture);
    Device->DrawPrimitive(D3DPT_TRIANGLELIST, any_cast<int>(vertex_indexes["house_door"]),
                          any_cast<int>(vertex_length["house_door"]) / 3);

    // draw the door 2
    Device->SetMaterial(&world.house_door2_material);
    Device->SetTexture(0, world.house_door2_texture);
    Device->DrawPrimitive(D3DPT_TRIANGLELIST, any_cast<int>(vertex_indexes["house_door2"]),
                          any_cast<int>(vertex_length["house_door2"]) / 3);

    // draw the door 3
    Device->SetMaterial(&world.house_door3_material);
    Device->SetTexture(0, world.house_door3_texture);
    Device->DrawPrimitive(D3DPT_TRIANGLELIST, any_cast<int>(vertex_indexes["house_door3"]),
                          any_cast<int>(vertex_length["house_door3"]) / 3);

    // draw the roof
    Device->SetMaterial(&world.house_roof_material);
    Device->SetTexture(0, world.house_roof_texture);
    Device->DrawPrimitive(D3DPT_TRIANGLELIST, any_cast<int>(vertex_indexes["house_roof"]),
                          any_cast<int>(vertex_length["house_roof"]) / 3);

    // draw the ceil
    Device->SetMaterial(&world.house_ceil_material);
    Device->SetTexture(0, world.house_ceil_texture);
    Device->DrawPrimitive(D3DPT_TRIANGLELIST, any_cast<int>(vertex_indexes["house_ceil"]),
                          any_cast<int>(vertex_length["house_ceil"]) / 3);

    // draw the window
    Device->SetMaterial(&world.house_window_material);
    Device->SetTexture(0, world.house_window_texture);
    Device->DrawPrimitive(D3DPT_TRIANGLELIST, any_cast<int>(vertex_indexes["house_window"]),
                          any_cast<int>(vertex_length["house_window"]) / 3);

    // draw the mirror
    Device->SetMaterial(&world.house_mirror_material);
    Device->SetTexture(0, world.house_mirror_texture);
    Device->DrawPrimitive(D3DPT_TRIANGLELIST, any_cast<int>(vertex_indexes["house_mirror"]),
                          any_cast<int>(vertex_length["house_mirror"]) / 3);

    // draw the space
    Device->SetMaterial(&world.house_space_material);
    Device->SetTexture(0, world.house_space_texture);
    Device->DrawPrimitive(D3DPT_TRIANGLELIST, any_cast<int>(vertex_indexes["house_space"]),
                          any_cast<int>(vertex_length["house_space"]) / 3);

    // draw the mount
    Device->SetMaterial(&world.base_mount_material);
    Device->SetTexture(0, world.base_mount_texture);
    Device->DrawPrimitive(D3DPT_TRIANGLELIST, any_cast<int>(vertex_indexes["base_mount"]),
                          any_cast<int>(vertex_length["base_mount"]) / 3);
}

void renderMirror() {
    //
    // Draw Mirror quad to stencil buffer ONLY.  In this way only the stencil bits that correspond to the mirror will
    // be on.  Therefore, the reflected can only be rendered where the stencil bits are turned on, and thus on
    // the mirror only.
    //

    Device->SetRenderState(D3DRS_STENCILENABLE, true);
    Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
    Device->SetRenderState(D3DRS_STENCILREF, 0x1);
    Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
    Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
    Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
    Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
    Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

    // disable writes to the depth and back buffers
    Device->SetRenderState(D3DRS_ZWRITEENABLE, false);
    Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
    Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

    // draw the mirror to the stencil buffer
    Device->SetStreamSource(0, VB, 0, sizeof(Vertex));
    Device->SetFVF(Vertex::FVF);
    Device->SetMaterial(&world.house_mirror_material);
    Device->SetTexture(0, world.house_mirror_texture);

    // index of mirror object
    int i = any_cast<int>(vertex_indexes["house_mirror"]);
    int l = any_cast<int>(vertex_length["house_mirror"]);

    D3DXMATRIX I;
    D3DXMatrixIdentity(&I);
    Device->SetTransform(D3DTS_WORLD, &I);
    Device->DrawPrimitive(D3DPT_TRIANGLELIST, i, l);

    // re-enable depth writes
    Device->SetRenderState(D3DRS_ZWRITEENABLE, true);

    // only draw reflected to the pixels where the mirror was drawn to.
    Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
    Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

    // position reflection
    D3DXMATRIX W;
    D3DXMATRIX ref = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, -1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };

    D3DXMATRIX tra = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, plane_z * 2, 1.0f
    };

    ref *= tra;

    // index of body object
    int object_index;

    // clear depth buffer and blend the reflected teapot with the mirror
    Device->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
    Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
    Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

    int needed_objects[] = {
            any_cast<int>(human_staff["hat"]),
            any_cast<int>(human_staff["head"]),
            any_cast<int>(human_staff["ear"]),
            any_cast<int>(human_staff["ear"]) + 1,
            any_cast<int>(human_staff["body"]),
            any_cast<int>(human_staff["hand"]),
            any_cast<int>(human_staff["hand"]) + 1,
            any_cast<int>(human_staff["hand"]) + 2,
            any_cast<int>(human_staff["hand"]) + 3,
            any_cast<int>(human_staff["foot"]),
            any_cast<int>(human_staff["foot"]) + 1,
            any_cast<int>(human_staff["leg"]),
            any_cast<int>(human_staff["leg"]) + 1,
            any_cast<int>(human_staff["finger"]),
            any_cast<int>(human_staff["finger"]) + 1,
            any_cast<int>(human_staff["finger"]) + 2,
            any_cast<int>(human_staff["finger"]) + 3,
            any_cast<int>(human_staff["finger"]) + 4,
            any_cast<int>(human_staff["finger"]) + 5,
            any_cast<int>(human_staff["finger"]) + 6,
            any_cast<int>(human_staff["finger"]) + 7,
            any_cast<int>(human_staff["finger"]) + 8,
            any_cast<int>(human_staff["finger"]) + 9,
    };

    for (int i = 0; i < sizeof(needed_objects) / sizeof(int); i++) {
        W = human_worlds[needed_objects[i]] * ref;

        // Finally, draw the reflected teapot
        Device->SetTransform(D3DTS_WORLD, &W);
        Device->SetMaterial(&human_materials[needed_objects[i]]);

        // reverse cull mode
        Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

        human_objects[needed_objects[i]]->DrawSubset(0);
    }

    // Restore render states.
    Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
    Device->SetRenderState(D3DRS_STENCILENABLE, false);
    Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void renderShadow() {
    //
    // Function to render shadow
    //

    Device->SetRenderState(D3DRS_STENCILENABLE, true);
    Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
    Device->SetRenderState(D3DRS_STENCILREF, 0x0);
    Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
    Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
    Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
    Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
    Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR); // increment to 1

    // position shadow
    D3DXVECTOR4 lightDirection(light_x, light_y, light_z, 0.0f);
    D3DXPLANE groundPlane(0.0f, 1.0f, 0.0f, 0.0f);

    D3DXMATRIX W, S;
    D3DXMATRIX tra = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            plane_y, plane_y, plane_y, 1.0f
    };

    D3DXMatrixShadow(
            &S,
            &lightDirection,
            &groundPlane);

    S *= tra;

    int object_index;

    // alpha blend the shadow
    Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    int needed_objects[] = {
            any_cast<int>(human_staff["hat"]),
            any_cast<int>(human_staff["head"]),
            any_cast<int>(human_staff["ear"]),
            any_cast<int>(human_staff["ear"]) + 1,
            any_cast<int>(human_staff["body"]),
            any_cast<int>(human_staff["hand"]),
            any_cast<int>(human_staff["hand"]) + 1,
            any_cast<int>(human_staff["hand"]) + 2,
            any_cast<int>(human_staff["hand"]) + 3,
            any_cast<int>(human_staff["foot"]),
            any_cast<int>(human_staff["foot"]) + 1,
            any_cast<int>(human_staff["leg"]),
            any_cast<int>(human_staff["leg"]) + 1,
            any_cast<int>(human_staff["finger"]),
            any_cast<int>(human_staff["finger"]) + 1,
            any_cast<int>(human_staff["finger"]) + 2,
            any_cast<int>(human_staff["finger"]) + 3,
            any_cast<int>(human_staff["finger"]) + 4,
            any_cast<int>(human_staff["finger"]) + 5,
            any_cast<int>(human_staff["finger"]) + 6,
            any_cast<int>(human_staff["finger"]) + 7,
            any_cast<int>(human_staff["finger"]) + 8,
            any_cast<int>(human_staff["finger"]) + 9,
    };

    D3DMATERIAL9 mtrl = d3d::InitMtrl(d3d::BLACK, d3d::BLACK, d3d::BLACK, d3d::BLACK, 0.0f);
    mtrl.Diffuse.a = 0.5f; // 50% transparency.

    // Disable depth buffer so that z-fighting doesn't occur when we render the shadow on top of the floor.
    Device->SetRenderState(D3DRS_ZENABLE, false);

    for (int i = 0; i < sizeof(needed_objects) / sizeof(int); i++) {
        W = human_worlds[needed_objects[i]] * S;
        Device->SetTransform(D3DTS_WORLD, &W);
        Device->SetMaterial(&mtrl);
        human_objects[needed_objects[i]]->DrawSubset(0);
    }

    Device->SetRenderState(D3DRS_ZENABLE, true);
    Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
    Device->SetRenderState(D3DRS_STENCILENABLE, false);
}

void renderColorText() {
    //
    // Function to render the text with color
    //

    int x = 0;
    // set material and world matrix for ith object, then render the ith object for TEXT
    for (auto i: world.out_matrix_text) {
        Device->SetTransform(D3DTS_WORLD, &i);
        Device->SetMaterial(text_color);
        world.garage_title_mesh[x++]->DrawSubset(0);

    }
}

void renderShip() {
    //
    // Function to render the ship
    //

    Device->SetTransform(D3DTS_WORLD, &accessory.ship_matrix);
    for (int i = 0; i < ship_materials.size(); i++) {
        Device->SetMaterial(&ship_materials[i]);
        Device->SetTexture(0, ship_textures[i]);
        ship_mesh->DrawSubset(i);
    }
}

void renderHuman() {
    //
    // Function to render the Human
    //
    // set material and world matrix for ith object, then render the ith object for HUMAN
    //

    for (int i = 0; i < human_object_count; i++) {
        Device->SetTransform(D3DTS_WORLD, &human_worlds[i]);
        Device->SetMaterial(&human_materials[i]);
        human_objects[i]->DrawSubset(0);
    }
}

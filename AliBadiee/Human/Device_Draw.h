#pragma once

#include <unordered_map>
#include <string>
#include "d3dUtility.h"
#include <iostream>
#include <any>
#include <vector>

using namespace std;

// Device
IDirect3DDevice9 *Device = 0;

// limits of house's plane with x, y, z
static float plane_x = 30.0f, plane_y = -7.4f, plane_z = 30.0f;

// Accessory
ID3DXMesh *ship_mesh = 0;
vector<D3DMATERIAL9> ship_materials(0);
vector<IDirect3DTexture9 *> ship_textures(0);

// Human Mesh, Material, Matrix
const int human_object_count = 27;
ID3DXMesh *human_objects[human_object_count];
D3DMATERIAL9 human_materials[human_object_count];
D3DXMATRIX human_worlds[human_object_count];

// dictionary keeping the start index of human objects
unordered_map<string, any> human_staff;

// Vertex buffer for floor, walls and mirror
IDirect3DVertexBuffer9 *VB = 0;

// dictionary to keep index of world objects
unordered_map<string, any> vertex_indexes;
unordered_map<string, any> vertex_length;
unordered_map<string, any> vertex_address;

// Vertex structure for planes
struct Vertex {
    Vertex() {}

    Vertex(float x, float y, float z,
           float nx, float ny, float nz,
           float u, float v) {
        _x = x;
        _y = y;
        _z = z;
        _nx = nx;
        _ny = ny;
        _nz = nz;
        _u = u;
        _v = v;
    }

    float _x, _y, _z;
    float _nx, _ny, _nz;
    float _u, _v;

    static const DWORD FVF;
};

// vertex for floor, walls and mirror
Vertex *v = 0;

// positions of walking movements
static float walk_position_left_right = 0.0f; // x
static float walk_position_up_down = 0.0f; // y
static float walk_position_forward_backward = 0.0f; // z

class World {
public:
    // squares
    const float floor_square = 3500;
    const float sky_square = 3500;
    const float sky_height = 150;

    // the roof variables
    float roof_height = -plane_y + 15;
    float roof_bottom = plane_y + roof_height * 2;
    float roof_top = roof_bottom + 10.0f;

    // space variable for inner and outer walls
    float space_side = 0.5f;
    float space_front = +0.01f;

    // font meshes
    ID3DXMesh *garage_title_mesh[6] = {};

    // font matrix
    vector<D3DXMATRIX> out_matrix_text;

    // world objects textures
    IDirect3DTexture9 *base_floor_texture = 0, *base_sky_texture = 0, *house_floor_texture = 0, *house_wall_texture = 0,
            *outer_wall_texture = 0, *house_mirror_texture = 0, *house_door_texture = 0, *house_door2_texture = 0,
            *house_door3_texture = 0, *house_roof_texture = 0, *house_ceil_texture = 0, *house_window_texture = 0,
            *house_space_texture = 0, *base_mount_texture = 0;

    // world objects materials
    D3DMATERIAL9 base_floor_material = d3d::WHITE_MTRL, base_sky_material = d3d::WHITE_MTRL,
            house_floor_material = d3d::WHITE_MTRL, house_wall_material = d3d::WHITE_MTRL,
            outer_wall_material = d3d::WHITE_MTRL, house_mirror_material = d3d::WHITE_MTRL,
            house_door_material = d3d::WHITE_MTRL, house_door2_material = d3d::WHITE_MTRL,
            house_door3_material = d3d::WHITE_MTRL, house_roof_material = d3d::WHITE_MTRL,
            house_ceil_material = d3d::WHITE_MTRL, house_window_material = d3d::WHITE_MTRL,
            house_space_material = d3d::WHITE_MTRL, base_mount_material = d3d::WHITE_MTRL;

    void createTexture() {
        //
        // Function to create the textures from images and set specular
        //

        // make walls have low specular reflectance - 20% to prevent showing mirror's object
        base_floor_material.Specular = d3d::WHITE * 0.2f;
        base_sky_material.Specular = d3d::WHITE * 0.2f;
        house_floor_material.Specular = d3d::WHITE * 0.2f;
        house_wall_material.Specular = d3d::WHITE * 0.2f;
        house_door_material.Specular = d3d::WHITE * 0.2f;
        house_door2_material.Specular = d3d::WHITE * 0.2f;
        house_door3_material.Specular = d3d::WHITE * 0.2f;
        house_roof_material.Specular = d3d::WHITE * 0.2f;
        house_ceil_material.Specular = d3d::WHITE * 0.2f;
        house_window_material.Specular = d3d::WHITE * 0.2f;
        house_space_material.Specular = d3d::WHITE * 0.2f;
        base_mount_material.Specular = d3d::WHITE * 0.2f;

        // images addresses
        vertex_address["base_floor"] = ".\\Images\\soil1.jpg"; // address of image of base floor
        vertex_address["base_sky"] = ".\\Images\\sky1.jpg";
        vertex_address["house_floor"] = ".\\Images\\Floor.jpg";
        vertex_address["house_mirror"] = ".\\Images\\mirror.jpg";
        vertex_address["house_wall"] = ".\\Images\\wall.jpg";
        vertex_address["outer_wall"] = ".\\Images\\outer_wall.jpg";
        vertex_address["house_door"] = ".\\Images\\door.jpg";
        vertex_address["house_door2"] = ".\\Images\\door2.jpg";
        vertex_address["house_door3"] = ".\\Images\\door2.jpg";
        vertex_address["house_roof"] = ".\\Images\\roof.jpg";
        vertex_address["house_ceil"] = ".\\Images\\ceil.jpg";
        vertex_address["house_window"] = ".\\Images\\window2.png";
        vertex_address["house_space"] = ".\\Images\\space.jpg";
        vertex_address["base_mount"] = ".\\Images\\new.png";

        // images to textures
        D3DXCreateTextureFromFileA(Device, any_cast<LPCSTR>(vertex_address["base_floor"]), &base_floor_texture);
        D3DXCreateTextureFromFileA(Device, any_cast<LPCSTR>(vertex_address["base_sky"]), &base_sky_texture);
        D3DXCreateTextureFromFileA(Device, any_cast<LPCSTR>(vertex_address["house_mirror"]),
                                   &house_mirror_texture);
        D3DXCreateTextureFromFileA(Device, any_cast<LPCSTR>(vertex_address["house_floor"]), &house_floor_texture);
        D3DXCreateTextureFromFileA(Device, any_cast<LPCSTR>(vertex_address["house_wall"]), &house_wall_texture);
        D3DXCreateTextureFromFileA(Device, any_cast<LPCSTR>(vertex_address["outer_wall"]), &outer_wall_texture);
        D3DXCreateTextureFromFileA(Device, any_cast<LPCSTR>(vertex_address["house_door"]), &house_door_texture);
        D3DXCreateTextureFromFileA(Device, any_cast<LPCSTR>(vertex_address["house_door2"]), &house_door2_texture);
        D3DXCreateTextureFromFileA(Device, any_cast<LPCSTR>(vertex_address["house_door3"]), &house_door3_texture);
        D3DXCreateTextureFromFileA(Device, any_cast<LPCSTR>(vertex_address["house_roof"]), &house_roof_texture);
        D3DXCreateTextureFromFileA(Device, any_cast<LPCSTR>(vertex_address["house_ceil"]), &house_ceil_texture);
        D3DXCreateTextureFromFileA(Device, any_cast<LPCSTR>(vertex_address["house_window"]),
                                   &house_window_texture);
        D3DXCreateTextureFromFileA(Device, any_cast<LPCSTR>(vertex_address["house_space"]),
                                   &house_space_texture);
        D3DXCreateTextureFromFileA(Device, any_cast<LPCSTR>(vertex_address["base_mount"]),
                                   &base_mount_texture);
    }

    void createVertexBuffer() {
        //
        // Function to create vertex index and length for world objects
        //

        vertex_indexes["base_floor"] = 0; // start of base floor
        vertex_length["base_floor"] = 6; // length of vertex of base floor

        vertex_indexes["base_sky"] = 6;
        vertex_length["base_sky"] = 6;

        vertex_indexes["house_floor"] = 12;
        vertex_length["house_floor"] = 6;

        vertex_indexes["house_mirror"] = 18;
        vertex_length["house_mirror"] = 6;

        vertex_indexes["house_door"] = 24;
        vertex_length["house_door"] = 6;

        vertex_indexes["house_roof"] = 30;
        vertex_length["house_roof"] = 12;

        vertex_indexes["house_wall"] = 42;
        vertex_length["house_wall"] = 24;

        vertex_indexes["outer_wall"] = 66;
        vertex_length["outer_wall"] = 18;

        vertex_indexes["house_ceil"] = 84;
        vertex_length["house_ceil"] = 6;

        vertex_indexes["house_window"] = 90;
        vertex_length["house_window"] = 6;

        vertex_indexes["house_space"] = 96;
        vertex_length["house_space"] = 12;

        vertex_indexes["house_door2"] = 108;
        vertex_length["house_door2"] = 6;

        vertex_indexes["house_door3"] = 114;
        vertex_length["house_door3"] = 6;

        vertex_indexes["base_mount"] = 120;
        vertex_length["base_mount"] = 6;

        // count the number of lengths all objects got
        int counter = 0;
        vector<string> keys;
        keys.reserve(vertex_length.size());
        for (const auto &kv: vertex_length) {
            keys.push_back(kv.first);
            counter += any_cast<int>(vertex_length[kv.first]);
        }

        // create the vertex buffer
        Device->CreateVertexBuffer(
                counter * sizeof(Vertex),
                0,
                Vertex::FVF,
                D3DPOOL_MANAGED,
                &VB,
                0);
    }

    void createBaseFloor() {
        //
        // Function to create base floor
        //

        int i = any_cast<int>(vertex_indexes["base_floor"]);
        int l = any_cast<int>(vertex_length["base_floor"]);

        int end = i + l;

        Vertex vertexes[] = {
                // floor vertex
                Vertex(-floor_square, plane_y - 0.01f, -floor_square, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
                Vertex(-floor_square, plane_y - 0.01f, floor_square, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
                Vertex(floor_square, plane_y - 0.01f, floor_square, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),

                Vertex(-floor_square, plane_y - 0.01f, -floor_square, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
                Vertex(floor_square, plane_y - 0.01f, floor_square, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),
                Vertex(floor_square, plane_y - 0.01f, -floor_square, 1.0f, 0.0f, -1.0f, 1.0f, 1.0f),
        };

        VB->Lock(0, 0, (void **) &v, 0);

        for (int x = 0; i < end; i++, x++) {
            v[i] = vertexes[x];
        }

        VB->Unlock();
    }

    void createBaseSky() {
        //
        // Function to create base sky
        //

        int i = any_cast<int>(vertex_indexes["base_sky"]);
        int l = any_cast<int>(vertex_length["base_sky"]);

        int end = i + l;

        Vertex vertexes[] = {
                // base sky vertex
                Vertex(-sky_square, sky_height, -sky_square, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
                Vertex(-sky_square, sky_height, sky_square, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
                Vertex(sky_square, sky_height, sky_square, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),

                Vertex(-sky_square, sky_height, -sky_square, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
                Vertex(sky_square, sky_height, sky_square, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),
                Vertex(sky_square, sky_height, -sky_square, 1.0f, 0.0f, -1.0f, 1.0f, 1.0f),
        };

        VB->Lock(0, 0, (void **) &v, 0);

        for (int x = 0; i < end; i++, x++) {
            v[i] = vertexes[x];
        }

        VB->Unlock();
    }

    void createMirror() {
        //
        // Function to create mirror
        //

        int i = any_cast<int>(vertex_indexes["house_mirror"]);
        int l = any_cast<int>(vertex_length["house_mirror"]);
        int end = i + l;

        float mirror_bottom = plane_y;
        float mirror_top = roof_height - 8;
        float mirror_s_position = -plane_x;
        float mirror_e_position = 0.0f;

        Vertex vertexes[] = {
                // mirror
                Vertex(mirror_s_position, mirror_bottom, plane_z - 0.01, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
                Vertex(mirror_s_position, mirror_top, plane_z - 0.01, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
                Vertex(mirror_e_position, mirror_top, plane_z - 0.01, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),

                Vertex(mirror_s_position, mirror_bottom, plane_z - 0.01, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
                Vertex(mirror_e_position, mirror_top, plane_z - 0.01, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),
                Vertex(mirror_e_position, mirror_bottom, plane_z - 0.01, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
        };

        VB->Lock(0, 0, (void **) &v, 0);

        for (int x = 0; i < end; i++, x++) {
            v[i] = vertexes[x];
        }

        VB->Unlock();


    }

    void createFloor() {
        //
        // Function to create floor
        //

        int i = any_cast<int>(vertex_indexes["house_floor"]);
        int l = any_cast<int>(vertex_length["house_floor"]);
        int end = i + l;

        Vertex vertexes[] = {
                // floor vertex
                Vertex(-plane_x, plane_y, -plane_z, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
                Vertex(-plane_x, plane_y, plane_z, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
                Vertex(plane_x, plane_y, plane_z, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),

                Vertex(-plane_x, plane_y, -plane_z, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
                Vertex(plane_x, plane_y, plane_z, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),
                Vertex(plane_x, plane_y, -plane_z, 1.0f, 0.0f, -1.0f, 1.0f, 1.0f),
        };

        VB->Lock(0, 0, (void **) &v, 0);

        for (int x = 0; i < end; i++, x++) {
            v[i] = vertexes[x];
        }

        VB->Unlock();

    }

    void createCeil() {
        //
        // Function to create ceil
        //

        int i = any_cast<int>(vertex_indexes["house_ceil"]);
        int l = any_cast<int>(vertex_length["house_ceil"]);
        int end = i + l;

        Vertex vertexes[] = {
                // floor vertex
                Vertex(-plane_x, roof_height - 8, -plane_z, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
                Vertex(-plane_x, roof_height - 8, plane_z, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
                Vertex(plane_x, roof_height - 8, plane_z, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),

                Vertex(-plane_x, roof_height - 8, -plane_z, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
                Vertex(plane_x, roof_height - 8, plane_z, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),
                Vertex(plane_x, roof_height - 8, -plane_z, 1.0f, 0.0f, -1.0f, 1.0f, 1.0f),
        };

        VB->Lock(0, 0, (void **) &v, 0);

        for (int x = 0; i < end; i++, x++) {
            v[i] = vertexes[x];
        }

        VB->Unlock();

    }

    void createRoof() {
        //
        // Function to create roof
        //

        int i = any_cast<int>(vertex_indexes["house_roof"]);
        int l = any_cast<int>(vertex_length["house_roof"]);
        int end = i + l;

        Vertex vertexes[] = {
                // roof vertex
                Vertex(-plane_x - space_side, roof_top, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
                Vertex(-plane_x - space_side, roof_bottom, plane_z, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
                Vertex(plane_x + space_side, roof_bottom, plane_z, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),

                Vertex(-plane_x - space_side, roof_top, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
                Vertex(plane_x + space_side, roof_bottom, plane_z, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),
                Vertex(plane_x + space_side, roof_top, 0.0f, 1.0f, 0.0f, -1.0f, 1.0f, 1.0f),

                Vertex(-plane_x - space_side, roof_bottom, -plane_z, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
                Vertex(-plane_x - space_side, roof_top, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
                Vertex(plane_x + space_side, roof_top, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),

                Vertex(-plane_x - space_side, roof_bottom, -plane_z, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
                Vertex(plane_x + space_side, roof_top, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),
                Vertex(plane_x + space_side, roof_bottom, -plane_z, 1.0f, 0.0f, -1.0f, 1.0f, 1.0f),
        };

        VB->Lock(0, 0, (void **) &v, 0);

        for (int x = 0; i < end; i++, x++) {
            v[i] = vertexes[x];
        }

        VB->Unlock();

    }

    void createWall() {
        //
        // Function to create inner wall
        //

        int i = any_cast<int>(vertex_indexes["house_wall"]);
        int l = any_cast<int>(vertex_length["house_wall"]);
        int end = i + l;

        Vertex vertexes[] = {
                // back wall vertex
                Vertex(-plane_x, plane_y, plane_z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
                Vertex(-plane_x, roof_height, plane_z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
                Vertex(plane_x, roof_height, plane_z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),

                Vertex(-plane_x, plane_y, plane_z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
                Vertex(plane_x, roof_height, plane_z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),
                Vertex(plane_x, plane_y, plane_z, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),

                // left wall vertex
                Vertex(-plane_x, plane_y, -plane_z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
                Vertex(-plane_x, roof_height, -plane_z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
                Vertex(-plane_x, roof_height, plane_z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),

                Vertex(-plane_x, plane_y, -plane_z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
                Vertex(-plane_x, roof_height, plane_z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),
                Vertex(-plane_x, plane_y, plane_z, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),

                // right wall vertex
                Vertex(+plane_x, plane_y, -plane_z, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f),
                Vertex(+plane_x, roof_height, -plane_z, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f),
                Vertex(+plane_x, roof_height, plane_z, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f),

                Vertex(+plane_x, plane_y, -plane_z, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f),
                Vertex(+plane_x, roof_height, plane_z, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f),
                Vertex(+plane_x, plane_y, plane_z, 1.0f, 1.0f, -1.0f, 0.0f, -1.0f),

                // two small triangle
                Vertex(+plane_x + space_side, roof_top, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f),
                Vertex(+plane_x + space_side, roof_bottom, -plane_z, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f),
                Vertex(+plane_x + space_side, roof_bottom, plane_z, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f),

                Vertex(-plane_x - space_side, roof_top, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f),
                Vertex(-plane_x - space_side, roof_bottom, -plane_z, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f),
                Vertex(-plane_x - space_side, roof_bottom, plane_z, 1.0f, 1.0f, -1.0f, 0.0f, -1.0f),

        };

        VB->Lock(0, 0, (void **) &v, 0);

        for (int x = 0; i < end; i++, x++) {
            v[i] = vertexes[x];
        }

        VB->Unlock();
    }

    void createMount() {
        //
        // Function to create mountain
        //

        int i = any_cast<int>(vertex_indexes["base_mount"]);
        int l = any_cast<int>(vertex_length["base_mount"]);
        int end = i + l;

        Vertex vertexes[] = {
                // Mountain vertex
                Vertex(-700, plane_y, -200, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
                Vertex(-700, sky_height, -200, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
                Vertex(-700, sky_height, 200, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),

                Vertex(-700, plane_y, -200, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
                Vertex(-700, sky_height, 200, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),
                Vertex(-700, plane_y, 200, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),

        };

        VB->Lock(0, 0, (void **) &v, 0);

        for (int x = 0; i < end; i++, x++) {
            v[i] = vertexes[x];
        }

        VB->Unlock();
    }

    void createOuterWall() {
        //
        // Function to create outer wall
        //

        int i = any_cast<int>(vertex_indexes["outer_wall"]);
        int l = any_cast<int>(vertex_length["outer_wall"]);
        int end = i + l;

        Vertex vertexes[] = {
                // back wall vertex
                Vertex(-plane_x - space_side, plane_y, plane_z + space_front, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
                Vertex(-plane_x - space_side, plane_y + roof_height * 2, plane_z + space_front, 0.0f, 0.0f, -1.0f, 0.0f,
                       0.0f),
                Vertex(plane_x + space_side, plane_y + roof_height * 2, plane_z + space_front, 0.0f, 0.0f, -1.0f, 1.0f,
                       0.0f),

                Vertex(-plane_x - space_side, plane_y, plane_z + space_front, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
                Vertex(plane_x + space_side, plane_y + roof_height * 2, plane_z + space_front, 0.0f, 0.0f, -1.0f, 1.0f,
                       0.0f),
                Vertex(plane_x + space_side, plane_y, plane_z + space_front, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),

                // left wall vertex
                Vertex(-plane_x - space_side, plane_y, -plane_z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
                Vertex(-plane_x - space_side, plane_y + roof_height * 2, -plane_z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
                Vertex(-plane_x - space_side, plane_y + roof_height * 2, plane_z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),

                Vertex(-plane_x - space_side, plane_y, -plane_z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
                Vertex(-plane_x - space_side, plane_y + roof_height * 2, plane_z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),
                Vertex(-plane_x - space_side, plane_y, plane_z, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),

                // right wall vertex
                Vertex(+plane_z + space_side, plane_y, -plane_z, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f),
                Vertex(+plane_z + space_side, plane_y + roof_height * 2, -plane_z, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f),
                Vertex(+plane_z + space_side, plane_y + roof_height * 2, plane_z, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f),

                Vertex(+plane_z + space_side, plane_y, -plane_z, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f),
                Vertex(+plane_z + space_side, plane_y + roof_height * 2, plane_z, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f),
                Vertex(+plane_z + space_side, plane_y, plane_z, 1.0f, 1.0f, -1.0f, 0.0f, -1.0f),

        };

        VB->Lock(0, 0, (void **) &v, 0);

        for (int x = 0; i < end; i++, x++) {
            v[i] = vertexes[x];
        }

        VB->Unlock();
    }

    void createDoor(float delta, string mode) {
        //
        // Function to create or move garage door
        //

        if (mode.compare("move") == 0) {
            if (delta < 1.0f || delta > roof_height || (walk_position_left_right > plane_x) ||
                (walk_position_left_right < -plane_x))
                return;

            delta -= 1;

        } else
            delta = 0;

        int i = any_cast<int>(vertex_indexes["house_door"]);
        int l = any_cast<int>(vertex_length["house_door"]);
        int end = i + l;

        Vertex vertexes[] = {
                // door vertex
                Vertex(-plane_x, plane_y + delta, -plane_z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
                Vertex(-plane_x, roof_height - 8 + delta, -plane_z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
                Vertex(plane_x, roof_height - 8 + delta, -plane_z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),

                Vertex(-plane_x, plane_y + delta, -plane_z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
                Vertex(plane_x, roof_height - 8 + delta, -plane_z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),
                Vertex(plane_x, plane_y + delta, -plane_z, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
        };

        VB->Lock(0, 0, (void **) &v, 0);

        for (int x = 0; i < end; i++, x++) {
            v[i] = vertexes[x];
        }

        VB->Unlock();
    }

    void createDoor2() {
        //
        // Cunction to create out door
        //

        int i = any_cast<int>(vertex_indexes["house_door2"]);
        int l = any_cast<int>(vertex_length["house_door2"]);
        int end = i + l;

        float door_height = 7;
        float door_s_position = plane_z - 5;
        float door_e_position = door_s_position - 8;

        Vertex vertexes[] = {
                // door out
                Vertex(plane_x + space_side + 0.01, plane_y, door_e_position, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f),
                Vertex(plane_x + space_side + 0.01, roof_height - 8, door_e_position, 1.0f, 1.0f, -1.0f, 1.0f,
                       0.0f),
                Vertex(plane_x + space_side + 0.01, roof_height - 8, door_s_position, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f),

                Vertex(plane_x + space_side + 0.01, plane_y, door_e_position, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f),
                Vertex(plane_x + space_side + 0.01, roof_height - 8, door_s_position, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f),
                Vertex(plane_x + space_side + 0.01, plane_y, door_s_position, 1.0f, 1.0f, -1.0f, 0.0f, -1.0f),
        };

        VB->Lock(0, 0, (void **) &v, 0);

        for (int x = 0; i < end; i++, x++) {
            v[i] = vertexes[x];
        }

        VB->Unlock();
    }

    void createDoor3() {
        //
        // Function to create in-door
        //

        int i = any_cast<int>(vertex_indexes["house_door3"]);
        int l = any_cast<int>(vertex_length["house_door3"]);
        int end = i + l;

        float door_height = 7;
        float door_s_position = plane_z - 5;
        float door_e_position = door_s_position - 8;

        Vertex vertexes[] = {
                // door in
                Vertex(plane_x - 0.01, plane_y, door_e_position, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f),
                Vertex(plane_x - 0.01, roof_height - 8, door_e_position, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f),
                Vertex(plane_x - 0.01, roof_height - 8, door_s_position, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f),

                Vertex(plane_x - 0.01, plane_y, door_e_position, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f),
                Vertex(plane_x - 0.01, roof_height - 8, door_s_position, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f),
                Vertex(plane_x - 0.01, plane_y, door_s_position, 1.0f, 1.0f, -1.0f, 0.0f, -1.0f),
        };

        VB->Lock(0, 0, (void **) &v, 0);

        for (int x = 0; i < end; i++, x++) {
            v[i] = vertexes[x];
        }

        VB->Unlock();
    }

    void createWindow() {
        //
        // Function to create window
        //

        int i = any_cast<int>(vertex_indexes["house_window"]);
        int l = any_cast<int>(vertex_length["house_window"]);
        int end = i + l;

        Vertex vertexes[] = {
                // window vertex
                Vertex(-plane_x - space_side, roof_height - 8, -plane_z - space_front, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
                Vertex(-plane_x - space_side, roof_bottom, -plane_z - space_front, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
                Vertex(plane_x + space_side, roof_bottom, -plane_z - space_front, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),

                Vertex(-plane_x - space_side, roof_height - 8, -plane_z - space_front, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
                Vertex(plane_x + space_side, roof_bottom, -plane_z - space_front, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),
                Vertex(plane_x + space_side, roof_height - 8, -plane_z - space_front, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
        };

        VB->Lock(0, 0, (void **) &v, 0);

        for (int x = 0; i < end; i++, x++) {
            v[i] = vertexes[x];
        }

        VB->Unlock();
    }

    void createFont() {
        //
        // Function to create garage font
        //

        string strFaceName = "Calibri";
        string str = "GARAGE";

        // Get a handle to a device context.
        HDC hdc = CreateCompatibleDC(0);
        HFONT hFont;
        HFONT hFontOld;

        // Describe the font we want.
        LOGFONT lf;
        ZeroMemory(&lf, sizeof(LOGFONT));

        lf.lfHeight = 1000;    // in logical units
        lf.lfWidth = 500;    // in logical units
        lf.lfEscapement = 0;
        lf.lfOrientation = 500;
        lf.lfWeight = 1000;   // boldness, range 0(light) - 1000(bold)
        lf.lfItalic = false;
        lf.lfUnderline = false;
        lf.lfStrikeOut = true;
        lf.lfCharSet = DEFAULT_CHARSET;
        lf.lfOutPrecision = 100;
        lf.lfClipPrecision = 0;
        lf.lfQuality = 0;
        lf.lfPitchAndFamily = 0;

        copy(strFaceName.begin(), strFaceName.end(), lf.lfFaceName);

        // Create the font and select it with the device context.
        hFont = CreateFontIndirect(&lf);
        hFontOld = (HFONT) SelectObject(hdc, hFont);

        // Create the text mesh based on the selected font in the HDC.
        for (int i = 0; i < str.length(); i++)
            D3DXCreateTextA(Device, hdc, str.substr(i, 1).c_str(), 0.001f, 0.4f, &garage_title_mesh[i], 0, 0);

        // Restore the old font and free the acquired HDC.
        SelectObject(hdc, hFontOld);
        DeleteObject(hFont);
        DeleteDC(hdc);

        // resize
        float scaler = 3.0f;
        D3DXMATRIX scale = {
                scaler, 0.0f, 0.0f, 0.0f,
                0.0f, scaler, 0.0f, 0.0f,
                0.0f, 0.0f, scaler, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
        };

        D3DXMATRIX temp1, temp2;
        vector<D3DXMATRIX> text;

        // set the position of text
        float x_start = 20, x_end = 28, y = 33, z = -30.01f;
        float height_size = 2.5;

        for (int i = 0; i < str.length(); i++) {
            D3DXMatrixTranslation(&temp2, x_start + ((x_end - x_start) / str.length()) * i, y - height_size * i,
                                  z);
            text.push_back(temp2);

            D3DXMatrixMultiply(&temp1, &scale, &text[i]);
            out_matrix_text.push_back(temp1);
        }
    }

    void createSpace() {
        //
        // Function to create space between walls
        //

        int i = any_cast<int>(vertex_indexes["house_space"]);
        int l = any_cast<int>(vertex_length["house_space"]);
        int end = i + l;

        Vertex vertexes[] = { // + roof_height * 2
                // space vertex right
                Vertex(plane_x, plane_y + roof_height * 2, -plane_z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
                Vertex(plane_x, plane_y, -plane_z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
                Vertex(plane_x + space_side, plane_y, -plane_z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),

                Vertex(plane_x, plane_y + roof_height * 2, -plane_z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
                Vertex(plane_x + space_side, plane_y, -plane_z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),
                Vertex(plane_x + space_side, plane_y + roof_height * 2, -plane_z, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),

                // space vertex left
                Vertex(-plane_x, plane_y + roof_height * 2, -plane_z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
                Vertex(-plane_x, plane_y, -plane_z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
                Vertex(-plane_x - space_side, plane_y, -plane_z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),

                Vertex(-plane_x, plane_y + roof_height * 2, -plane_z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
                Vertex(-plane_x - space_side, plane_y, -plane_z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),
                Vertex(-plane_x - space_side, plane_y + roof_height * 2, -plane_z, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
        };

        VB->Lock(0, 0, (void **) &v, 0);

        for (int x = 0; i < end; i++, x++) {
            v[i] = vertexes[x];
        }

        VB->Unlock();
    }
};

// World Object
World world;

class Human {
public:
    // array to keep all object's default positions
    float object_positions[human_object_count][3] = {
            {0.0f,   1.2f,   0.0f}, // hat
            {0.0f,   0.0f,   0.0f}, // head
            {0.3f,   0.0f,   -0.9}, // eyes
            {-0.3f,  0.0f,   -0.9f},
            {0.0f,   -0.0f,  -0.9f}, // nose

            {1.0f,   0.0f,   0.0f}, // ears
            {-1.0f,  0.0f,   0.0f},
            {0.0f,   -0.6f,  -0.8f}, // mouth
            {0.0f,   -3.0f,  0.0f}, // body
            {-3.5f,  -2.0f,  0.0f}, // hands

            {3.5f,   -2.0f,  0.0f},
            {-1.2f,  -5.8f,  0.0f}, // legs
            {1.2f,   -5.8f,  0.0f},
            {-1.2f,  -7.35f, -0.1f}, // feet
            {1.2f,   -7.35f, -0.1f},

            {-1.93f, -5.05f, +0.10f}, // fingers
            {-1.99f, -5.05f, +0.05f},
            {-2.0f,  -5.05f, -0.02f},
            {-1.96f, -5.05f, -0.08f},
            {-1.90f, -5.05f, -0.10f},

            {1.93f,  -5.05f, +0.10f}, // fingers
            {1.99f,  -5.05f, +0.05f},
            {2.0f,   -5.05f, -0.02f},
            {1.96f,  -5.05f, -0.08f},
            {1.90f,  -5.05f, -0.10f},

            //{0.0f,   -3.0f,  0.0f}, // body
            {-1.8f,  -2.0f,  0.0f}, // hand
            {1.8f,   -2.0f,  0.0f},  // hand
    };

    void createIndexObjects() {
        //
        // Function to create default index of objects,
        //
        // Note: in this way we are not have to change indexes of object when we add a new object.
        //

        human_staff["hat"] = 0; // start of hat
        human_staff["head"] = 1;
        human_staff["eye"] = 2;  // two
        human_staff["nose"] = 4;
        human_staff["ear"] = 5; // two
        human_staff["mouth"] = 7;
        human_staff["body"] = 8;
        human_staff["hand"] = 9; // four
        human_staff["leg"] = 13; // two
        human_staff["foot"] = 15; // two
        human_staff["finger"] = 17; // ten
    }

    D3DXMATRIX translate(D3DXMATRIX base, int objCount, float x, float y, float z) {
        //
        // Function to translate a matrix to given position,
        //
        // Note: The DirectX default function for translating isn't working correctly sometimes,
        // so we need to create our own.
        //

        D3DXMATRIX translateMatrix(1, 0, 0, 0,
                                   0, 1, 0, 0,
                                   0, 0, 1, 0,
                                   x, y, z, 1);

        D3DXMATRIX result = base * translateMatrix;

        // update the object position
        object_positions[objCount][0] += x;
        object_positions[objCount][1] += y;

        return result;
    }

    D3DXMATRIX positionFinder(float length, float lengthBefore, float elementPosition[], float degreeOrigin,
                              float degreeFinal, string directionOrigin, string directionFinal) {
        //
        // Function to find position of object after translating and rotating in given positions
        //

        D3DXMATRIX tra2Origin(1, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 0, 1, 0,
                              0, 0, 0, 1);

        D3DXMATRIX rotInOrigin;
        if (directionOrigin == "x")
            rotInOrigin = D3DXMATRIX(1, 0, 0, 0,
                                     0, cosf(degreeOrigin), sinf(degreeOrigin), 0,
                                     0, -sinf(degreeOrigin), cosf(degreeOrigin), 0,
                                     0, 0, 0, 1);

        else if (directionOrigin == "y")
            rotInOrigin = D3DXMATRIX(cosf(degreeOrigin), 0, -sinf(degreeOrigin), 0,
                                     0, 1, 0, 0,
                                     sinf(degreeOrigin), 0, cosf(degreeOrigin), 0,
                                     0, 0, 0, 1);

        else
            rotInOrigin = D3DXMATRIX(cosf(degreeOrigin), sinf(degreeOrigin), 0, 0,
                                     -sinf(degreeOrigin), cosf(degreeOrigin), 0, 0,
                                     0, 0, 1, 0,
                                     0, 0, 0, 1);

        D3DXMATRIX tra2Length(1, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 0, 1, 0,
                              0, -lengthBefore - length / 2, 0, 1);

        D3DXMATRIX rotFromLength;
        if (directionFinal == "x")
            rotFromLength = D3DXMATRIX(1, 0, 0, 0,
                                       0, cosf(degreeFinal), sinf(degreeFinal), 0,
                                       0, -sinf(degreeFinal), cosf(degreeFinal), 0,
                                       0, 0, 0, 1);

        else if (directionFinal == "y")
            rotFromLength = D3DXMATRIX(cosf(degreeFinal), 0, -sinf(degreeFinal), 0,
                                       0, 1, 0, 0,
                                       sinf(degreeFinal), 0, cosf(degreeFinal), 0,
                                       0, 0, 0, 1);

        else
            rotFromLength = D3DXMATRIX(cosf(degreeFinal), sinf(degreeFinal), 0, 0,
                                       -sinf(degreeFinal), cosf(degreeFinal), 0, 0,
                                       0, 0, 1, 0,
                                       0, 0, 0, 1);

        D3DXMATRIX tra2Final(1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                             elementPosition[0], elementPosition[1] + lengthBefore + length / 2, elementPosition[2], 1);

        D3DXMATRIX result = tra2Origin * rotInOrigin * tra2Length * rotFromLength * tra2Final;
        
        return result;
    }

    void drawHat() {
        //
        // Function to create a hat,
        // Note : All duplicated comments in other functions moved to this one !
        //

        // find index of this object
        int object_index = any_cast<int>(human_staff["hat"]);

        // create the object !
        D3DXCreateTorus(Device,
                        1.0f, // inner radius
                        0.5f, // outer radius
                        100, // sides
                        100,  // rings
                        &human_objects[object_index],
                        0);

        // Build world matrices - position the objects in world space_side.
        D3DXMatrixTranslation(&human_worlds[object_index], 0.0f, 1.2f, 0.0f);

        // Set up the object's materials.
        human_materials[object_index] = d3d::RED_MTRL;
        human_materials[object_index].Specular = d3d::WHITE * 0.2f;

        // rotate
        D3DXMATRIX hat, hatRotate;
        D3DXMatrixRotationX(&hatRotate, 1.5f);
        D3DXMatrixMultiply(&hat, &hatRotate, &human_worlds[object_index]);

        // save changed matrix to human_worlds matrix
        human_worlds[object_index] = hat;
    }

    void drawHead() {
        //
        // Function to create a head
        //

        // find index of this object
        int object_index = any_cast<int>(human_staff["head"]);

        D3DXCreateSphere(Device,
                         1.0f, // Radius
                         100, // Slices
                         100, // Stacks
                         &human_objects[object_index], // ppMesh
                         0);

        D3DXMatrixTranslation(&human_worlds[object_index], 0.0f, 0.0f, 0.0f);

        human_materials[object_index] = d3d::SKIN_MTRL;
        human_materials[object_index].Specular = d3d::WHITE * 0.2f;
    }

    void drawEyes() {
        //
        // Function to create two eyes
        //

        // find index of these objects
        int object_index = any_cast<int>(human_staff["eye"]);

        D3DXCreateSphere(Device,
                         0.1f, // Radius
                         100, // Slices
                         100, // Stacks
                         &human_objects[object_index], // ppMesh
                         0);

        D3DXCreateSphere(Device,
                         0.1f, // Radius
                         100, // Slices
                         100, // Stacks
                         &human_objects[object_index + 1], // ppMesh
                         0);

        D3DXMatrixTranslation(&human_worlds[object_index], 0.3f, 0.0f, -0.9f);  // eyes
        D3DXMatrixTranslation(&human_worlds[object_index + 1], -0.3f, 0.0f, -0.9f); //eyes

        human_materials[object_index] = d3d::GREEN_MTRL;
        human_materials[object_index].Specular = d3d::WHITE * 0.2f;
        human_materials[object_index + 1] = d3d::GREEN_MTRL;
        human_materials[object_index + 1].Specular = d3d::WHITE * 0.2f;
    }

    void drawNose() {
        //
        // Function to create a nose
        //

        // find index of this object
        int object_index = any_cast<int>(human_staff["nose"]);

        D3DXCreateCylinder(Device,
                           0.08f, // radius at negative z end
                           0.08f, // radius at positive z end
                           0.7f,  // length of cylinder
                           100, // slices
                           100, // stacks
                           &human_objects[object_index],
                           0);

        D3DXMatrixTranslation(&human_worlds[object_index], 0.0f, -0.0f, -0.9f);// nose

        human_materials[object_index] = d3d::SKIN_MTRL;
        human_materials[object_index].Specular = d3d::WHITE * 0.2f;

        // rotate
        D3DXMATRIX nose, noseRotate;
        D3DXMatrixRotationX(&noseRotate, -1.2f);
        D3DXMatrixMultiply(&nose, &noseRotate, &human_worlds[object_index]);

        // save changed matrix to human_worlds matrix
        human_worlds[object_index] = nose;
    }

    void drawEars() {
        //
        // Function to create two ears
        //

        // find index of these objects
        int object_index = any_cast<int>(human_staff["ear"]);

        D3DXCreateCylinder(Device,
                           0.1f, // radius at negative z end
                           0.2f, // radius at positive z end
                           0.1f,  // length of cylinder
                           100, // slices
                           100, // stacks
                           &human_objects[object_index],
                           0);

        D3DXCreateCylinder(Device,
                           0.1f, // radius at negative z end
                           0.2f, // radius at positive z end
                           0.1f,  // length of cylinder
                           100, // slices
                           100, // stacks
                           &human_objects[object_index + 1],
                           0);

        D3DXMatrixTranslation(&human_worlds[object_index], 1.0f, 0.0f, 0.0f);
        D3DXMatrixTranslation(&human_worlds[object_index + 1], -1.0f, 0.0f, 0.0f);

        human_materials[object_index] = d3d::SKIN_MTRL;
        human_materials[object_index].Specular = d3d::WHITE * 0.2f;
        human_materials[object_index + 1] = d3d::SKIN_MTRL;
        human_materials[object_index + 1].Specular = d3d::WHITE * 0.2f;

        // rotate
        D3DXMATRIX ear1, ear2, earRotate1, earRotate2;
        D3DXMatrixRotationY(&earRotate1, 0.7f);
        D3DXMatrixRotationY(&earRotate2, -0.7f);

        D3DXMatrixMultiply(&ear1, &earRotate1, &human_worlds[object_index + 1]);
        D3DXMatrixMultiply(&ear2, &earRotate2, &human_worlds[object_index]);

        // save changed matrix to human_worlds matrix
        human_worlds[object_index + 1] = ear1;
        human_worlds[object_index] = ear2;
    }

    void drawMouth() {
        //
        // Function to create a mouth
        //

        // find index of this object
        int object_index = any_cast<int>(human_staff["mouth"]);

        D3DXCreateTorus(Device,
                        0.1f, // inner radius
                        0.15f, // outer radius
                        100, // sides
                        100,  // rings
                        &human_objects[object_index],
                        0);

        D3DXMatrixTranslation(&human_worlds[object_index], 0.0f, -0.6f, -0.8f); // mouth

        human_materials[object_index] = d3d::BLACK_MTRL;
        human_materials[object_index].Specular = d3d::WHITE * 0.2f;
    }

    void drawBody() {
        //
        // Function to create a body
        //

        // find index of this object
        int object_index = any_cast<int>(human_staff["body"]);

        D3DXCreateTorus(Device,
                        1.1f, // inner radius
                        1.0f, // outer radius
                        100, // sides
                        100,  // rings
                        &human_objects[object_index],
                        0);


        D3DXMatrixTranslation(&human_worlds[object_index], 0.0f, -3.0f, 0.0f);  // body

        human_materials[object_index] = d3d::GREEN_MTRL;
        human_materials[object_index].Specular = d3d::WHITE * 0.2f;
    }

    void drawHand() {
        //
        // Function to create two hands and two shoulders
        //

        // find index of these objects
        int object_index = any_cast<int>(human_staff["hand"]);

        float length = 3;

        D3DXCreateCylinder(Device,
                           0.35f, // radius at negative z end
                           0.15f, // radius at positive z end
                           length,  // length of cylinder
                           100, // slices
                           100, // stacks
                           &human_objects[object_index],
                           0);

        D3DXCreateCylinder(Device,
                           0.35f, // radius at negative z end
                           0.15f, // radius at positive z end
                           length,  // length of cylinder
                           100, // slices
                           100, // stacks
                           &human_objects[object_index + 1],
                           0);


        D3DXCreateSphere(Device,
                         0.5f, // Radius
                         100, // Slices
                         100, // Stacks
                         &human_objects[object_index + 2], // ppMesh
                         0);

        D3DXCreateSphere(Device,
                         0.5f, // Radius
                         100, // Slices
                         100, // Stacks
                         &human_objects[object_index + 3], // ppMesh
                         0);

        D3DXMatrixTranslation(&human_worlds[object_index], -3.5f, -2.0f, 0.0f);
        D3DXMatrixTranslation(&human_worlds[object_index + 1], 2.5f, -2.0f, 0.0f);
        D3DXMatrixTranslation(&human_worlds[object_index + 2], -1.8f, -2.0f, 0.0f);
        D3DXMatrixTranslation(&human_worlds[object_index + 3], +1.8f, -2.0f, 0.0f);

        human_materials[object_index] = d3d::SKIN_MTRL;
        human_materials[object_index].Specular = d3d::WHITE * 0.2f;
        human_materials[object_index + 1] = d3d::SKIN_MTRL;
        human_materials[object_index + 1].Specular = d3d::WHITE * 0.2f;
        human_materials[object_index + 2] = d3d::BLUE_MTRL;
        human_materials[object_index + 2].Specular = d3d::WHITE * 0.2f;
        human_materials[object_index + 3] = d3d::BLUE_MTRL;
        human_materials[object_index + 3].Specular = d3d::WHITE * 0.2f;

        human_worlds[object_index] = positionFinder(length, 0, new float[]{-1.9f, -3.5f, 0.0f}, 3.14 / 2, 0, "x", "y");
        human_worlds[object_index + 1] = positionFinder(length, 0, new float[]{+1.9f, -3.5f, 0.0f}, 3.14 / 2, 0, "x",
                                                        "y");
    }

    void drawLeg() {
        //
        // Function to create two legs
        //

        // find index of these objects
        int object_index = any_cast<int>(human_staff["leg"]);

        float length = 3;

        D3DXCreateCylinder(Device,
                           0.4f, // radius at negative z end
                           0.25f, // radius at positive z end
                           length,  // length of cylinder
                           100, // slices
                           100, // stacks
                           &human_objects[object_index],
                           0);

        D3DXCreateCylinder(Device,
                           0.4f, // radius at negative z end
                           0.25f, // radius at positive z end
                           length,  // length of cylinder
                           100, // slices
                           100, // stacks
                           &human_objects[object_index + 1],
                           0);

        human_materials[object_index] = d3d::VIVID2_MTRL;
        human_materials[object_index].Specular = d3d::WHITE * 0.2f;
        human_materials[object_index + 1] = d3d::VIVID2_MTRL;
        human_materials[object_index + 1].Specular = d3d::WHITE * 0.2f;

        human_worlds[object_index] = positionFinder(length, 0, new float[]{-1.2f, -5.8f, 0.0f}, 3.14 / 2, 0, "x", "x");
        human_worlds[object_index + 1] = positionFinder(length, 0, new float[]{1.2f, -5.8f, 0.0f}, 3.14 / 2, 0, "x",
                                                        "x");
    }

    void drawFoot() {
        //
        // Function to create feet
        //

        // find index of these objects
        int object_index = any_cast<int>(human_staff["foot"]);

        float length = 0.15f;

        D3DXCreateCylinder(Device,
                           0.35f, // radius at negative z end
                           0.4f, // radius at positive z end
                           length,  // length of cylinder
                           100, // slices
                           100, // stacks
                           &human_objects[object_index],
                           0);

        D3DXCreateCylinder(Device,
                           0.35f, // radius at negative z end
                           0.4f, // radius at positive z end
                           length,  // length of cylinder
                           100, // slices
                           100, // stacks
                           &human_objects[object_index + 1],
                           0);

        human_materials[object_index] = d3d::BLUE_MTRL;
        human_materials[object_index].Specular = d3d::WHITE * 0.2f;
        human_materials[object_index + 1] = d3d::BLUE_MTRL;
        human_materials[object_index + 1].Specular = d3d::WHITE * 0.2f;

        D3DXMatrixTranslation(&human_worlds[object_index], -1.2f, -7.35f, -0.1f);
        D3DXMatrixTranslation(&human_worlds[object_index + 1], 1.2f, -7.35f, -0.1f);

        D3DXMATRIX foot1, foot2, footRotate1, footRotate2;
        D3DXMatrixRotationX(&footRotate1, 3.14f / 2);
        D3DXMatrixMultiply(&foot1, &footRotate1, &human_worlds[object_index]);
        D3DXMatrixRotationX(&footRotate2, 3.14f / 2);
        D3DXMatrixMultiply(&foot2, &footRotate2, &human_worlds[object_index + 1]);

        human_worlds[object_index] = foot1;
        human_worlds[object_index + 1] = foot2;
    }

    void drawFingers() {
        //
        // Function to create 10 fingers
        //

        // find index of these objects
        int object_index = any_cast<int>(human_staff["finger"]);

        // small
        D3DXCreateCylinder(Device,
                           0.04f, // radius at negative z end
                           0.01f, // radius at positive z end
                           0.15f,  // length of cylinder
                           100, // slices
                           100, // stacks
                           &human_objects[object_index],
                           0);

        // ring
        D3DXCreateCylinder(Device,
                           0.04f, // radius at negative z end
                           0.01f, // radius at positive z end
                           0.2f,  // length of cylinder
                           100, // slices
                           100, // stacks
                           &human_objects[object_index + 1],
                           0);

        // middle
        D3DXCreateCylinder(Device,
                           0.04f, // radius at negative z end
                           0.01f, // radius at positive z end
                           0.25f,  // length of cylinder
                           100, // slices
                           100, // stacks
                           &human_objects[object_index + 2],
                           0);

        // point
        D3DXCreateCylinder(Device,
                           0.04f, // radius at negative z end
                           0.01f, // radius at positive z end
                           0.23f,  // length of cylinder
                           100, // slices
                           100, // stacks
                           &human_objects[object_index + 3],
                           0);

        // thumb
        D3DXCreateCylinder(Device,
                           0.04f, // radius at negative z end
                           0.01f, // radius at positive z end
                           0.21f,  // length of cylinder
                           100, // slices
                           100, // stacks
                           &human_objects[object_index + 4],
                           0);

        ////////////////////////////////////

        // small
        D3DXCreateCylinder(Device,
                           0.04f, // radius at negative z end
                           0.01f, // radius at positive z end
                           0.15f,  // length of cylinder
                           100, // slices
                           100, // stacks
                           &human_objects[object_index + 5],
                           0);

        // ring
        D3DXCreateCylinder(Device,
                           0.04f, // radius at negative z end
                           0.01f, // radius at positive z end
                           0.2f,  // length of cylinder
                           100, // slices
                           100, // stacks
                           &human_objects[object_index + 6],
                           0);

        // middle
        D3DXCreateCylinder(Device,
                           0.04f, // radius at negative z end
                           0.01f, // radius at positive z end
                           0.25f,  // length of cylinder
                           100, // slices
                           100, // stacks
                           &human_objects[object_index + 7],
                           0);

        // point
        D3DXCreateCylinder(Device,
                           0.04f, // radius at negative z end
                           0.01f, // radius at positive z end
                           0.23f,  // length of cylinder
                           100, // slices
                           100, // stacks
                           &human_objects[object_index + 8],
                           0);

        // thumb
        D3DXCreateCylinder(Device,
                           0.04f, // radius at negative z end
                           0.01f, // radius at positive z end
                           0.21f,  // length of cylinder
                           100, // slices
                           100, // stacks
                           &human_objects[object_index + 9],
                           0);


        D3DXMatrixTranslation(&human_worlds[object_index], -1.93f, -5.05f, +0.10f);
        D3DXMatrixTranslation(&human_worlds[object_index + 1], -1.99f, -5.05f, +0.05f);
        D3DXMatrixTranslation(&human_worlds[object_index + 2], -2.0f, -5.05f, -0.02f);
        D3DXMatrixTranslation(&human_worlds[object_index + 3], -1.96f, -5.05f, -0.08f);
        D3DXMatrixTranslation(&human_worlds[object_index + 4], -1.90f, -5.05f, -0.10f);

        D3DXMatrixTranslation(&human_worlds[object_index + 5], 1.93f, -5.05f, +0.10f);
        D3DXMatrixTranslation(&human_worlds[object_index + 6], 1.99f, -5.05f, +0.05f);
        D3DXMatrixTranslation(&human_worlds[object_index + 7], 2.0f, -5.05f, -0.02f);
        D3DXMatrixTranslation(&human_worlds[object_index + 8], 1.96f, -5.05f, -0.08f);
        D3DXMatrixTranslation(&human_worlds[object_index + 9], 1.90f, -5.05f, -0.10f);

        human_materials[object_index].Specular = d3d::WHITE * 0.2f;
        human_materials[object_index + 1].Specular = d3d::WHITE * 0.2f;
        human_materials[object_index + 2].Specular = d3d::WHITE * 0.2f;
        human_materials[object_index + 3].Specular = d3d::WHITE * 0.2f;
        human_materials[object_index + 4].Specular = d3d::WHITE * 0.2f;
        human_materials[object_index + 5].Specular = d3d::WHITE * 0.2f;
        human_materials[object_index + 6].Specular = d3d::WHITE * 0.2f;
        human_materials[object_index + 7].Specular = d3d::WHITE * 0.2f;
        human_materials[object_index + 8].Specular = d3d::WHITE * 0.2f;
        human_materials[object_index + 9] = d3d::SKIN_MTRL; // small
        human_materials[object_index + 1] = d3d::SKIN_MTRL; // ring
        human_materials[object_index + 2] = d3d::SKIN_MTRL; // middle
        human_materials[object_index + 3] = d3d::SKIN_MTRL; // point
        human_materials[object_index + 4] = d3d::SKIN_MTRL; // thumb
        human_materials[object_index + 5] = d3d::SKIN_MTRL; // small
        human_materials[object_index + 6] = d3d::SKIN_MTRL; // ring
        human_materials[object_index + 7] = d3d::SKIN_MTRL; // middle
        human_materials[object_index + 8] = d3d::SKIN_MTRL; // point
        human_materials[object_index + 9] = d3d::SKIN_MTRL; // thumb

        D3DXMATRIX finger1, finger2, finger3, finger4, finger5, finger6, finger7, finger8, finger9, finger10,
                fingerRotate1, fingerRotate2, fingerRotate3, fingerRotate4, fingerRotate5, fingerRotate6, fingerRotate7, fingerRotate8, fingerRotate9, fingerRotate10;

        D3DXMatrixRotationX(&fingerRotate1, +3.14f / 2);
        D3DXMatrixMultiply(&finger1, &fingerRotate1, &human_worlds[object_index]);
        D3DXMatrixRotationX(&fingerRotate2, +3.14f / 2);
        D3DXMatrixMultiply(&finger2, &fingerRotate2, &human_worlds[object_index + 1]);
        D3DXMatrixRotationX(&fingerRotate3, +3.14f / 2);
        D3DXMatrixMultiply(&finger3, &fingerRotate3, &human_worlds[object_index + 2]);
        D3DXMatrixRotationX(&fingerRotate4, +3.14f / 2);
        D3DXMatrixMultiply(&finger4, &fingerRotate4, &human_worlds[object_index + 3]);
        D3DXMatrixRotationX(&fingerRotate5, +3.14f / 2);
        D3DXMatrixMultiply(&finger5, &fingerRotate5, &human_worlds[object_index + 4]);

        D3DXMatrixRotationX(&fingerRotate6, +3.14f / 2);
        D3DXMatrixMultiply(&finger6, &fingerRotate6, &human_worlds[object_index + 5]);
        D3DXMatrixRotationX(&fingerRotate7, +3.14f / 2);
        D3DXMatrixMultiply(&finger7, &fingerRotate7, &human_worlds[object_index + 6]);
        D3DXMatrixRotationX(&fingerRotate8, +3.14f / 2);
        D3DXMatrixMultiply(&finger8, &fingerRotate8, &human_worlds[object_index + 7]);
        D3DXMatrixRotationX(&fingerRotate9, +3.14f / 2);
        D3DXMatrixMultiply(&finger9, &fingerRotate9, &human_worlds[object_index + 8]);
        D3DXMatrixRotationX(&fingerRotate10, +3.14f / 2);
        D3DXMatrixMultiply(&finger10, &fingerRotate10, &human_worlds[object_index + 9]);

        human_worlds[object_index] = finger1;
        human_worlds[object_index + 1] = finger2;
        human_worlds[object_index + 2] = finger3;
        human_worlds[object_index + 3] = finger4;
        human_worlds[object_index + 4] = finger5;
        human_worlds[object_index + 5] = finger6;
        human_worlds[object_index + 6] = finger7;
        human_worlds[object_index + 7] = finger8;
        human_worlds[object_index + 8] = finger9;
        human_worlds[object_index + 9] = finger10;
    }
};

// Human Object
Human human;

class Accessory {
public:
    D3DXMATRIX ship_matrix;

    void createShip() {
        //
        // Function to create and fit ship
        //

        HRESULT hr = 0;

        // Load the XFile data.
        ID3DXBuffer *adjBuffer = 0;
        ID3DXBuffer *mtrlBuffer = 0;
        DWORD numMtrls = 0;

        hr = D3DXLoadMeshFromXA(
                ".\\XFile\\bigship.x",
                D3DXMESH_MANAGED,
                Device,
                &adjBuffer,
                &mtrlBuffer,
                0,
                &numMtrls,
                &ship_mesh);


        // Extract the materials, and load textures.
        if (mtrlBuffer != 0 && numMtrls != 0) {
            D3DXMATERIAL *mtrls = (D3DXMATERIAL *) mtrlBuffer->GetBufferPointer();

            for (int i = 0; i < numMtrls; i++) {
                // the MatD3D property doesn't have an ambient value set when its loaded, so set it now:
                mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

                // save the ith material
                ship_materials.push_back(mtrls[i].MatD3D);

                // check if the ith material has an associative texture
                if (mtrls[i].pTextureFilename != 0) {
                    // yes, load the texture for the ith subset
                    IDirect3DTexture9 *tex = 0;
                    D3DXCreateTextureFromFileA(
                            Device,
                            mtrls[i].pTextureFilename,
                            &tex);

                    // save the loaded texture
                    ship_textures.push_back(tex);

                } else {
                    // no texture for the ith subset
                    ship_textures.push_back(0);
                }
            }
        }

        d3d::Release<ID3DXBuffer *>(mtrlBuffer); // done w/ buffer

        // Optimize the mesh.
        hr = ship_mesh->OptimizeInplace(
                D3DXMESHOPT_ATTRSORT |
                D3DXMESHOPT_COMPACT |
                D3DXMESHOPT_VERTEXCACHE,
                (DWORD *) adjBuffer->GetBufferPointer(),
                0, 0, 0);

        d3d::Release<ID3DXBuffer *>(adjBuffer); // done w/ buffer


        //
        // scale and translate the ship
        //
        D3DXMATRIX scaling, translating;

        D3DXMatrixScaling(&scaling, 4.0f, 4.0f, 4.0f);
        D3DXMatrixTranslation(&translating, 60.0f, 20.0f, 0.0f);

        D3DXMatrixMultiply(&ship_matrix, &scaling, &translating);
    }
};

// Accessory Object
Accessory accessory;
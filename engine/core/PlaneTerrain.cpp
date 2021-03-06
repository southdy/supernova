#include "PlaneTerrain.h"

#include "platform/Log.h"
#include "render/ObjectRender.h"

using namespace Supernova;

PlaneTerrain::PlaneTerrain(): Mesh() {
    primitiveType = S_PRIMITIVE_TRIANGLES_STRIP;
}

PlaneTerrain::PlaneTerrain(float width, float depth): PlaneTerrain() {
    this->width = width;
    this->depth = depth;
}

PlaneTerrain::~PlaneTerrain() {
}

bool PlaneTerrain::load(){

    vertices.push_back(Vector3(0, 0, 0));
    vertices.push_back(Vector3(0, 0, depth));
    vertices.push_back(Vector3(width, 0, 0));
    vertices.push_back(Vector3(width, 0, depth));

    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));

    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));

    return Mesh::load();
}

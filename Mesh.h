#pragma once
#include <vector>
#include "Math.h"

struct Vertex {
    Vector2 position;
    Vector2 texCoord;
    Vector2 normal;

    // 用于顶点去重比较
    bool operator==(const Vertex& other) const {
        return position == other.position &&
            texCoord == other.texCoord &&
            normal == other.normal;
    }
};
struct FaceIndex {
    int v;   // 顶点索引
    int vt;  // 纹理索引 (-1 表示无)
    int vn;  // 法线索引 (-1 表示无)
};

class Mesh {
    std::vector<Vector3> positions;
    std::vector<Vector3> texCoords;
    std::vector<Vector3> normals;

};
#pragma once

#include "../VecMath.h"

struct Triangle
{
    Vector3 p0;
    Vector3 p1;
    Vector3 p2;
    Vector3 normal;

    void ComputeNormal() {
        normal = Cross(p1 - p0, p2 - p0).Normalized();
    }
};
#include "pch.h"
#include "func.h"

Vec3 cross(const Vec3& V1, const Vec3& V2)
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(&V1);
    XMVECTOR v2 = XMLoadFloat3(&V2);
    XMVECTOR R = XMVector3Cross(v1, v2);

    Vec3 result;
    XMStoreFloat3(&result, R);
    return result;
}
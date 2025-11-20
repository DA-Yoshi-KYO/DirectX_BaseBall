#include "Collision.h"
#include "Geometory.h"
#undef min

Collision::Result Collision::Hit(Info a, Info b)
{
	Result out = {};
	if (a.type == b.type)
	{
		switch (a.type)
		{
		case eBox: out = Hit(a.box, b.box);	break;
		case eSphere: out = Hit(a.sphere, b.sphere); break;
		}
	}
    else
    {
        switch (a.type)
        {
        case eNone:
            break;
        case eBox:
            break;
        case eSphere:
            break;
        case ePlane:
            if (b.type == eLine) out = Hit(a.plane, b.line);
            if (b.type == eRay) out = Hit(a.plane, b.ray, FLT_MAX);
            break;
        case eRay:
            if (b.type == ePlane) out = Hit(b.plane, a.ray, FLT_MAX);
            if (b.type == eTriangle) out = Hit(a.ray, b.triangle);
            break;
        case eLine:
            if (b.type == ePlane) out = Hit(b.plane, a.line);
            if (b.type == eTriangle) out = Hit(a.line, b.triangle);
            break;
        case ePoint:
            if (b.type == eTriangle) out = Hit(a.point, b.triangle);
            break;
        case eTriangle:
            if (b.type == ePoint) out = Hit(b.point, a.triangle);
            if (b.type == eRay) out = Hit(b.ray, a.triangle);
            if (b.type == eLine) out = Hit(b.line, a.triangle);
            break;
        }
    }

	return out;
}

Collision::Result Collision::Hit(Box a, Box b)
{
	Result out = {};

	//VECTOR型に変換
	DirectX::XMVECTOR vPosA = DirectX::XMLoadFloat3(&a.center);
	DirectX::XMVECTOR vPosB = DirectX::XMLoadFloat3(&b.center);
	DirectX::XMVECTOR vSizeA = DirectX::XMLoadFloat3(&a.size);
	DirectX::XMVECTOR vSizeB = DirectX::XMLoadFloat3(&b.size);

	//ボックスの半分のサイズを取得
	vSizeA = DirectX::XMVectorScale(vSizeA, 0.5f);
	vSizeB = DirectX::XMVectorScale(vSizeB, 0.5f);

	//ボックスの各軸の最大値、最小値を取得
	DirectX::XMVECTOR vMaxA = DirectX::XMVectorAdd(vPosA, vSizeA);
	DirectX::XMVECTOR vMinA = DirectX::XMVectorSubtract(vPosA, vSizeA);
	DirectX::XMVECTOR vMaxB = DirectX::XMVectorAdd(vPosB, vSizeB);
	DirectX::XMVECTOR vMinB = DirectX::XMVectorSubtract(vPosB, vSizeB);
	DirectX::XMFLOAT3 maxA, minA, maxB, minB;
	DirectX::XMStoreFloat3(&maxA, vMaxA);
	DirectX::XMStoreFloat3(&minA, vMinA);
	DirectX::XMStoreFloat3(&maxB, vMaxB);
	DirectX::XMStoreFloat3(&minB, vMinB);

	//ボックスの面同士の関係から当たり判定を行う
	out.isHit = false;
	if (maxA.x >= minB.x && minA.x <= maxB.x) {
		if (maxA.y >= minB.y && minA.y <= maxB.y) {
			if (maxA.z >= minB.z && minA.z <= maxB.z) {
				out.isHit = true;
				// 各軸のめり込み量を計算
				out.normal.x = std::min(maxA.x - minB.x, maxB.x - minA.x);
				out.normal.y = std::min(maxA.y - minB.y, maxB.y - minA.y) * 0.5f;
				out.normal.z = std::min(maxA.z - minB.z, maxB.z - minA.z);
				// めり込みが少ない面を当たった面とみなし、面の法線を計算する
				if (out.normal.x < out.normal.y)
				{
					if (out.normal.x < out.normal.z) out.normal = DirectX::XMFLOAT3(a.center.x >= b.center.x ? 1.0f : -1.0f, 0.0f, 0.0f);
					else out.normal = DirectX::XMFLOAT3(0.0f, 0.0f, a.center.z >= b.center.z ? 1.0f : -1.0f);
				}
				else
				{
					if (out.normal.y < out.normal.z) out.normal = DirectX::XMFLOAT3(0.0f, a.center.y >= b.center.y ? 1.0f : -1.0f, 0.0f);
					else out.normal = DirectX::XMFLOAT3(0.0f, 0.0f, a.center.z >= b.center.z ? 1.0f : -1.0f);
				}
			}
		}
	}

	return out;
}

Collision::Result Collision::Hit(Sphere a, Sphere b)
{
	Result out = {};

	//必要なデータを計算
	DirectX::XMVECTOR vPosA = DirectX::XMLoadFloat3(&a.center);
	DirectX::XMVECTOR vPosB = DirectX::XMLoadFloat3(&b.center);
	DirectX::XMVECTOR vDist = DirectX::XMVectorSubtract(vPosA, vPosB);
	DirectX::XMVECTOR vLen = DirectX::XMVector3Length(vDist);

	float length;
	DirectX::XMStoreFloat(&length, vLen);

	out.isHit = (a.radius + b.radius >= length);

	return out;
}

Collision::Result Collision::Hit(Point point, Triangle triangle)
{

    Result out = {};

    // 計算用の型に変換 

    DirectX::XMVECTOR vTriPoint[3] = {

     DirectX::XMLoadFloat3(&triangle.point[0]),

     DirectX::XMLoadFloat3(&triangle.point[1]),

     DirectX::XMLoadFloat3(&triangle.point[2]),

    };

    DirectX::XMVECTOR vPoint = DirectX::XMLoadFloat3(&point.pos);

    // 三角形の頂点から別の頂点に向かうベクトルと、点に向かうベクトルを計算 

    DirectX::XMVECTOR vEdge[3];

    DirectX::XMVECTOR vToPoint[3];

    for (int i = 0; i < 3; ++i) {

        // 辺のベクトル 

        vEdge[i] = DirectX::XMVectorSubtract(vTriPoint[(i + 1) % 3], vTriPoint[i]);

        vEdge[i] = DirectX::XMVector3Normalize(vEdge[i]);

        // 点に向かうベクトル 

        vToPoint[i] = DirectX::XMVectorSubtract(vPoint, vTriPoint[i]);

        vToPoint[i] = DirectX::XMVector3Normalize(vToPoint[i]);

    }

    // 三角形の法線を計算 

    DirectX::XMVECTOR vNormal;

    vNormal = DirectX::XMVector3Orthogonal(vPoint);

    vNormal = DirectX::XMVector3Normalize(vNormal);

    // 点が三角形の面上に存在するか判定 

    float dot[3];

    DirectX::XMStoreFloat(&dot[0], DirectX::XMVector3Dot(vNormal, vToPoint[0]));

    if (fabsf(dot[0]) > FLT_EPSILON) { // FLT_EPSILON ≠ 0（ほぼ0、float型の計算誤差の値） 

        return out; // 面の法線と点に向かうベクトルが垂直になってなければ平面上にない 

    }

    // 各ベクトルの外積を計算 

    DirectX::XMVECTOR vCross[3];

    for (int i = 0; i < 3; ++i) {

        vCross[i] = DirectX::XMVector3Cross(vEdge[i], vToPoint[i]);

    }

    // 外積のベクトルと面のベクトルが同じ方向を向いているか計算 

    for (int i = 0; i < 3; ++i) {

        DirectX::XMStoreFloat(&dot[i], DirectX::XMVector3Dot(vNormal, vCross[i]));

    }

    // 当たり判定の結果を格納 

    if (dot[0] == dot[1] == dot[2]) {

        out.isHit = true;

        out.point = point.pos;

        DirectX::XMStoreFloat3(&out.normal, vNormal);

    }

    return out;

}

Collision::Result Collision::Hit(Ray ray, Triangle triangle)
{
    // レイを求める式
    // O: レイの始点
    // D : レイの方向ベクトル
    // t : レイのパラメータ(交点までの距離)
    // P(t) = O + tD, t ≥ 0
    Result out = {};

    // 計算に使用する変数を定義
    DirectX::XMVECTOR vecPoint[3];
    vecPoint[0] = DirectX::XMLoadFloat3(&triangle.point[0]);
    vecPoint[1] = DirectX::XMLoadFloat3(&triangle.point[1]);
    vecPoint[2] = DirectX::XMLoadFloat3(&triangle.point[2]);
    DirectX::XMVECTOR vecRayDir = DirectX::XMLoadFloat3(&ray.dir);
    DirectX::XMVECTOR vecRayOrigin = DirectX::XMLoadFloat3(&ray.origin);

    // 三角形の辺ベクトルを計算
    DirectX::XMVECTOR vecEdge1 = DirectX::XMVectorSubtract(vecPoint[1], vecPoint[0]);
    DirectX::XMVECTOR vecEdge2 = DirectX::XMVectorSubtract(vecPoint[2], vecPoint[0]);

    // Pベクトル(RayDir * Edge2)の定義
    DirectX::XMVECTOR vecP = DirectX::XMVector3Cross(vecRayDir, vecEdge2);
    
    // 面法線とPベクトルの内積を行列から取得する
    float det = DirectX::XMVectorGetX(DirectX::XMVector3Dot(vecEdge1, vecP));

    // 内積が限りなく0に近かったら並行しているとみなし、接触しない
    constexpr float ce_fEpsilon = 1e-6f;    // 限りなく0に近い数値
    if (fabsf(det) < ce_fEpsilon)
    {
        out.isHit = false;
        return out;
    }

    // Tベクトル(RayOrigin - Point[0])の定義
    DirectX::XMVECTOR vecT = DirectX::XMVectorSubtract(vecRayOrigin, vecPoint[0]);
    
    // 取得した内積の逆数を取得
    float invDet = 1.0f / det;
    
    // Uパラメータ(バリセン座標)を求める
    out.u = DirectX::XMVectorGetX(DirectX::XMVector3Dot(vecT, vecP)) * invDet;
    if (out.u < 0.0f || out.u > 1.0f)
    {
        out.isHit = false;
        return out;
    }

    // Qベクトル(vecT * vecEdge1)の定義
    DirectX::XMVECTOR vecQ = DirectX::XMVector3Cross(vecT, vecEdge1);
    
    // Vパラメータ(バリセン座標)を求める
    out.v = DirectX::XMVectorGetX(DirectX::XMVector3Dot(vecRayDir, vecQ)) * invDet;
    if (out.v < 0.0f || out.u + out.v > 1.0f)
    {
        out.isHit = false;
        return out;
    }

    // Tパラメータ(レイの始点から交点までの距離)を求める
    out.t = DirectX::XMVectorGetX(DirectX::XMVector3Dot(vecEdge2, vecQ)) * invDet;
    //OutputDebugStringA(std::to_string(out.t).c_str());
    //OutputDebugStringA("\n");
    
    out.isHit = out.t >= 0; // t < 0 の場合、レイの負方向に交差している
    
    return out;
}

Collision::Result Collision::Hit(Line line, Triangle triangle)
{
    // 計算に使用する変数を定義
    DirectX::XMVECTOR vecStart = DirectX::XMLoadFloat3(&line.start);
    DirectX::XMVECTOR vecEnd = DirectX::XMLoadFloat3(&line.end);
    DirectX::XMVECTOR vecDir = DirectX::XMVectorSubtract(vecEnd, vecStart);
    float length = DirectX::XMVectorGetX(DirectX::XMVector3Length(vecDir)); // 長さを求める
    vecDir = DirectX::XMVector3Normalize(vecDir); // 正規化

    DirectX::XMFLOAT3 fStart;
    DirectX::XMFLOAT3 fDir;
    DirectX::XMStoreFloat3(&fStart, vecStart);
    DirectX::XMStoreFloat3(&fDir, vecDir);

    Ray ray;
    ray.origin = fStart;
    ray.dir = fDir;

    Result out = Hit(ray, triangle);
    if (!out.isHit) return out;

    out.isHit = out.t >= 0 && out.t <= length; // 線分の範囲内なら交差

    return out;
}

Collision::Result Collision::Hit(Plane plane, Line line)
{
    Result out = {};
    // 計算に必要なパラメータを定義 
    DirectX::XMVECTOR vPlanePos = DirectX::XMLoadFloat3(&plane.pos);
    DirectX::XMVECTOR vPlaneN = DirectX::XMLoadFloat3(&plane.normal);
    DirectX::XMVECTOR vLineStart = DirectX::XMLoadFloat3(&line.start);
    DirectX::XMVECTOR vLineEnd = DirectX::XMLoadFloat3(&line.end);
    vPlaneN = DirectX::XMVector3Normalize(vPlaneN);

    // 平面の一点から線分の始点と終点に向かうベクトルを計算 
    DirectX::XMVECTOR vToStart = DirectX::XMVectorSubtract(vLineStart, vPlanePos);
    DirectX::XMVECTOR vToEnd = DirectX::XMVectorSubtract(vLineEnd, vPlanePos);
    DirectX::XMVECTOR vDotStart = DirectX::XMVectorSubtract(vToStart, vPlaneN);
    DirectX::XMVECTOR vDotEnd = DirectX::XMVectorSubtract(vToEnd, vPlaneN);

    // 各内積から平面を貫通しているか判定 
    float dotStart, dotEnd;
    DirectX::XMStoreFloat(&dotStart, vDotStart);
    DirectX::XMStoreFloat(&dotEnd, vDotEnd);

    if (dotStart > 0.0f && dotEnd < 0.0f)
    {
        // 各内積の値を絶対値に変換して、衝突位置までのベクトルを計算 
        dotStart = fabsf(dotStart);
        dotEnd = fabsf(dotEnd);
        float rate = dotStart / (dotStart + dotEnd); // 衝突面までの距離の割合を計算 
        DirectX::XMVECTOR vDist = DirectX::XMVectorSubtract(vLineEnd, vLineStart);
        vDist = DirectX::XMVectorScale(vDist, rate); // 線分のベクトルを衝突面までの長さに変換 
        vDist = DirectX::XMVectorAdd(vLineStart, vDist); // 始点からベクトル分移動

        // 平面との当たり判定の結果を格納 
        out.isHit = true;
        out.normal = plane.normal;
        DirectX::XMStoreFloat3(&out.point, vDist);
    }
    return out;
}
Collision::Result Collision::Hit(Plane plane, Ray ray, float lenght)
{

    // 計算用の型に変換 

    DirectX::XMVECTOR vPos = DirectX::XMLoadFloat3(&ray.origin);

    DirectX::XMVECTOR vRay = DirectX::XMLoadFloat3(&ray.dir);

    // ベクトルの長さを計算 

    vRay = DirectX::XMVector3Normalize(vRay);

    vRay = DirectX::XMVectorScale(vRay, lenght);

    // 計算結果を元に線分の情報を作成 

    Line line = {};

    line.start = ray.origin;

    DirectX::XMStoreFloat3(&line.end, DirectX::XMVectorAdd(vPos, vRay));

    // 平面と直線の当たり判定を実行 

    return Hit(plane, line);

};


Collision::Result2D Collision::Hit2D(Info2D a, Info2D b)
{
    Result2D out = {};
    if (a.type == b.type)
    {
        switch (a.type)
        {
        case eSquare: out = Hit2D(a.square, b.square);	break;
        case eCircle: out = Hit2D(a.circle, b.circle); break;
        }
    }
    return out;
}

Collision::Result2D Collision::Hit2D(Square a, Square b)
{
    Result2D out = {};

	DirectX::XMFLOAT2 aHalf = { a.size.x * 0.5f, a.size.y * 0.5f };
    DirectX::XMFLOAT2 bHalf = { b.size.x * 0.5f, b.size.y * 0.5f };

	DirectX::XMFLOAT2 aMin = { a.pos.x - aHalf.x, a.pos.y - aHalf.y };
	DirectX::XMFLOAT2 aMax = { a.pos.x + aHalf.x, a.pos.y + aHalf.y };
	DirectX::XMFLOAT2 bMin = { b.pos.x - bHalf.x, b.pos.y - bHalf.y };
	DirectX::XMFLOAT2 bMax = { b.pos.x + bHalf.x, b.pos.y + bHalf.y };

    out.isHit = aMin.x <= bMax.x && aMax.x >= bMin.x && aMin.y <= bMax.y && aMax.y >= bMin.y;
    out.posAtoB = { a.pos.x - b.pos.x, a.pos.y - b.pos.y };
    out.posAtoB = { b.pos.x - a.pos.x, b.pos.y - a.pos.y };

    return out;
}

Collision::Result2D Collision::Hit2D(Circle a, Circle b)
{
    Result2D out = {};

    out.isHit = powf(a.pos.x - b.pos.x, 2) + powf(a.pos.x - b.pos.x, 2) <= powf(a.radius + b.radius, 2);

    out.distance.x = out.distance.y = a.radius + b.radius;

    out.posAtoB = { a.pos.x - b.pos.x, a.pos.y - b.pos.y };
    out.posBtoA = { b.pos.x - a.pos.x, b.pos.y - a.pos.y };

    return out;
}

void Collision::DrawCollision(Collision::Info collision)
{
    float size = 1.0f; // 平面のサイズ（必要に応じて変更）
    switch (collision.type)
    {
    case eBox:
        // 原点(-x,-y,-z)

        DirectX::XMFLOAT3 harfSize = { collision.box.size.x / 2.0f,collision.box.size.y / 2.0f,collision.box.size.z / 2.0f };
        DirectX::XMFLOAT3 vertex[2][4];

        vertex[0][0] = { collision.box.center.x - harfSize.x,collision.box.center.y + harfSize.y,collision.box.center.z - harfSize.z };
        vertex[0][1] = { collision.box.center.x - harfSize.x,collision.box.center.y - harfSize.y,collision.box.center.z - harfSize.z };
        vertex[0][2] = { collision.box.center.x + harfSize.x,collision.box.center.y + harfSize.y,collision.box.center.z - harfSize.z };
        vertex[0][3] = { collision.box.center.x + harfSize.x,collision.box.center.y - harfSize.y,collision.box.center.z - harfSize.z };

        vertex[1][0] = { collision.box.center.x - harfSize.x,collision.box.center.y + harfSize.y,collision.box.center.z + harfSize.z };
        vertex[1][1] = { collision.box.center.x - harfSize.x,collision.box.center.y - harfSize.y,collision.box.center.z + harfSize.z };
        vertex[1][2] = { collision.box.center.x + harfSize.x,collision.box.center.y + harfSize.y,collision.box.center.z + harfSize.z };
        vertex[1][3] = { collision.box.center.x + harfSize.x,collision.box.center.y - harfSize.y,collision.box.center.z + harfSize.z };

        Geometory::AddLine(vertex[0][0], vertex[0][1], DirectX::XMFLOAT4(1, 0, 0, 1));
        Geometory::AddLine(vertex[0][0], vertex[0][2], DirectX::XMFLOAT4(1, 0, 0, 1));
        Geometory::AddLine(vertex[0][3], vertex[0][1], DirectX::XMFLOAT4(1, 0, 0, 1));
        Geometory::AddLine(vertex[0][3], vertex[0][2], DirectX::XMFLOAT4(1, 0, 0, 1));

        Geometory::AddLine(vertex[1][0], vertex[1][1], DirectX::XMFLOAT4(0, 1, 0, 1));
        Geometory::AddLine(vertex[1][0], vertex[1][2], DirectX::XMFLOAT4(0, 1, 0, 1));
        Geometory::AddLine(vertex[1][3], vertex[1][1], DirectX::XMFLOAT4(0, 1, 0, 1));
        Geometory::AddLine(vertex[1][3], vertex[1][2], DirectX::XMFLOAT4(0, 1, 0, 1));

        Geometory::AddLine(vertex[0][0], vertex[1][0], DirectX::XMFLOAT4(0, 0, 1, 1));
        Geometory::AddLine(vertex[0][1], vertex[1][1], DirectX::XMFLOAT4(0, 0, 1, 1));
        Geometory::AddLine(vertex[0][2], vertex[1][2], DirectX::XMFLOAT4(0, 0, 1, 1));
        Geometory::AddLine(vertex[0][3], vertex[1][3], DirectX::XMFLOAT4(0, 0, 1, 1));
        break;
    case eSphere:
        break;
    case eLine:
        Geometory::AddLine(collision.line.start, collision.line.end, DirectX::XMFLOAT4(0, 1, 0, 1));
        break;
    case ePlane:
        DirectX::XMVECTOR vNormal = DirectX::XMVectorSet(collision.plane.normal.x, collision.plane.normal.y, collision.plane.normal.z, 0);

        // 法線がY軸方向に近い場合はX軸を使う、それ以外はY軸を使う
        DirectX::XMVECTOR vTemp = (fabs(collision.plane.normal.y) < 0.9f) ? DirectX::XMVectorSet(0, 1, 0, 0) : DirectX::XMVectorSet(1, 0, 0, 0);

        // 外積を取って接線ベクトルを求める
        DirectX::XMVECTOR vTangent1 = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vTemp, vNormal));
        DirectX::XMVECTOR vTangent2 = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vNormal, vTangent1));

        DirectX::XMVECTOR vCenter = DirectX::XMVectorSet(collision.plane.pos.x, collision.plane.pos.y, collision.plane.pos.z, 1);


        DirectX::XMVECTOR v0 = DirectX::XMVectorAdd(DirectX::XMVectorAdd(vCenter, DirectX::XMVectorScale(vTangent1, size)), DirectX::XMVectorScale(vTangent2, size));
        DirectX::XMVECTOR v1 = DirectX::XMVectorAdd(DirectX::XMVectorSubtract(vCenter, DirectX::XMVectorScale(vTangent1, size)), DirectX::XMVectorScale(vTangent2, size));
        DirectX::XMVECTOR v2 = DirectX::XMVectorSubtract(DirectX::XMVectorSubtract(vCenter, DirectX::XMVectorScale(vTangent1, size)), DirectX::XMVectorScale(vTangent2, size));
        DirectX::XMVECTOR v3 = DirectX::XMVectorSubtract(DirectX::XMVectorAdd(vCenter, DirectX::XMVectorScale(vTangent1, size)), DirectX::XMVectorScale(vTangent2, size));
        break;

    case eTriangle:
        Geometory::AddLine(collision.triangle.point[0], collision.triangle.point[1], DirectX::XMFLOAT4(1, 0, 0, 1));
        Geometory::AddLine(collision.triangle.point[1], collision.triangle.point[2], DirectX::XMFLOAT4(1, 0, 0, 1));
        Geometory::AddLine(collision.triangle.point[2], collision.triangle.point[0], DirectX::XMFLOAT4(1, 0, 0, 1));
        
        break;
    default:
        break;
    }
}

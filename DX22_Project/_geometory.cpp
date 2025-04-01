#include "Geometory.h"
#include "Defines.h"

#define ANGLENUM (8)
#define CYLINDER_SUMIT ((8 + 1) * 2)
#define CYLINDER_SUMIT_FLAT ((ANGLENUM + 1) * 2)
#define θ (360 / ANGLENUM)

void Geometory::MakeBox()
{
	//--- 頂点の作成
	Vertex vtxBox[] = {
		// -Z面
		{{-0.5f, 0.5f,-0.5f},{0.0f,0.0f}},
		{{ 0.5f, 0.5f,-0.5f},{1.0f,0.0f}},
		{{-0.5f,-0.5f,-0.5f},{0.0f,1.0f}},
		{{ 0.5f,-0.5f,-0.5f},{1.0f,1.0f}},
		// Z面
		{{ 0.5f, 0.5f, 0.5f},{0.0f,0.0f}},
		{{-0.5f, 0.5f, 0.5f},{1.0f,0.0f}},
		{{ 0.5f,-0.5f, 0.5f},{0.0f,1.0f}},
		{{-0.5f,-0.5f, 0.5f},{1.0f,1.0f}},
		// -Y面
		{{ 0.5f,-0.5f, 0.5f},{0.0f,0.0f}},
		{{-0.5f,-0.5f, 0.5f},{1.0f,0.0f}},
		{{ 0.5f,-0.5f,-0.5f},{0.0f,1.0f}},
		{{-0.5f,-0.5f,-0.5f},{1.0f,1.0f}},
		// Y面
		{{-0.5f, 0.5f, 0.5f},{0.0f,0.0f}},
		{{ 0.5f, 0.5f, 0.5f},{1.0f,0.0f}},
		{{-0.5f, 0.5f,-0.5f},{0.0f,1.0f}},
		{{ 0.5f, 0.5f,-0.5f},{1.0f,1.0f}},
		// -X面
		{{-0.5f, 0.5f, 0.5f},{0.0f,0.0f}},
		{{-0.5f, 0.5f,-0.5f},{1.0f,0.0f}},
		{{-0.5f,-0.5f, 0.5f},{0.0f,1.0f}},
		{{-0.5f,-0.5f,-0.5f},{1.0f,1.0f}},
		// X面
		{{ 0.5f, 0.5f,-0.5f},{0.0f,0.0f}},
		{{ 0.5f, 0.5f, 0.5f},{1.0f,0.0f}},
		{{ 0.5f,-0.5f,-0.5f},{0.0f,1.0f}},
		{{ 0.5f,-0.5f, 0.5f},{1.0f,1.0f}}
	};
	//--- インデックスの作成
	int idxBox[] = {
			 0,  1,  2,  1,  3,  2,	//-Z面
			 4,  5,  6,  5,  7,  6,	//Z面
			 8,  9, 10,  9, 11, 10,	//-Y面
			12, 13, 14, 13, 15, 14,	//Y面
			16, 17, 18, 17, 19, 18,	//-X面
			20, 21, 22, 21, 23, 22	//X面
	};
	// バッファの作成
	MeshBuffer::Description desc = {};
	desc.pVtx = vtxBox;
	desc.vtxCount = 24;
	desc.vtxSize = sizeof(Vertex);
	desc.pIdx = idxBox;
	desc.idxCount = 36;
	desc.idxSize = sizeof(int);
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_pBox = new MeshBuffer();
	m_pBox->Create(desc);
}

void Geometory::MakeCylinder()
{
	//--- 頂点の作成
	Vertex vtxCylinder[36];
	// 天面、底面
	for (int i = 0; i < 9; i++)
	{
		switch (i % 4)
		{
		case 0: vtxCylinder[i] = { cosf(DirectX::XMConvertToRadians(i * 45.0f)) * 0.5f, 0.5f,sinf(DirectX::XMConvertToRadians(i * 45.0f)) * 0.5f,{0.5f * cosf(DirectX::XMConvertToRadians(i * 45.0f)) - 0.5f,0.5f * sinf(DirectX::XMConvertToRadians(i * 45.0f)) - 0.5f} };
			  break;
		case 1: vtxCylinder[i] = { cosf(DirectX::XMConvertToRadians(i * 45.0f)) * 0.5f, 0.5f,sinf(DirectX::XMConvertToRadians(i * 45.0f)) * 0.5f,{0.5f * cosf(DirectX::XMConvertToRadians(i * 45.0f)) - 0.5f,0.5f * sinf(DirectX::XMConvertToRadians(i * 45.0f)) - 0.5f} };
			  break;
		case 2: vtxCylinder[i] = { cosf(DirectX::XMConvertToRadians(i * 45.0f)) * 0.5f, 0.5f,sinf(DirectX::XMConvertToRadians(i * 45.0f)) * 0.5f,{0.5f * cosf(DirectX::XMConvertToRadians(i * 45.0f)) - 0.5f,0.5f * sinf(DirectX::XMConvertToRadians(i * 45.0f)) - 0.5f} };
			  break;
		case 3: vtxCylinder[i] = { cosf(DirectX::XMConvertToRadians(i * 45.0f)) * 0.5f, 0.5f,sinf(DirectX::XMConvertToRadians(i * 45.0f)) * 0.5f,{0.5f * cosf(DirectX::XMConvertToRadians(i * 45.0f)) - 0.5f,0.5f * sinf(DirectX::XMConvertToRadians(i * 45.0f)) - 0.5f} };
			  break;
		default:
			break;
		}
	}
	for (int i = 9; i < 18; i++)
	{
		switch ((i - 9) % 4)
		{
		case 0: vtxCylinder[i] = { cosf(DirectX::XMConvertToRadians((i - 9) * 45.0f)) * 0.5f,-0.5f,sinf(DirectX::XMConvertToRadians((i - 9) * 45.0f)) * 0.5f,{0.5f * cosf(DirectX::XMConvertToRadians((i - 9) * 45.0f)) - 0.5f ,0.5f * sinf(DirectX::XMConvertToRadians((i - 9) * 45.0f)) - 0.5f} };
			  break;
		case 1: vtxCylinder[i] = { cosf(DirectX::XMConvertToRadians((i - 9) * 45.0f)) * 0.5f,-0.5f,sinf(DirectX::XMConvertToRadians((i - 9) * 45.0f)) * 0.5f,{0.5f * cosf(DirectX::XMConvertToRadians((i - 9) * 45.0f)) - 0.5f ,0.5f * sinf(DirectX::XMConvertToRadians((i - 9) * 45.0f)) - 0.5f} };
			  break;
		case 2: vtxCylinder[i] = { cosf(DirectX::XMConvertToRadians((i - 9) * 45.0f)) * 0.5f,-0.5f,sinf(DirectX::XMConvertToRadians((i - 9) * 45.0f)) * 0.5f,{0.5f * cosf(DirectX::XMConvertToRadians((i - 9) * 45.0f)) - 0.5f ,0.5f * sinf(DirectX::XMConvertToRadians((i - 9) * 45.0f)) - 0.5f} };
			  break;
		case 3: vtxCylinder[i] = { cosf(DirectX::XMConvertToRadians((i - 9) * 45.0f)) * 0.5f,-0.5f,sinf(DirectX::XMConvertToRadians((i - 9) * 45.0f)) * 0.5f,{0.5f * cosf(DirectX::XMConvertToRadians((i - 9) * 45.0f)) - 0.5f ,0.5f * sinf(DirectX::XMConvertToRadians((i - 9) * 45.0f)) - 0.5f} };
			  break;
		default:
			break;
		}
	}
	// 側面
	float flag = 1.0f;
	for (int i = 18; i < 36; i++)
	{
		if (i % 2 == 0)flag = 1.0f;
		if (i % 2 == 1)flag = -1.0f;
		switch ((i - 18) % 4)
		{
		case 0: vtxCylinder[i] = { cosf(DirectX::XMConvertToRadians((i - 18) / 2 * 45.0f)) * 0.5f, flag * 0.5f,sinf(DirectX::XMConvertToRadians((i - 18) / 2 * 45.0f)) * 0.5f,{0.0f,0.0f} };
			  break;
		case 1: vtxCylinder[i] = { cosf(DirectX::XMConvertToRadians((i - 18) / 2 * 45.0f)) * 0.5f, flag * 0.5f,sinf(DirectX::XMConvertToRadians((i - 18) / 2 * 45.0f)) * 0.5f,{1.0f,0.0f} };
			  break;
		case 2: vtxCylinder[i] = { cosf(DirectX::XMConvertToRadians((i - 18) / 2 * 45.0f)) * 0.5f, flag * 0.5f,sinf(DirectX::XMConvertToRadians((i - 18) / 2 * 45.0f)) * 0.5f,{0.0f,1.0f} };
			  break;
		case 3: vtxCylinder[i] = { cosf(DirectX::XMConvertToRadians((i - 18) / 2 * 45.0f)) * 0.5f, flag * 0.5f,sinf(DirectX::XMConvertToRadians((i - 18) / 2 * 45.0f)) * 0.5f,{1.0f,1.0f} };
			  break;
		default:
			break;
		}
	}

	//--- インデックスの作成
	int idxCylinder[84];
	// 天面、底面
	for (int i = 0; i < 18; i++)
	{
		switch (i % 3)
		{
		case 0:idxCylinder[i] = 0;
			break;
		case 1:idxCylinder[i] = 2 + i / 3;
			break;
		case 2:idxCylinder[i] = 1 + i / 3;
			break;
		default:
			break;
		}
	}
	for (int i = 18; i < 36; i++)
	{
		switch ((i - 18) % 3)
		{
		case 0:idxCylinder[i] = 9;
			break;
		case 1:idxCylinder[i] = 10 + (i - 18) / 3;
			break;
		case 2:idxCylinder[i] = 11 + (i - 18) / 3;
			break;
		default:
			break;
		}
	}
	// 側面
	for (int i = 36; i < 84; i++)
	{
		switch (i % 6)
		{
		case 0:idxCylinder[i] = 18 + 2 * ((i - 36) / 6);
			break;
		case 1:idxCylinder[i] = 20 + 2 * ((i - 36) / 6);
			break;
		case 2:idxCylinder[i] = 19 + 2 * ((i - 36) / 6);
			break;
		case 3:idxCylinder[i] = 19 + 2 * ((i - 36) / 6);
			break;
		case 4:idxCylinder[i] = 20 + 2 * ((i - 36) / 6);
			break;
		case 5:idxCylinder[i] = 21 + 2 * ((i - 36) / 6);
			break;
		default:
			break;
		}
	}
	//-- バッファの作成
	MeshBuffer::Description desc = {};
	desc.pVtx = vtxCylinder;
	desc.vtxCount = 36;
	desc.vtxSize = sizeof(Vertex);
	desc.pIdx = idxCylinder;
	desc.idxCount = 84;
	desc.idxSize = sizeof(int);
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_pCylinder = new MeshBuffer();
	m_pCylinder->Create(desc);
}

void Geometory::MakeSphere()
{
	//--- 頂点の作成
	Vertex vtxSphere[81];
	for (int j = 0; j < 9; j++)
	{
		for (int i = 0; i < 9; i++)
		{
			switch (j % 2)
			{
			case 0:if (i % 2 == 0)vtxSphere[i + j * 9] = { sinf(DirectX::XMConvertToRadians(i * 45.0f)) * cosf(DirectX::XMConvertToRadians(j * 45.0f)) * 0.5f,cosf(DirectX::XMConvertToRadians(i * 45.0f)) * 0.5f,sinf(DirectX::XMConvertToRadians(i * 45.0f)) * sinf(DirectX::XMConvertToRadians(j * 45.0f)) * 0.5f,0.0f,0.0f };
				  else vtxSphere[i + j * 9] = { sinf(DirectX::XMConvertToRadians(i * 45.0f)) * cosf(DirectX::XMConvertToRadians(j * 45.0f)) * 0.5f,cosf(DirectX::XMConvertToRadians(i * 45.0f)) * 0.5f,sinf(DirectX::XMConvertToRadians(i * 45.0f)) * sinf(DirectX::XMConvertToRadians(j * 45.0f)) * 0.5f,1.0f,0.0f };
				  break;
			case 1:if (i % 2 == 0)vtxSphere[i + j * 9] = { sinf(DirectX::XMConvertToRadians(i * 45.0f)) * cosf(DirectX::XMConvertToRadians(j * 45.0f)) * 0.5f,cosf(DirectX::XMConvertToRadians(i * 45.0f)) * 0.5f,sinf(DirectX::XMConvertToRadians(i * 45.0f)) * sinf(DirectX::XMConvertToRadians(j * 45.0f)) * 0.5f,0.0f,1.0f };
				  else vtxSphere[i + j * 9] = { sinf(DirectX::XMConvertToRadians(i * 45.0f)) * cosf(DirectX::XMConvertToRadians(j * 45.0f)) * 0.5f,cosf(DirectX::XMConvertToRadians(i * 45.0f)) * 0.5f,sinf(DirectX::XMConvertToRadians(i * 45.0f)) * sinf(DirectX::XMConvertToRadians(j * 45.0f)) * 0.5f,1.0f,1.0f };
				  break;
			default:
				break;
			}
		}
	}
	//--- インデックスの作成
	int idxSphere[384];
	for (int j = 0; j < 8; j++)
	{
		for (int i = 0; i < 48; i++)
		{

			switch (i % 6)
			{
			case 0:idxSphere[i + j * 48] = 0 + i / 6 + j * 9;
				break;
			case 1:idxSphere[i + j * 48] = 1 + i / 6 + j * 9;
				break;
			case 2:idxSphere[i + j * 48] = 9 + i / 6 + j * 9;
				break;
			case 3:idxSphere[i + j * 48] = 9 + i / 6 + j * 9;
				break;
			case 4:idxSphere[i + j * 48] = 1 + i / 6 + j * 9;
				break;
			case 5:idxSphere[i + j * 48] = 10 + i / 6 + j * 9;
				break;
			default:
				break;
			}
		}
	}
	// バッファの作成
	//--- バッファの作成
	MeshBuffer::Description desc = {};
	desc.pVtx = vtxSphere;
	desc.vtxCount = 81;
	desc.vtxSize = sizeof(Vertex);
	desc.pIdx = idxSphere;
	desc.idxCount = 384;
	desc.idxSize = sizeof(int);
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_pSphere = new MeshBuffer();
	m_pSphere->Create(desc);
}
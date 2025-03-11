#include "Object.h"

constexpr float OBJECT_SIZE_POW = 20.0f;
constexpr float TREE_SIZE = 10.0f;

CObject::CObject()
{
	//モデルの初期化
	for (int i = 0; i < MAX_MODEL; i++)
	{
		m_pModel[i] = new Model();
	}
	if (!m_pModel[TREE]->Load(PASS("tree_forest.fbx"))) MessageBox(NULL, "Icon.png", "Error", MB_OK);	// エラーメッセージの表示 
	if (!m_pModel[TREE2]->Load(PASS("tree_desert.fbx"))) MessageBox(NULL, "tree_desert.fbx", "Error", MB_OK);	// エラーメッセージの表示 
	if (!m_pModel[CUP]->Load(PASS("flag_teamBlue.fbx"))) MessageBox(NULL, "flag_teamBlue.fbx", "Error", MB_OK);	// エラーメッセージの表示 
	if (!m_pModel[BEAR]->Load(PASS("character_bear.fbx"))) MessageBox(NULL, "character_bear.fbx", "Error", MB_OK);	// エラーメッセージの表示 
	if (!m_pModel[DETAIL]->Load(PASS("detail_forest.fbx"))) MessageBox(NULL, "detail_forest.fbx", "Error", MB_OK);	// エラーメッセージの表示 
	InitText();

	for (int j = 0; j < MAP_Y; j++)
	{
		for (int i = 0; i < MAP_X; i++)
		{
			m_pos = { ((float)i - MAP_X / 2) * OBJECT_SIZE_POW + WORLD_AJUST, CMETER(0.0f) + WORLD_AJUST, ((float)j - MAP_Y / 2) * OBJECT_SIZE_POW + WORLD_AJUST };
			m_CollisionArray[j][i].type = Collision::eBox;
			m_CollisionArray[j][i].box.center = m_pos;
			switch (ObjectMap[j][i])
			{
			case TREE:
				m_CollisionArray[j][i].box.center = m_pos;
				m_CollisionArray[j][i].box.size = { METER(TREE_SIZE),METER(TREE_SIZE / 2.0f),METER(TREE_SIZE) };
				break;
			case TREE2:
				m_CollisionArray[j][i].box.center = m_pos;
				m_CollisionArray[j][i].box.size = { METER(1.5f),METER(1.0f),METER(1.5f) };
				break;
			case CUP:
				m_CollisionArray[j][i].box.center = m_pos;
				m_CollisionArray[j][i].box.size = { METER(1.0f),METER(1.0f),METER(1.0f) };
				break;
			case BEAR:
				m_CollisionArray[j][i].box.center = m_pos;
				m_CollisionArray[j][i].box.size = { METER(1.0f),METER(1.0f),METER(1.0f) };
				break;
			default:
				m_CollisionArray[j][i].box.center = {0.0f,10000.0f,0.0f};
				m_CollisionArray[j][i].box.size = { 1.0f,1.0f,1.0f };
				break;
			}
		}
	}
}

CObject::~CObject()
{
	for (int i = 0; i < MAX_MODEL; i++)
	{
		if (m_pModel[i])
		{
			delete m_pModel[i];
			m_pModel[i] = nullptr;
		}
	}
}

void CObject::Update()
{
}

void CObject::Draw()
{
	for (int j = 0; j < MAP_Y; j++)
	{
		for (int i = 0; i < MAP_X; i++)
		{
			m_pos = { ((float)i - MAP_X / 2) * OBJECT_SIZE_POW + WORLD_AJUST, METER(-1.0f) + WORLD_AJUST, ((float)j - MAP_Y / 2) * OBJECT_SIZE_POW + WORLD_AJUST };
			switch (ObjectMap[j][i])
			{
			case NONE_MODEL:
				break;
			case TREE:
				m_pos.y = METER(-0.5f) + WORLD_AJUST;
				m_size = { METER(TREE_SIZE),METER(TREE_SIZE),METER(TREE_SIZE) };
				m_rotate = { 0.0f,0.0f,0.0f };
				SetModel(m_pos, m_size, m_rotate, TREE);
				break;
			case TREE2:
				m_pos.y = METER(0.0f) + WORLD_AJUST;
				m_size = { METER(1.0f),METER(1.0f),METER(1.0f) };
				m_rotate = { 0.0f,0.0f,0.0f };
				SetModel(m_pos, m_size, m_rotate, TREE2);
				break;
			case CUP:
				m_pos.y = METER(0.0f) + WORLD_AJUST;
				m_size = { METER(1.0f),METER(1.0f),METER(1.0f) };
				m_rotate = { 0.0f,0.0f,0.0f };
				SetModel(m_pos, m_size, m_rotate, CUP);
				break;
			case BEAR:
				m_pos.y = METER(0.0f) + WORLD_AJUST;
				m_size = { METER(1.0f),METER(1.0f),METER(1.0f) };
				m_rotate = { 0.0f,0.0f,0.0f };
				SetModel(m_pos, m_size, m_rotate, BEAR);
				break;
			case DETAIL:
				m_pos.y = METER(0.0f) + WORLD_AJUST;
				m_size = { METER(1.0f),METER(1.0f),METER(1.0f) };
				m_rotate = { 0.0f,0.0f,0.0f };
				SetModel(m_pos, m_size, m_rotate, DETAIL);
				break;
			case MAX_MODEL:
			default:
				break;
			}
		}
	}

}

void CObject::SetModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 rotate, int ModelType)
{
	// ワールド行列変換
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);	// 座標
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(size.x, size.y, size.z);		// 拡縮
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(rotate.x);				// 回転X
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(rotate.y);				// 回転y
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(rotate.z);				// 回転z
	DirectX::XMMATRIX world = S * Rx * Ry * Rz * T;	// 拡縮・回転・座標の順番でかけ合わせる

	DirectX::XMFLOAT4X4 wvp[3] = {};	// 各行列変換の受け入れ先

	// 計算用のデータから読み取り用のデータに変換(転置)
	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));	// ワールド行列を転置して設定
	// view行列とprojection行列はカメラの物を持ってくる
	wvp[1] = m_pCamera->GetViewMatrix();		// view行列
	wvp[2] = m_pCamera->GetProjectionMatrix();	// projection行列
	// カメラ行列を設定
	Geometory::SetView(wvp[1]);
	Geometory::SetProjection(wvp[2]);

	// シェーダーへ変換行列を設定
	ShaderList::SetWVP(wvp); // 引数にはXMFloat4X4型の、要素数3のアドレスを渡すこと

	//モデルに使用する頂点シェーダーを設定
	m_pModel[ModelType]->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	// モデルに使用する頂点ピクセルシェーダーを設定
	m_pModel[ModelType]->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

	for (int i = 0; i < m_pModel[ModelType]->GetMeshNum(); i++)
	{
		// モデルのメッシュの取得
		Model::Mesh mesh = *m_pModel[ModelType]->GetMesh(i);

		// メッシュに割り当てられているマテリアルを取得
		Model::Material material = *m_pModel[ModelType]->GetMaterial(mesh.materialID);

		// シェーダーへマテリアルを設定
		ShaderList::SetMaterial(material);

		// モデルの描画
		m_pModel[ModelType]->Draw(i);
	}
}

void CObject::SetCamera(CCamera* camera)
{
	m_pCamera = camera;
}

void CObject::OnCollision(Collision::Result collision)
{

}

Collision::Info CObject::GetCollision(int i, int j)
{
	return m_CollisionArray[j][i];
}

Collision::Box CObject::GetBox(int i, int j)
{
	return m_Box[j][i];
}

void CObject::InitText()
{
	// テキストから一文字ずつ読み取る
	char word;
	// 読み取った文字を格納する
	string num;
	num.clear();

	ifstream file_Object(TXTPASS("Object.txt"), ios::in);
	if (file_Object)
	{
		for (int j = 0; j < MAP_Y; j++)
		{
			for (int i = 0; i < MAP_X;)
			{
				file_Object.get(word);
				if (word == ' ')
				{
					i++;
					num.clear();
				}
				else
				{
					num += word;
					ObjectMap[j][i] = atoi(num.c_str());	//格納した文字をint型に変える
				}
			}
		}
		file_Object.close();
	}
	else MessageBox(NULL, "Object.txtエラー", "SceneGame.cpp", MB_OK);
}

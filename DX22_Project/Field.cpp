#include "Field.h"

constexpr float FIELD_SIZE_POW = 20.0f;

CField::CField()
{
	//モデルの初期化
	for (int i = 0; i < MAX_FIELD; i++)
	{
		m_pModel[i] = new Model();
	}
	if (!m_pModel[FAIRWAY]->Load(PASS("tileLarge_forest.fbx"))) MessageBox(NULL, "tileLarge_forest.fbx", "Error", MB_OK);	// エラーメッセージの表示 
	if (!m_pModel[BUNKER]->Load(PASS("tileLow_desert.fbx"))) MessageBox(NULL, "tree_desert.fbx", "Error", MB_OK);	// エラーメッセージの表示 

	InitText();
}

CField::~CField()
{
	for (int i = 0; i < MAX_FIELD; i++)
	{
		if (m_pModel[i])
		{
			delete m_pModel[i];
			m_pModel[i] = nullptr;
		}
	}
}

void CField::Update()
{
}

void CField::Draw()
{
	for (int j = 0; j < MAP_Y; j++)
	{
		for (int i = 0; i < MAP_X; i++)
		{
			m_pos = { ((float)i - MAP_X / 2) * FIELD_SIZE_POW + WORLD_AJUST, METER(-1.0f) + WORLD_AJUST, ((float)j - MAP_Y / 2) * FIELD_SIZE_POW + WORLD_AJUST };
			switch (FieldMap[j][i])
			{
			case NONE_FIELD:
				break;
			case FAIRWAY:
				m_size = { METER(0.17f * FIELD_SIZE_POW),METER(0.5f),METER(0.17f * FIELD_SIZE_POW) };
				m_rotate = { 0.0f,0.0f,0.0f };
				SetModel(m_pos, m_size, m_rotate, FAIRWAY);
				break;
			case BUNKER:
				m_size = { METER(0.52f * FIELD_SIZE_POW),METER(0.5f),METER(0.52f * FIELD_SIZE_POW) };
				m_rotate = { 0.0f,0.0f,0.0f };
				SetModel(m_pos, m_size, m_rotate, BUNKER);
				break;
			case MAX_FIELD:
			default:
				break;
			}
		}
	}

}

void CField::SetModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 rotate, int ModelType)
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

void CField::SetCamera(CCamera* camera)
{
	m_pCamera = camera;
}

void CField::OnCollision(Collision::Result collision)
{

}

Collision::Info CField::GetCollision()
{
	return m_Collision;
}

// ファイルの初期化
void CField::InitText()
{
	// テキストから一文字ずつ読み取る
	char word;
	// 読み取った文字を格納する
	string num;
	num.clear();

	ifstream file_Field(TXTPASS("Ground.txt"), ios::in);
	if (file_Field)
	{
		for (int j = 0; j < MAP_Y; j++)
		{
			for (int i = 0; i < MAP_X;)
			{
				file_Field.get(word);
				if (word == ' ')
				{
					i++;
					num.clear();
				}
				else
				{
					num += word;
					FieldMap[j][i] = atoi(num.c_str());	//格納した文字をint型に変える
				}
			}
		}
		file_Field.close();
	}
	else MessageBox(NULL, "Field.txtエラー", "SceneGame.cpp", MB_OK);

	num.clear();



}

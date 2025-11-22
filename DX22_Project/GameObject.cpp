// インクルード部
#include "GameObject.h"
#include "DebugSystem.h"
#include "Main.h"

CGameObject::CGameObject()
    : m_bDestroy(false)
    , m_eTag(Tag::None)
    , m_tID{ "Object", 0 }
{
    // 汎用パラメータの初期化
    m_tParam.m_f3Pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_tParam.m_f3Size = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
    m_tParam.m_f3Rotate = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_tParam.m_f4Color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_tParam.m_f2UVPos = DirectX::XMFLOAT2(0.0f, 0.0f);
    m_tParam.m_f2UVSize = DirectX::XMFLOAT2(1.0f, 1.0f);
    m_tParam.m_eCulling = D3D11_CULL_BACK;
    m_f3OldPos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

CGameObject::~CGameObject()
{

}

void CGameObject::Init()
{

}

void CGameObject::Uninit()
{
	for (auto comp : m_pComponent_List)
	{
        if (!comp) continue;
		comp->Uninit();
		delete comp;
	}
	m_pComponent_List.clear();
}

void CGameObject::Update()
{
	for (auto comp : m_pComponent_List)
	{
        if (!comp) continue;
		comp->Update();
	}
}

void CGameObject::WriteBuffer()
{

}

void CGameObject::Draw()
{
    for (auto comp : m_pComponent_List)
	{
        if (!comp) continue;
        // 描画用コンポーネントを使用する場合、汎用パラメータをコンポーネントに渡す
		if (dynamic_cast<CRendererComponent*>(comp))dynamic_cast<CRendererComponent*>(comp)->SetRendererParam(m_tParam);
		comp->Draw();
	}	
}

void CGameObject::OnDestroy()
{

}

void CGameObject::OnCollision(CCollisionBase* other, std::string thisTag, Collision::Result result)
{
}

void CGameObject::Destroy()
{
	m_bDestroy = true;
}

bool CGameObject::IsDestroy()
{
	return m_bDestroy;
}

int CGameObject::Inspecter(bool isEnd)
{
    // 子要素の数
    int nChildCnt = 0;

    // IMGUIウィンドウの初期化
    ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - 300, 20));
    ImGui::SetNextWindowSize(ImVec2(280, 260));
    ImGui::Begin("Inspecter");

    // 子要素の初期化
    ImGui::BeginChild(ImGui::GetID((void*)nChildCnt), ImVec2(250, 30), ImGuiWindowFlags_NoTitleBar);

    // インスペクターに名前を表示
    ObjectID id = m_tID;
    std::string name = id.m_sName;

    // 同オブジェクトが2つ以上ある場合、そのindexも名前に表示する
    if (id.m_nSameCount != 0) name += std::to_string(id.m_nSameCount);
    name = "Name:" + name;
    ImGui::Text(name.c_str());

    // 子要素の終了
    ImGui::EndChild();
    // 子要素の数をインクリメント
    nChildCnt++;

    // 子要素の初期化
    ImGui::BeginChild(ImGui::GetID((void*)nChildCnt), ImVec2(250, 190), ImGuiWindowFlags_NoTitleBar);

    if (CDebugSystem::GetInstance()->IsUpdate())
    {
        // アップデートが有効な時はパラメータの表示のみ行う
        // トランスフォームの表示
        if (ImGui::CollapsingHeader(std::string("[Transform]").c_str()))
        {
            // 座標の表示
            ImGui::Text(std::string("Position").c_str());
            DirectX::XMFLOAT3 pos = m_tParam.m_f3Pos;
            ImGui::Text("PosX: %.3f", pos.x);
            ImGui::Text("PosY: %.3f", pos.y);
            ImGui::Text("PosZ: %.3f", pos.z);
            ImGui::Spacing();

            // サイズの表示
            ImGui::Text(std::string("Size").c_str());
            DirectX::XMFLOAT3 size = m_tParam.m_f3Size;
            ImGui::Text("SizeX: %.3f", size.x);
            ImGui::Text("SizeY: %.3f", size.y);
            ImGui::Text("SizeZ: %.3f", size.z);
            ImGui::Spacing();

            // 回転の表示
            ImGui::Text(std::string("Rotation").c_str());
            DirectX::XMFLOAT3 rotate = m_tParam.m_f3Rotate;
            ImGui::Text("RotateX: %3f", DirectX::XMConvertToDegrees(rotate.x));
            ImGui::Text("RotateY: %3f", DirectX::XMConvertToDegrees(rotate.y));
            ImGui::Text("RotateZ: %3f", DirectX::XMConvertToDegrees(rotate.z));
            ImGui::Spacing();

            // 色の表示
            ImGui::Text(std::string("Color").c_str());
            DirectX::XMFLOAT4 color = m_tParam.m_f4Color;
            ImGui::Text("Red: %3f", color.x);
            ImGui::Text("Blue: %3f", color.y);
            ImGui::Text("Green: %3f", color.z);
            ImGui::Text("Alpha: %3f", color.w);
            ImGui::Spacing();
        }
    }
    else
    {
        // アップデートが無効な時はパラメータの変更も行う
        // トランスフォームの表示
        if (ImGui::CollapsingHeader(std::string("[Transform]").c_str()))
        {
            // 座標の表示と変更
            DirectX::XMFLOAT3* pos = &m_tParam.m_f3Pos;
            float inputPos[3] = { pos->x,pos->y,pos->z };
            ImGui::InputFloat3("Position", inputPos);
            ImGui::Spacing();
            *pos = DirectX::XMFLOAT3(inputPos[0], inputPos[1], inputPos[2]);

            // サイズの表示と変更
            DirectX::XMFLOAT3* size = &m_tParam.m_f3Size;
            float inputSize[3] = { size->x,size->y,size->z };
            ImGui::InputFloat3("Size", inputSize);
            ImGui::Spacing();
            *size = DirectX::XMFLOAT3(inputSize[0], inputSize[1], inputSize[2]);
            // 回転の表示と変更
            DirectX::XMFLOAT3 rotate = m_tParam.m_f3Rotate;
            float inputRotate[3] = {
                DirectX::XMConvertToDegrees(rotate.x),
                DirectX::XMConvertToDegrees(rotate.y),
                DirectX::XMConvertToDegrees(rotate.z)
            };
            ImGui::InputFloat3("Rotate", inputRotate);
            ImGui::Spacing();

            inputRotate[0] = DirectX::XMConvertToRadians(inputRotate[0]);
            inputRotate[1] = DirectX::XMConvertToRadians(inputRotate[1]);
            inputRotate[2] = DirectX::XMConvertToRadians(inputRotate[2]);

            m_tParam.m_f3Rotate = DirectX::XMFLOAT3(inputRotate[0], inputRotate[1], inputRotate[2]);

            // 色の表示と変更
            DirectX::XMFLOAT4* color = &m_tParam.m_f4Color;
            float inputColor[4] = { color->x,color->y,color->z,color->w };
            ImGui::ColorEdit4("Color", inputColor);
            ImGui::Spacing();
            *color = DirectX::XMFLOAT4(inputColor[0], inputColor[1], inputColor[2], inputColor[3]);
        }
    }

    // 子要素の終了
    ImGui::EndChild();
    // 子要素の数をインクリメント
    nChildCnt++;

    // IMGUIウィンドウの終了
    if (isEnd) ImGui::End();

    // 子要素の数を返し、派生先のインスペクターに使用する
    return nChildCnt;
}

DirectX::XMFLOAT3 CGameObject::GetForward()
{
    DirectX::XMMATRIX mRotate = DirectX::XMMatrixRotationRollPitchYaw(m_tParam.m_f3Rotate.x, m_tParam.m_f3Rotate.y, m_tParam.m_f3Rotate.z);

    DirectX::XMFLOAT3 f3ForWard;
    DirectX::XMStoreFloat3(&f3ForWard, mRotate.r[2]);

    return f3ForWard;
}

DirectX::XMFLOAT3 CGameObject::GetRight()
{
    DirectX::XMMATRIX mRotate = DirectX::XMMatrixRotationRollPitchYaw(m_tParam.m_f3Rotate.x, m_tParam.m_f3Rotate.y, m_tParam.m_f3Rotate.z);

    DirectX::XMFLOAT3 f3Right;
    DirectX::XMStoreFloat3(&f3Right, mRotate.r[0]);

    return f3Right;
}

DirectX::XMFLOAT3 CGameObject::GetUp()
{
    DirectX::XMMATRIX mRotate = DirectX::XMMatrixRotationRollPitchYaw(m_tParam.m_f3Rotate.x, m_tParam.m_f3Rotate.y, m_tParam.m_f3Rotate.z);

    DirectX::XMFLOAT3 f3Up;
    DirectX::XMStoreFloat3(&f3Up, mRotate.r[1]);

    return f3Up;
}

DirectX::XMFLOAT4X4* CGameObject::GetWorld()
{
    DirectX::XMFLOAT4X4 world;

    DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixScaling(m_tParam.m_f3Size.x, m_tParam.m_f3Size.y, m_tParam.m_f3Size.z) *
        DirectX::XMMatrixRotationRollPitchYaw(m_tParam.m_f3Rotate.x, m_tParam.m_f3Rotate.y, m_tParam.m_f3Rotate.z) *
        DirectX::XMMatrixTranslation(m_tParam.m_f3Pos.x, m_tParam.m_f3Pos.y, m_tParam.m_f3Pos.z));

    return &world;
}

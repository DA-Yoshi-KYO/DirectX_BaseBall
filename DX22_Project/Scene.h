#pragma once

#include "Defines.h"
#include "GameObject.h"
#include <array>
#include <list>

class CScene
{
public:
    CScene();
	virtual ~CScene();
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

    // ゲームオブジェクトを追加する
	template<typename T = CGameObject>
	T* AddGameObject(std::string inName, Tag inTag)
	{
		T* gameObject;
		gameObject = new T();
		m_pGameObject_List[(int)inTag].push_back(gameObject);

        ObjectID id{};
        for (auto itr : m_tIDVec)
        {
            if (itr.m_sName == inName)
            {
                id.m_nSameCount++;
            }
        }
        id.m_sName = inName;
        m_tIDVec.push_back(id);
        gameObject->SetID(id);

		gameObject->Init();
        gameObject->SetTag(inTag);

		return gameObject;
	}

    // ゲームオブジェクトを取得する
	template<typename T = CGameObject>
	T* GetGameObject()
	{
        for (auto list : m_pGameObject_List)
        {
            for (auto obj : list)
            {
                T* ret = dynamic_cast<T*>(obj);
                if (ret != nullptr) return ret;
            }
        }
		return nullptr;
	}

	CGameObject* GetGameObject(ObjectID inID)
	{
        for (auto list : m_pGameObject_List)
        {
            for (auto obj : list)
            {
                ObjectID id = obj->GetID();
                if (id.m_sName == inID.m_sName &&
                    id.m_nSameCount == inID.m_nSameCount)
                {
                    return obj;
                }
            }
        }
		return nullptr;
	}

    CGameObject* GetGameObject(std::string inName)
	{
        for (auto list : m_pGameObject_List)
        {
            for (auto obj : list)
            {
                ObjectID id = obj->GetID();
                if (id.m_sName == inName)
                {
                    return obj;
                }
            }
        }
		return nullptr;
	}

    std::vector<ObjectID> GetIDVec();
    std::array<std::list<CGameObject*>, (int)Tag::Max> GetGameObjectList();

    void DrawGrid();

protected:
    std::array<std::list<CGameObject*>,(int)Tag::Max> m_pGameObject_List; // シーン内のゲームオブジェクトリスト

private:
    std::vector<ObjectID> m_tIDVec;

};

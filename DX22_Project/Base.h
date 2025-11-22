#pragma once
#include "GameObject.h"

enum class BaseKind
{
	Home,
	First,
	Second,
	Third,

	Max
};

class CBase : public CGameObject
{
public:
	CBase();
	~CBase();
	void Init() override;
	void Init(BaseKind kind);
	BaseKind GetKind() { return m_eBaseKind; }
	bool IsBaseCover() { return m_bIsBaseCovered; }
	void SetBaseCover(bool isBaseCover) { m_bIsBaseCovered = isBaseCover; }

private:
	BaseKind m_eBaseKind;
	bool m_bIsBaseCovered;

};

#include "UIManager.h"
#include"GameInstance.h"

//싱글톤

CUIManager::CUIManager()
	:m_pGameInstance{ CGameInstance::GetInstance()}
{
}

void CUIManager::Update(_float fTimeDelta)
{
	CheckSelectObject();
}

void CUIManager::UpdateSetZOrder()
{
	vector<shared_ptr<CUIObject>> vecReorder;
	for (auto Pair : m_UIs)
	{
		if (UIMODE::UI_MOVABLE == Pair.second->GetUIMode())
			vecReorder.push_back(Pair.second);
	}
	sort(vecReorder.begin(), vecReorder.end(),
		[this](shared_ptr<CUIObject> p1, shared_ptr<CUIObject> p2)
		{
			return p1->GetZOrder() < p1->GetZOrder();
		}
	);
	if (!vecReorder.empty()) {
		m_NowZOrderID = 10;
		for (auto Object : vecReorder)
		{
			Object->SetZOrder(m_NowZOrderID++);
		}
	}


}

void CUIManager::AddUIObject(_wstring UITag, shared_ptr<CUIObject> pUIObject)
{
	if (nullptr == pUIObject)return; 

	if (UIMODE::UI_FIXED == pUIObject->GetUIMode())
	{
		//pUIObject->SetZOrder();
	}
	else
	{
		pUIObject->SetZOrder(m_NowZOrderID++);
	}
	m_UIs.emplace(make_pair( UITag, pUIObject));
}

void CUIManager::DeleteUIObjectByName(_wstring UITag)
{
	auto iter = m_UIs.find(UITag);
	if(iter != m_UIs.end())
	m_UIs.erase(iter);
}

void CUIManager::ClearUIObjects()
{
	for (auto& Pair : m_UIs)
	{
		Pair.second.reset(); 
	}
	m_UIs.clear();
	m_pSelectedObject.reset();
}

void CUIManager::CheckSelectObject()
{
	if ((MOUSE_UP(MOUSE::LB)))
	{
		/*movable일때만 마우스 놓으면 초기화*/
		if (m_pSelectedObject  
			&& UIMODE::UI_MOVABLE == dynamic_pointer_cast <CUIObject>(m_pSelectedObject ->GetRootParent())->GetUIMode()) {
			
			m_pSelectedObject->SetSelected(false);
			m_pSelectedObject.reset();
		}
		return;
	}
	if ( !(MOUSE_DOWN(MOUSE::LB)))
		return;

	/*왼쪽 버튼 눌렀을 경우 새로운 거 클릭 시 초기화*/
	if(m_pSelectedObject&& UIMODE::UI_FIXED == dynamic_pointer_cast <CUIObject>(m_pSelectedObject->GetRootParent())->GetUIMode())
		m_pSelectedObject->SetSelected(false);


	/*마우스가 클릭하고 있는 동안에만 */
	vector<shared_ptr<CUIObject>> vecMovableReorder; 
	vector<shared_ptr<CUIObject>> vecFixedSelect;
	for (auto Pair : m_UIs)
	{
		if (UIMODE::UI_MOVABLE == Pair.second->GetUIMode())
		{
			vecMovableReorder.push_back(Pair.second);
		}
		else if(UIMODE::UI_FIXED == Pair.second->GetUIMode()){
			vecFixedSelect.push_back(Pair.second);
		}
	}
	sort(vecMovableReorder.begin(), vecMovableReorder.end(),
		[this](shared_ptr<CUIObject> p1, shared_ptr<CUIObject> p2)
		{
			return p1->GetZOrder() < p2->GetZOrder();
		}
		);
	sort(vecFixedSelect.begin(), vecFixedSelect.end(),
		[this](shared_ptr<CUIObject> p1, shared_ptr<CUIObject> p2)
		{
			return p1->GetZOrder() < p2->GetZOrder();
		}
	);
	/*Movable 로직 처리 */
	if (!vecMovableReorder.empty()) {
		m_NowZOrderID = 10;
		for (auto Object : vecMovableReorder)
		{
			Object->SetZOrder(m_NowZOrderID++);
			if (Object->IsMouseInDomain())
				m_pSelectedObject= Object;
		}
		//m_pSelectedObject = vecReorder.back();
		if (m_pSelectedObject) {

			shared_ptr<CUIObject> tmp_Object = {}; 
			tmp_Object = dynamic_pointer_cast<CUIObject>(m_pSelectedObject->FindChildOnMouse());
			m_pSelectedObject->SetZOrder(m_NowZOrderID++);
			if (tmp_Object.get())//dfs로 최종 자식을 찾는다. 
			{
				m_pSelectedObject = tmp_Object;//대상을 자식으로 교체
			}
			m_pSelectedObject->SetSelected(true);
			
			return;
		}
	}
	///*Fixed  로직 처리*/
	if (!m_bToolMode) {
		if (!vecFixedSelect.empty())
		{
			for (auto Object : vecFixedSelect)
			{
				if (Object->IsMouseInDomain())
					m_pSelectedObject = Object;
			}
			if (m_pSelectedObject)
			{
				shared_ptr<CUIObject> tmp_Object = {};
				tmp_Object = dynamic_pointer_cast<CUIObject>(m_pSelectedObject->FindChildOnMouse());
				if (tmp_Object.get())//dfs로 최종 자식을 찾는다. 
				{
					m_pSelectedObject = tmp_Object;//대상을 자식으로 교체
				}
				m_pSelectedObject->SetSelected(true);

			}

		}
	}


	return; 
}

shared_ptr<CUIObject> CUIManager::GetUIObjectByTag(_wstring UITag)
{
	auto iter = m_UIs.find(UITag);
	if ((iter) == m_UIs.end()) return nullptr; 
	

	return iter->second;
}

shared_ptr<CUIObject> CUIManager::GetSelectedObject()
{
	return m_pSelectedObject;
}

CUIManager* CUIManager::Create()
{
	CUIManager* pInstance = new CUIManager(); 
	if (nullptr == pInstance)
	{
		MSG_BOX("Failed To Make UIManager");
		Safe_Delete(pInstance);
	}

	return pInstance;
}

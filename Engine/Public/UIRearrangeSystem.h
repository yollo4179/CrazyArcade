#pragma once
#include"GameObject.h"
BEGIN(Engine)
namespace RearrangeUISystem
{
	/*UI 내부에서 호출*/
	void RearrangeGridUI(
		const shared_ptr<CGameObject>&ParentObject,
		const vector<shared_ptr<CGameObject>>&  ChildrenPtr,
		_float2 Padding,
		_float2 Space,
		_float2 CellSize,
		_uint NumColumns =1
		)
	{
		_float4 vPanelInfo = ParentObject->ComputeCollisionRect();

		_float2 PanelSize = { 
			(vPanelInfo.y - vPanelInfo.x)  ,
			(vPanelInfo.w - vPanelInfo.z) 
		};
		//0을 기준으로 잡는다 Parent의 position은 (0,0)을 기준 
		_float2 vPivot = _float2{
			-1.f* PanelSize.x * 0.5f ,
			 1.f* PanelSize.y * 0.5f
		};
		Padding.y *= -1.f;
		_vector vInitPosition = {
			vPivot.x + Padding.x+ (CellSize.x* PanelSize.x * 0.5f),
			vPivot.y + Padding.y- (CellSize.y* PanelSize.y * 0.5f),
			0.f,
			1.f
		};
		_vector vCursor = vInitPosition;

		_float SpaceY = {}; 
		int iIndex = 0;
		for (auto Child: ChildrenPtr)
		{
			if ( 0 == (iIndex % NumColumns))
			{
				vCursor.m128_f32[0] = vInitPosition.m128_f32[0];
				if(iIndex) vCursor.m128_f32[1] -= (Space.y + (CellSize.y * PanelSize.y));

			}
			else
			vCursor.m128_f32[0] += (Space.x + (CellSize.x* PanelSize.x));
			
			Child->GetTransform()->SetState(CTransform::STATE_POSITION, vCursor);
			Child->GetTransform()->Scailing(_float3(CellSize.x, CellSize.y ,1.f));
			
			
			
			iIndex++;
		}

	}

}
END
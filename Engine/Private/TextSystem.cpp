#include"TextSystem.h"
#include"GameInstance.h"

BEGIN(TextSystem)
_float2 ENGINE_DLL TextSystem::GetTextPos(_vector WorldPos, _float2 Offset)
{
	shared_ptr<CGameInstance> m_pGameInstance = CGameInstance::GetInstance();

	/*뷰는 항등이다*/
	 _matrix matProj = XMLoadFloat4x4( m_pGameInstance->GetPipeLineMatrixPtr(PIPELINE_MATRICES::D3DTS_PROJ));
	 //z 나누기는 알아서 쿠드가 어차피 1이지만 
	 WorldPos += XMLoadFloat2(&Offset);//오프셋 더하고 
	_vector NDC = XMVector3TransformCoord(WorldPos, matProj);
	/*Ndc*/
	_float2 ViewPortSize = m_pGameInstance->GetViewPortSize(); 
	//ToScreenSpace 
	_float2 NDCPos = { NDC.m128_f32[0],NDC.m128_f32[1] };
	_float2 ScreenPos = { (NDCPos.x + 1.f) * 0.5f * ViewPortSize.x  , (NDCPos.y - 1.f) * -0.5f * ViewPortSize.y };


	return _float2( ScreenPos.x, ScreenPos.y );
}

END

TextSystem::TextInputState::TextInputState()
	:m_pGameInstance{ CGameInstance::GetInstance() }
{
};

void TextSystem::TextInputState::Initialize(_wstring _FontTag, _matrix _MatWorld, _float _HeightOffset, _vector _vColor)
{
	FontTag = _FontTag;
	SRTMatrix = _MatWorld;
	vColor = _vColor;
	HeightOffset = _HeightOffset;

	NextLineSpacing = m_pGameInstance
		->GetSpriteFont(_FontTag)->GetLineSpacing(); 

	/*WhiteSpace Width*/
	WhiteSpaceWidth = m_pGameInstance
		->GetSpriteFont(_FontTag)
		->MeasureString(L" ", false).m128_f32[0];

	/*Caret Width*/
	CaretWidth = m_pGameInstance
		->GetSpriteFont(_FontTag)
		->MeasureString(L"|", false).m128_f32[0];

}

void TextSystem::TextInputState::UpdateBlinkTerm(_float fTimeDelta)
{
	BlinkPeriod += fTimeDelta;
	if (1.f <= BlinkPeriod)
		BlinkPeriod = 0.f;
}

void TextSystem::TextInputState::UpdateInput(_float fTimeDelta)
{
	CaretPos.x = m_pGameInstance->GetSpriteFont(FontTag)
		->MeasureString(FullText.substr(0,CaretIndex).c_str(), false).m128_f32[0];

	if (FullText.size()
		&& CaretIndex >= 1
		&& FullText[CaretIndex- 1] == L' '
		)
	{
		CaretPos.x -= WhiteSpaceWidth * 0.5F;
	}

	auto CompositionSize = m_pGameInstance->GetSpriteFont(FontTag)
		->MeasureString(CharacterFactory.c_str(), false);

	CaretPos.x = CaretPos.x  - CaretWidth*0.5f + CompositionSize.m128_f32[0];
	FullText = {};
	UpdateBlinkTerm(fTimeDelta);
	FullText = Buffer;
	FullText += CharacterFactory;

}

void TextSystem::TextInputState::Render_Text()
{
	_vector vPos = SRTMatrix.r[3] - SRTMatrix.r[0] * 0.5f+ SRTMatrix.r[1] * HeightOffset;
	m_pGameInstance->RenderFont(
		FontTag, FullText.c_str()
		, GetTextPos(vPos)
		, vColor
		, 0
		, _float2(0.f, 0.f)
		,1.f
	);
	if (IsSelected && static_cast<_int>(BlinkPeriod * 2))
	{
		m_pGameInstance->RenderFont(
			FontTag,L"|"
			, GetTextPos(vPos, CaretPos)
			, vColor
			, 0
			, _float2(0.f, 0.f)
			,1.f
		);
	}
}

void TextSystem::TextInputState::Render_Text(_wstring _String, _vector vColor)
{
	_vector vPos = SRTMatrix.r[3] -SRTMatrix.r[0] * 0.5f + SRTMatrix.r[1] * HeightOffset;
	m_pGameInstance->RenderFont(
		FontTag, _String.c_str()
		, GetTextPos(vPos)
		, vColor
		, 0
		, _float2(0.f, 0.f)
		, 1.f
	);

}

void TextSystem::TextInputState::Render_Texts(deque<pair<_wstring, _vector> >& Bunches, _int Index1 , _int NumDisplayLogs)
{

	/*Setting*/
	_vector vPos = SRTMatrix.r[3] - SRTMatrix.r[0] * 0.5f + SRTMatrix.r[1] * 0.5f;
	NumDisplayLogs = min(NumDisplayLogs, Bunches.size());

	
	for (int i=Index1 ; i < NumDisplayLogs;  ++i)
	{
		_float2 Offset = _float2( 0.f , (-i + Index1) * (5.f+NextLineSpacing*0.5f));
		m_pGameInstance->RenderFont(
			FontTag, Bunches[i].first.c_str()
			, GetTextPos(vPos, Offset)
			, Bunches[i].second//vColor
			, 0
			, _float2(0.f, 0.f)
			, 1.f
		);
	}
}

void TextSystem::TextInputState::ClearAllBuffer()
{

	FullText.clear(); 
	Buffer.clear(); 
	CharacterFactory.clear();
	CaretPos = {}; 
	InputFinished = true;
	CaretIndex = 0; 




}

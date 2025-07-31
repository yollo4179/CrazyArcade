#pragma once
#include"Base.h"

namespace TextSystem
{

	typedef struct ENGINE_DLL TextInputState {

		TextInputState();
		void Initialize(_wstring _FontTag, _matrix _MatWorld, _float HeightOffset = 0.7f, _vector _vColor = Colors::Black);
		void SetMatrix(_matrix _MatWorld) {
			SRTMatrix = _MatWorld;
		};
		void UpdateBlinkTerm(_float fTimeDelta);
		void UpdateInput(_float TimeDelta);
		void Render_Text();
		void Render_Text(_wstring _String,_vector vColor =Colors::White);
		void Render_Texts(deque<pair<_wstring, _vector> >& Bunches,_int Index1=0,_int NumDisplayLogs =0 );

		void SelectStateIs(_bool _State) { IsSelected = _State; };
		_bool GetSelectState() { return IsSelected; };

		void ClearAllBuffer(); 
		wstring FullText;
		wstring Buffer;
		wstring CharacterFactory;
		size_t CaretIndex = { 0 }; //문자열의 위치 (인덱스 )
		_float2 CaretPos = {};
		_float BlinkPeriod = { 0.f };
		_matrix SRTMatrix = {};

		/*Input*/
		_bool InputFinished = { false };
		//FontScale
		_float HeightOffset = { 1.f };
		/*Size*/
		_float CaretWidth;
		_float WhiteSpaceWidth;

		_float NextLineSpacing = {};
		_wstring FontTag;
		/*색상*/
		_vector vColor = {}; 
		_bool IsSelected = {false};
		/*GameInstnace*/
		shared_ptr<CGameInstance> m_pGameInstance = {};
	}TEXT_INPUT_STATE;

	_float2 ENGINE_DLL GetTextPos(_vector WorldPos, _float2 Offset = {});


}
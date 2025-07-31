#include "Texture.h"
#include "Shader.h"
#include"GameInstance.h"
CTexture::CTexture(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    : CComponent{ pDevice, pContext }
{
}

CTexture::CTexture(const CTexture& Prototype)
    : CComponent{ Prototype }
    , m_iNumSRVs{ Prototype.m_iNumSRVs }
    , m_SRVs{ Prototype.m_SRVs }
{
}

HRESULT CTexture::Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTexture)
{
	m_iNumSRVs = iNumTexture;

	_tchar			szTextureFilePath[MAX_PATH] = TEXT("");

	for (size_t i = 0; i < m_iNumSRVs; i++)
	{
		wsprintf(szTextureFilePath, pTextureFilePath, i);

		/* 드라이브경로, 디렉토리경로, 파일네임, 파일확장자. */
		_tchar		szEXT[MAX_PATH] = TEXT("");

		/* D:\정의훈\147\3d\Framework\Client\Bin\Resources\Textures\Default.jpg */
		_wsplitpath_s(szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);

		HRESULT		hr = {};

		ComPtr<ID3D11ShaderResourceView> pSRV = { nullptr };

		if (false == lstrcmpW(szEXT, TEXT(".dds")))
		{
			hr = DirectX::CreateDDSTextureFromFile(m_pDevice.Get(), szTextureFilePath, nullptr, pSRV.GetAddressOf());
		}
		else if (false == lstrcmpW(szEXT, TEXT(".tga")))
		{

		}
		else
		{
			hr = DirectX::CreateWICTextureFromFile(m_pDevice.Get(), szTextureFilePath, nullptr, pSRV.GetAddressOf());
		}


		if (FAILED(hr))
			return E_FAIL;

		m_SRVs.push_back(pSRV);
	}


	return S_OK;
}

HRESULT CTexture::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CTexture::Bind_ShaderResource(CShader* pShader, const _char* pConstantName, _uint iSRVIndex)
{
	if (iSRVIndex >= m_iNumSRVs)
		return E_FAIL;

	return pShader->Bind_SRV(pConstantName, m_SRVs[iSRVIndex].Get());
}

HRESULT CTexture::Bind_ShaderResources(CShader* pShader, const _char* pConstantName)
{
	return pShader->Bind_SRVs(pConstantName, &m_SRVs.front(), m_SRVs.size());
}

#ifdef  _DEBUG
void CTexture::Inspector()
{
	ImGui::Text("텍스처 정보");
	ImGui::Separator();
	static int NowIndex = 0;
	if(m_iNumSRVs)
	ImGui::SliderInt("텍스처 넘버", &NowIndex, 0, m_iNumSRVs - 1);
	ImGui::Separator();


	
	
	ImGui::ImageButton("텍스처", m_SRVs[NowIndex].Get(),ImVec2(100.f,100.f));
	

	ImGui::Separator();
	char Char[MAX_PATH] = {};
	string Str = m_pGameInstance->Wbcs2Mbcs(m_ProtoTag);
	strcpy_s(Char, Str.c_str());
	
	ImGui::InputText("프로토 타입 태그", Char, m_pGameInstance->Wbcs2Mbcs(m_ProtoTag).size());
	ImGui::Separator();


}
#endif
shared_ptr<CTexture> CTexture::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, const _tchar* pTextureFilePath, _uint iNumTextures)
{
	shared_ptr<CTexture> pInstance ( new CTexture(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype(pTextureFilePath, iNumTextures)))
	{
		MSG_BOX("Failed to Created : CTexture");
		;
	}
	return pInstance;
}

shared_ptr<CComponent> CTexture::Clone(void* pArg)
{
	shared_ptr<CTexture> pInstance ( new CTexture(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTexture");
	
	}

	return pInstance;
}

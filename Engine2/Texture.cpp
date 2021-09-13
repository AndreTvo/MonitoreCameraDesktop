
#include <assert.h>
#include "Texture.h"

//---------------------------------------------------------------------------------
static HRESULT CreateTextureFromBitmapWIC(_In_ ID3D11Device* d3dDevice,
	_In_opt_ ID3D11DeviceContext* d3dContext,
	_In_ UINT width,
	_In_ UINT height,
	_Out_opt_ ID3D11Resource** texture,
	_Out_opt_ ID3D11ShaderResourceView** textureViewY,
	_Out_opt_ ID3D11ShaderResourceView** textureViewUV,
	_In_ size_t maxsize)
{
	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_NV12;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	ID3D11Texture2D* tex = nullptr;
	HRESULT hr = d3dDevice->CreateTexture2D(&desc, nullptr, &tex);

	if (SUCCEEDED(hr) && tex != 0)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		memset(&SRVDesc, 0, sizeof(SRVDesc));
		SRVDesc.Format = DXGI_FORMAT_R8_UNORM;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;

		hr = d3dDevice->CreateShaderResourceView(tex, &SRVDesc, textureViewY);
		if (FAILED(hr))
		{
			tex->Release();
			return hr;
		}

		SRVDesc.Format = DXGI_FORMAT_R8G8_UNORM;

		hr = d3dDevice->CreateShaderResourceView(tex, &SRVDesc, textureViewUV);
		if (FAILED(hr))
		{
			tex->Release();
			return hr;
		}

		if (texture)
		{
			*texture = tex;
		}
	}

	return hr;
}

HRESULT D3D11CreateTextureFromMemory(_In_ ID3D11Device* d3dDevice,
	_In_opt_ ID3D11DeviceContext* d3dContext,
	_Out_opt_ ID3D11Resource** texture,
	_Out_opt_ ID3D11ShaderResourceView** textureViewY,
	_Out_opt_ ID3D11ShaderResourceView** textureViewUV,
	unsigned & width,
	unsigned & height,
	_In_ size_t maxsize)
{
	HRESULT hr = CreateTextureFromBitmapWIC(d3dDevice, d3dContext, width, height, texture, textureViewY, textureViewUV, 0);
	return hr;
}

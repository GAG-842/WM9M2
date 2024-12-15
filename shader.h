#pragma once
#include <d3d11.h>
#include <iostream>
#include <fstream>
#include <D3Dcompiler.h>
#include <d3d11shader.h>
#include <sstream>
#include "MdxCore.h"
#pragma comment(lib, "dxguid.lib")
#include "ShaderReflection.h"
using namespace std;

class shader {
public:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	std::vector<ConstantBuffer> psConstantBuffers;
	std::vector<ConstantBuffer> vsConstantBuffers;
	std::map<std::string, int> textureBindPointsVS;
	std::map<std::string, int> textureBindPointsPS;
	ID3D11Buffer* constantBuffer;
	string loadfile(string filename) {
		stringstream buffer;
		ifstream file(filename);
		buffer << file.rdbuf();
		file.close();
		return buffer.str();
	}

	void loadStatic(DxCore* core, string vsfilename, string psfilename) {
		string vshlsl = loadfile(vsfilename);
		string pshlsl = loadfile(psfilename);
		initConstBuffer(core, sizeof(ConstantBuffer) + (16 - sizeof(ConstantBuffer) % 16));
		loadVSStatic(core, vshlsl);
		loadPS(core, pshlsl);
	}
	void loadAnimation(DxCore* core, string vsfilename, string psfilename) {
		string vshlsl = loadfile(vsfilename);
		string pshlsl = loadfile(psfilename);
		initConstBuffer(core, sizeof(ConstantBuffer) + (16 - sizeof(ConstantBuffer) % 16));
		loadVSAnimation(core, vshlsl);
		loadPS(core, pshlsl);
	}

	void initConstBuffer(DxCore* core, int sizeInBytes) {
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA data;
		bd.ByteWidth = sizeInBytes;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		core->device->CreateBuffer(&bd, NULL, &constantBuffer);
	}

	void loadVSStatic(DxCore *core, string vshlsl) {
		ID3DBlob* compiledVertexShader;
		ID3DBlob* status;
		HRESULT hr = D3DCompile(vshlsl.c_str(), strlen(vshlsl.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &compiledVertexShader, &status);
		if (FAILED(hr))
		{
			(char*)status->GetBufferPointer();
		}
		core->device->CreateVertexShader(compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), NULL, &vertexShader);
		// 2D三角形
		//D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		//{
		//	{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//	{ "COLOUR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//};

		//core->device->CreateInputLayout(layoutDesc, 2, compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &layout);
		
		// 3D图形
		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		core->device->CreateInputLayout(layoutDesc, 4, compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &layout);
		
		ConstantBufferReflection reflection;
		reflection.build(core, compiledVertexShader, vsConstantBuffers, textureBindPointsVS, ShaderStage::VertexShader);

	}

	void loadVSAnimation(DxCore* core, string vshlsl) {
		ID3DBlob* compiledVertexShader;
		ID3DBlob* status;
		HRESULT hr = D3DCompile(vshlsl.c_str(), strlen(vshlsl.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &compiledVertexShader, &status);
		if (FAILED(hr))
		{
			(char*)status->GetBufferPointer();
		}
		core->device->CreateVertexShader(compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), NULL, &vertexShader);
		
		D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
			{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 								D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 								D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 								D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 								D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		core->device->CreateInputLayout(layoutDesc, 6, compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &layout);

		ConstantBufferReflection reflection;
		reflection.build(core, compiledVertexShader, vsConstantBuffers, textureBindPointsVS, ShaderStage::VertexShader);
	}

	void loadPS(DxCore* core, std::string hlsl)
	{
		ID3DBlob* compiledPixelShader;
		ID3DBlob* status;
		HRESULT hr = D3DCompile(hlsl.c_str(), strlen(hlsl.c_str()), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, &compiledPixelShader, &status);
		if (FAILED(hr))
		{
			printf("%s\n", (char*)status->GetBufferPointer());
			exit(0);
		}
		core->device->CreatePixelShader(compiledPixelShader->GetBufferPointer(), compiledPixelShader->GetBufferSize(), NULL, &pixelShader);
		ConstantBufferReflection reflection;
		reflection.build(core, compiledPixelShader, psConstantBuffers, textureBindPointsPS, ShaderStage::PixelShader);
	}

	void updateConstant(std::string constantBufferName, std::string variableName, void* data, std::vector<ConstantBuffer>& buffers)
	{
		for (int i = 0; i < buffers.size(); i++)
		{
			if (buffers[i].name == constantBufferName)
			{
				buffers[i].update(variableName, data);
				return;
			}
		}
	}
	void updateConstantVS(std::string constantBufferName, std::string variableName, void* data)
	{
		for (int i = 0; i < vsConstantBuffers.size(); i++) {
			if (vsConstantBuffers[i].name == constantBufferName) {
				vsConstantBuffers[i].update(variableName, data);
			}
		}
	}
	void updateConstantPS(std::string constantBufferName, std::string variableName, void* data)
	{
		for (int i = 0; i < psConstantBuffers.size(); i++) {
			if (psConstantBuffers[i].name == constantBufferName) {
				psConstantBuffers[i].update(variableName, data);
			}
		}
	}

	//find bind slots from shader reflection
	//void updateTextureVS(DxCore* core, std::string name, ID3D11ShaderResourceView* tex) {
	//	core->devicecontext->VSSetShaderResources(textureBindPointsVS[name], 1, &tex);
	//}

	void updateTexturePS(DxCore* core, std::string name, ID3D11ShaderResourceView* tex) {
		core->devicecontext->PSSetShaderResources(textureBindPointsPS[name], 1, &tex);
	}

	void apply(DxCore* core) {
		core->devicecontext->IASetInputLayout(layout);
		core->devicecontext->VSSetShader(vertexShader, NULL, 0);
		core->devicecontext->PSSetShader(pixelShader, NULL, 0);
		core->devicecontext->PSSetConstantBuffers(0, 1, &constantBuffer);

		for (int i = 0; i < vsConstantBuffers.size();i++) {
			vsConstantBuffers[i].upload(core);
		}
		for (int i = 0; i < psConstantBuffers.size();i++) {
			psConstantBuffers[i].upload(core);
		}
	}

};



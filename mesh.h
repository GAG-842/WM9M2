#pragma once
#include "mathcal.h"
#include "MdxCore.h"
#include "shader.h"
#include "GEMLoader.h"
#include "Texture.h"
#define _USE_MATH_DEFINES
#define SQ(x) (x * x)

class Triangle {
public:
	struct Vertex {
		Vec3 position;
		Colour colour;
	};
	ID3D11Buffer* vertexBuffer;

	void init(DxCore* dc, int N = 10 ) {
		Vertex vertices[3];
		vertices[0].position = Vec3(0, 1.0f, 0);
		vertices[0].colour = Colour(0, 1.0f, 0, 0);
		vertices[1].position = Vec3(-1.0f, -1.0f, 0);
		vertices[1].colour = Colour(1.0f, 0, 0, 0);
		vertices[2].position = Vec3(1.0f, -1.0f, 0);
		vertices[2].colour = Colour(0, 0, 1.0f, 0);

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA uploadData;
		bd.ByteWidth = sizeof(Vertex) * N;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		uploadData.pSysMem = vertices;
		uploadData.SysMemPitch = 0;
		uploadData.SysMemSlicePitch = 0;
		dc->device->CreateBuffer(&bd, &uploadData, &vertexBuffer);

	}
	void draw(DxCore* dc) {
		UINT offsets;
		offsets = 0;
		UINT strides = sizeof(Vertex);
		dc->devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		dc->devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
		dc->devicecontext->Draw(3, 0);

	}
};

struct STATIC_VERTEX
{
	Vec3 pos;
	Vec3 normal;
	Vec3 tangent;
	float tu;
	float tv;
};

struct ANIMATED_VERTEX
{
	Vec3 pos;
	Vec3 normal;
	Vec3 tangent;
	float tu;
	float tv;
	unsigned int bonesIDs[4];
	float boneWeights[4];
};

	
class mesh {
public:
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	int indicesSize;
	UINT strides;

	void init(DxCore* dc, void* vertices, int vertexSizeInBytes, int numVertices, unsigned int* indices, int numIndices) {
		D3D11_BUFFER_DESC bd;
		memset(&bd, 0, sizeof(D3D11_BUFFER_DESC));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * numIndices;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA data;
		memset(&data, 0, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = indices;
		dc->device->CreateBuffer(&bd, &data, &indexBuffer);
		bd.ByteWidth = vertexSizeInBytes * numVertices;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		data.pSysMem = vertices;
		dc->device->CreateBuffer(&bd, &data, &vertexBuffer);
		indicesSize = numIndices;
		strides = vertexSizeInBytes;
	}
	void init(DxCore* dc, std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices)
	{
		init(dc,&vertices[0], sizeof(STATIC_VERTEX), vertices.size(), &indices[0], indices.size());
	}

	void init(DxCore* dc, std::vector<ANIMATED_VERTEX> vertices, std::vector<unsigned int> indices)
	{
		init(dc, &vertices[0], sizeof(ANIMATED_VERTEX), vertices.size(), &indices[0], indices.size());
	}

	void draw(DxCore* dc) {
		UINT offsets = 0;
		dc->devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		dc->devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
		dc->devicecontext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		dc->devicecontext->DrawIndexed(indicesSize, 0, 0);
	}
};

class plane {
public:
	mesh geometry;
	STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = Vec3(0, 0, 0);
		v.tu = tu;
		v.tv = tv;
		return v;
	}
	
	TextureManager planeTextures;
	void init(DxCore* core) {
		std::vector<STATIC_VERTEX> vertices;
		vertices.push_back(addVertex(Vec3(-20, 0, -20), Vec3(0, 1, 0), 0, 0));
		vertices.push_back(addVertex(Vec3(20, 0, -20), Vec3(0, 1, 0), 1, 0));
		vertices.push_back(addVertex(Vec3(-20, 0, 20), Vec3(0, 1, 0), 0, 1));
		vertices.push_back(addVertex(Vec3(20, 0, 20), Vec3(0, 1, 0), 1, 1));
		std::vector<unsigned int> indices;
		indices.push_back(2); indices.push_back(1); indices.push_back(0);
		indices.push_back(1); indices.push_back(2); indices.push_back(3);
		planeTextures.load(core, "EnvironmentTextures/Grass.jpg");
		geometry.init(core, vertices, indices);
	}

	void draw(shader* shaders, DxCore* core, float dt) {
		Matrix worldPlane;
		worldPlane.Scaling(Vec3(10.f, 10.f, 10.f));
		shaders->updateConstantVS("MeshBuffer", "W", &worldPlane);
		shaders->updateTexturePS(core, "tex", planeTextures.find("EnvironmentTextures/Grass.jpg"));
		shaders->apply(core);
		geometry.draw(core);
	}
};

class cube {

public:
	mesh geometry;
	STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = Vec3(0, 0, 0);
		v.tu = tu;
		v.tv = tv;
		return v;
	}

	void init(DxCore* core) {
		std::vector<STATIC_VERTEX> vertices;
		Vec3 p0 = Vec3(-1.0f, -1.0f, -1.0f);
		Vec3 p1 = Vec3(1.0f, -1.0f, -1.0f);
		Vec3 p2 = Vec3(1.0f, 1.0f, -1.0f);
		Vec3 p3 = Vec3(-1.0f, 1.0f, -1.0f);
		Vec3 p4 = Vec3(-1.0f, -1.0f, 1.0f);
		Vec3 p5 = Vec3(1.0f, -1.0f, 1.0f);
		Vec3 p6 = Vec3(1.0f, 1.0f, 1.0f);
		Vec3 p7 = Vec3(-1.0f, 1.0f, 1.0f);

		vertices.push_back(addVertex(p0, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p1, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p2, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p3, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f));

		vertices.push_back(addVertex(p5, Vec3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p4, Vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p7, Vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p6, Vec3(0.0f, 0.0f, 1.0f), 0.0f, 0.0f));

		vertices.push_back(addVertex(p4, Vec3(-1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p0, Vec3(-1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p3, Vec3(-1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p7, Vec3(-1.0f, 0.0f, 0.0f), 0.0f, 0.0f));

		vertices.push_back(addVertex(p1, Vec3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p5, Vec3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p6, Vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p2, Vec3(1.0f, 0.0f, 0.0f), 0.0f, 0.0f));
		//top
		vertices.push_back(addVertex(p3, Vec3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p2, Vec3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p6, Vec3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p7, Vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f));
		//bottom
		vertices.push_back(addVertex(p4, Vec3(0.0f, -1.0f, 0.0f), 0.0f, 1.0f));
		vertices.push_back(addVertex(p5, Vec3(0.0f, -1.0f, 0.0f), 1.0f, 1.0f));
		vertices.push_back(addVertex(p1, Vec3(0.0f, -1.0f, 0.0f), 1.0f, 0.0f));
		vertices.push_back(addVertex(p0, Vec3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f));

		std::vector<unsigned int> indices;
		indices.push_back(0); indices.push_back(1); indices.push_back(2);
		indices.push_back(0); indices.push_back(2); indices.push_back(3);

		indices.push_back(4); indices.push_back(5); indices.push_back(6);
		indices.push_back(4); indices.push_back(6); indices.push_back(7);

		indices.push_back(8); indices.push_back(9); indices.push_back(10);
		indices.push_back(8); indices.push_back(10); indices.push_back(11);

		indices.push_back(12); indices.push_back(13); indices.push_back(14);
		indices.push_back(12); indices.push_back(14); indices.push_back(15);

		indices.push_back(16); indices.push_back(17); indices.push_back(18);
		indices.push_back(16); indices.push_back(18); indices.push_back(19);

		indices.push_back(20); indices.push_back(21); indices.push_back(22);
		indices.push_back(20); indices.push_back(22); indices.push_back(23);
		geometry.init(core, vertices, indices);
	}
	void draw(shader* shaders, DxCore* core, float dt) {
		Matrix worldPlane;
		shaders->updateConstantVS("MeshBuffer", "W", &worldPlane);
		shaders->apply(core);
		geometry.draw(core);
	}
};



class sphere {
public:
	mesh geometry;
	TextureManager skytextures;
	STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv)
	{
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = Vec3(0, 0, 0);
		v.tu = tu;
		v.tv = tv;
		return v;
	}

	void init(DxCore* core, int rings, int segments, float radius) {
		std::vector<STATIC_VERTEX> vertices;
		for (int lat = 0; lat <= rings; lat++) {
			float theta = lat * M_PI / rings;
			float sinTheta = sinf(theta);
			float cosTheta = cosf(theta);
			for (int lon = 0; lon <= segments; lon++) {
				float phi = lon * 2.0f * M_PI / segments;
				float sinPhi = sinf(phi);
				float cosPhi = cosf(phi);
				Vec3 position(radius * sinTheta * cosPhi, radius * cosTheta, radius * sinTheta * sinPhi);
				Vec3 normal = position.normalize();
				float tu = 1.0f - (float)lon / segments;
				float tv =(float)lat / rings;
				vertices.push_back(addVertex(position, normal, tu, tv));
			}
		}
		std::vector<unsigned int> indices;
		for (int lat = 0; lat < rings; lat++)
		{
			for (int lon = 0; lon < segments; lon++)
			{
				int current = lat * (segments + 1) + lon;
				int next = current + segments + 1;
				indices.push_back(current);
				indices.push_back(next);
				indices.push_back(current + 1);
				indices.push_back(current + 1);
				indices.push_back(next);
				indices.push_back(next + 1);
			}
		}
		skytextures.load(core, "EnvironmentTextures/overcast_soil_puresky.jpg");
		geometry.init(core, vertices, indices);
	}
	void draw(shader* shaders, DxCore* core, float dt) {
		float t = dt / 1000;
		Vec3 from = Vec3(11 * cos(t), 5, 11 * sinf(t));
		Matrix v;
		Matrix p;
		Matrix worldPlane;
		p = p.perspective(0.9f, 1024.0f / 768.0f, 80.f, 100.f);
		v = v.lookAt(from, Vec3(0.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f));
		Matrix vp = v * p;
		shaders->updateConstantVS("MeshBuffer", "W", &worldPlane);
		//shaders->updateConstantVS("staticMeshBuffer", "VP", &vp)
		shaders->updateTexturePS(core, "tex", skytextures.find("EnvironmentTextures/overcast_soil_puresky.jpg"));
		shaders->apply(core);
		geometry.draw(core);
	} 
};

class tree {
public:
	std::vector<mesh> meshes;
	std::vector<std::string> textureFilenames;
	TextureManager treetextures;
	string filename;
	void init(DxCore* core, string _filename) {
		//old
		//GEMLoader::GEMModelLoader loader;
		//std::vector<GEMLoader::GEMMesh> gemmeshes;
		//loader.load(filename, gemmeshes);
		//for (int i = 0; i < gemmeshes.size(); i++) {
		//	mesh mesh;
		//	std::vector<STATIC_VERTEX> vertices;
		//	for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
		//		STATIC_VERTEX v;
		//		memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
		//		vertices.push_back(v);
		//	}
		//	mesh.init(core, vertices, gemmeshes[i].indices);
		//	meshes.push_back(mesh);
		//}


		//New
		
		GEMLoader::GEMModelLoader loader;
		std::vector<GEMLoader::GEMMesh> gemmeshes;
		filename = _filename;
		loader.load(filename, gemmeshes);
		for (int i = 0; i < gemmeshes.size(); i++) {
			mesh mesh;
			std::vector<STATIC_VERTEX> vertices;
			for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
				STATIC_VERTEX v;
				memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
				vertices.push_back(v);
			}
			textureFilenames.push_back(gemmeshes[i].material.find("diffuse").getValue());
			// Load texture with filename: gemmeshes[i].material.find("diffuse").getValue()
			treetextures.load(core, gemmeshes[i].material.find("diffuse").getValue());

			mesh.init(core, vertices, gemmeshes[i].indices);
			meshes.push_back(mesh);
			//treetextures.unload(gemmeshes[i].material.find("diffuse").getValue());
		}
	}
	void draw(shader* shaders, DxCore* core, Vec3 tm) {
		Matrix worldPlane;
		Matrix translationMatrix;
		translationMatrix.Translation(tm); // X: 摄像头zuoyou，y：高低，z：qian hou
		Matrix scalingMatrix;
		scalingMatrix.Scaling(Vec3(0.01f, 0.01f, 0.01f));
		// 组合变换矩阵
		Matrix worldPosition = scalingMatrix * translationMatrix;
		shaders->updateConstantVS("MeshBuffer", "W", &worldPosition);
		shaders->apply(core);
		for (int i = 0; i < meshes.size(); i++)
		{
			shaders->updateTexturePS(core, "tex", treetextures.find(textureFilenames[i]));
			meshes[i].draw(core);
		}
	}
};


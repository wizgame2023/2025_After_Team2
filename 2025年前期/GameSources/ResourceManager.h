/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
namespace basecross{

	class ResourceManager {
		static wstring g_ResourcesFilePath;
		static Json g_ResourceJson;
	public:
		static void SetResourcesFilePath(const wstring& path) {
			g_ResourcesFilePath = path;
		}
		static void Load(const wstring& filename) {
			g_ResourceJson = Json(g_ResourcesFilePath + filename);
		}
		static void RegisterTexture(const wstring& foldername) {
			auto& app = App::GetApp();
			auto folder = g_ResourceJson.At<JsonObject>(foldername);
			vector<wstring> filenames = folder->GetKeys();
			wstring path = app->GetDataDirWString() + foldername + L"/";
			for (auto& filename : filenames) {
				wstring texKey = folder->At<JsonString>(filename)->GetValue();
				app->RegisterTexture(texKey, path + filename);
			}
		}
		static void RegisterResource(const wstring& foldername) {
			auto& app = App::GetApp();
			auto folder = g_ResourceJson.At<JsonObject>(foldername);
			wstring path = app->GetDataDirWString() + foldername + L"/";
			vector<wstring> filenames = folder->GetKeys();
			for (auto& filename : filenames) {
				auto object = folder->At<JsonObject>(filename);
				auto meshKey = object->At<JsonString>(L"model")->GetValue();//登録名
				auto texKey = object->At<JsonString>(L"texture")->GetValue();//テクスチャ
				auto type = object->At<JsonString>(L"type")->GetValue();//モデルタイプ(static or bone)

				shared_ptr<MeshResource> modelMesh;
				if (type == L"static") {
					modelMesh = MeshResource::CreateStaticModelMesh(path, filename);
				}
				else{
					modelMesh = MeshResource::CreateBoneModelMesh(path, filename);
				}
				app->RegisterResource(meshKey, modelMesh);
			}
		}

		static void RegisterWav(const wstring& foldername) {
			auto& app = App::GetApp();
			auto folder = g_ResourceJson.At<JsonObject>(foldername);
			vector<wstring> filenames = folder->GetKeys();
			wstring path = app->GetDataDirWString() + foldername + L"/";
			for (auto& filename : filenames) {
				wstring wavKey = folder->At<JsonString>(filename)->GetValue();
				auto audioRes = App::GetApp()->RegisterWav(wavKey, path + filename);
			}
		}
	};
}
//end basecross

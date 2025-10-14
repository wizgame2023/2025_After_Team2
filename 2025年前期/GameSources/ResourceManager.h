/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
namespace basecross{


	class ResourceManager {
		static Json g_ResourceJson;
	public:

		static void Load(const wstring& filename) {
			g_ResourceJson = Json(filename);
		}
		static void RegisterTexture(const wstring& foldername) {
			auto& app = App::GetApp();
			auto folder = g_ResourceJson.At<JsonObject>(foldername);
			vector<wstring> keys = folder->GetKeys();
			wstring path = app->GetDataDirWString() + foldername + L"/";
			for (auto& key : keys) {
				wstring texKey = folder->At<JsonString>(key)->GetValue();
				app->RegisterTexture(texKey, path + key);
			}
		}
		static void RegisterResource(const wstring& foldername) {
			auto& app = App::GetApp();
			auto folder = g_ResourceJson.At<JsonObject>(foldername);
			wstring path = app->GetDataDirWString() + foldername + L"/";
			vector<wstring> keys = folder->GetKeys();
			for (auto& key : keys) {
				auto object = folder->At<JsonObject>(key);
				auto meshKey = object->At<JsonString>(L"model")->GetValue();//登録名
				auto texKey = object->At<JsonString>(L"texture")->GetValue();//テクスチャ
				auto type = object->At<JsonString>(L"type")->GetValue();//モデルタイプ(static or bone)

				shared_ptr<MeshResource> modelMesh;
				if (type == L"static") {
					modelMesh = MeshResource::CreateStaticModelMesh(path, key);
				}
				else{
					modelMesh = MeshResource::CreateBoneModelMesh(path, key);
				}
				app->RegisterResource(meshKey, modelMesh);
			}
		}
	};
}
//end basecross

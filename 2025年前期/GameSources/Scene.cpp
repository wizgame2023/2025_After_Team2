
/*!
@file Scene.cpp
@brief シーン実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	void Scene::OnCreate(){
		try {

			auto& app = App::GetApp();

			wstring path = app->GetDataDirWString() + L"Model/";
			auto staticModel = MeshResource::CreateStaticModelMesh(path, L"Player.bmf");
			app->RegisterResource(L"PLAYER_MD", staticModel);
			staticModel = MeshResource::CreateStaticModelMesh(path, L"Goal.bmf");
			app->RegisterResource(L"GOAL_MD", staticModel);


			//クリアする色を設定
			Col4 Col;
			Col.set(31.0f / 255.0f, 30.0f / 255.0f, 71.0f / 255.0f, 255.0f / 255.0f);
			SetClearColor(Col);
			//自分自身にイベントを送る
			//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToTitleStage");
		}
		catch (...) {
			throw;
		}
	}

	Scene::~Scene() {
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToGameStage") {
			//最初のアクティブステージの設定
			ResetActiveStage<MStage>();
		}
		if (event->m_MsgStr == L"ToTitleStage")
		{
			//タイトルステージの設定
			ResetActiveStage<TitleStage>();
		}
		if (event->m_MsgStr == L"ToSelectStage")
		{
			//セレクトステージの設定
			ResetActiveStage<SelectStage>();
		}
	}

}
//end basecross

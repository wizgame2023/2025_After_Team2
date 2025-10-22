
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
			InputManager::CreateInputManager();

			Json::SetDefaultFilePath(App::GetApp()->GetDataDirWString());
			ResourceManager::Load(L"Json/resorce.json");

			ResourceManager::RegisterResource(L"Model");

			ResourceManager::RegisterTexture(L"Texture");

			ResourceManager::RegisterTexture(L"UI");


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
	void Scene::OnUpdate() {
		SceneBase::OnUpdate();
		InputManager::GetInputManager()->Update();
	}
	void Scene::OnDestroy() {
		InputManager::DeleteInputManager();
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


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
			ResourceManager::SetResourcesFilePath(L"Json/Resources/");
			ResourceManager::Load(L"sceneResource.json");

			ResourceManager::RegisterResource(L"Model");

			ResourceManager::RegisterTexture(L"Texture");

			ResourceManager::RegisterTexture(L"UI");

			SoundManager::GetInstance().RegisterSounds();
			//クリアする色を設定
			Col4 Col = Col4(0, 0, 0, 1);
			Col.set(31.0f / 255.0f, 30.0f / 255.0f, 71.0f / 255.0f, 255.0f / 255.0f);
			SetClearColor(Col);
			//自分自身にイベントを送る
			//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToTitleStage");


			m_StageFile = Json(L"Json/stage.json");
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
			auto info = static_pointer_cast<int>(event->m_Info).get();
			int index = *(info);
			auto dataArray = m_StageFile.At<JsonArray>(L"tutorial");

			auto data = dataArray->GetObjectArray()[index];
			
			ResetActiveStage<GameStage>(data);
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

/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"
namespace basecross {
	struct TutorialTask {
		enum class Task {
			Connect, Put, Put_Color, Param
		};
		bool m_IsCompleted;
		wstring m_EventName;
		wstring m_TutorialTextureKey;
		Vec3 m_DrawPosition;
		Task m_Task;
		function<void()> m_Start;	//チュートリアル開始時
		function<void()> m_Update;	//チュートリアル中
		function<void()> m_End;		//チュートリアル終了時

	};
	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------

	class GameStage : public Stage {
		TutorialTask::Task m_CurrentTask;
		//ビューの作成
		void CreateViewLight();
		void CreateResorce();
	public:
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnEvent(const shared_ptr<Event>& event)override;
	};


}
//end basecross


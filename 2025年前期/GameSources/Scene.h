/*!
@file Scene.h
@brief シーン
*/
#pragma once

#include "stdafx.h"
#include "Json.h"
namespace basecross{
	struct GimmickData;
	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase{

		Viewport m_GameViewport;

		Json m_StageFile;

		vector<int> m_MaxAnserCounts;
		vector<vector<vector<GimmickData>>> m_ClearPath;
		void LoadStageData();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		virtual void OnUpdate()override;
		virtual void OnDestroy()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief イベント取得
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) override;

		void AddClearPath(int stage,const vector<GimmickData>& data) {
			m_ClearPath[stage].push_back(data);
		}
		bool CheckClearPath(int stage, const vector<GimmickData>& data);
		vector<vector<GimmickData>>& GetClearPath(int stage) { return m_ClearPath[stage]; }
		int GetAnserCount(int stage) { return m_MaxAnserCounts[stage]; }

		void SetViewport(Viewport& viewport) {
			m_GameViewport = viewport;
		}
		Viewport GetViewport() {
			return m_GameViewport;
		}
	};

}

//end basecross

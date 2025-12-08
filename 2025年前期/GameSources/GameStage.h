/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"
namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------

	class GameStage : public Stage {
		//ビューの作成
		void CreateViewLight();
		void CreateResorce();

		wstring m_StageFilename;
	public:
		//構築と破棄
		GameStage(const wstring& filename = L"TestMap") :Stage(), m_StageFilename(filename) {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDestroy()override;
	};


}
//end basecross


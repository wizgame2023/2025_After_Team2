/*!
@file Gimmick.h
@brief ステージ上のギミック（仕掛け）に関するクラス定義
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@brief ギミックの種類を定義する列挙型
	*/
	enum class GimmickObjects
	{
		None,               //!< ギミックなし
		Goal,               //!< ゴール地点
		SetPlayer,          //!< プレイヤー初期配置
		CourseCorrection,   //!< 進行方向補正
		Upper               //!< 上昇ギミック
	};

	/*!
	@brief ギミックの基底クラス
	@details 各種ギミックの共通機能を提供し、派生クラスで具体的な挙動を実装する。
	*/
	class Gimmicks : public Object
	{
	protected:
		shared_ptr<MoveCube> m_Cube;

	public:
		/*!
		@brief コンストラクタ
		@param ptrStage ステージの共有ポインタ
		*/
		Gimmicks(const shared_ptr<Stage>& ptrStage);

		/*!
		@brief デストラクタ
		*/
		~Gimmicks();

		/*!
		@brief オブジェクト生成時の初期化処理
		*/
		virtual void OnCreate();

		/*!
		@brief ゲーム開始時の初期化処理
		*/
		virtual void Begin();

		/*!
		@brief 毎フレームの更新処理
		*/
		virtual void Update();

		/*!
		@brief 終了処理
		*/
		virtual void End();

		/*!
		@brief ギミック削除処理
		*/
		virtual void GimmickDelete();

		/*!
		@brief ギミックの種類を取得
		@return GimmickObjects::None（派生クラスでオーバーライド）
		*/
		virtual GimmickObjects GetGimmickType()
		{
			return GimmickObjects::None;
		}
	};

	/*!
	@brief ゴールギミッククラス
	@details プレイヤーが到達すべきゴール地点を表すギミック
	*/
	class GimmickGoal : public Gimmicks
	{
	public:
		GimmickGoal(const shared_ptr<Stage>& ptrGimmick);
		~GimmickGoal();

		/*!
		@brief ギミックの種類を取得
		@return GimmickObjects::Goal
		*/
		GimmickObjects GetGimmickType() override
		{
			return GimmickObjects::Goal;
		};

		virtual void OnCreate();
		virtual void Begin();
		virtual void Update();
		virtual void End() {}
	};

	/*!
	@brief プレイヤー配置ギミッククラス
	@details ステージ開始時にプレイヤーを特定位置に配置するギミック
	*/
	class GimmickSetPlayer : public Gimmicks
	{
		Vec3 m_Velocity; //!< プレイヤーに与える初期速度

	public:
		GimmickSetPlayer(const shared_ptr<Stage>& ptrGimmick);
		~GimmickSetPlayer();

		/*!
		@brief ギミックの種類を取得
		@return GimmickObjects::SetPlayer
		*/
		GimmickObjects GetGimmickType() override
		{
			return GimmickObjects::SetPlayer;
		};

		virtual void OnCreate();
		virtual void Begin();
		virtual void Update();
		virtual void End() {}

		/*!
		@brief プレイヤーに与える初期速度を設定
		@param velocity 初期速度ベクトル
		*/
		void SetVelocity(Vec3 velocity)
		{
			m_Velocity = velocity;
		}
	};
}
//end basecross

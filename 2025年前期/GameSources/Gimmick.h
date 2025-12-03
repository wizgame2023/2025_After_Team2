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
		None,               // ギミックなし
		Goal,               // ゴール地点
		SetPlayer,          // プレイヤー初期配置
		CourseCorrection,   // 進行方向補正
		Teleporter,			// テレポーター
		Killer,             // 即死ギミック
		Roll,				// 回転ギミック
		Inverter			// 矢印の方向の反転
	};

	/*!
	@brief ギミックの基底クラス
	@details 各種ギミックの共通機能を提供し、派生クラスで具体的な挙動を実装する。
	*/
	class Gimmicks : public Object
	{
		shared_ptr<Board> m_Board;
		
		bool m_IsEffect;
	protected:
		shared_ptr<MoveCube> m_Cube;
		Vec3 m_Value; // ギミックの方向ベクトルなどの値を格納する変数

		Vec3 m_RollVal;
		int m_Count;
		int m_MaxCount;

		shared_ptr<Effect> m_PutEffect;
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
		@brief オブジェクトの更新処理
		*/
		virtual void OnUpdate();

		/*!
		@brief ゲーム開始時の初期化処理
		*/
		virtual void Begin();

		/*!
		@brief 毎ターンの更新処理
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

		/*!
		@brief ギミックの方向ベクトルなどの値を格納する変数を設定
		@param value 数値ベクトル
		@return なし
		*/
		void SetValue(Vec3 value)
		{
			m_Value = value;
		}
		/*!
		@brief ギミックの方向ベクトルなどの値を取得する関数
		@return ギミックの方向ベクトルなどの値
		*/
		Vec3 GetValue()
		{
			return m_Value;
		}
		/*!
		@brief 実行回数の設定
		@param count 実行回数
		@return なし
		*/
		void SetCount(int count)
		{
			m_MaxCount = count;
			m_Count = m_MaxCount;
		}
	protected:
		bool CheckCount()
		{
			if (m_Cube == nullptr)
			{
				m_Count = m_MaxCount % 4;
			}
			else if (m_Count > 0)
			{
				m_Count--;
				return true;
			}
	
			return false;
		}
	};

	/*!
	@brief ゴールギミッククラス
	@details プレイヤーが到達すべきゴール地点を表すギミック
	*/
	class GimmickGoal : public Gimmicks
	{
		shared_ptr<Effect> m_GoalEffect;

		bool m_Goal;
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
		virtual void OnUpdate();
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
		virtual void OnUpdate();
		virtual void Begin();
		virtual void Update();
		virtual void End() {}

	};

	class GimmickArrow : public Gimmicks
	{
	public:
		GimmickArrow(const shared_ptr<Stage>& ptrGimmick);
		~GimmickArrow();
		/*!
		@brief ギミックの種類を取得
		@return GimmickObjects::CourseCorrection
		*/
		GimmickObjects GetGimmickType() override
		{
			return GimmickObjects::CourseCorrection;
		};

		virtual void OnCreate();
		virtual void OnUpdate(){}
		virtual void Begin();
		virtual void Update();
		virtual void End();

	};

	class GimmickTeleporter : public Gimmicks
	{
		shared_ptr<Effect> m_TeleportFastEffect;
		shared_ptr<Effect> m_TeleportEndEffect;

		Vec3 m_PlayerVal;

		bool m_IsFastTeleport;
		bool m_WasStepped = false;

		float m_Time;
	public:
		GimmickTeleporter(const shared_ptr<Stage>& ptrGimmick);
		~GimmickTeleporter();
		/*!
		@brief ギミックの種類を取得
		@return GimmickObjects::Upper
		*/
		GimmickObjects GetGimmickType() override
		{
			return GimmickObjects::Teleporter;
		};
		virtual void OnCreate();
		virtual void Begin();
		virtual void OnUpdate();
		virtual void Update();
		virtual void End() {}
	};


	class GimmickKiller : public Gimmicks
	{
	public:
		GimmickKiller(const shared_ptr<Stage>& ptrGimmick);
		~GimmickKiller();
		/*!
		@brief ギミックの種類を取得
		@return GimmickObjects::Killer
		*/
		GimmickObjects GetGimmickType() override
		{
			return GimmickObjects::Killer;
		};
		virtual void OnCreate();
		virtual void Begin();
		virtual void Update();
		virtual void End() {}
	};

	class GimmickRoll : public Gimmicks
	{
		bool m_IsLeftRoll = false;

	public:
		GimmickRoll(const shared_ptr<Stage>& ptrGimmick);
		~GimmickRoll();
		/*!
		@brief ギミックの種類を取得
		@return GimmickObjects::Roll
		*/
		GimmickObjects GetGimmickType() override
		{
			return GimmickObjects::Roll;
		};
		virtual void OnCreate();
		virtual void Begin();
		virtual void Update();
		virtual void End();

		void OnLeftRoll(bool on)
		{
			m_IsLeftRoll = on;
		}
	};

	class GimmickInverter : public Gimmicks
	{
	public:
		GimmickInverter(const shared_ptr<Stage>& ptrGimmick);
		~GimmickInverter();
		/*!
		@brief ギミックの種類を取得
		@return GimmickObjects::Inverter
		*/
		GimmickObjects GetGimmickType() override
		{
			return GimmickObjects::Inverter;
		};
		virtual void OnCreate();
		virtual void Begin();
		virtual void Update();
		virtual void End() {}

	};
}
//end basecross

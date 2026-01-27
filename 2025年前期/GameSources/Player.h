/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"
namespace basecross{
	class Board;
	class Gimmicks;
	struct GimmickData;

	enum class MoveState {
		Telepote,
		Move,
		ChangeVelocity
	};
	class MoveCube : public Object {
		bool m_IsEffecting;		//演出中か(移動、テレポートなど)
		bool m_IsBeforeEffecting;//前フレームの演出状態
		bool m_IsDead;			//死亡判定
		Vec3 m_TelepoteTarget;	//テレポート先

		Vec3 m_StartVelocity;	//移動方向の変更開始地点
		Vec3 m_TargetVelocity;	//移動方向の変更時の値
		float m_VelocitySlerpFactor;
		float m_MoveVelocityAngleSpeed;	//移動方向の変更速度
		Vec3 m_Velocity;		//移動方向
		Vec3 m_Target;			//通常移動先
		AABB m_MoveArea;		//移動範囲

		float m_MoveSec;		//移動時間(一マス移動にかかる時間)
		float m_MoveSpeed;		//移動速度
		float m_RotateSpeed;	//回転速度
		float m_RotateRad;		//現在の回転角度(ラジアン)
		float m_CurrentHeight;	//現在の高さ
		float m_TeleportTimer;
		float m_AfterTeloprotTime;
		float m_TeleportTime;

		MoveState m_State;		//現在の行動

		shared_ptr<PNTStaticModelDraw> m_Draw;

		vector<GimmickData> m_GimmickPath;
	public:
		MoveCube(const shared_ptr<Stage>& ptr);
		virtual ~MoveCube(){}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		/// <summary>
		/// 指定された位置にスポーンさせる。
		/// プレイヤーを出す時はこれを使う
		/// </summary>
		/// <param name="position">位置</param>
		void Spawn(Vec3 position) {
			SetPosition(position);
			m_CurrentHeight = position.y;
		}
		/// <summary>
		/// 移動先が範囲内か判定
		/// </summary>
		/// <returns></returns>
		bool CheckArea();

		/// <summary>
		/// 回転中のY座標を計算
		/// </summary>
		/// <param name="rot">現在の回転角度</param>
		/// <returns></returns>
		float CalcRotatingCenterY(float rot);

		bool IsEffecting() {
			return m_IsEffecting;
		}
		void SetMoveSec(float sec) {
			m_MoveSec = sec;
		}
		void SetMoveArea(AABB aabb) {
			m_MoveArea = aabb;
		}
		void SetVelocity(Vec3 velocity) {
			m_Velocity = velocity;
		}
		void ChangeVelocity(Vec3 velocity);

		Vec3 GetVelocity()const {
			return m_Velocity;
		}

		AABB GetMoveArea() const {
			return m_MoveArea;
		}

		void Move();
		void Telepote(Vec3 target,float time,float afterTime = 0.0f) {
			if (m_IsEffecting || m_IsDead) return;
			m_IsEffecting = true;
			m_TeleportTime = time;
			m_AfterTeloprotTime = afterTime;
			m_TeleportTimer = 0.0f;
			if (target.y < 0) {
				target.y = 0;
			}
			m_TelepoteTarget = target;
			m_State = MoveState::Telepote;
		}

		void Destroy();

		void AddGimmickPath(const shared_ptr<Gimmicks>& gimmick);
		const vector<GimmickData>& GetGimmickPath();
	};
}
//end basecross


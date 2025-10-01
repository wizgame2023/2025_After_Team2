/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	enum class MoveState {
		Telepote,
		Move
	};
	class MoveCube : public Object {
		bool m_IsEffecting;		//演出中か(移動、テレポートなど)
		bool m_IsDead;			//死亡判定
		Vec3 m_TelepoteTarget;	//テレポート先
		Vec3 m_Velocity;		//移動方向
		Vec3 m_Target;			//通常移動先
		AABB m_MoveArea;		//移動範囲

		float m_MoveSec;		//移動時間(一マス移動にかかる時間)
		float m_MoveSpeed;		//移動速度
		float m_RotateSpeed;	//回転速度

		MoveState m_State;		//現在の行動

		shared_ptr<PNTStaticModelDraw> m_Draw;
	public:
		MoveCube(const shared_ptr<Stage>& ptr);
		virtual ~MoveCube(){}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		bool CheckArea();

		void SetMoveSec(float sec) {
			m_MoveSec = sec;
		}
		void SetMoveArea(AABB aabb) {
			m_MoveArea = aabb;
		}
		void SetVelocity(Vec3 velocity) {
			m_Velocity = velocity;
		}
		Vec3 GetVelocity()const {
			return m_Velocity;
		}

		AABB GetMoveArea() const {
			return m_MoveArea;
		}

		void Move() {
			if (m_IsEffecting) return;
			m_IsEffecting = true;

			if (!CheckArea() || m_IsDead) {
				m_IsEffecting = false;
				m_Target = GetPosition();
			}
			m_Target = GetPosition() + m_Velocity.normalize();
			m_MoveSpeed = (m_Target - GetPosition()).length() / m_MoveSec;
			m_RotateSpeed = XM_PIDIV2 / m_MoveSec;
			m_State = MoveState::Move;
		}
		void Telepote(Vec3 target) {
			if (m_IsEffecting || m_IsDead) return;
			m_IsEffecting = true;

			if (target.y < 0) {
				target.y = 0;
			}
			m_TelepoteTarget = target;
			m_State = MoveState::Telepote;
		}

		void Destroy();
	};

	class MoveBall : public Object {
		float m_Speed;	//移動速度
		Vec3 m_Velocity;	//進行方向
		Vec3 m_TargetPosition;	//目標地点
		AABB m_MoveArea;	//移動範囲
		bool m_IsTarget;	//目標地点に向かうか
		bool m_IsActive;	//移動中か

		shared_ptr<PNTStaticDraw> m_Draw;
	public:
		MoveBall(const shared_ptr<Stage>& ptr, float speed, Vec3 velocity): Object(ptr){}
		MoveBall(const shared_ptr<Stage>& ptr) : MoveBall(ptr,1.0f,Vec3(1.0f,0.0f,0.0f)){}
		virtual ~MoveBall(){}

		virtual void OnCreate()override{}
		virtual void OnUpdate()override{}

		Vec3 LimitArea(Vec3 position){}
		void SetVelocity(Vec3 velocity) {
			m_Velocity = velocity;
		}
		Vec3 GetVelocity() {
			return m_Velocity;
		}

		void SetTarget(Vec3 position) {
			m_TargetPosition = position;
			m_IsTarget = true;
		}
		void SetMoveArea(AABB aabb) {
			m_MoveArea = aabb;
		}

		AABB GetMoveArea() const{
			return m_MoveArea;
		}

		bool GetIsActive()const {
			return m_IsActive;
		}
	};
}
//end basecross


/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	class MainCamera : public Camera {
		const float m_FixedAngleVirtical;
		vector<float> m_FixedAngle;
		float m_UpperAngle;

		float m_ArmLength;
		float m_RotateSpeed;

		Vec2 m_TargetAngle;

		bool m_IsUpperAngle;

		Vec2 m_Angle;

		Vec3 CalcPosition(float virticalAngle, float horizonAngle);
	public:
		MainCamera(float rotateSpeed) : 
			Camera(),
			m_IsUpperAngle(false),
			m_UpperAngle(0), m_ArmLength(5.0f), m_FixedAngleVirtical(XM_PIDIV4),m_RotateSpeed(rotateSpeed){}
		virtual ~MainCamera(){}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		void SetFixedPoint(shared_ptr<Map> map);
	};
}
//end basecross

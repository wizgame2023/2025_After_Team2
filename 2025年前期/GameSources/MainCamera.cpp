/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	void MainCamera::OnCreate() {
		m_TargetAngle.y = m_FixedAngleVirtical;
		m_TargetAngle.x = XMConvertToRadians(270.0f);
		m_Angle = m_TargetAngle;
		
		Vec3 position = CalcPosition(m_Angle.y, m_Angle.x);
		SetEye(position);
	}
	void MainCamera::OnUpdate() {
		auto& app = App::GetApp();

		auto device = app->GetInputDevice().GetControlerVec()[0];
		auto elapsed = app->GetElapsedTime();
		if (m_Angle == m_TargetAngle) {
			
			auto& input = InputManager::GetInputManager();
			auto& gameManager = GameManager::GetInstance();
			Vec2 stick = input->GetRStick();
			
			float deadzone = 0.2f;
			if(stick.y > deadzone){
				m_IsUpperAngle = true;

				m_TargetAngle.y = XM_PIDIV2;
			}
			else if (stick.y < -deadzone) {
				m_IsUpperAngle = false;
				m_TargetAngle.y = m_FixedAngleVirtical;
				SetUp(Vec3(0, 1, 0));
			}
			if (!m_IsUpperAngle) {
				if(stick.x < -deadzone){
					if (m_TargetAngle.x <= 0) {
						m_TargetAngle.x += XM_2PI;
						m_Angle.x += XM_2PI;
					}
					m_TargetAngle.x -= XM_PIDIV2;
				}
				else if(stick.x > deadzone){
					if (m_TargetAngle.x >= XM_2PI) {
						m_TargetAngle.x -= XM_2PI;
						m_Angle.x -= XM_2PI;
					}
					m_TargetAngle.x += XM_PIDIV2;
				}
			}
		}

		Vec2 diff = (m_TargetAngle - m_Angle);
		Vec2 moveAmount = diff.normalize() * m_RotateSpeed * elapsed;
		if (abs(moveAmount.x) > abs(m_TargetAngle.x - m_Angle.x)) {
			moveAmount.x = m_TargetAngle.x - m_Angle.x;
		}
		if (abs(moveAmount.y) > abs(m_TargetAngle.y - m_Angle.y)) {
			moveAmount.y = m_TargetAngle.y - m_Angle.y;
			if (m_IsUpperAngle) {
				SetUp(Vec3(0, -1, 0));
			}
		}

		m_Angle += moveAmount;
		Vec3 position = CalcPosition(m_Angle.y, m_Angle.x);
		SetEye(position);
	}
	Vec3 MainCamera::CalcPosition(float virticalAngle, float horizonAngle) {
		Vec3 position = Vec3();

		position.y = sin(virticalAngle) * m_ArmLength;
		float horizonLength = cos(virticalAngle) * m_ArmLength;

		position.x = cos(horizonAngle) * horizonLength;
		position.z = sin(horizonAngle) * horizonLength;

		Vec3 at = GetAt();
		return at + position;

	}
	void MainCamera::SetFixedPoint(shared_ptr<Map> map) {

		Vec3 mapSize = map->GetMapSize();

		Vec3 center = map->GetMapCenter();

		m_ArmLength = max(mapSize.x, max(mapSize.y, mapSize.z)) * 2.0f;

		m_Angle = Vec2(XMConvertToRadians(270.0f), m_FixedAngleVirtical);
		SetEye(CalcPosition(m_Angle.y, m_Angle.x));
		SetAt(center);
	}
}
//end basecross

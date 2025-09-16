/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class Object : public GameObject {
		shared_ptr<Transform> m_Transform;

	public:
		Object(const shared_ptr<Stage>& ptr) : GameObject(ptr){}
		virtual ~Object(){}

		virtual void OnCreate() override;
		virtual void OnUpdate()override;

		Vec3 GetPosition() const{
			return m_Transform->GetPosition();
		}
		void SetPosition(Vec3& position) {
			m_Transform->SetPosition(position);
		}
		Vec3 GetRotation() const{
			return m_Transform->GetRotation();
		}
		void SetRotation(Vec3& rotation) {
			m_Transform->SetRotation(rotation);
		}
		Vec3 GetForward() const{
			return m_Transform->GetForward();
		}
		Vec3 GetScale() const{
			return m_Transform->GetScale();
		}
		void SetScale(Vec3& scale) {
			m_Transform->SetScale(scale);
		}
		
	};
}
//end basecross

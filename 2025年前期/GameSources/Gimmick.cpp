/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	Gimmicks::Gimmicks(const shared_ptr<Stage>& ptrStage, Objects obj) :
	Object(ptrStage),
	m_Obj(obj)
		
	{
	}

	Gimmicks::~Gimmicks()
	{
	}

	void Gimmicks::OnCreate()
	{
		Object::OnCreate();

		switch (m_Obj)
		{
		case basecross::Gimmicks::Objects::Goal:
			m_Draw->SetMeshResource(L"DEFAULT_CUBE");
			m_Draw->SetDiffuse(Col4(1, 0, 0, 0));
			break;
		case basecross::Gimmicks::Objects::SetPlayer:
			break;
		case basecross::Gimmicks::Objects::CourseCorrection:
			break;
		case basecross::Gimmicks::Objects::Upper:
			break;
		default:
			break;
		}

	}

	void Gimmicks::OnUpdate()
	{
		auto stage = GetStage();
		if (dynamic_pointer_cast<GameStage>(stage) == nullptr) return;

		auto gameObjectVec = stage->GetGameObjectVec();
		for (auto& obj : gameObjectVec)
		{
			auto player = dynamic_pointer_cast<MoveSphere>(obj);
			if (!player) continue;

			Vec3 playerPos = player->GetComponent<Transform>()->GetPosition();
			Vec3 dist = playerPos - GetPosition();

			float distance = dist.length();
			float rot = GetForward().normalize().dot(dist.normalize());

			if (m_Obj == Objects::Goal && distance < 0.1f && rot > 0.95f)
			{
				player->SetVelocity(Vec3(0.0f)); // 動きを止める
				//IsGoal(); // ゴール演出など
			}
		}
	}
}
//end basecross

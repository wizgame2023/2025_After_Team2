/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	Gimmicks::Gimmicks(const shared_ptr<Stage>& ptrStage, Objects obj) :
	Object(ptrStage),
	m_Obj(obj),
	m_IsGoalFlag(false)
		
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
			m_Draw->SetMeshResource(L"DEFAULT_CYLINDER");
			m_Draw->SetDiffuse(Col4(1, 0, 0, 1));
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

		switch (m_Obj)
		{
		case basecross::Gimmicks::Objects::Goal:
			Goal();
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


	void Gimmicks::Goal()
	{
		if (m_IsGoalFlag) return; //ゴールしていたら

		auto stage = GetStage();

		Vec3 forward = GetForward().normalize();
		Vec3 pos = GetPosition();
		auto gameObjectVec = stage->GetGameObjectVec();
		for (auto& obj : gameObjectVec)
		{
			auto player = dynamic_pointer_cast<MoveSphere>(obj);
			if (!player) continue;

			Vec3 playerPos = player->GetComponent<Transform>()->GetPosition();
			Vec3 dist = playerPos - pos;

			float distanceSq = dist.lengthSqr();
			float rot = forward.dot(dist.normalize());

			if (distanceSq < (0.1f * 0.1f) && rot > 0.95f)
			{
				IsGoalDirection(player); // ゴール演出など
				m_IsGoalFlag = true;
				break; //一体でもゴールしたら終了;
			}
		}

	}

	void Gimmicks::IsGoalDirection(const shared_ptr<MoveSphere>& player)
	{
		player->SetVelocity(Vec3(0.0f)); // 動きを止める

	}
}
//end basecross

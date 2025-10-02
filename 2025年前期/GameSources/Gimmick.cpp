/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	Gimmicks::Gimmicks(const shared_ptr<Stage>& ptrStage) :
	Object(ptrStage)	
	{
	}

	Gimmicks::~Gimmicks()
	{
	}

	void Gimmicks::OnCreate()
	{
		Object::OnCreate();
	}

	void Gimmicks::Begin()
	{
		//Object::OnCreate();

	}
	void Gimmicks::Update()
	{
		auto playerVec = GameManager::GetInstance().GetCubes();
		auto pos = GetPosition();
		for (auto& ball : playerVec)
		{
			auto player = dynamic_pointer_cast<MoveCube>(ball);
			if (player)
			{
				Vec3 dist = player->GetPosition() - pos;
				float distanceSq = dist.lengthSqr();

				if (distanceSq < (0.1f * 0.1f))
				{
					m_Cube = player;
					break;
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}
		}
	}

	void Gimmicks::End()
	{
	}

	void Gimmicks::GimmickDelete()
	{
		m_Stage->RemoveGameObject<Gimmicks>(GetThis<Gimmicks>());
	}



	GimmickGoal::GimmickGoal(const shared_ptr<Stage>& ptrStage) :
		Gimmicks(ptrStage)
	{
	}
	GimmickGoal::~GimmickGoal()
	{
	}
	
	void GimmickGoal::OnCreate()
	{
		Gimmicks::OnCreate();

		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");
		draw->SetDiffuse(Col4(0, 1, 0, 1));
	}

	void GimmickGoal::Begin()
	{
		Gimmicks::Begin();
	}
	void GimmickGoal::Update()
	{
		Gimmicks::Update();

		if (m_Cube)
		{
			GameManager::GetInstance().DrawGoalEffect();

			m_Cube = nullptr;

		}
	}


	GimmickSetPlayer::GimmickSetPlayer(const shared_ptr<Stage>& ptrStage) :
		Gimmicks(ptrStage)
	{
	}
	GimmickSetPlayer::~GimmickSetPlayer()
	{
	}

	void GimmickSetPlayer::OnCreate()
	{
		Gimmicks::OnCreate();
		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");
		draw->SetDiffuse(Col4(1, 0, 0, 1));

	}
	void GimmickSetPlayer::Begin()
	{
		Gimmicks::Begin();
		auto player = m_Stage->AddGameObject<MoveCube>();
		auto pos = GetPosition();

		player->Spawn(pos);
		player->SetVelocity(/*m_Velocity*/Vec3(1.0f, 0.0f, 0.0f));
	}
	void GimmickSetPlayer::Update()
	{
		Gimmicks::Update();
	}
}
//end basecross

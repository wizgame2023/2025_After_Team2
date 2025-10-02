/*!
@file Character.cpp
@brief ƒLƒƒƒ‰ƒNƒ^[‚È‚ÇŽÀ‘Ì
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
			Vec3 pos = m_Transform->GetPosition();
			Vec3 playerPos = GetPosition();

			if (playerPos.y > 0)
			{
				m_Cube->Telepote(pos + /*Vec3(0.0f, -m_Value, 0.0f)*/ Vec3(0.0f, -1.0f, 0.0f));
			}
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

		player->SetPosition(pos);
		player->SetVelocity(/*m_Velocity*/Vec3(1.0f, 0.0f, 0.0f));
	}
	void GimmickSetPlayer::Update()
	{
		Gimmicks::Update();
	}


	GimmickCourseCorrection::GimmickCourseCorrection(const shared_ptr<Stage>& ptrStage) :
		Gimmicks(ptrStage)
	{
	}
	GimmickCourseCorrection::~GimmickCourseCorrection()
	{
	}

	void GimmickCourseCorrection::OnCreate()
	{
		Gimmicks::OnCreate();
		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");
		draw->SetDiffuse(Col4(0, 0, 1, 1));

	}

	void GimmickCourseCorrection::Begin()
	{
		Gimmicks::Begin();
	}

	void GimmickCourseCorrection::Update()
	{
		Gimmicks::Update();
		if (m_Cube)
		{
			m_Cube->SetVelocity(/*m_Direction*/Vec3(-1.0f, 0.0f, 0.0f));

			m_Cube = nullptr;
		}
	}

	GimmickUpper::GimmickUpper(const shared_ptr<Stage>& ptrStage) :
		Gimmicks(ptrStage)
	{
	}
	GimmickUpper::~GimmickUpper()
	{
	}

	void GimmickUpper::OnCreate()
	{
		Gimmicks::OnCreate();
		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");
		draw->SetDiffuse(Col4(1, 1, 0, 1));
	}

	void GimmickUpper::Begin()
	{
		Gimmicks::Begin();
	}
	void GimmickUpper::Update()
	{
		Gimmicks::Update();
		if (m_Cube)
		{
			Vec3 pos = m_Transform->GetPosition();

			m_Cube->Telepote(pos + /*Vec3(0.0f, m_Value, 0.0f)*/ Vec3(0.0f, 1.0f, 0.0f));
			m_Cube = nullptr;
		}

	}

	GimmickLower::GimmickLower(const shared_ptr<Stage>& ptrStage) :
		Gimmicks(ptrStage)
	{
	}
	GimmickLower::~GimmickLower()
	{
	}

	void GimmickLower::OnCreate()
	{
		Gimmicks::OnCreate();
		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");
		draw->SetDiffuse(Col4(1, 0, 1, 1));
	}

	void GimmickLower::Begin()
	{
		Gimmicks::Begin();
	}
	void GimmickLower::Update()
	{
		Gimmicks::Update();
		if (m_Cube)
		{
			Vec3 pos = m_Transform->GetPosition();
			Vec3 playerPos = GetPosition();

			if (playerPos.y > 0)
			{
				m_Cube->Telepote(pos + /*Vec3(0.0f, -m_Value, 0.0f)*/ Vec3(0.0f, -1.0f, 0.0f));
			}
			m_Cube = nullptr;
		}

	}

}
//end basecross

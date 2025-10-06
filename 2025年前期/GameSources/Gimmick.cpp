/*!
@file Character.cpp
@brief ?L??????N?^?[???????e?
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	Gimmicks::Gimmicks(const shared_ptr<Stage>& ptrStage) :
	Object(ptrStage),
	m_Cube(nullptr),
	m_MaxCount(1),
	m_Count(m_MaxCount),
	m_Value(Vec3(0.0f))
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
		shared_ptr<MoveCube> cube;

		for (auto& ball : playerVec)
		{
			auto player = dynamic_pointer_cast<MoveCube>(ball);
			if (player)
			{
				Vec3 dist = player->GetPosition() - pos;
				float distanceSq = dist.lengthSqr();

				if (distanceSq < (0.1f * 0.1f))
				{
					cube = player;
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

			m_Cube = cube;
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

		auto draw = AddComponent<PNTStaticModelDraw>();
		draw->SetMeshResource(L"GOAL_MD");
		draw->SetDiffuse(Col4(0, 1, 0, 1));

		Mat4x4 mat;
		mat.affineTransformation(Vec3(0.9f), Vec3(), Vec3(), Vec3(0.0f, -0.9f, 0.0f));
		draw->SetMeshToTransformMatrix(mat);

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
		player->SetVelocity(/*Vec3(1.0f, 0.0f, 0.0f)*/m_Value);
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
		if (CheckCount())
		{
			m_Cube->SetVelocity(m_Value);
		}
	}

	GimmickTeleporter::GimmickTeleporter(const shared_ptr<Stage>& ptrStage) :
		Gimmicks(ptrStage)
	{
	}
	GimmickTeleporter::~GimmickTeleporter()
	{
	}

	void GimmickTeleporter::OnCreate()
	{
		Gimmicks::OnCreate();
		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");
		draw->SetDiffuse(Col4(1, 1, 0, 1));
	}

	void GimmickTeleporter::Begin()
	{
		Gimmicks::Begin();
	}
	void GimmickTeleporter::Update()
	{
		Gimmicks::Update();
		if (CheckCount())
		{
			Vec3 pos = m_Transform->GetPosition();

			m_Cube->Telepote(pos + /*m_Value*/ Vec3(0.0f, 1.0f, 0.0f));
		}

	}



	GimmickKiller::GimmickKiller(const shared_ptr<Stage>& ptrStage) :
		Gimmicks(ptrStage)
	{
	}
	GimmickKiller::~GimmickKiller()
	{
	}

	void GimmickKiller::OnCreate()
	{
		Gimmicks::OnCreate();
		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");
		draw->SetDiffuse(Col4(0, 1, 1, 1));
	}

	void GimmickKiller::Begin()
	{
		Gimmicks::Begin();
	}

	void GimmickKiller::Update()
	{
		Gimmicks::Update();
		if (CheckCount())
		{
			m_Cube->Destroy();
		}
	}

	GimmickRoll::GimmickRoll(const shared_ptr<Stage>& ptrStage) :
		Gimmicks(ptrStage)
	{
	}
	GimmickRoll::~GimmickRoll()
	{
	}

	void GimmickRoll::OnCreate()
	{
		Gimmicks::OnCreate();
		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");
		draw->SetDiffuse(Col4(1, 1, 1, 1));
	}

	void GimmickRoll::Begin()
	{
		Gimmicks::Begin();
	}

	void GimmickRoll::Update()
	{
		Gimmicks::Update();
		if (CheckCount())
		{
			m_Cube->ChangeVelocity(m_Direction);
			//m_Cube = nullptr;
		}
	}
}
//end basecross

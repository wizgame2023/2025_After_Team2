/*!
@file Character.cpp
@brief ?L??????N?^?[???????e?
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	GimmickData::GimmickData():m_Type(GimmickObjects::None),m_Direction(Vec3()){}
	GimmickData::GimmickData(const shared_ptr<Gimmicks>& gimmick) {
		m_Type = gimmick->GetGimmickType();
		m_Direction = gimmick->GetValue();
	}

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

		m_Board = m_Stage->AddGameObject<Board>(L"TEMP_ARROW_SPRITE", Vec3(), Vec3(1, 1, 0), false);
		m_Board->AddComponent<UVScroll>(Vec2(0.0f, 1.0f), m_Board->GetVertices());

		m_PutEffect.reset();
	}

	void Gimmicks::OnUpdate()
	{
		Object::OnUpdate();

		if (m_PutEffect == nullptr)
		{
			m_PutEffect = m_Stage->AddGameObject<Effect>(L"PutGimmickEffect.efk", GetPosition() + Vec3(0, 0.75f, 0));
			m_PutEffect->SetEffectSize(Vec3(0.7f));
		}
		m_Board->GetDraw()->SetDrawActive(false);
		return;
		if (m_Value.lengthSqr() == 0) {
			
		}

		m_Board->GetTrans()->SetPosition(GetPosition() + Vec3(0, 0.75f, 0));
		m_Board->RotateVector(Vec3(0, 1, 0));

		Vec3 normalizeVec = (Vec3)XMVector3Normalize(m_Value);

		float angle = 0.0f;  
		if (normalizeVec == Vec3(-1, 0, 0)) {
			angle = XM_PI;
		}
		else if (normalizeVec == Vec3(0, 0, 1)) {
			angle = -XM_PIDIV2;
		}
		else if (normalizeVec == Vec3(0, 0, -1)) {
			angle = XM_PIDIV2;
		}
		auto rot = XMMatrixRotationAxis(Vec3(0, 1, 0), angle);

		auto world = m_Board->GetTrans()->GetWorldMatrix();
		world.rotation((Quat)XMQuaternionRotationMatrix(rot));

		m_Board->GetTrans()->SetQuaternion(m_Board->GetTrans()->GetQuaternion() * world.quatInMatrix());
	}

	void Gimmicks::Begin()
	{
		//Object::OnCreate();

	}
	void Gimmicks::Update()
	{
		auto playerVec = GameManager::GetInstance().GetEntityManager()->GetPlayers();
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


		}

		m_Cube = cube;

	}

	void Gimmicks::End()
	{
	}

	void Gimmicks::GimmickDelete()
	{
		if (m_Board) {
			m_Stage->RemoveGameObject<Board>(m_Board);
		}
		m_Stage->RemoveGameObject<Gimmicks>(GetThis<Gimmicks>());
	}
	void Gimmicks::AddPlayerPath() {
		if (!m_Cube) return;

		m_Cube->AddGimmickPath(GetThis<Gimmicks>());
	}


	GimmickGoal::GimmickGoal(const shared_ptr<Stage>& ptrStage) :
		Gimmicks(ptrStage),
		m_GoalEffect(nullptr),
		m_Goal(false)
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
		SetAlphaActive(true);
		draw->SetDiffuse(Col4(1, 1, 1, 0.75f));

		Mat4x4 mat;
		mat.affineTransformation(Vec3(0.9f), Vec3(), Vec3(0,XM_PIDIV2,0), Vec3(0.0f, -0.9f, 0.0f));
		draw->SetMeshToTransformMatrix(mat);

		//Quat quat = XMQuaternionRotationAxis(Vec3(0, 1, 0), );
	}

	void GimmickGoal::OnUpdate()
	{
		Gimmicks::OnUpdate();

		auto scene = App::GetApp()->GetScene<Scene>();

		if (m_Goal)
		{
			if (!m_GoalEffect)
			{
				m_GoalEffect = m_Stage->AddGameObject<Effect>(L"GoalGimmickEffect.efk", GetPosition());
				SoundManager::GetInstance().PlaySE(L"Clear");
			}

			if (m_GoalEffect)
			{
				GameManager::GetInstance().DrawGoalEffect();
				if (m_GoalEffect->EffectEnd())
				{
					m_Goal = false;
				}
			}
		}

		float angle = 0.0f;
		if (m_Value == Vec3(-1, 0, 0)) {
			angle = XM_PIDIV2;
		}
		else if (m_Value == Vec3(1, 0, 0)) {
			angle = -XM_PIDIV2;
		}
		else if (m_Value == Vec3(0, 0, 1)) {
			angle = XM_PI;
		}
		auto rot = XMMatrixRotationAxis(Vec3(0, 1, 0), angle);

		auto world = m_Transform->GetWorldMatrix();
		world.rotation((Quat)XMQuaternionRotationMatrix(rot));

		m_Transform->SetQuaternion(world.quatInMatrix());
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
			Vec3 playerVel = m_Cube->GetVelocity();

			// ?[???x?N?g????????????m?F
			if (playerVel.lengthSqr() > 0.0001f)
			{
				Vec3 normalizedVel = playerVel.normalize();
				Vec3 goalDir = m_Transform->GetForward().normalize();

				float dot = normalizedVel.dot(goalDir);

				if (dot > 0.9f) // ??????x?t??????????l
				{
					m_Goal = true;
					m_Cube = nullptr;
					AddPlayerPath();
				}
				else
				{
					m_Cube->Destroy();
					m_Cube = nullptr;
				}
			}
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
		draw->SetMeshResource(L"PLAYER_MD");
		Mat4x4 mat;
		mat.affineTransformation(Vec3(0.8f), Vec3(), Vec3(0, -XM_PIDIV2, 0), Vec3(0.0f, -0.75f, 0.25f));
		draw->SetMeshToTransformMatrix(mat);
	}
	void GimmickSetPlayer::OnUpdate() {
		Gimmicks::OnUpdate();

		float angle = 0.0f;
		if (m_Value == Vec3(-1, 0, 0)) {
			angle = XM_PIDIV2;
		}
		else if (m_Value == Vec3(1, 0, 0)) {
			angle = -XM_PIDIV2;
		}
		else if (m_Value == Vec3(0, 0, 1)) {
			angle = XM_PI;
		}
		auto rot = XMMatrixRotationAxis(Vec3(0, 1, 0), angle);

		auto world = m_Transform->GetWorldMatrix();
		world.rotation((Quat)XMQuaternionRotationMatrix(rot));

		m_Transform->SetQuaternion(world.quatInMatrix());
	}
	void GimmickSetPlayer::Begin()
	{
		Gimmicks::Begin();
		auto player = m_Stage->AddGameObject<MoveCube>();
		auto pos = GetPosition();

		player->Spawn(pos);
		player->SetVelocity(m_Value);
		player->AddGimmickPath(GetThis<Gimmicks>());

		auto draw = GetComponent<PNTStaticDraw>();
		draw->SetDrawActive(false);
	}
	void GimmickSetPlayer::Update()
	{
		Gimmicks::Update();
	}


	GimmickArrow::GimmickArrow(const shared_ptr<Stage>& ptrStage) :
		Gimmicks(ptrStage)
	{
	}
	GimmickArrow::~GimmickArrow()
	{
	}

	void GimmickArrow::OnCreate()
	{
		Gimmicks::OnCreate();
		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DIRECTION_MD");
		draw->SetDiffuse(Col4(1, 0, 0, 1));
		Mat4x4 mat;
		mat.affineTransformation(Vec3(1.0f), Vec3(), Vec3(0, XM_PIDIV2, 0), Vec3(0.0f, -0.75f, 0.0f));
		draw->SetMeshToTransformMatrix(mat);


	}

	void GimmickArrow::Begin()
	{
		Gimmicks::Begin();
	}

	void GimmickArrow::Update()
	{
		Gimmicks::Update();
		CheckCount();
	}
	void GimmickArrow::End() {
		Gimmicks::End();
		Gimmicks::Update();
		if (CheckCount())
		{
			m_Cube->ChangeVelocity(m_Value);
			AddPlayerPath();
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
		m_TeleportFastEffect = nullptr;
		m_TeleportEndEffect = nullptr;

	}

	void GimmickTeleporter::OnUpdate()
	{
		Gimmicks::OnUpdate();

		if (m_IsFastTeleport)
		{
			if (m_TeleportFastEffect && m_TeleportFastEffect->EffectEnd())
			{
				m_TeleportFastEffect->EffectDelete();
				m_TeleportFastEffect = nullptr;

				Vec3 endPos = m_Transform->GetPosition() + m_Value;
				m_TeleportEndEffect = m_Stage->AddGameObject<Effect>(L"TeleportGimmickEndEffect.efk", endPos);
				m_TeleportEndEffect->SetEffectSize(Vec3(0.5f));
				m_TeleportEndEffect->SetEffectSpeed(1.7f);
			}

			if (m_TeleportEndEffect && m_TeleportEndEffect->EffectEnd())
			{
				m_TeleportEndEffect->EffectDelete();
				m_TeleportEndEffect = nullptr;
				m_IsFastTeleport = false;
			}
		}
	}


	void GimmickTeleporter::Update()
	{
		Gimmicks::Update();

		bool isStepped = CheckCount();
		if (isStepped && !m_WasStepped)
		{
			// ?????u?????????
			Vec3 pos = m_Transform->GetPosition();
			m_Cube->SetDrawActive(false);
			m_Cube->Telepote(pos + m_Value, 0.7f, 0.3f); // Cube????P????????????OK

			// ?G?t?F?N?g?J?n
			m_TeleportFastEffect = m_Stage->AddGameObject<Effect>(L"TeleportGimmickFastEffect.efk", m_Cube->GetPosition());
			m_TeleportFastEffect->SetEffectSize(Vec3(0.5f));
			m_TeleportFastEffect->SetEffectSpeed(1.7f);

			m_IsFastTeleport = true;
			AddPlayerPath();
		}

		m_WasStepped = isStepped;
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
		Gimmicks(ptrStage),
		m_IsLeftRoll(false)
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
		
	}
	void GimmickRoll::End() {
		Gimmicks::Update();
		if (CheckCount())
		{
			AddPlayerPath();
			if (m_IsLeftRoll)
			{
				Vec3 CubeVel = m_Cube->GetVelocity();

				Vec3 RollVel = Vec3(-CubeVel.z, CubeVel.y, CubeVel.x);

				m_Cube->ChangeVelocity(RollVel);
			}
			else
			{
				Vec3 CubeVel = m_Cube->GetVelocity();

				m_RollVal = Vec3(CubeVel.z, CubeVel.y, -CubeVel.x);

				m_Cube->ChangeVelocity(m_RollVal);

			}
		}
	}
	GimmickInverter::GimmickInverter(const shared_ptr<Stage>& ptrStage) :
		Gimmicks(ptrStage)
	{
	}

	GimmickInverter::~GimmickInverter()
	{
	}

	void GimmickInverter::OnCreate()
	{
		Gimmicks::OnCreate();
		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");
		draw->SetDiffuse(Col4(0, 0, 0, 1));
	}

	void GimmickInverter::Begin()
	{
		Gimmicks::Begin();
	}

	void GimmickInverter::Update()
	{
		Gimmicks::Update();

		if (CheckCount())
		{
			AddPlayerPath();
			auto CourseCorrectionVec = GameManager::GetInstance().GetLevelManager()->GetMap()->GetGimmicks<GimmickArrow>();

			for (auto& course : CourseCorrectionVec)
			{
				if (course == nullptr) return;

				Vec3 Val = course->GetValue();
				course->SetValue(-Val);
				continue;
			}
		}
	}
}
//end basecross

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

	}

	void Gimmicks::OnUpdate()
	{
		switch (m_Obj)
		{
		case basecross::Gimmicks::Objects::Goal:
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
}
//end basecross

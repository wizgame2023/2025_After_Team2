/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	class GimmickCard : public Object {
		shared_ptr<Sprite> m_Card;
	public:
		GimmickCard(const shared_ptr<Stage>& ptr) : Object(ptr){}
		virtual ~GimmickCard(){}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		shared_ptr<Sprite> GetCardSprite() {
			return m_Card;
		}
	};

	class GimmickHand : public Object {
		vector<shared_ptr<Gimmicks>> m_Hand;
		vector<shared_ptr<GimmickCard>> m_HandSprite;

		int m_SelectIndex;
	public:
		GimmickHand(const shared_ptr<Stage>& ptr) : Object(ptr),m_SelectIndex(0){}
		virtual ~GimmickHand(){}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		void Add(shared_ptr<Gimmicks> gimmicks) {
			m_Hand.push_back(gimmicks);
			m_HandSprite.push_back(GetStage()->AddGameObject<GimmickCard>());
		}

		/// <summary>
		/// �I�𒆂̃J�[�h�̃f�[�^���擾�E��D����폜
		/// </summary>
		/// <returns></returns>
		shared_ptr<Gimmicks> Use();

		/// <summary>
		/// �w�肵���ԍ��̃J�[�h��I��
		/// </summary>
		/// <param name="index"></param>
		void Select(int index) {
			if (m_Hand.size() <= index) return;
			m_SelectIndex = index;
		}
		/// <summary>
		/// ���̃J�[�h��I��
		/// </summary>
		void Next() {
			m_SelectIndex++;
			m_SelectIndex = min(m_Hand.size() - 1, m_SelectIndex);
		}
		/// <summary>
		/// �O�̃J�[�h��I��
		/// </summary>
		void Back() {
			m_SelectIndex--;
			m_SelectIndex = max(0, m_SelectIndex);
		}

		/// <summary>
		/// ��D�����邩����
		/// </summary>
		/// <returns></returns>
		bool IsEmpty() {
			return m_Hand.size() == 0;
		}
	};
}
//end basecross

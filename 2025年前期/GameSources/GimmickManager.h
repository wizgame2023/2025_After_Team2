/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	// int�^�̂��̂͂��ׂăM�~�b�N�p�N���X�ɒu�������܂��傤

	class GimmickManager : public SingletonBase<GimmickManager> {
		friend class SingletonBase<GimmickManager>;
		GimmickManager() = default;

		vector<int> m_Gimmicks;	//�莝���̃M�~�b�N
		vector<bool> m_UsedGimmicks;	//�莝���̃M�~�b�N���g�p�ς݂��ǂ���
	public:
		void Reset() {
			m_Gimmicks.clear();
			m_UsedGimmicks.clear();
		}

		void AddGimmick(int i) {
			m_Gimmicks.push_back(i);
			m_UsedGimmicks.push_back(false);
		}

		int GetGimmick(int i) {
			if (m_Gimmicks.size() <= i) return -1;
			return m_Gimmicks[i];
		}
	};
}
//end basecross

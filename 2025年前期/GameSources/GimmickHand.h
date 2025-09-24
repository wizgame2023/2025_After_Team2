/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	class GimmickCard : public Object {
		static map<Gimmicks::Objects, wstring> m_CoverTexKeys;
		shared_ptr<Sprite> m_Card;
		shared_ptr<Sprite> m_CoverSprite;

		Gimmicks::Objects m_Type;
	public:
		GimmickCard(const shared_ptr<Stage>& ptr,const Gimmicks::Objects type) : Object(ptr), m_Type(type){}
		GimmickCard(const shared_ptr<Stage>& ptr) : GimmickCard(ptr, Gimmicks::Objects::None) {}

		virtual ~GimmickCard(){}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		void Remove() {
			m_Stage->RemoveGameObject<Sprite>(m_Card);
			m_Stage->RemoveGameObject<Sprite>(m_CoverSprite);
			m_Stage->RemoveGameObject<GimmickCard>(GetThis<GimmickCard>());
		}
		shared_ptr<Sprite> GetCardSprite() {
			return m_Card;
		}

		void SetCover(Gimmicks::Objects type);
	};

	class GimmickHand : public Object {
		vector<Gimmicks::Objects> m_Hand;
		vector<shared_ptr<GimmickCard>> m_HandSprite;

		Vec2 m_CardSize;
		int m_SelectIndex;
	public:
		GimmickHand(const shared_ptr<Stage>& ptr) : Object(ptr),m_SelectIndex(0){}
		virtual ~GimmickHand(){}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		/// <summary>
		/// カードの描画サイズを設定
		/// </summary>
		/// <param name="size">描画サイズ</param>
		void SetCardSize(Vec2 size) {
			m_CardSize = size;
		}

		/// <summary>
		/// 手札を追加する
		/// </summary>
		/// <param name="gimmicks">追加するギミック</param>
		void Add(Gimmicks::Objects gimmicks) {
			m_Hand.push_back(gimmicks);
			m_HandSprite.push_back(m_Stage->AddGameObject<GimmickCard>(gimmicks));
		}

		/// <summary>
		/// 選択中のカードのデータを取得・手札から削除
		/// </summary>
		/// <returns></returns>
		Gimmicks::Objects Use();

		/// <summary>
		/// 指定した番号のカードを選択
		/// </summary>
		/// <param name="index"></param>
		void Select(int index) {
			if (m_Hand.size() <= index) return;
			m_SelectIndex = index;
		}
		/// <summary>
		/// 次のカードを選択
		/// </summary>
		void Next() {
			m_SelectIndex++;
			m_SelectIndex = min(m_Hand.size() - 1, m_SelectIndex);
		}
		/// <summary>
		/// 前のカードを選択
		/// </summary>
		void Back() {
			m_SelectIndex--;
			m_SelectIndex = max(0, m_SelectIndex);
		}

		/// <summary>
		/// 手札があるか判定
		/// </summary>
		/// <returns></returns>
		bool IsEmpty() {
			return m_Hand.size() == 0;
		}
	};
}
//end basecross

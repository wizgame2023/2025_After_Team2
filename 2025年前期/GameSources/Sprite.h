/*!
@file Sprite.h
@brief �X�v���C�g
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	struct SpriteAnimation {
		size_t		m_CurrentOrder;		//���݂̃A�j���[�V�����ԍ�
		float		m_AnimationTime;	//�A�j���[�V�����^�C�}�[
		float		m_UpdateInterval;	//�X�V�p�x
		bool		m_IsLoop;			//���[�v���邩
		bool		m_IsReverse;		//�t�Đ���
		vector<int> m_Order;			//�`�揇


		/// <summary>
		/// ��̃A�j���[�V�������ǂ����擾
		/// </summary>
		/// <returns>�ݒ肳��Ă��Ȃ��A�j���[�V�����Ȃ�true</returns>
		bool Empty() const {
			return m_Order.empty();
		}

		/// <summary>
		/// �A�j���[�V�����I������
		/// </summary>
		void EndAnimation() {
			if (Empty()) return;

			m_CurrentOrder = m_IsReverse ? 0 : m_Order.size() - 1;

			if (m_IsLoop) {
				m_CurrentOrder = m_IsReverse ? m_Order.size() - 1 : 0;
			}
		}

		/// <summary>
		/// �A�j���[�V�����X�V����
		/// </summary>
		/// <param name="elapsed"> : �o�ߎ���</param>
		/// <returns>�A�j���[�V�������X�V���ꂽ��(�摜���؂�ւ������)true</returns>
		bool Update(float elapsed) {
			if (Empty()) return false;

			m_AnimationTime += elapsed;

			if (m_AnimationTime > m_UpdateInterval) {
				m_AnimationTime = 0.0f;
				m_CurrentOrder = m_IsReverse ? m_CurrentOrder - 1 : m_CurrentOrder + 1;

				if (m_CurrentOrder < 0 || m_CurrentOrder >= m_Order.size()) {
					EndAnimation();
					return true;
				}
			}

			return false;
		}

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		SpriteAnimation() {
			m_Order = {};
			m_CurrentOrder = 0;
			m_AnimationTime = 0.0f;
			m_UpdateInterval = 0;
			m_IsLoop = false;
			m_IsReverse = false;
		}

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="order"> : �`�揇</param>
		/// <param name="time"> : �s��</param>
		/// <param name="interval"> : �X�V�p�x</param>
		/// <param name="isLoop"> : ���[�v���邩</param>
		/// <param name="isReverse"> : �t�Đ���</param>
		SpriteAnimation(vector<int> order, float interval, const bool isLoop = false, const bool isReverse = false){
			m_Order = order;
			m_CurrentOrder = isReverse ? m_Order.size() - 1 : 0;
			m_AnimationTime = 0.0f;
			m_UpdateInterval = interval;
			m_IsLoop = isLoop;
			m_IsReverse = isReverse;
		}

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="startOrder"> : �`��J�n</param>
		/// <param name="endOrder"> : �`��I��</param>
		/// <param name="time"> : �s��</param>
		/// <param name="interval"> : �X�V�p�x</param>
		/// <param name="isLoop"> : ���[�v���邩</param>
		/// <param name="isReverse"> : �t�Đ���</param>
		SpriteAnimation(int startOrder,int endOrder,float interval,const bool isLoop = false,const bool isReverse = false){
			vector<int> order;
			for (int i = startOrder;i <= endOrder;i++) {
				order.push_back(i);
			}
			m_Order = order;
			m_CurrentOrder = isReverse ? m_Order.size() - 1 : 0;
			m_AnimationTime = 0.0f;
			m_UpdateInterval = interval;
			m_IsLoop = isLoop;
			m_IsReverse = isReverse;
		}
	};

	enum class Anchor {
		Center,
		Top,
		Bottom,
		Left,
		Right,
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight
	};

	//----------------------------------------------------------
	//
	//	�摜�\���N���X						
	//																																
	//----------------------------------------------------------
	class Sprite : public GameObject {
		wstring m_TexKey;				//�e�N�X�`���L�[
		Vec2	m_Size;					//�T�C�Y
		Vec3	m_Pos;					//�ʒu
		Vec2	m_Pivot;				//�d�S
		size_t	m_MaxAnimationFrame;	//�g�p�ł���A�j���[�V�����t���[���̍ő吔
		Vec2	m_cutUV;				//�摜�̐؂��萔
		Vec2	m_ScreenHalfSize;			//�X�N���[���T�C�Y

		vector<vector<Vec2>>				m_AnimationUV;	//�A�j���[�V������UV
		vector<VertexPositionColorTexture>	m_Vertices;		//���_

		shared_ptr<PCTSpriteDraw>	m_Draw;			//�`��R���|�[�l���g
		shared_ptr<Transform>		m_Transform;	//�ʒu�R���|�[�l���g

		bool							m_IsAnimation;		//�A�j���[�V���������邩
		map<wstring, SpriteAnimation>	m_Animations;		//�A�j���[�V�����}�b�v
		SpriteAnimation					m_CurrentAnimation;	//���݂̃A�j���[�V����
		SpriteAnimation					m_BeforeAnimation;	//�O�̃A�j���[�V����


		/// <summary>
		/// �A�j���[�V��������
		/// </summary>
		void Animation();

	public:

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="ptr"> : �X�e�[�W</param>
		/// <param name="texKey"> : �e�N�X�`���L�[</param>
		/// <param name="pos"> : �\���ʒu</param>
		/// <param name="size"> : �\���T�C�Y</param>
		/// <param name="cutUV"> : �؂��萔</param>
		/// <param name="useCenter"> : �\������S��</param>
		/// <param name="useIndex"> : �����\���ԍ�</param>
		/// <param name="isAnimation"> : �A�j���[�V�������邩</param>
		Sprite(const shared_ptr<Stage>& ptr, const wstring& texKey, Vec3 pos, Vec2 size, Vec2 pivot) :
			GameObject(ptr),
			m_TexKey(texKey),
			m_Pos(pos), m_Size(size), m_Pivot(pivot),
			m_cutUV(Vec2(1, 1)),
			m_MaxAnimationFrame(1), m_IsAnimation(false),
			m_CurrentAnimation{}, m_BeforeAnimation{},
			m_ScreenHalfSize(0, 0) {
		}

		Sprite(const shared_ptr<Stage>& ptr, const wstring& texKey, Vec3 pos, Vec2 size) : Sprite(ptr, texKey, pos, size, { 0,0 }) {}
		Sprite(const shared_ptr<Stage>& ptr, const wstring& texKey, Vec3 pos, Vec2 size, Anchor anchor);

		virtual ~Sprite() {}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		/// <summary>
		/// �\���T�C�Y�ύX
		/// </summary>
		/// <param name="size"> : �T�C�Y(uv)</param>
		void SetSize(Vec2 size);

		/// <summary>
		/// �\���T�C�Y�擾
		/// </summary>
		/// <returns>�T�C�Y</returns>
		Vec2 GetSize() const {
			return m_Size;
		}

		/// <summary>
		/// �\���ʒu�ύX
		/// </summary>
		/// <param name="pos"> : �\���ʒu</param>
		void SetPosition(Vec3 pos);

		/// <summary>
		/// �\���ʒu�擾
		/// </summary>
		/// <returns>�\���ʒu</returns>
		Vec3 GetPosition() const{
			return m_Pos;
		}

		/// <summary>
		/// �d�S��ݒ�
		/// </summary>
		/// <param name="pivot"> : �d�S</param>
		void SetPivot(Vec2 pivot) {
			m_Pivot = pivot;
		}

		/// <summary>
		/// �d�S���擾
		/// </summary>
		/// <returns>�d�S</returns>
		Vec2 GetPivot() const{
			return m_Pivot;
		}

		/// <summary>
		/// �F�̕ύX
		/// </summary>
		/// <param name="color"> : �F</param>
		void SetDiffuse(Col4 color);

		/// <summary>
		/// �F�̎擾
		/// </summary>
		/// <returns>�F</returns>
		Col4 GetDiffuse()const;

		/// <summary>
		/// ���_���̍쐬�E�X�V
		/// </summary>
		/// <param name="size"> : �\���T�C�Y</param>
		/// <param name="uv"> : uv</param>
		void CreateVertex(Vec2 size, vector<Vec2> uv);

		/// <summary>
		/// ���_�ʒu�̍X�V
		/// </summary>
		/// <param name="positions"> : �ʒu���</param>
		void SetVertex(vector<Vec3> positions);

		/// <summary>
		/// �ő�A�j���[�V�����t���[�����擾
		/// </summary>
		/// <returns>�ő�A�j���[�V�����t���[��</returns>
		size_t GetMaxAnimationFrame() const {
			return m_MaxAnimationFrame;
		}

		/// <summary>
		/// �A�j���[�V�����̍X�V��Ԃ�ݒ�
		/// </summary>
		/// <param name="flag"> : �X�V���</param>
		void SetAnimationActive(bool flag) {
			m_IsAnimation = flag;
		}

		/// <summary>
		/// �A�j���[�V�����̍X�V��Ԃ��擾
		/// </summary>
		/// <returns>�X�V���</returns>
		bool GetAnimationActive() const{
			return m_IsAnimation;
		}

		/// <summary>
		/// �`��D��x��ݒ�
		/// </summary>
		/// <param name="index">�D��x</param>
		void SetLayer(int index) {
			SetDrawLayer(index);
		}

		//----------------------------------------------------------
		//
		//	UV����		
		//																																
		//----------------------------------------------------------

		/// <summary>
		/// �A�j���[�V�����pUV�̍쐬
		/// </summary>
		/// <param name="cut"> : �؂��萔</param>
		/// <param name="maxIndex"> : �g�p����uv�̍ő吔</param>
		/// <returns>�A�j���[�V�����pUV</returns>
		vector<vector<Vec2>> CreateAnimationUV(Vec2 cut, const size_t& maxIndex = 1024);

		/// <summary>
		/// UV�̍X�V
		/// </summary>
		/// <param name="uv">UV</param>
		void UpdateUV(vector<Vec2> uv);

		/// <summary>
		/// �A�j���[�V�����pUV�̎擾(�z��)
		/// </summary>
		/// <returns>�A�j���[�V�����pUV</returns>
		vector<vector<Vec2>> GetUvVec() {
			return m_AnimationUV;
		}

		/// <summary>
		/// �A�j���[�V�����pUV�̎擾(�P��)
		/// </summary>
		/// <param name="index"> : �ԍ�</param>
		/// <returns>�A�j���[�V�����pUV</returns>
		vector<Vec2> GetUv(int index) {
			if (m_AnimationUV.size() <= index) {
				return {};
			}
			return m_AnimationUV[index];
		}

		//----------------------------------------------------------
		//
		//	�A�j���[�V��������		
		//																																
		//----------------------------------------------------------


		/// <summary>
		/// �O�̃A�j���[�V�������擾
		/// </summary>
		/// <returns>�A�j���[�V����</returns>
		SpriteAnimation GetBeforeAnimation() {
			return m_BeforeAnimation;
		}

		/// <summary>
		/// ���݂̃A�j���[�V�������擾
		/// </summary>
		/// <returns>�A�j���[�V����</returns>
		SpriteAnimation GetCurrentAnimation() {
			return m_CurrentAnimation;
		}

		/// <summary>
		/// �w��̃A�j���[�V�������擾
		/// </summary>
		/// <param name="key"> : �A�j���[�V�����L�[</param>
		/// <returns>�A�j���[�V����</returns>
		SpriteAnimation GetAnimation(const wstring& key) {
			if (m_Animations.find(key) != m_Animations.end()) {
				return m_Animations[key];
			}
			return SpriteAnimation();
		}

		/// <summary>
		/// �Đ�����A�j���[�V������ݒ�
		/// </summary>
		/// <param name="key"> : �A�j���[�V�����L�[</param>
		void SetCurrentAnimation(const wstring& key) {
			if (m_Animations.find(key) != m_Animations.end()) {
				m_CurrentAnimation.EndAnimation();
				m_BeforeAnimation = m_CurrentAnimation;
				m_CurrentAnimation = m_Animations[key];
			}
		}

		/// <summary>
		/// �A�j���[�V������ǉ�
		/// </summary>
		/// <param name="key"> : �ݒ肷��L�[</param>
		/// <param name="startOrder"> : �`��J�n</param>
		/// <param name="endOrder"> : �`��I��</param>
		/// <param name="interval"> : �X�V�p�x</param>
		/// <param name="isLoop"> : ���[�v���邩</param>
		/// <param name="isReverse"> : �t�Đ���</param>
		void AddAnimation(const wstring& key, int startOrder, int endOrder, float interval, const bool isLoop = false, const bool isReverse = false) {
			SpriteAnimation animation = SpriteAnimation(startOrder, endOrder, interval, isLoop, isReverse);
			m_Animations.insert(pair<wstring, SpriteAnimation>(key, animation));
		}

		/// <summary>
		/// �A�j���[�V������ǉ�
		/// </summary>
		/// <param name="key"> : �ݒ肷��L�[</param>
		/// <param name="order"> : �`�揇</param>
		/// <param name="interval"> : �X�V�p�x</param>
		/// <param name="isLoop"> : ���[�v���邩</param>
		/// <param name="isReverse"> : �t�Đ���</param>
		void AddAnimation(const wstring& key, vector<int> order, float interval, const bool isLoop = false, const bool isReverse = false) {
			SpriteAnimation animation = SpriteAnimation(order, interval, isLoop, isReverse);
			m_Animations.insert(pair<wstring, SpriteAnimation>(key, animation));
		}

		/// <summary>
		/// �A�j���[�V���������X�V
		/// </summary>
		/// <param name="key"> : �X�V����L�[</param>
		/// <param name="newAnimation"> : �V�����A�j���[�V����</param>
		void UpdateAnimationData(const wstring& key, SpriteAnimation newAnimation) {
			if (m_Animations.find(key) != m_Animations.end()) {
				m_Animations[key] = newAnimation;
			}
		}

		//----------------------------------------------------------
		//
		//	�ʒu�ݒ�e���v���[�g		
		//																																
		//----------------------------------------------------------

		/// <summary>
		/// �A���J�[�ʒu�ݒ�
		/// </summary>
		/// <param name="achor"> : �A���J�[</param>
		/// <param name="offset"> : ����</param>
		void ScreenAnchor(Anchor achor, const Vec3& offset = Vec3());

		void ChangeWindowSize(float width, float height);
	};


	//----------------------------------------------------------
	//																																
	//	���l�\���N���X																												
	//																																
	//----------------------------------------------------------
	class NumberSprite : public GameObject {
		vector<shared_ptr<Sprite>> m_Numbers;
		int m_CutNum;
		int m_DisplayNumber;
		int m_DisplayDigit;

		wstring m_TexKey;
		Vec3 m_Pos;
		Vec2 m_Size;

		vector<Vec2> GetUV(int displayDigit);
	public:
		NumberSprite(const shared_ptr<Stage>& ptr,const wstring& texKey) : NumberSprite(ptr,texKey,Vec3(0,0,0),Vec2(200,100),8){}
		NumberSprite(const shared_ptr<Stage>& ptr,const wstring& texKey,Vec3 pos,Vec2 size,int displayDigit) : NumberSprite(ptr, texKey, pos, size, displayDigit,10) {}
		NumberSprite(const shared_ptr<Stage>& ptr, const wstring& texKey, Vec3 pos, Vec2 size, int displayDigit,int cutNum) : 
			GameObject(ptr) ,
			m_TexKey(texKey),
			m_Pos(pos),m_Size(size),
			m_DisplayDigit(displayDigit),m_DisplayNumber(1234), m_CutNum(cutNum)
		{}
		virtual ~NumberSprite(){}

		virtual void OnCreate();
		virtual void OnUpdate();
		
		void UpdateNumber(int number);
		void SetDiffuse(Col4 color) {
			for (auto& number : m_Numbers) {
				number->SetDiffuse(color);
			}
		}
		void SetActive(bool flag) {
			for (auto& number : m_Numbers) {
				number->SetDrawActive(flag);
			}
		}
		void Destroy();
	};

	//----------------------------------------------------------
	//																																
	//	Sprite����R���|�[�l���g�N���X
	// 
	//	< �g���� >
	//  AddComponent�Ŏg�������h���N���X��ݒ�
	//  Play() : �X�V
	//  Stop() : ��~																				
	//																																
	//----------------------------------------------------------
	class SpriteAction : public Component {
		bool m_IsPlay;
	protected:
		shared_ptr<SpriteBaseDraw> m_Draw;
		shared_ptr<Transform> m_Trans;
	public:
		SpriteAction(const shared_ptr<GameObject>& ptr) : Component(ptr),m_IsPlay(true){}
		virtual ~SpriteAction() {}

		virtual void OnCreate()override;
		virtual void OnDraw()override {}

		virtual void Reset(){}
		void Play() {
			m_IsPlay = true;
		}
		void Stop() {
			m_IsPlay = false;
		}

		bool IsPlay() {
			return m_IsPlay;
		}
	};
	//----------------------------------------------------------
	//																																
	//	Sprite���� : �_��																								
	//																																
	//----------------------------------------------------------
	class SpriteFlash : public SpriteAction {
		float m_FlashSpeed;
	public:
		SpriteFlash(const shared_ptr<GameObject>& ptr,float flashSpeed) : SpriteAction(ptr),
			m_FlashSpeed(flashSpeed){}
		virtual ~SpriteFlash(){}

		virtual void OnUpdate()override;
		virtual void Reset()override;
		void SetFlashSpeed(float flashSpeed) {
			m_FlashSpeed = flashSpeed;
		}

	};
	//----------------------------------------------------------
	//																																
	//	Sprite���� : �g��k��																								
	//																																
	//----------------------------------------------------------
	class SpriteScaling : public SpriteAction {
		float m_ScalingSpeed;
		Vec3 defaultSize;
		float m_Ratio;
		float m_MaxRatio;
		float m_MinRatio;

	public:
		SpriteScaling(const shared_ptr<GameObject>& ptr, float scalingSpeed, float max, float min) : SpriteAction(ptr), 
			m_ScalingSpeed(scalingSpeed), m_MaxRatio(max), m_MinRatio(min),defaultSize(0,0,0),m_Ratio(1.0f) {}
		virtual ~SpriteScaling() {}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		void SetScalingSpeed(float scalingSpeed) {
			m_ScalingSpeed = scalingSpeed;
		}
		void SetMax(float max) {
			m_MaxRatio = max;
		}
		void SetMin(float min) {
			m_MinRatio = min;
		}

	};
	//----------------------------------------------------------
	//																																
	//	Sprite���� : �t�F�[�h�C���E�t�F�[�h�A�E�g																								
	//																																
	//----------------------------------------------------------
	class SpriteFade : public SpriteAction {
		float m_FadeSpeed;
		bool m_IsFadeOut;
		bool m_IsFinished;
	public:
		SpriteFade(const shared_ptr<GameObject>& ptr,float fadeSpeed) : SpriteAction(ptr),m_FadeSpeed(fadeSpeed),m_IsFadeOut(true),m_IsFinished(false){}
		virtual ~SpriteFade(){}

		virtual void OnUpdate()override;

		bool IsFadeOut() {
			return m_IsFadeOut;
		}
		void FadeOut() {
			m_IsFadeOut = true;
			m_IsFinished = false;
			Col4 coler = m_Draw->GetDiffuse();
			coler.w = 0.0f;
			m_Draw->SetDiffuse(coler);
		}
		void FadeIn() {
			m_IsFadeOut = false;
			m_IsFinished = false;
			Col4 coler = m_Draw->GetDiffuse();
			coler.w = 1.0f;
			m_Draw->SetDiffuse(coler);
		}
		bool IsFinish() {
			return m_IsFinished;
		}
	};
	//----------------------------------------------------------
	//																																
	//	�{�^��																		
	//																																
	//----------------------------------------------------------
	class SpriteButton : public SpriteAction {
		shared_ptr<SpriteBaseDraw> m_SpriteDraw;
		shared_ptr<SpriteAction> m_Effect;
		function<void(shared_ptr<ObjectInterface>&)> m_Function;
		function<void(shared_ptr<SpriteButton>&)> m_AddFunction;
		shared_ptr<ObjectInterface> m_ArgmentObject;
		bool m_IsSelect;
		bool m_IsActive;
		wstring m_BelongGroup;

		vector<shared_ptr<Sprite>> m_FrontSprite;

		wstring m_UnSelectTexture;
		wstring m_SelectedTexture;
		Col4 m_UnSelectColor;
		Col4 m_SelectedColor;

		int m_GroupOrder;
		SpriteButton(shared_ptr<GameObject>& ptr, const wstring& defaultTexture, const wstring& group, const wstring& selectedTexture,Col4 selectedColor) :
			SpriteAction(ptr),
			m_BelongGroup(group),
			m_UnSelectTexture(defaultTexture),m_UnSelectColor(Col4()),
			m_SelectedTexture(selectedTexture), m_SelectedColor(selectedColor),
			m_IsSelect(false),m_IsActive(true),
			m_GroupOrder(0)
		{}
	public:

		SpriteButton(shared_ptr<GameObject>& ptr, const wstring& defaultTexture,const wstring& group,const wstring& selectedTexture) :
			SpriteButton(ptr,defaultTexture,group,selectedTexture,Col4(0,0,0,0))
		{}
		SpriteButton(shared_ptr<GameObject>& ptr, const wstring& defaultTexture, const wstring& group, Col4 selectedColor) :
			SpriteButton(ptr,defaultTexture,group,L"",selectedColor)
		{}
		virtual ~SpriteButton(){}

		virtual void OnCreate()override;

		virtual void OnUpdate()override;

		template<class Comp,typename... params>
		void AddEffect(params&&... param) {
			m_Effect = GetGameObject()->AddComponent<Comp>(param...);
		}
		shared_ptr<SpriteAction> GetEffect() {
			return m_Effect;
		}
		void AddFunction(function<void(shared_ptr<SpriteButton>&)> func) {
			m_AddFunction = func;
		}
		vector<shared_ptr<Sprite>> GetFrontSprite() {
			return m_FrontSprite;
		}
		shared_ptr<Sprite> GetFrontSprite(int index) {
			if (m_FrontSprite.size() <= index) {
				return nullptr;
			}
			return m_FrontSprite[index];
		}
		void AddFrontSprite(shared_ptr<Sprite>& sprite) {
			sprite->GetComponent<Transform>()->SetParent(GetGameObject());
			m_FrontSprite.push_back(sprite);
		}
		void SetFunction(function<void(shared_ptr<ObjectInterface>&)> func,const shared_ptr<ObjectInterface>& object) {
			m_Function = func;
			if (object == nullptr) {
				m_ArgmentObject = GetStage();
			}
			else {
				m_ArgmentObject = object;
			}
		}
		void Func() {
			m_Function(m_ArgmentObject);
		}
		void Select() {
			m_IsSelect = true;
		}
		void UnSelect() {
			m_IsSelect = false;
		}
		void Open() {
			SetUpdateActive(true);
			m_SpriteDraw->SetDrawActive(true);
		}
		void Close() {
			SetUpdateActive(false);
			m_SpriteDraw->SetDrawActive(false);
			UnSelect();
		}
		void SetOrder(int order) {
			m_GroupOrder = order;
		}
		int GetOrder() {
			return m_GroupOrder;
		}
		void SetActive(bool flag) {
			m_IsActive = flag;
		}
		bool GetActive() {
			return m_IsActive;
		}
	};
	//----------------------------------------------------------
	//																																
	//	�{�^�����̓f�[�^																							
	//																																
	//----------------------------------------------------------
	enum class InputMode {
		Button,
		Stick
	};
	enum class StickMode {
		LX,
		LY,
		RX,
		RY
	};
	struct InputData {
		int m_Input;
		int m_MoveAmount;

		InputMode m_Mode;
		StickMode m_StickMode;

		bool m_BeforeStickState;
		float m_StickDeadZone;

		InputData(int input,int amount) : m_Input(input),m_MoveAmount(amount),m_Mode(InputMode::Button),m_StickMode(StickMode::LX), m_BeforeStickState(false), m_StickDeadZone(0.0f) {}
		InputData(StickMode mode, int amount,float deadZone) : m_Input(0), m_MoveAmount(amount), m_Mode(InputMode::Stick),m_StickMode(mode), m_BeforeStickState(false), m_StickDeadZone(deadZone) {}

		bool CheckInput(WORD input){
			return input & m_Input;
		}
		bool CheckInput(float input) {
			if (m_BeforeStickState) {
				if (abs(input) <= m_StickDeadZone) {
					m_BeforeStickState = false;
				}
				return false;
			}
			if (abs(input) > m_StickDeadZone) {
				if (input < 0) {
					m_MoveAmount = -abs(m_MoveAmount);
				}
				else {
					m_MoveAmount = abs(m_MoveAmount);
				}
				m_BeforeStickState = true;
				return true;
			}
			return false;

		}
	};
	
	//----------------------------------------------------------
	//																																
	//	�{�^���p�}�l�[�W���[																							
	//																																
	//----------------------------------------------------------

	class ButtonManager : public GameObject{

		template<typename T>
		using group = map<wstring, T>;	//using�e���v���[�g

		group<vector<shared_ptr<SpriteButton>>> m_ButtonGroup;			//�{�^���i�[�p�z��
		group<size_t>							m_SelectIndexes;		//�I�𒆂̔ԍ�
		group<	Vec3>							m_GroupMovementAmount;	//���f������ړ���
		group<vector<InputData>>				m_InputDates;			//�R���g���[���[����
		group<vector<InputData>>				m_KeyboradInputDates;	//�L�[�{�[�h����
		group<vector<WORD>>						m_AcceptButtons;		//�R���g���[���[����{�^��
		group<vector<WORD>>						m_KeyboradAcceptButtons;//�L�[�{�[�h����{�^��
		group<	WORD>							m_PressedAccept;		//���̏u�Ԃɉ����ꂽ����{�^��

		wstring m_UsingGroup;	//�g�p���̃O���[�v��
		wstring m_ClickSound;	//�N���b�N���̃L�[
		wstring m_SelectSound;	//�I�����̃L�[

		bool	m_IsActive;		//Update�����邩


		shared_ptr<Sprite> Create(shared_ptr<Stage>& stage, const wstring& group, const wstring& defaultTex, const wstring& selectedTex, Col4 selectedColor, Vec3 pos, Vec2 size, const shared_ptr<ObjectInterface>& object, function<void(shared_ptr<ObjectInterface>&)> func);

		template<typename Comp, typename... params>
		shared_ptr<Sprite> CreateComp(shared_ptr<Stage>& stage, const wstring& group, const wstring& defaultTex, Vec3 pos, Vec2 size, const shared_ptr<ObjectInterface>& object, function<void(shared_ptr<ObjectInterface>&)> func, params&&... param) {
			auto sprite = stage->AddGameObject<Sprite>(defaultTex, pos, size, true);
			sprite->AddTag(L"Button");
			shared_ptr<SpriteButton> button = sprite->AddComponent<SpriteButton>(defaultTex, group, L"");
			button->SetFunction(func,object);
			button->AddEffect<Comp>(param...);

			return sprite;
		}
		/// <summary>
		/// �w�肵���O���[�v�̏�����
		/// </summary>
		/// <param name="group">�O���[�v��</param>
		void InitGroup(const wstring& group);
	public:
		static shared_ptr<ButtonManager> instance;

		ButtonManager(const shared_ptr<Stage>& ptr) : 
			GameObject(ptr),
			m_IsActive(true),m_UsingGroup(L""),m_ClickSound(L"")
		{}
		virtual ~ButtonManager(){}

		static shared_ptr<Sprite> Create(shared_ptr<Stage>& stage, const wstring& group, const wstring& defaultTex, const wstring& selectedTex, Vec3 pos, Vec2 size,function<void(shared_ptr<ObjectInterface>&)> func);
		static shared_ptr<Sprite> Create(shared_ptr<Stage>& stage, const wstring& group, const wstring& defaultTex, Col4 selectedColor, Vec3 pos, Vec2 size,function<void(shared_ptr<ObjectInterface>&)> func);

		static shared_ptr<Sprite> Create(shared_ptr<Stage>& stage, const wstring& group, const wstring& defaultTex, const wstring& selectedTex, Vec3 pos, Vec2 size,const shared_ptr<ObjectInterface>& object, function<void(shared_ptr<ObjectInterface>&)> func);
		static shared_ptr<Sprite> Create(shared_ptr<Stage>& stage, const wstring& group, const wstring& defaultTex, Col4 selectedColor, Vec3 pos, Vec2 size, const shared_ptr<ObjectInterface>& object, function<void(shared_ptr<ObjectInterface>&)> func);

		template<class Comp, typename... params>
		static shared_ptr<Sprite> Create(shared_ptr<Stage>& stage, const wstring& group, const wstring& defaultTex, Vec3 pos, Vec2 size, function<void(shared_ptr<ObjectInterface>&)> func, params&&... param) {
			return ButtonManager::instance->CreateComp<Comp>(stage, group, defaultTex, pos, size, nullptr, func, param...);
		}
		template<class Comp, typename... params>
		static shared_ptr<Sprite> Create(shared_ptr<Stage>& stage, const wstring& group, const wstring& defaultTex, Vec3 pos, Vec2 size, const shared_ptr<ObjectInterface>& object, function<void(shared_ptr<ObjectInterface>&)> func, params&&... param) {
			return ButtonManager::instance->CreateComp<Comp>(stage, group, defaultTex, pos, size, object, func, param...);
		}
		

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDestroy()override;

		/// <summary>
		/// Updata�����s�ł����Ԃ��m�F
		/// </summary>
		/// <returns>���</returns>
		bool CheckUpdate();

		/// <summary>
		/// �R���g���[���[�̓��͊m�F
		/// </summary>
		/// <param name="input">���̓f�[�^</param>
		/// <returns>���̗͂L��</returns>
		bool CheckMoveInput(InputData& input);

		/// <summary>
		/// �w�肵���O���[�v���o�^����Ă��邩
		/// </summary>
		/// <typeparam name="T">�z��f�[�^�̌^</typeparam>
		/// <param name="vec">��������z��</param>
		/// <param name="group">�O���[�v��</param>
		/// <returns>�o�^����Ă��邩</returns>
		template<typename T>
		inline bool FindGroup(group<T> vec,const wstring& group) {
			return vec.find(group) != end(vec);
		}

		/// <summary>
		/// �{�^���̃X�v���C�g���擾
		/// </summary>
		/// <param name="group">�O���[�v��</param>
		/// <param name="index">�o�^�ԍ�</param>
		/// <returns>�{�^���̃X�v���C�g</returns>
		shared_ptr<Sprite> GetButtonSprite(const wstring& group, int index) {
			if (FindGroup(m_ButtonGroup,group)) {
				auto vec = m_ButtonGroup[group];
				if (vec.size() > index) {
					auto obj = vec[index]->GetGameObject();
					return static_pointer_cast<Sprite>(obj);
				}
			}
			return nullptr;
		}

		/// <summary>
		/// �I����͂̊m�F
		/// </summary>
		/// <param name="group">�O���[�v��</param>
		/// <param name="data">�f�[�^�i�[�p</param>
		/// <returns>���̗͂L��</returns>
		bool PressSelect(const wstring& group,InputData& data) {
			auto keyborad = App::GetApp()->GetInputDevice().GetKeyState();

			if (FindGroup(m_KeyboradInputDates, group)) {
				for (auto& inputData : m_KeyboradInputDates[group]) {
					if (keyborad.m_bPressedKeyTbl[inputData.m_Input]) {
						if (abs(inputData.m_MoveAmount) > 1) {
							if (!CheckOverIndex(inputData.m_MoveAmount)) continue;
						}
						data = inputData;
						return true;
					}
				}
			}
			if (FindGroup(m_InputDates, group)) {
				for (auto& inputData : m_InputDates[group]) {
					if (CheckMoveInput(inputData)) {
						if (abs(inputData.m_MoveAmount) > 1) {
							if (!CheckOverIndex(inputData.m_MoveAmount)) continue;
						}
						data = inputData;
						return true;
					}
				}
			}
			return false;
		}

		/// <summary>
		/// ������͂̊m�F
		/// </summary>
		/// <param name="group">�O���[�v��</param>
		/// <param name="data">�f�[�^�i�[�p</param>
		/// <returns>���̗͂L��</returns>
		bool PressAccept(const wstring& group,WORD& data) {
			auto& inputState = App::GetApp()->GetInputDevice().GetControlerVec()[0];
			auto keyborad = App::GetApp()->GetInputDevice().GetKeyState();
			if (FindGroup(m_KeyboradInputDates, group)) {
				for (auto& inputData : m_KeyboradAcceptButtons[group]) {
					if (keyborad.m_bPressedKeyTbl[inputData]) {
						data = inputData;
						return true;
					}
				}
			}
			if (FindGroup(m_InputDates, group)) {
				for (auto& inputData : m_AcceptButtons[group]) {
					if (inputState.wPressedButtons & inputData) {
						data = inputData;
						return true;
					}
				}
			}
			data = 0;
			return false;
		}

		/// <summary>
		/// �I�𒆂̔ԍ����擾
		/// </summary>
		/// <param name="group">�O���[�v��</param>
		/// <returns>�ԍ�</returns>
		size_t GetSelectIndex(const wstring& group) {
			if (FindGroup(m_ButtonGroup, group)) {
				return m_SelectIndexes[group];
			}
			return -1;
		}

		/// <summary>
		/// �I������ԍ��̎w��
		/// </summary>
		/// <param name="group">�O���[�v��</param>
		/// <param name="index">�ԍ�</param>
		void SetSelectIndex(const wstring& group,int index) {
			if (group == L"") {
				if (index < m_ButtonGroup[m_UsingGroup].size()) {
					m_SelectIndexes[m_UsingGroup] = index;
				}
				return;
			}

			if (FindGroup(m_ButtonGroup, group)) {
				if (index < m_ButtonGroup[group].size()) {
					m_SelectIndexes[group] = index;
				}
			}
		}

		/// <summary>
		/// �g�p���̃O���[�v�����邩
		/// </summary>
		/// <returns>�g�p���̃O���[�v�̗L��</returns>
		bool ExistOpenGroup() {
			for (auto& buttons : m_ButtonGroup) {
				if (buttons.second[0]->GetDrawActive()) {
					return true;
				}
			}
			return false;
		}

		/// <summary>
		/// �o�^���ꂽ�{�^���̐����擾
		/// </summary>
		/// <param name="group">�O���[�v��</param>
		/// <returns>�{�^���̐�</returns>
		size_t GetSize(const wstring& group) {
			if (FindGroup(m_ButtonGroup, group)) {
				return m_ButtonGroup[group].size();
			}
			else {
				return 0;
			}
		}

		/// <summary>
		/// �ʒu�ړ��ʂ̐ݒ�
		/// </summary>
		/// <param name="group">�O���[�v��</param>
		/// <param name="target">�ړ���</param>
		void SetMoveAmount(const wstring& group, Vec3 target);

		/// <summary>
		/// �ʒu�ړ��ʂ̎擾
		/// </summary>
		/// <param name="group">�O���[�v��</param>
		/// <returns>�ړ���</returns>
		Vec3 GetMoveAmount(const wstring& group) {
			if (FindGroup(m_GroupMovementAmount, group)) {
				return m_GroupMovementAmount[group];
			}
		}

		/// <summary>
		/// ���艹�̎擾
		/// </summary>
		/// <param name="sound">�T�E���h�L�[</param>
		void SetSound(const wstring& sound);

		/// <summary>
		/// �I�����̐ݒ�
		/// </summary>
		/// <param name="sound">�T�E���h�L�[</param>
		void SetSelectSound(const wstring& sound);

		/// <summary>
		/// �R���g���[���[����{�^���̒ǉ�
		/// </summary>
		/// <param name="group">�O���[�v��</param>
		/// <param name="accept">�{�^���̒l</param>
		void AddAcceptButton(const wstring& group, WORD accept) {
			if (FindGroup(m_AcceptButtons, group)) {
				m_AcceptButtons[group].push_back(accept);
			}
			else {
				vector<WORD> button = {};
				button.push_back(accept);
				m_AcceptButtons.emplace(group, button);
				m_PressedAccept.emplace(group, 0);
			}
		}

		/// <summary>
		/// �L�[�{�[�h����L�[�̒ǉ�
		/// </summary>
		/// <param name="group">�O���[�v��</param>
		/// <param name="accept">�L�[</param>
		void AddKeyboradAccept(const wstring& group, WORD accept) {
			if (FindGroup(m_KeyboradAcceptButtons, group)) {
				m_KeyboradAcceptButtons[group].push_back(accept);
			}
			else {
				vector<WORD> button = {};
				button.push_back(accept);
				m_KeyboradAcceptButtons.emplace(group, button);
				//m_PressedAccept.emplace(group, 0);
			}
		}

		/// <summary>
		/// �����ꂽ����{�^���̎擾
		/// </summary>
		/// <param name="group">�O���[�v��</param>
		/// <returns>�{�^���̒l</returns>
		WORD GetPressedAccept(const wstring& group) {
			if (FindGroup(m_PressedAccept, group)) {
				return m_PressedAccept[group];
			}
			return 0;
		}
		/// <summary>
		/// �R���g���[���[�I���{�^���̒ǉ�
		/// </summary>
		/// <param name="group">�O���[�v��</param>
		/// <param name="input">�{�^���̒l</param>
		/// <param name="amount">�ړ���</param>
		void SetInput(const wstring& group,int input,int amount) {
			SetInput(group, InputData(input, amount));
		}

		/// <summary>
		/// �R���g���[���[�I���{�^���̒ǉ�
		/// </summary>
		/// <param name="group">�O���[�v��</param>
		/// <param name="data">���̓f�[�^</param>
		void SetInput(const wstring& group, InputData data) {
			if (FindGroup(m_InputDates, group)) {
				m_InputDates[group].push_back(data);
			}
			else {
				vector<InputData> dates = {};
				dates.push_back(data);
				m_InputDates.emplace(group, dates);
			}
		}

		/// <summary>
		/// �L�[�{�[�h�I���L�[�̒ǉ�
		/// </summary>
		/// <param name="group">�O���[�v��</param>
		/// <param name="input">�L�[</param>
		/// <param name="amount">�ړ���</param>
		void SetKeyborad(const wstring& group, int input, int amount) {
			SetKeyborad(group, InputData(input, amount));
		}
		
		/// <summary>
		/// �L�[�{�[�h�I���L�[�̒ǉ�
		/// </summary>
		/// <param name="group">�O���[�v��</param>
		/// <param name="data">���̓f�[�^</param>
		void SetKeyborad(const wstring& group, InputData data) {
			if (FindGroup(m_KeyboradInputDates, group)) {
				m_KeyboradInputDates[group].push_back(data);
			}
			else {
				vector<InputData> dates = {};
				dates.push_back(data);
				m_KeyboradInputDates.emplace(group, dates);
			}
		}

		/// <summary>
		/// �o�^����Ă���O���[�v���J��
		/// </summary>
		void OpenAll() {
			for (auto& buttons : m_ButtonGroup) {
				Open(buttons.first);
			}
		}

		/// <summary>
		/// �o�^����Ă���O���[�v�����
		/// </summary>
		void CloseAll() {
			for (auto& buttons : m_ButtonGroup) {
				Close(buttons.first);
			}
		}

		/// <summary>
		/// �w�肵���O���[�v���J���A�g�p����
		/// </summary>
		/// <param name="group">�O���[�v��</param>
		void OpenAndUse(const wstring& group) {
			Open(group);
			UseGroup(group);
		}

		/// <summary>
		/// �w�肵���O���[�v���J��
		/// </summary>
		/// <param name="group">�O���[�v��</param>
		void Open(const wstring& group) {
			if (FindGroup(m_ButtonGroup, group)) {
				for (auto& button : m_ButtonGroup[group]) {
					button->Open();
				}
			}
			SetActive(true);
		}

		/// <summary>
		/// �w�肵���O���[�v�����
		/// </summary>
		/// <param name="group">�O���[�v��</param>
		void Close(const wstring& group) {
			if (FindGroup(m_ButtonGroup, group)) {
				for (auto& button : m_ButtonGroup[group]) {
					button->Close();
				}
				m_SelectIndexes[group] = 0;
				SetActive(false);
			}
		}

		/// <summary>
		/// Update�̐ݒ�
		/// </summary>
		/// <param name="flag">Update���邩</param>
		void SetActive(bool flag) {
			m_IsActive = flag;
		}

		/// <summary>
		/// Updata��Ԃ̎擾
		/// </summary>
		/// <returns>Update���</returns>
		bool GetActive() {
			return m_IsActive;
		}

		/// <summary>
		/// �I�𒆂̔ԍ����͈͊O�ɍs���Ȃ��悤�ɐ�������
		/// </summary>
		void LimitIndex() {
			size_t maxIndex = m_ButtonGroup[m_UsingGroup].size() - 1;
			size_t minIndex = 0;
			m_SelectIndexes[m_UsingGroup] = max(minIndex, m_SelectIndexes[m_UsingGroup]);
			m_SelectIndexes[m_UsingGroup] = min(maxIndex, m_SelectIndexes[m_UsingGroup]);
		}
		/// <summary>
		/// �ړ���̔ԍ����͈͊O�ɍs���Ă��Ȃ������肷��
		/// </summary>
		/// <param name="amount">�ړ���</param>
		/// <returns>�͈͓����ǂ���</returns>
		bool CheckOverIndex(int amount) {
			size_t index = m_SelectIndexes[m_UsingGroup];
			index += amount;
			if (index >= m_ButtonGroup[m_UsingGroup].size() || index < 0) {
				return false;
			}
			return true;
		}

		/// <summary>
		/// ���ݎg�p���̃O���[�v��
		/// </summary>
		/// <returns>�O���[�v��</returns>
		wstring GetUseGroup() {
			return m_UsingGroup;
		}

		/// <summary>
		/// �g�p����O���|�v�̐ݒ�
		/// </summary>
		/// <param name="group">�O���[�v��</param>
		void UseGroup(const wstring& group) {
			if (FindGroup(m_SelectIndexes, group)) {
				InitGroup(m_UsingGroup);
				//m_SelectIndexes[m_UsingGroup] = 0;
				m_UsingGroup = group;
			}
		}

		/// <summary>
		/// �w�肵���O���[�v���ƌ��ݎg�p���Ă���O���[�v������v���Ă��邩���肷��
		/// </summary>
		/// <param name="group">�O���[�v��</param>
		/// <returns>��v���Ă��邩</returns>
		bool CompareUseGroup(const wstring& group) {
			return m_UsingGroup == group;
		}
		void Register(const wstring& group, shared_ptr<SpriteButton>& button) {
			if (FindGroup(m_ButtonGroup, group)) {
				m_ButtonGroup[group].push_back(button);
			}
			else {
				if (m_ButtonGroup.size() == 0) {
					m_UsingGroup = group;
				}
				vector<shared_ptr<SpriteButton>> buttons = {};
				buttons.push_back(button);
				m_ButtonGroup.emplace(group, buttons);
				m_SelectIndexes.emplace(group, 0);
				m_GroupMovementAmount.emplace(group, Vec3());
			}
			button->SetOrder(static_cast<int>(m_ButtonGroup[group].size()) - 1);
		}

		/// <summary>
		/// �O���[�v���폜
		/// </summary>
		/// <param name="group"></param>
		void DeleteGroup(const wstring& group) {
			if (FindGroup(m_ButtonGroup, group)) {
				m_ButtonGroup.erase(group);
				m_SelectIndexes.erase(group);
			}
		}

		void AddFrontSprite(const wstring& group,int index,shared_ptr<Sprite>& sprite) {
			if (FindGroup(m_ButtonGroup, group)) {
				if (m_ButtonGroup[group].size() > index) {
					m_ButtonGroup[group][index]->AddFrontSprite(sprite);
				}
			}
		}
		void AddFunction(const wstring& group,function<void(shared_ptr<SpriteButton>&)> func) {
			if (FindGroup(m_ButtonGroup, group)) {
				for (auto& buttons : m_ButtonGroup[group]) {
					buttons->AddFunction(func);
				}
			}
		}
		void AddFunction(const wstring& group,int index, function<void(shared_ptr<SpriteButton>&)> func) {
			if (FindGroup(m_ButtonGroup, group)) {
				if (index < 0 || index >= m_ButtonGroup[group].size()) return;
				m_ButtonGroup[group][index]->AddFunction(func);
			}
		}
		
	};

	//----------------------------------------------------------
	//																																
	//	�|���p																					
	//																																
	//----------------------------------------------------------

	class Board : public GameObject {
		shared_ptr<PNTStaticDraw> m_Draw;
		shared_ptr<Transform> m_Trans;

		Vec3 m_StartPos;
		Vec3 m_Size;
		Vec3 m_Offset;
		wstring m_TexKey;
		bool m_IsBillBoard;

		vector<VertexPositionNormalTexture> m_Vertices;
	public:
		Board(shared_ptr<Stage>& ptr, const wstring& key,Vec3 pos, Vec3 size,const bool& isBillBoard = true) : 
			GameObject(ptr),
			m_TexKey(key), m_StartPos(pos),m_Offset(Vec3()), m_Size(size), m_IsBillBoard(isBillBoard) { }
		virtual ~Board() {}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		void SetOffset(Vec3 offset) {
			m_Offset = offset;
		}
		shared_ptr<PNTStaticDraw> GetDraw() {
			return m_Draw;
		}
		void SetColor(Col4 color) {
			m_Draw->SetDiffuse(color);
		}
		vector<VertexPositionNormalTexture> GetVertices() {
			return m_Vertices;
		}
	};

	class SlideInSprite : public GameObject
	{
	public:

		bool m_pushState = false;
		shared_ptr<PCTSpriteDraw> m_DrawComp;
		vector<VertexPositionColorTexture> m_Vertices;
		shared_ptr<GameObject> m_selectCharge;
		shared_ptr<GameObject> m_titleCharge;
		float m_fade = 1.0f;
		float m_time;
		float m_maxtime;
		const float m_fadeSpeed = 2.0f;
		bool m_Trace;
		Vec2 m_StartScale;
		Vec3 m_StartPos;
		wstring m_TextureKey;
		float UVCharge;

		const float windowWidth = App::GetApp()->GetGameWidth();
		const float windowHeight = App::GetApp()->GetGameHeight();

		SlideInSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec3& StartPos, const float& Maxtime);
		virtual ~SlideInSprite() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void ChargeUV(const float& time);
		void UpdateProgress(float time);
		void SetDiffuse(Col4 rgba);
	};
}
//end basecross

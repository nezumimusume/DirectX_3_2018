#pragma once

#include "C3DModelDraw.h"
#include "RenderTarget.h"
#include "ShadowMap.h"
#include "Sprite.h"

/// <summary>
/// �Q�[���N���X�B
/// </summary>
class Game
{
	
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	Game();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~Game();

	/// <summary>
	/// �X�V�����B
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈���B
	/// </summary>
	void Render();
	/// <summary>
	/// �V���h�E�}�b�v���擾�B
	/// </summary>
	/// <returns></returns>
	ShadowMap* GetShadowMap()
	{
		return &m_shadowMap;
	}
private:
	/// <summary>
	/// �����������̃u�����h�X�e�[�g���������B
	/// </summary>
	void InitTranslucentBlendState();
private:
	CVector3 m_unityChanPos = { 0.0f, 0.0f, 0.0f };			//���j�e�B�����̍��W�B
	C3DModelDraw m_unityChanModelDraw;						//���j�e�B�����̃��f���̕`�揈���B�B
	C3DModelDraw m_bgModelDraw;								//�w�i�̃��f���̕`�揈���B
	ID3D11BlendState* m_translucentBlendState = nullptr;	//�����������p�̃u�����h�X�e�[�g�B
	Sprite m_sprite;										//�X�v���C�g�B
	ShadowMap m_shadowMap;									//�V���h�E�}�b�v�B
};


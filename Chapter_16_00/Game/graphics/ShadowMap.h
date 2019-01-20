#pragma once
#include "graphics/RenderTarget.h"

class C3DModelDraw;

/// <summary>
/// �V���h�E�}�b�v�N���X�B
/// </summary>
/// <code>
/// //�V���h�E�}�b�v�𐶐�����T���v���R�[�h�B
/// int main()
/// {
///		ShadowMap g_shadowMap;
///		C3DModelDraw playerModel;
///		C3DModelDraw bgModel;
///			�E
///			�ȗ�
///			�E
///		//�Q�[�����[�v�B
///		while( true )
///		{
///				�E
///				�ȗ�
///				�E
///			//�V���h�E�L���X�^�[��o�^�B
///			g_shadowMap.RegistShadowCaster(&playerModel);
///			g_shadowMap.Update();
///				�E
///				�ȗ�
///				�E
///			//�V���h�E�}�b�v���쐬�A
///			g_shadowMap.RenderToShadowMap();
///				�E
///				�ȗ�
///				�E
///		}
/// }
/// </code>
class ShadowMap
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	ShadowMap();

	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~ShadowMap();

	/// <summary>
	/// ���C�g�r���[�s����擾�B
	/// </summary>
	/// <returns></returns>
	CMatrix GetLighViewMatrix() const
	{
		return m_lightViewMatrix;
	}

	/// <summary>
	/// ���C�g�v���W�F�N�V�����s����擾�B
	/// </summary>
	/// <returns></returns>
	CMatrix GetLightProjMatrix() const
	{
		return m_lightProjMatrix;
	}
	
	/// <summary>
	/// �X�V(���C�g�J�����̒����_���w�肷��o�[�W�����B)
	/// </summary>
	/// <remarks>
	/// ���C�g�r���[�s���A���C�g�v���W�F�N�V�����s���
	/// �X�V���܂��BUpdateFromLightTarget��UpdateFromLightDirection�̂ǂ��炩��
	/// ���t���[���Ăяo���Ă��������B
	/// </remarks>
	/// <param name="pos">���C�g�J�����̎��_�̍��W</param>
	/// <param name="target">���C�g���߂�̒����_�̍��W</param>
	void UpdateFromLightTarget(CVector3 lightCameraPos, CVector3 lightCameraTarget);

	/// <summary>
	/// �X�V(���C�g�J�����̌������w�肷��o�[�W�����B
	/// </summary>
	/// <remarks>
	/// ���C�g�r���[�s���A���C�g�v���W�F�N�V�����s���
	/// �X�V���܂��BUpdateFromLightTarget��UpdateFromLightDirection�̂ǂ��炩��
	/// ���t���[���Ăяo���Ă��������B
	/// </remarks>
	/// <param name="lightCameraPos">���C�g�J�����̎��_�̍��W</param>
	/// <param name="lightDir">���C�g�̕���</param>
	void UpdateFromLightDirection(CVector3 lightCameraPos, CVector3 lightDir);

	/// <summary>
	/// �V���h�E�}�b�v�ɃV���h�E�L���X�^�[�������_�����O�B
	/// ���t���[���Ăяo���ĉ������B
	/// </summary>
	/// <remarks>
	/// �V���h�E�}�b�v�ɕ`�悳���V���h�E�L���X�^�[��
	/// RegistShadowCaster�֐����g�p���ēo�^���ꂽ�L���X�^�[�݂̂ł��B
	/// ���̊֐����Ăяo���ƁA�V���h�E�L���X�^�[�̃��X�g�̓N���A����܂��B
	/// </remarks>
	void RenderToShadowMap();

	/// <summary>
	/// �V���h�E�L���X�^�[��o�^�B
	/// </summary>
	/// <remarks>
	/// ���̊֐����g���āA�o�^���ꂽ�L���X�^�[��
	/// �V���h�E�}�b�v�ɕ`�悳��܂��B
	/// �o�^���ꂽ�L���X�^�[�̃��X�g��RenderToShadowMap���Ăяo������
	/// �N���A����邽�߁A��ɉe�𐶐��������ꍇ�́A
	/// ���t���[�����̊֐����Ăяo���K�v������܂��B
	/// </remarks>
	/// <param name="shadowCaster">�V���h�E�L���X�^�[</param>
	void RegistShadowCaster(C3DModelDraw* shadowCaster)
	{
		m_shadowCasters.push_back(shadowCaster);
	}

	/// <summary>
	/// �V���h�E�}�b�v��SRV���擾�B
	/// </summary>
	/// <returns>�V���h�E�}�b�v��SRV</returns>
	ID3D11ShaderResourceView* GetShadowMapSRV()
	{
		return m_shadowMapRT.GetRenderTargetSRV();
	}
private:
	CVector3 m_lightCameraPosition = CVector3::Zero();	//���C�g�J�����̎��_�B
	CVector3 m_lightCameraTarget = CVector3::Zero();	//���C�g�J�����̒����_�B
	CMatrix m_lightViewMatrix = CMatrix::Identity();	//���C�g�r���[�s��B
	CMatrix m_lightProjMatrix = CMatrix::Identity();	//���C�g�v���W�F�N�V�����s��B
	RenderTarget m_shadowMapRT;						//�V���h�E�}�b�v��`�悷�郌���_�����O�^�[�Q�b�g�B
	std::vector< C3DModelDraw*> m_shadowCasters;	//�V���h�E�L���X�^�[�̔z��B
};

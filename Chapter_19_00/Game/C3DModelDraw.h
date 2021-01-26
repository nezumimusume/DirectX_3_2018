#pragma once

#include "C3DModelEffect.h"
#include "ConstantBufferGPU.h"

const int NUM_DIRECTION_LIG = 4;
/*!
*@brief	�f�B���N�V�������C�g�B
*/
struct SDirectionLight {
	CVector4 direction[NUM_DIRECTION_LIG];				//���C�g�̕����B
	CVector4 color[NUM_DIRECTION_LIG];					//���C�g�̃J���[�B
	CVector4 ambinetLight = {0.4f, 0.4f, 0.4f, 1.0f};	//�A���r�G���g���C�g�B
	CVector3 eyePos;									//���_�B
};


/*!
*@brief
*  3D���f���`��N���X�B
*/
class C3DModelDraw {
	/// <summary>
	/// ���f���G�t�F�N�g�p�̍\���́B
	/// </summary>
	/// <remarks>
	/// �����ύX������AAsserts/shader/model.fx��
	/// VSCb���ύX����K�v������܂��B
	/// ���̍\���̂�16byte�A���C�����g�ɂȂ��Ă���K�v������܂��B
	/// </remarks>
	struct SModelFxConstantBuffer {
		CMatrix mWorld;			//���[���h�s��B
		CMatrix mView;			//�r���[�s��B
		CMatrix mProj;			//�v���W�F�N�V�����s��B
		CMatrix mLightView;		//���C�g�r���[�s��B
		CMatrix mLightProj;		//���C�g�v���W�F�N�V�����s��B
		int isShadowReciever;	//�V���h�E���V�[�o�[�̃t���O�B
		int isHasNormalMap;		//�@���}�b�v��ێ����Ă���H
		int isHasSpecuraMap;	//�X�y�L�����}�b�v��ێ����Ă���H
	};
	std::unique_ptr<DirectX::Model>		m_modelDx;				//DirectXTK���񋟂��郂�f���B
	ID3D11Buffer*						m_cb = nullptr;			//!<�萔�o�b�t�@�B
	ConstantBufferGPU					m_lightCbGPU;			//!<���C�g�p�̒萔�o�b�t�@�B
	SDirectionLight						m_dirLight;				//!<�f�B���N�V�������C�g�B
	ID3D11SamplerState* m_samplerState = nullptr;			//!<�T���v���[�X�e�[�g�B
	CMatrix m_worldMatrix = CMatrix::Identity();			//!<���[���h�s��B
	bool m_isShadowReciever = false;						//�V���h�E���V�[�o�[�̃t���O�B
	ID3D11ShaderResourceView* m_normalMapSRV = nullptr;		//���}�b�v��SRV
	ID3D11ShaderResourceView* m_specularMapSRV = nullptr;	//�X�y�L�����}�b�v��SRV
public:
	//�f�X�g���N�^�B
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~C3DModelDraw();

	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="filePath">���[�h����cmo�t�@�C���̃t�@�C���p�X�B</param>
	void Init(const wchar_t* filePath);

	/// <summary>
	/// �X�V�B
	/// </summary>
	/// <param name="trans">���s�ړ�</param>
	/// <param name="qRot">��]</param>
	/// <param name="scale">�g�嗦</param>
	void Update(CVector3 trans);
	/// <summary>
	/// �`��B
	/// </summary>
	/// <param name="renderStep">
	///	�`�惂�[�h�BGraphicsEngine.h��EnRenderMode���Q�ƁB
	/// </param>
	/// <param name="viewMatrix">�r���[�s��</param>
	/// <param name="projMatrix">�v���W�F�N�V�����s��s��</param>
	void Draw(EnRenderMode renderMode, CMatrix viewMatrix, CMatrix projMatrix);
	/// <summary>
	/// �}�e���A���ɑ΂��ăN�G�����s���B
	/// </summary>
	/// <param name="func">�₢���킹�֐�</param>
	void QueryMaterials(std::function<void(C3DModelEffect*)> func)
	{
		m_modelDx->UpdateEffects([&](DirectX::IEffect* material) {
			func(reinterpret_cast<C3DModelEffect*>(material));
		});
	}
	/// <summary>
	/// �V���h�E���V�[�o�[�̃t���O��ݒ肷��B
	/// </summary>
	/// <param name="flag">true��n���ƃV���h�E���V�[�o�[�ɂȂ�</param>
	/// <remarks>
	/// �V���h�E���V�[�o�[�Ƃ͉e�𗎂Ƃ����I�u�W�F�N�g�̂��Ƃł��B
	/// �V���h�E�L���X�^�[�ɂ���Đ������ꂽ�A�V���h�E�}�b�v�𗘗p����
	/// ���g�ɉe�𗎂Ƃ��܂��B
	/// �I�u�W�F�N�g���V���h�E���V�[�o�[���V���h�E�L���X�^�[�ɂȂ��Ă���ꍇ��
	/// �Z���t�V���h�E(�����̉e�������ɗ�����)���s�����Ƃ��ł��܂��B
	/// </remarks>
	void SetShadowReciever(bool flag)
	{
		m_isShadowReciever = flag;
	}
	/// <summary>
	/// �f�B���N�V�������C�g�̃J���[��ݒ�
	/// </summary>
	/// <param name="ligNo">���C�g�̔ԍ�</param>
	/// <param name="color">���C�g�̃J���[</param>
	void SetDirectionLightColor(int ligNo, CVector4 color)
	{
		if (ligNo < NUM_DIRECTION_LIG) {
			m_dirLight.color[ligNo] = color;
		}
		else {
			std::abort();
		}
	}
	/// <summary>
	/// �f�B���N�V�������C�g�̕�����ݒ�B
	/// </summary>
	/// <param name="ligNo">���C�g�̔ԍ�</param>
	/// <param name="dir">���C�g�̕���</param>
	void SetDirectionLightDirection(int ligNo, CVector3 dir)
	{
		if (ligNo < NUM_DIRECTION_LIG) {
			m_dirLight.direction[ligNo] = dir;
		}
		else {
			std::abort();
		}
	}
	/// <summary>
	/// �����̐ݒ�B
	/// </summary>
	/// <param name="ambinetLight"></param>
	void SetAmbientLight(CVector3 ambinetLight)
	{
		m_dirLight.ambinetLight = ambinetLight;
	}
	/// <summary>
	/// �@���}�b�v�̐ݒ�B
	/// </summary>
	void SetNormalMap(ID3D11ShaderResourceView* srv)
	{
		m_normalMapSRV = srv;
	}
	/// <summary>
	/// �X�y�L�����}�b�v��ݒ�B
	/// </summary>
	/// <param name="srv"></param>
	void SetSpecularMap(ID3D11ShaderResourceView* srv)
	{
		m_specularMapSRV = srv;
	}

private:
	//�f�B���N�V�������C�g�̏������B
	/// <summary>
	/// �f�B���N�V�������C�g�̏������B
	/// </summary>
	void InitDirectionLight();

	/// <summary>
	/// �T���v���X�e�[�g�̏������B
	/// </summary>
	void InitSamplerState();

	/// <summary>
	/// �萔�o�b�t�@�̏������B
	/// </summary>
	void InitConstantBuffer();
	
};
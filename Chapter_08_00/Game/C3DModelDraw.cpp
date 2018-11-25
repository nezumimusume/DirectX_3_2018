#include "stdafx.h"
#include "C3DModelDraw.h"
#include "graphics/Camera.h"


C3DModelDraw::~C3DModelDraw()
{
	//�萔�o�b�t�@������B
	if (m_cb != nullptr) {
		m_cb->Release();
	}
	//���C�g�p�̒萔�o�b�t�@�̉���B
	if (m_lightCb != nullptr) {
		m_lightCb->Release();
	}
	//�T���v���X�e�[�g������B
	if (m_samplerState != nullptr) {
		m_samplerState->Release();
	}
}
void C3DModelDraw::Update(CVector3 trans)
{
	m_worldMatrix.MakeTranslation(trans);
}
void C3DModelDraw::Init(const wchar_t* filePath)
{
	//�萔�o�b�t�@���������B
	InitConstantBuffer();
	//�T���v���X�e�[�g���������B
	InitSamplerState();

	//�f�B���N�V�������C�g�̏������B
	InitDirectionLight();

	//�G�t�F�N�g�t�@�N�g�����쐬
	C3DModelEffectFactory effectFactory(g_graphicsEngine->GetD3DDevice());
	//�e�N�X�`��������t�H���_��ݒ肷��B
	effectFactory.SetDirectory(L"Assets/modelData");
	//cmo�t�@�C������DirectX::Model���쐬����B
	m_modelDx = DirectX::Model::CreateFromCMO(	//CMO�t�@�C�����烂�f�����쐬����֐��́ACreateFromCMO�����s����B
		g_graphicsEngine->GetD3DDevice(),			//��������D3D�f�o�C�X�B
		filePath,									//�������͓ǂݍ���CMO�t�@�C���̃t�@�C���p�X�B
		effectFactory,								//��O�����̓G�t�F�N�g�t�@�N�g���B
		false,										//��l������Cull���[�h�B���͋C�ɂ��Ȃ��Ă悢�B
		false
	);
}
void C3DModelDraw::Draw()
{
	auto deviceContext = g_graphicsEngine->GetD3DDeviceContext();
	DirectX::CommonStates state(g_graphicsEngine->GetD3DDevice());


	//�萔�o�b�t�@���X�V�B
	SVSConstantBuffer vsCb;
	vsCb.mWorld = m_worldMatrix;
	vsCb.mProj = g_camera3D.GetProjectionMatrix();
	vsCb.mView = g_camera3D.GetViewMatrix();
	deviceContext->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);
	//���C�g�p�̒萔�o�b�t�@���X�V�B
	deviceContext->UpdateSubresource(m_lightCb, 0, nullptr, &m_dirLight, 0, 0);

	//�萔�o�b�t�@���V�F�[�_�[�X���b�g�ɐݒ�B
	deviceContext->VSSetConstantBuffers(0, 1, &m_cb);
	deviceContext->PSSetConstantBuffers(0, 1, &m_lightCb);
	//�T���v���X�e�[�g��ݒ肷��B
	deviceContext->PSSetSamplers(0, 1, &m_samplerState);

	m_modelDx->Draw(
		deviceContext,
		state,
		m_worldMatrix,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}


void C3DModelDraw::InitDirectionLight()
{
	m_dirLight.direction[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
	m_dirLight.color[0] = { 0.7f, 0.7f, 0.7f, 1.0f };

	m_dirLight.direction[1] = { -1.0f, 0.0f, 0.0f, 0.0f };
	m_dirLight.color[1] = { 0.4f, 0.4f, 0.4f, 1.0f };

	m_dirLight.direction[2] = { 0.0f, 0.0f, 1.0f, 0.0f };
	m_dirLight.color[2] = { 0.4f, 0.4f, 0.4f, 1.0f };

	m_dirLight.direction[3] = { 1.0f, 0.0f, -1.0f, 0.0f };
	m_dirLight.color[3] = { 0.4f, 0.4f, 0.4f, 1.0f };
}


void C3DModelDraw::InitSamplerState()
{
	//�T���v���X�e�[�g�̓e�N�X�`�����T���v�����O������@���w�肷��B
	CD3D11_DEFAULT defDesc;
	CD3D11_SAMPLER_DESC desc(defDesc);
	//
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		//U�����̓��b�v
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		//V�����̓��b�v
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;		//W�����̓��b�v(W������3�����e�N�X�`���̎��Ɏg�p�����B)
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	//�e�N�X�`���t�B���^�̓o�C���j�A�t�B���^
	//�T���v���X�e�[�g���쐬�B
	g_graphicsEngine->GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
}

void C3DModelDraw::InitConstantBuffer()
{
	//�쐬����o�b�t�@�̃T�C�Y��sizeof���Z�q�ŋ��߂�B
	int bufferSize = sizeof(SVSConstantBuffer);
	//�ǂ�ȃo�b�t�@���쐬����̂�������bufferDesc�ɐݒ肷��B
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));				//�O�ŃN���A�B
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//�o�b�t�@�őz�肳��Ă���A�ǂݍ��݂���я������ݕ��@�B
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//�o�b�t�@��16�o�C�g�A���C�����g�ɂȂ��Ă���K�v������B
																//�A���C�����g���ā��o�b�t�@�̃T�C�Y��16�̔{���Ƃ������Ƃł��B
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//�o�b�t�@���ǂ̂悤�ȃp�C�v���C���Ƀo�C���h���邩���w�肷��B
																//�萔�o�b�t�@�Ƀo�C���h����̂ŁAD3D11_BIND_CONSTANT_BUFFER���w�肷��B
	bufferDesc.CPUAccessFlags = 0;								//CPU �A�N�Z�X�̃t���O�ł��B
																//CPU�A�N�Z�X���s�v�ȏꍇ��0�B
	//�쐬�B
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_cb);

	//�����āA���C�g�p�̒萔�o�b�t�@���쐬�B
	//�쐬����o�b�t�@�̃T�C�Y��ύX���邾���B
	bufferDesc.ByteWidth = sizeof(SDirectionLight);				//SDirectionLight��16byte�̔{���ɂȂ��Ă���̂ŁA�؂�グ�͂��Ȃ��B
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_lightCb);
}
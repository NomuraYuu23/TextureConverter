#pragma once
#include <string>
#include "../externals/DirectXTex/DirectXTex.h"
class TextureConverter
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	TextureConverter();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~TextureConverter();

	/// <summary>
	/// �e�N�X�`����WIC����DDS�ɕϊ�
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	/// <param name="numOptions">�I�v�V�����̐�</param>
	/// <param name="options">�I�v�V�����z��</param>
	void ConverterTextureWICToDDS(const std::string& filePath
	, int numOptions = 0, char* options[] = nullptr);

private: 

	/// <summary>
	/// �e�N�X�`���t�@�C���ǂݍ���
	/// </summary>
	/// <param name="filePath"></param>
	void LoadWICTextureFromFile(const std::string& filePath);

	/// <summary>
	/// �t�@�C���p�X�ƃt�@�C�����𕪉�����
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	void SeparateFilePath(const std::wstring& filePath);

	/// <summary>
	/// DDS�e�N�X�`���Ƃ��ăt�@�C�������o��
	/// </summary>
	/// <param name="numOptions">�I�v�V�����̐�</param>
	/// <param name="opitions">�I�v�V�����z��</param>
	void SaveDDSTextureToFile(int numOptions, char* opitions[]);

public:
	
	/// <summary>
	/// �g�p���@���o��(�\��)����
	/// </summary>
	static void OutputUsage();

private:

	/// <summary>
	/// �}���`�o�C�g����������C�h������ɕϊ�
	/// </summary>
	/// <param name="str">�}���`�o�C�g������</param>
	/// <returns>���C�h������</returns>
	static std::wstring ConvertString(const std::string& str);

private:

	// �摜�̏��
	DirectX::TexMetadata metadata_;

	// �摜�C���[�W�̃R���e�i
	DirectX::ScratchImage scratchImage_;

	// �f�B���N�g���p�X
	std::wstring directoryPath_;

	// �t�@�C����
	std::wstring fileName_;

	// �t�@�C���g���q
	std::wstring fileExt_;

};


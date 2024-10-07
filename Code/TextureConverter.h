#pragma once
#include <string>
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
	void ConverterTextureWICToDDS(const std::string& filePath);

private: 

	/// <summary>
	/// �e�N�X�`���t�@�C���ǂݍ���
	/// </summary>
	/// <param name="filePath"></param>
	void LoadWICTextureFromFile(const std::string& filePath);

private:

	/// <summary>
	/// �}���`�o�C�g����������C�h������ɕϊ�
	/// </summary>
	/// <param name="str">�}���`�o�C�g������</param>
	/// <returns>���C�h������</returns>
	static std::wstring ConvertString(const std::string& str);

};


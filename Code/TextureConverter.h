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

};


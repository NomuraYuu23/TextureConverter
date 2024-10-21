#include "TextureConverter.h"
#include <Windows.h>

TextureConverter::TextureConverter()
{
}

TextureConverter::~TextureConverter()
{
}

void TextureConverter::ConverterTextureWICToDDS(const std::string& filePath)
{

	LoadWICTextureFromFile(filePath);

	SaveDDSTextureToFile();

}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath)
{

	// 1.�t�@�C���p�X�����C�h������ɕϊ�
	std::wstring wFilePath = ConvertString(filePath);

	// 2.�e�N�X�`����ǂݍ���
	HRESULT result = LoadFromWICFile(wFilePath.c_str(), DirectX::WIC_FLAGS_NONE, &metadata_, scratchImage_);
	assert(SUCCEEDED(result));

	// �t�H���_�p�X�ƃt�@�C�����𕪗�����
	SeparateFilePath(wFilePath);

}

void TextureConverter::SeparateFilePath(const std::wstring& filePath)
{

	size_t pos1;
	std::wstring excptExt;

	// ��؂蕶��'.'���o�Ă����ԍŌ�̕���������
	pos1 = filePath.rfind('.');
	// �������q�b�g������
	if (pos1 != std::wstring::npos) {
		// ��؂蕶���̌����t�@�C���g���q�Ƃ��ĕۑ�
		fileExt_ = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
		// ��؂蕶���̑O�܂ł𔲂��o��
		excptExt = filePath.substr(0, pos1);
	}
	else {
		fileExt_ = L"";
		excptExt = filePath;
	}

	// ��؂蕶�� '\\'���o�Ă����ԍŌ�̕���������
	pos1 = excptExt.rfind('\\');
	if (pos1 != std::wstring::npos) {
		// ��؂蕶���̑O�܂ł��f�B���N�g���p�X�Ƃ��ĕۑ�
		directoryPath_ = excptExt.substr(0, pos1 + 1);
		// ��؂蕶���̌����t�@�C�����Ƃ��ĕۑ�
		fileName_ = excptExt.substr(pos1 + 1, excptExt.size() - pos1 - 1);
		return;
	}

	// ��؂蕶�� '/'���o�Ă����ԍŌ�̕���������
	pos1 = excptExt.rfind('/');
	if (pos1 != std::wstring::npos) {
		// ��؂蕶���̑O�܂ł��f�B���N�g���p�X�Ƃ��ĕۑ�
		directoryPath_ = excptExt.substr(0, pos1 + 1);
		// ��؂蕶���̌����t�@�C�����Ƃ��ĕۑ�
		fileName_ = excptExt.substr(pos1 + 1, excptExt.size() - pos1 - 1);
		return;
	}

	// ��؂蕶�����Ȃ��̂Ńt�@�C�����݂̂Ƃ��Ĉ���
	directoryPath_ = L"";
	fileName_ = excptExt;

}

void TextureConverter::SaveDDSTextureToFile()
{

	// �ǂݍ��񂾃e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata_.format = DirectX::MakeSRGB(metadata_.format);

	HRESULT result;

	// �o�̓t�@�C������ݒ肷��
	std::wstring filePath = directoryPath_ + fileName_ + L".dds";

	// DDS�t�@�C�������o��
	result = DirectX::SaveToDDSFile(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(),
		metadata_, DirectX::DDS_FLAGS_NONE, filePath.c_str());

	assert(SUCCEEDED(result));

}

std::wstring TextureConverter::ConvertString(const std::string& str)
{
	if (str.empty()) {
		return std::wstring();
	}

	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
	if (sizeNeeded == 0) {
		return std::wstring();
	}
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
	return result;
}

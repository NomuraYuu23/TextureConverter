#include "TextureConverter.h"
#include <Windows.h>

TextureConverter::TextureConverter()
{
}

TextureConverter::~TextureConverter()
{
}

void TextureConverter::ConverterTextureWICToDDS(const std::string& filePath
	, int numOptions, char* options[])
{

	LoadWICTextureFromFile(filePath);

	SaveDDSTextureToFile(numOptions, options);

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

void TextureConverter::SaveDDSTextureToFile(int numOptions, char* opitions[])
{

	HRESULT result;

	// �I�v�V�����̒��o
	size_t mipLevel = 0;

	// �~�b�v�}�b�v���x���w�������
	for (int i = 0; i < numOptions; i++) {
		if (std::string(opitions[i]) == "-ml") {
			// �~�b�v���x���w��
			mipLevel = std::stoi(opitions[i + 1]);
			break;
		}
	}

	DirectX::ScratchImage mipChain;
	// �~�b�v�}�b�v����
	result = GenerateMipMaps(
		scratchImage_.GetImages(),
		scratchImage_.GetImageCount(),
		scratchImage_.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT, mipLevel, mipChain);

	if (SUCCEEDED(result)) {
		// �C���[�W�ƃ��^�f�[�^���A�~�b�v�}�b�v�łŒu��������
		scratchImage_ = std::move(mipChain);
		metadata_ = scratchImage_.GetMetadata();
	}

	// ���k�`���ɕϊ�
	DirectX::ScratchImage converted;
	result = Compress(
		scratchImage_.GetImages(),
		scratchImage_.GetImageCount(),
		metadata_,
		DXGI_FORMAT_BC7_UNORM_SRGB,
		DirectX::TEX_COMPRESS_FLAGS::TEX_COMPRESS_BC7_QUICK |
		DirectX::TEX_COMPRESS_FLAGS::TEX_COMPRESS_SRGB_OUT |
		DirectX::TEX_COMPRESS_FLAGS::TEX_COMPRESS_PARALLEL,
		1.0f, converted);
	if (SUCCEEDED(result)) {
		// �C���[�W�ƃ��^�f�[�^���A�~�b�v�}�b�v�łŒu��������
		scratchImage_ = std::move(converted);
		metadata_ = scratchImage_.GetMetadata();
	}

	// �ǂݍ��񂾃e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata_.format = DirectX::MakeSRGB(metadata_.format);

	// �o�̓t�@�C������ݒ肷��
	std::wstring filePath = directoryPath_ + fileName_ + L".dds";

	// DDS�t�@�C�������o��
	result = DirectX::SaveToDDSFile(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(),
		metadata_, DirectX::DDS_FLAGS_NONE, filePath.c_str());

	assert(SUCCEEDED(result));

}

void TextureConverter::OutputUsage()
{

	printf("�摜�t�@�C����WIC�`������DDS�`���ɕϊ����܂��B\n");
	printf("\n");
	printf("TextureConverter [�h���C�u:][�p�X][�t�@�C����][-ml level]\n");
	printf("\n");
	printf("[�h���C�u:][�p�X][�t�@�C����]: �ϊ�������WIC�`���̉摜�t�@�C�����w�肵�܂��B\n");
	printf("\n");
	printf("[-ml level]: �~�b�v���x�����w�肵�܂��B0���w�肷���1x1�܂ł̃t���~�b�v�}�b�v�`�F�[���𐶐����܂��B\n");

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

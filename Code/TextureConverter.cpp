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

	// 1.ファイルパスをワイド文字列に変換
	std::wstring wFilePath = ConvertString(filePath);

	// 2.テクスチャを読み込む
	HRESULT result = LoadFromWICFile(wFilePath.c_str(), DirectX::WIC_FLAGS_NONE, &metadata_, scratchImage_);
	assert(SUCCEEDED(result));

	// フォルダパスとファイル名を分離する
	SeparateFilePath(wFilePath);

}

void TextureConverter::SeparateFilePath(const std::wstring& filePath)
{

	size_t pos1;
	std::wstring excptExt;

	// 区切り文字'.'が出てくる一番最後の部分を検索
	pos1 = filePath.rfind('.');
	// 検索がヒットしたら
	if (pos1 != std::wstring::npos) {
		// 区切り文字の後ろをファイル拡張子として保存
		fileExt_ = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
		// 区切り文字の前までを抜き出す
		excptExt = filePath.substr(0, pos1);
	}
	else {
		fileExt_ = L"";
		excptExt = filePath;
	}

	// 区切り文字 '\\'が出てくる一番最後の部分を検索
	pos1 = excptExt.rfind('\\');
	if (pos1 != std::wstring::npos) {
		// 区切り文字の前までをディレクトリパスとして保存
		directoryPath_ = excptExt.substr(0, pos1 + 1);
		// 区切り文字の後ろをファイル名として保存
		fileName_ = excptExt.substr(pos1 + 1, excptExt.size() - pos1 - 1);
		return;
	}

	// 区切り文字 '/'が出てくる一番最後の部分を検索
	pos1 = excptExt.rfind('/');
	if (pos1 != std::wstring::npos) {
		// 区切り文字の前までをディレクトリパスとして保存
		directoryPath_ = excptExt.substr(0, pos1 + 1);
		// 区切り文字の後ろをファイル名として保存
		fileName_ = excptExt.substr(pos1 + 1, excptExt.size() - pos1 - 1);
		return;
	}

	// 区切り文字がないのでファイル名のみとして扱う
	directoryPath_ = L"";
	fileName_ = excptExt;

}

void TextureConverter::SaveDDSTextureToFile(int numOptions, char* opitions[])
{

	HRESULT result;

	// オプションの抽出
	size_t mipLevel = 0;

	// ミップマップレベル指定を検索
	for (int i = 0; i < numOptions; i++) {
		if (std::string(opitions[i]) == "-ml") {
			// ミップレベル指定
			mipLevel = std::stoi(opitions[i + 1]);
			break;
		}
	}

	DirectX::ScratchImage mipChain;
	// ミップマップ生成
	result = GenerateMipMaps(
		scratchImage_.GetImages(),
		scratchImage_.GetImageCount(),
		scratchImage_.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT, mipLevel, mipChain);

	if (SUCCEEDED(result)) {
		// イメージとメタデータを、ミップマップ版で置き換える
		scratchImage_ = std::move(mipChain);
		metadata_ = scratchImage_.GetMetadata();
	}

	// 圧縮形式に変換
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
		// イメージとメタデータを、ミップマップ版で置き換える
		scratchImage_ = std::move(converted);
		metadata_ = scratchImage_.GetMetadata();
	}

	// 読み込んだテクスチャをSRGBとして扱う
	metadata_.format = DirectX::MakeSRGB(metadata_.format);

	// 出力ファイル名を設定する
	std::wstring filePath = directoryPath_ + fileName_ + L".dds";

	// DDSファイル書き出し
	result = DirectX::SaveToDDSFile(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(),
		metadata_, DirectX::DDS_FLAGS_NONE, filePath.c_str());

	assert(SUCCEEDED(result));

}

void TextureConverter::OutputUsage()
{

	printf("画像ファイルをWIC形式からDDS形式に変換します。\n");
	printf("\n");
	printf("TextureConverter [ドライブ:][パス][ファイル名][-ml level]\n");
	printf("\n");
	printf("[ドライブ:][パス][ファイル名]: 変換したいWIC形式の画像ファイルを指定します。\n");
	printf("\n");
	printf("[-ml level]: ミップレベルを指定します。0を指定すると1x1までのフルミップマップチェーンを生成します。\n");

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

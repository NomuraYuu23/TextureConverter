#pragma once
#include <string>
class TextureConverter
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TextureConverter();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TextureConverter();

	/// <summary>
	/// テクスチャをWICからDDSに変換
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void ConverterTextureWICToDDS(const std::string& filePath);

};


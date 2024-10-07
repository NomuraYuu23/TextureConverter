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

private: 

	/// <summary>
	/// テクスチャファイル読み込み
	/// </summary>
	/// <param name="filePath"></param>
	void LoadWICTextureFromFile(const std::string& filePath);

private:

	/// <summary>
	/// マルチバイト文字列をワイド文字列に変換
	/// </summary>
	/// <param name="str">マルチバイト文字列</param>
	/// <returns>ワイド文字列</returns>
	static std::wstring ConvertString(const std::string& str);

};


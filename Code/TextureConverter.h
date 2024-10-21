#pragma once
#include <string>
#include "../externals/DirectXTex/DirectXTex.h"
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

	/// <summary>
	/// ファイルパスとファイル名を分解する
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void SeparateFilePath(const std::wstring& filePath);

	/// <summary>
	/// DDSテクスチャとしてファイル書き出し
	/// </summary>
	void SaveDDSTextureToFile();

private:

	/// <summary>
	/// マルチバイト文字列をワイド文字列に変換
	/// </summary>
	/// <param name="str">マルチバイト文字列</param>
	/// <returns>ワイド文字列</returns>
	static std::wstring ConvertString(const std::string& str);

private:

	// 画像の情報
	DirectX::TexMetadata metadata_;

	// 画像イメージのコンテナ
	DirectX::ScratchImage scratchImage_;

	// ディレクトリパス
	std::wstring directoryPath_;

	// ファイル名
	std::wstring fileName_;

	// ファイル拡張子
	std::wstring fileExt_;

};


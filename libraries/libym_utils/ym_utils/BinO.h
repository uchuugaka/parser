#ifndef YM_UTILS_BINO_H
#define YM_UTILS_BINO_H

/// @file ym_utils/BinO.h
/// @brief BinO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BinO BinO.h "ym_utils/BinO.h"
/// @ingroup YmUtils
/// @brief バイナリ出力ストリームの基底クラス
//////////////////////////////////////////////////////////////////////
class BinO
{
public:

  /// @brief コンストラクタ
  BinO() { }

  /// @brief デストラクタ
  virtual
  ~BinO() { }


public:
  //////////////////////////////////////////////////////////////////////
  // BinO の公開関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 1バイトの書き込み
  /// @param[in] val 値
  void
  write_8(ymuint8 val);

  /// @brief 2バイトの書き込み
  /// @param[in] val 値
  void
  write_16(ymuint16 val);

  /// @brief 4バイトの書き込み
  /// @param[in] val 値
  void
  write_32(ymuint32 val);

  /// @brief 8バイトの書き込み
  /// @param[in] val 値
  void
  write_64(ymuint64 val);

  /// @brief 単精度浮動小数点数の書き込み
  /// @param[in] val 値
  void
  write_float(float val);

  /// @brief 倍精度浮動小数点数の書き込み
  /// @param[in] val 値
  void
  write_double(double val);

  /// @brief 文字列の書き込み
  /// @param[in] val 値
  void
  write_str(const char* val);

  /// @brief 文字列の書き込み
  /// @param[in] val 値
  void
  write_str(const string& val);


public:
  //////////////////////////////////////////////////////////////////////
  // BinO の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief nバイトのデータを書き出す．
  /// @param[in] n データサイズ
  /// @param[in] buff データを収めた領域のアドレス
  /// @return 実際に書き出した量を返す．
  virtual
  ymuint
  write(ymuint64 n,
	const ymuint8* buff) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class FileBinO BinO.h "ym_utils/BinO.h"
/// @ingroup YmUtils
/// @brief 通常のファイルを用いた BinO の継承クラス
//////////////////////////////////////////////////////////////////////
class FileBinO :
  public BinO
{
public:

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  FileBinO(const char* filename);

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  FileBinO(const string& filename);

  /// @brief デストラクタ
  virtual
  ~FileBinO();


public:
  //////////////////////////////////////////////////////////////////////
  // FileBinO の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 書き込み可能なら true を返す．
  bool
  ok() const;

  /// @brief ファイルを閉じる．
  /// @note 以降の書き込みは行われない．
  void
  close();


public:
  //////////////////////////////////////////////////////////////////////
  // BinO の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを書き出す．
  /// @param[in] n データサイズ
  /// @param[in] buff データを収めた領域のアドレス
  /// @return 実際に書き出した量を返す．
  virtual
  ymuint
  write(ymuint64 n,
	const ymuint8* buff);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる定数
  //////////////////////////////////////////////////////////////////////

  static
  const ymuint BUFF_SIZE = 4096;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイルディスクリプタ
  int mFd;

  // バッファ
  ymuint8 mBuff[BUFF_SIZE];

  // バッファ上の書き込み位置
  ymuint16 mPos;

};


//////////////////////////////////////////////////////////////////////
/// @class BinOStream BinO.h "ym_utils/BinO.h"
/// @ingroup YmUtils
/// @brief ostream を用いた BinO の継承クラス
//////////////////////////////////////////////////////////////////////
class BinOStream :
  public BinO
{
public:

  /// @brief コンストラクタ
  /// @param[in] s 出力先のストリーム
  BinOStream(ostream& s);

  /// @brief デストラクタ
  virtual
  ~BinOStream();


public:
  //////////////////////////////////////////////////////////////////////
  // BinO の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを書き出す．
  /// @param[in] n データサイズ
  /// @param[in] buff データを収めた領域のアドレス
  /// @return 実際に書き出した量を返す．
  virtual
  ymuint
  write(ymuint64 n,
	const ymuint8* buff);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力先のストリーム
  ostream& mS;

};


//////////////////////////////////////////////////////////////////////
// BinO に対するストリーム出力演算子
//////////////////////////////////////////////////////////////////////

/// @brief ブール値の書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return BinO を返す．
BinO&
operator<<(BinO& s,
	   bool val);

/// @brief 1バイトの書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return BinO を返す．
BinO&
operator<<(BinO& s,
	   ymuint8 val);

/// @brief 2バイトの書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return BinO を返す．
BinO&
operator<<(BinO& s,
	   ymuint16 val);

/// @brief 4バイトの書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return BinO を返す．
BinO&
operator<<(BinO& s,
	   ymuint32 val);

/// @brief 8バイトの書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return BinO を返す．
BinO&
operator<<(BinO& s,
	   ymuint64 val);

/// @brief 単精度浮動小数点数の書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return BinO を返す．
BinO&
operator<<(BinO& s,
	   float val);

/// @brief 倍精度浮動小数点数の書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return BinO を返す．
BinO&
operator<<(BinO& s,
	   double val);

/// @brief 文字列の書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return BinO を返す．
BinO&
operator<<(BinO& s,
	   const char* val);

/// @brief 文字列の書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return BinO を返す．
BinO&
operator<<(BinO& s,
	   const string& val);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 文字列の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
inline
void
BinO::write_str(const string& val)
{
  write_str(val.c_str());
}

// @brief ブール値の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return BinO を返す．
inline
BinO&
operator<<(BinO& s,
	   bool val)
{
  s.write_8(val);
  return s;
}

// @brief 1バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return BinO を返す．
inline
BinO&
operator<<(BinO& s,
	   ymuint8 val)
{
  s.write_8(val);
  return s;
}

// @brief 2バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return BinO を返す．
inline
BinO&
operator<<(BinO& s,
	   ymuint16 val)
{
  s.write_16(val);
  return s;
}

// @brief 4バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return BinO を返す．
inline
BinO&
operator<<(BinO& s,
	   ymuint32 val)
{
  s.write_32(val);
  return s;
}

// @brief 8バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return BinO を返す．
inline
BinO&
operator<<(BinO& s,
	   ymuint64 val)
{
  s.write_64(val);
  return s;
}

// @brief 単精度浮動小数点数の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return BinO を返す．
inline
BinO&
operator<<(BinO& s,
	   float val)
{
  s.write_float(val);
  return s;
}

// @brief 倍精度浮動小数点数の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return BinO を返す．
inline
BinO&
operator<<(BinO& s,
	   double val)
{
  s.write_double(val);
  return s;
}

// @brief 文字列の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return BinO を返す．
inline
BinO&
operator<<(BinO& s,
	   const char* val)
{
  s.write_str(val);
  return s;
}

// @brief 文字列の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return BinO を返す．
inline
BinO&
operator<<(BinO& s,
	   const string& val)
{
  s.write_str(val.c_str());
  return s;
}

END_NAMESPACE_YM

#endif // YM_UTILS_BINO_H

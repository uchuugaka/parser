
/// @file IDO.cc
/// @brief IDO の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/IDO.h"
#include "ym_utils/FileIDO.h"
#include "ym_utils/StreamIDO.h"
#include "ym_utils/CompIDO.h"

#include "ym_utils/ODO.h"
#include "ym_utils/FileODO.h"
#include "ym_utils/StreamODO.h"

#include "ym_utils/MsgMgr.h"
#include "ym_utils/FileRegion.h"

#include "FileBuff.h"
#include "ZState.h"

#include <fcntl.h>


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// 作業用のバッファ
ymuint8 buff[8];

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス IDO
//////////////////////////////////////////////////////////////////////

// @brief read() を呼び出して結果をチェックする．
void
IDO::_read(ymuint8* buff,
	   ymuint64 n)
{
  ssize_t ret = read(buff, n);
  if ( static_cast<ymuint64>(ret) != n ) {
    ostringstream buf;
    buf << "IDO::read(" << n << ") failed. read " << ret << " bytes.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion(),
		    kMsgFailure,
		    "IDO",
		    buf.str());
  }
}

// @brief 1バイトの読み出し
// @param[in] s 入力元のストリーム
ymuint8
IDO::read_8()
{
  _read(buff, 1);
  return buff[0];
}

// @brief 2バイトの読み出し
// @param[in] s 入力元のストリーム
ymuint16
IDO::read_16()
{
  _read(buff, 2);
  ymuint16 val =
    (static_cast<ymuint16>(buff[0]) <<  0) |
    (static_cast<ymuint16>(buff[1]) <<  8);
  return val;
}

// @brief 4バイトの読み出し
// @param[in] s 入力元のストリーム
ymuint32
IDO::read_32()
{
  _read(buff, 4);
  ymuint32 val =
    (static_cast<ymuint32>(buff[0]) <<  0) |
    (static_cast<ymuint32>(buff[1]) <<  8) |
    (static_cast<ymuint32>(buff[2]) << 16) |
    (static_cast<ymuint32>(buff[3]) << 24);
  return val;
}

// @brief 8バイトの読み出し
// @param[in] s 入力元のストリーム
ymuint64
IDO::read_64()
{
  _read(buff, 8);
  ymuint64 val =
    (static_cast<ymuint64>(buff[0]) <<  0) |
    (static_cast<ymuint64>(buff[1]) <<  8) |
    (static_cast<ymuint64>(buff[2]) << 16) |
    (static_cast<ymuint64>(buff[3]) << 24) |
    (static_cast<ymuint64>(buff[4]) << 32) |
    (static_cast<ymuint64>(buff[5]) << 40) |
    (static_cast<ymuint64>(buff[6]) << 48) |
    (static_cast<ymuint64>(buff[7]) << 56);
  return val;
}

// @brief 単精度不動週数点数の読み出し
// @param[in] s 入力元のストリーム
float
IDO::read_float()
{
  // かなり強引
  _read(buff, sizeof(float));
  return *(reinterpret_cast<float*>(buff));
}

// @brief 倍精度不動週数点数の読み出し
// @param[in] s 入力元のストリーム
double
IDO::read_double()
{
  // かなり強引
  _read(buff, sizeof(double));
  return *(reinterpret_cast<double*>(buff));
}

// @brief 文字列の読み出し
// @param[in] s 入力元のストリーム
string
IDO::read_str()
{
  ymuint64 l = read_64();
  if ( l > 0 ) {
    ymuint8* strbuf = new ymuint8[l + 1];
    _read(strbuf, l);
    strbuf[l] = '\0';
    string ans(reinterpret_cast<char*>(strbuf));
    delete [] strbuf;
    return ans;
  }
  else {
    return string();
  }
}


//////////////////////////////////////////////////////////////////////
// クラス FileIDO
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
FileIDO::FileIDO(ymuint buff_size)
{
  mFileBuff = new FileBuff(buff_size);
}

// @brief コンストラクタ
// @param[in] filename ファイル名
FileIDO::FileIDO(const char* filename,
		 ymuint buff_size)
{
  mFileBuff = new FileBuff(buff_size);
  open(filename);
}

// @brief コンストラクタ
// @param[in] filename ファイル名
FileIDO::FileIDO(const string& filename,
		 ymuint buff_size)
{
  mFileBuff = new FileBuff(buff_size);
  open(filename);
}

// @brief デストラクタ
FileIDO::~FileIDO()
{
  close();
  delete mFileBuff;
}

// @brief 読み出し可能なら true を返す．
FileIDO::operator bool() const
{
  return mFileBuff->is_ready();
}

// @brief ファイルを開く
// @param[in] filename ファイル名
bool
FileIDO::open(const char* filename)
{
  return mFileBuff->open(filename, O_RDONLY);
}

// @brief ファイルを開く
// @param[in] filename ファイル名
bool
FileIDO::open(const string& filename)
{
  return open(filename.c_str());
}

// @brief ファイルを閉じる．
// @note 以降の読み出しは行われない．
void
FileIDO::close()
{
  mFileBuff->close();
}

// @brief データを読み込む．
// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
// @param[in] n 読み込むデータサイズ
// @return 実際に読み込んだ量を返す．
ssize_t
FileIDO::read(ymuint8* buff,
	      ymuint64 n)
{
  return mFileBuff->read(buff, n);
}


//////////////////////////////////////////////////////////////////////
// クラス CompIDO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CompIDO::CompIDO()
{
  mZ = new ZStateR();
}

// @brief コンストラクタ
// @param[in] filename ファイル名
// @note 意味的にはコンストラクタ + open()
CompIDO::CompIDO(const char* filename)
{
  mZ = new ZStateR();
  open(filename);
}

// @brief コンストラクタ
// @param[in] filename ファイル名
// @note 意味的にはコンストラクタ + open()
CompIDO::CompIDO(const string& filename)
{
  mZ = new ZStateR();
  open(filename);
}

// @brief デストラクタ
CompIDO::~CompIDO()
{
  delete mZ;
}

// @brief 読み出し可能なら true を返す．
CompIDO::operator bool() const
{
  return mZ->is_ready();
}

// @brief ファイルをオープンする．
// @param[in] filename ファイル名
// @retval true オープンが成功した．
// @retval false オープンが失敗した．
bool
CompIDO::open(const char* filename)
{
  return mZ->open(filename, O_RDONLY, 0);
}

// @brief ファイルをオープンする．
// @param[in] filename ファイル名
// @retval true オープンが成功した．
// @retval false オープンが失敗した．
bool
CompIDO::open(const string& filename)
{
  return open(filename.c_str());
}

// @brief ファイルをクローズする．
void
CompIDO::close()
{
  return mZ->close();
}

// @brief 圧縮されたデータを伸長してバッファに書き込む．
// @param[in] buff 伸長したデータを格納するバッファ
// @param[in] size バッファの空きサイズ
// @return バッファに書き込まれたデータサイズを返す．
// @note エラーが起こった場合には負の値が返される．
ssize_t
CompIDO::read(ymuint8* buff,
	      size_t size)
{
  return mZ->read(buff, size);
}


//////////////////////////////////////////////////////////////////////
// クラス StreamIDO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 入力ストリーム
StreamIDO::StreamIDO(istream& s) :
  mS(s)
{
}

// @brief デストラクタ
StreamIDO::~StreamIDO()
{
}

// @brief データを読み込む．
// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
// @param[in] n 読み込むデータサイズ
// @return 実際に読み込んだ量を返す．
ssize_t
StreamIDO::read(ymuint8* buff,
		ymuint64 n)
{
  mS.read(reinterpret_cast<char*>(buff), n);
  #warning"TODO: istream::read() の仕様をチェック"
  return n;
}


//////////////////////////////////////////////////////////////////////
// クラス ODO
//////////////////////////////////////////////////////////////////////

// @brief write() を読み出して結果をチェックする．
inline
void
ODO::_write(const ymuint8* buff,
	    ymuint64 n)
{
  ssize_t ret = write(buff, n);
  if ( static_cast<ymuint64>(ret) != n ) {
    ostringstream buf;
    buf << "ODO::write(" << n << ") failed. wrote " << ret << " bytes.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion(),
		    kMsgFailure,
		    "ODO",
		    buf.str());
  }
}

// @brief 1バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
ODO::write_8(ymuint8 val)
{
  buff[0] = val & 255U;
  _write(buff, 1);
}

// @brief 2バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
ODO::write_16(ymuint16 val)
{
  buff[0] = val & 255U; val >>= 8;
  buff[1] = val & 255U;
  _write(buff, 2);
}

// @brief 4バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
ODO::write_32(ymuint32 val)
{
  buff[0] = val & 255U; val >>= 8;
  buff[1] = val & 255U; val >>= 8;
  buff[2] = val & 255U; val >>= 8;
  buff[3] = val & 255U;
  _write(buff, 4);
}

// @brief 8バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
ODO::write_64(ymuint64 val)
{
  buff[0] = val & 255U; val >>= 8;
  buff[1] = val & 255U; val >>= 8;
  buff[2] = val & 255U; val >>= 8;
  buff[3] = val & 255U; val >>= 8;
  buff[4] = val & 255U; val >>= 8;
  buff[5] = val & 255U; val >>= 8;
  buff[6] = val & 255U; val >>= 8;
  buff[7] = val & 255U;
  _write(buff, 8);
}

// @brief 単精度浮動小数点数の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
ODO::write_float(float val)
{
  // かなり強引
  *(reinterpret_cast<float*>(buff)) = val;
  _write(buff, sizeof(float));
}

// @brief 倍精度浮動小数点数の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
ODO::write_double(double val)
{
  // かなり強引
  *(reinterpret_cast<double*>(buff)) = val;
  _write(buff, sizeof(double));
}

// @brief 文字列の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
ODO::write_str(const char* val)
{
  if ( val ) {
    ymuint64 l = strlen(val);
    write_64(l);
    _write(reinterpret_cast<const ymuint8*>(val), l);
  }
  else {
    write_64(0);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス FileODO
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @param[in] buff_size バッファサイズ
FileODO::FileODO(ymuint buff_size)
{
  mFileBuff = new FileBuff(buff_size);
}

// @brief コンストラクタ
// @param[in] filename ファイル名
// @param[in] buff_size バッファサイズ
FileODO::FileODO(const char* filename,
		 ymuint buff_size)
{
  mFileBuff = new FileBuff(buff_size);
  open(filename);
}

// @brief コンストラクタ
// @param[in] filename ファイル名
// @param[in] buff_size バッファサイズ
FileODO::FileODO(const string& filename,
		 ymuint buff_size)
{
  mFileBuff = new FileBuff(buff_size);
  open(filename);
}

// @brief デストラクタ
FileODO::~FileODO()
{
  close();
  delete mFileBuff;
}

// @brief 書き込み可能なら true を返す．
FileODO::operator bool() const
{
  return mFileBuff->is_ready();
}

// @brief ファイルを開く
// @param[in] filename ファイル名
void
FileODO::open(const char* filename)
{
  close();
  mFileBuff->open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
}

// @brief ファイルを開く
// @param[in] filename ファイル名
void
FileODO::open(const string& filename)
{
  open(filename.c_str());
}

// @brief ファイルを閉じる．
// @note 以降の書き込みは行われない．
void
FileODO::close()
{
  mFileBuff->close();
}

// @brief データを書き出す．
// @param[in] buff データを収めた領域のアドレス
// @param[in] n データサイズ
// @return 実際に書き出した量を返す．
ssize_t
FileODO::write(const ymuint8* buff,
	       ymuint64 n)
{
  return mFileBuff->write(buff, n);
}


//////////////////////////////////////////////////////////////////////
// クラス StreamODO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 出力先のストリーム
StreamODO::StreamODO(ostream& s) :
  mS(s)
{
}

// @brief デストラクタ
StreamODO::~StreamODO()
{
}

// @brief データを書き出す．
// @param[in] buff データを収めた領域のアドレス
// @param[in] n データサイズ
// @return 実際に書き出した量を返す．
ssize_t
StreamODO::write(const ymuint8* buff,
		 ymuint64 n)
{
  mS.write(reinterpret_cast<const char*>(buff), n);
  return n;
}

END_NAMESPACE_YM

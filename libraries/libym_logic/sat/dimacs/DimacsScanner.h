#ifndef LIBYM_LOGIC_SAT_DIMACS_DIMACSSCANNER_H
#define LIBYM_LOGIC_SAT_DIMACS_DIMACSSCANNER_H

/// @file libym_logic/sat/dimacs/DimacsScanner.h
/// @brief DimacsScanner のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DimacsScanner.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/sat_nsdef.h"
#include "ym_utils/FileScanner.h"


BEGIN_NAMESPACE_YM_SAT

/// @brief トークンの型定義
enum tToken {
  kC,
  kP,
  kNUM,
  kZERO,
  kNL,
  kEOF,
  kERR
};

//////////////////////////////////////////////////////////////////////
/// @class DimacsScanner DimacsScanner.h "DimacsScanner.h"
/// @ingroup SatGroup
/// @brief DIMACS形式の CNFファイルを読み込む字句解析器
//////////////////////////////////////////////////////////////////////
class DimacsScanner :
  public FileScanner
{
public:

  /// @brief コンストラクタ
  DimacsScanner();

  /// @brief デストラクタ
  ~DimacsScanner();


public:

  /// @brief トークンの読込み
  /// @param[out] loc 対応するファイル上の位置情報を格納する変数
  tToken
  read_token(FileRegion& loc);

  /// @brief 直前の read_token() が kNUM だった場合の値を返す．
  int
  cur_val();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief read_token() の下請け関数
  /// @return トークンを返す．
  tToken
  scan();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // read_token で読んだトークンが kNUM の場合の値
  int mCurVal;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 直前の read_token() が kNUM だった場合の値を返す．
inline
int
DimacsScanner::cur_val()
{
  return mCurVal;
}

END_NAMESPACE_YM_SAT

#endif // LIBYM_LOGIC_SAT_DIMACS_DIMACSSCANNER_H
#ifndef LUT444MATCH_H
#define LUT444MATCH_H

/// @file Lut444Match.h
/// @brief Lut444Match のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "RcfNetwork.h"
#include "logic/TvFunc.h"
#include "GbmSolver.h"
#include "LnMatch.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class Lut444Match Lut444Match.h "Lut444Match.h"
/// @brief 4-4-4 の LUT ネットワークに対するマッチングを行うクラス
//////////////////////////////////////////////////////////////////////
class Lut444Match
{
public:

  /// @brief コンストラクタ
  Lut444Match();

  /// @brief デストラクタ
  ~Lut444Match();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 与えられた関数を LUT ネットワークで実現できるか調べる．
  /// @param[in] func 対象の関数
  /// @param[in] solver GBM ソルバ
  bool
  match(const TvFunc& func,
	GbmSolver& solver);

  /// @brief カウンタをリセットする．
  void
  reset_count();

  /// @brief カウンタの値を得る．
  void
  get_count(ymuint& trivial_num,
	    ymuint& a0_num,
	    ymuint& a1_num,
	    ymuint& a2_num,
	    ymuint& a3_num,
	    ymuint& a4_num,
	    ymuint& b0_num,
	    ymuint& b1_num,
	    ymuint& b2_num,
	    ymuint& b3_num,
	    ymuint& c0_num,
	    ymuint& c1_num,
	    ymuint& c2_num,
	    ymuint& d0_num,
	    ymuint& d1_num,
	    ymuint& d2_num,
	    ymuint& e0_num,
	    ymuint& e1_num,
	    ymuint& e2_num,
	    ymuint& f0_num,
	    ymuint& f1_num,
	    ymuint& g0_num,
	    ymuint& fail_num);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  void
  make_lut444(ymuint input_num,
	      ymuint iorder[],
	      ymuint sym_list[]);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  LnMatch mMatcher;

};

END_NAMESPACE_YM

#endif // LUT444MATCH_H
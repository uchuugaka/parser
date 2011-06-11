#ifndef LIBYM_SAT_YMSAT_YMSATR_H
#define LIBYM_SAT_YMSAT_YMSATR_H

/// @file libym_sat/YmSatR.h
/// @brief YmSatR のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: YmSatR.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmSat.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class YmSatR YmSatR.h "YmSatR.h"
/// @brief ログ記録機能付きの YmSat
//////////////////////////////////////////////////////////////////////
class YmSatR :
  public YmSat
{
public:

  /// @brief コンストラクタ
  /// @param[in] out 出力ストリーム
  /// @param[in] analyzer 解析器のポインタ
  YmSatR(ostream& out,
	 SatAnalyzer* analyer);

  /// @brief デストラクタ
  virtual
  ~YmSatR();


public:
  //////////////////////////////////////////////////////////////////////
  // SatSolver で定義されている仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数を追加する．
  /// @return 新しい変数番号を返す．
  /// @note 変数番号は 0 から始まる．
  virtual
  tVarId
  new_var();

  /// @brief 節を追加する．
  /// @param[in] lits リテラルのベクタ
  virtual
  void
  add_clause(const vector<Literal>& lits);

  /// @brief SAT 問題を解く．
  /// @param[in] assumptions あらかじめ仮定する変数の値割り当てリスト
  /// @param[out] model 充足するときの値の割り当てを格納する配列．
  /// @retval kB3True 充足した．
  /// @retval kB3False 充足不能が判明した．
  /// @retval kB3X わからなかった．
  /// @note i 番めの変数の割り当て結果は model[i] に入る．
  virtual
  Bool3
  solve(const vector<Literal>& assumptions,
	vector<Bool3>& model);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ログの出力先のストリーム
  ostream& mOut;

};

END_NAMESPACE_YM_SAT

#endif // LIBYM_SAT_YMSAT_YMSATR_H

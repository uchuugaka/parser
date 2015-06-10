﻿#ifndef DTPGSATS2_H
#define DTPGSATS2_H

/// @file DtpgSatS2.h
/// @brief DtpgSatS2 のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatBaseS.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgSatS2 DtpgSatS2.h "DtpgSatS2.h"
/// @brief 1つの故障を対象とした CNF を生成する SatEngine
//////////////////////////////////////////////////////////////////////
class DtpgSatS2 :
  public DtpgSatBaseS
{
public:

  /// @brief コンストラクタ
  /// @param[in] th_val しきい値
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  DtpgSatS2(ymuint th_val,
	    const string& sat_type,
	    const string& sat_option,
	    ostream* sat_outp,
	    BackTracer& bt,
	    DetectOp& dop,
	    UntestOp& uop);

  /// @brief デストラクタ
  virtual
  ~DtpgSatS2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト生成を行なう．
  /// @param[in] node_set 対象のノード集合
  /// @param[in] f_tgt 対象の故障
  virtual
  void
  run_single(const NodeSet& node_set,
	     TpgFault* f_tgt);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // しきい値
  ymuint32 mThVal;

  // 処理済みのノードのマーク
  vector<ymuint> mMark;

};

END_NAMESPACE_YM_SATPG

#endif // DTPGSATS2_H

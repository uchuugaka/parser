﻿#ifndef RTPG_H
#define RTPG_H

/// @file Rtpg.h
/// @brief Rtpg のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "ym_YmUtils/USTime.h"
#include "ym_YmUtils/RandGen.h"


BEGIN_NAMESPACE_YM_SATPG

class AtpgMgr;

//////////////////////////////////////////////////////////////////////
/// @class RtpgStats Rtpg.h "Rtpg.h"
/// @brief RTPG の statistics data を表すクラス
//////////////////////////////////////////////////////////////////////
class RtpgStats
{
  friend class Rtpg;

public:

  /// @brief コンストラクタ
  RtpgStats();

  /// @brief デストラクタ
  ~RtpgStats();

public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 検出した故障数を得る．
  ymuint
  detected_faults() const;

  /// @brief シミュレーションしたパタン数を得る．
  ymuint
  simulated_patterns() const;

  /// @brief 有効なパタン数を得る．
  ymuint
  effective_patterns() const;

  /// @brief 計算時間を得る．
  USTime
  time() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 検出した故障数
  ymuint32 mDetectNum;

  // 総パタン数
  ymuint32 mPatNum;

  // 有効パタン数
  ymuint32 mEfPatNum;

  // 計算時間
  USTime mTime;

};


//////////////////////////////////////////////////////////////////////
/// @class Rtpg Rtpg.h "Rtpg.h"
/// @brief random test pattern generation を行うクラス
//////////////////////////////////////////////////////////////////////
class Rtpg
{
public:

  /// @brief コンストラクタ
  Rtpg(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~Rtpg();


public:

  /// @brief ランダムパタンを用いた故障シミュレーションを行う．
  /// @param[in] min_f 1回のシミュレーションで検出する故障数の下限
  /// @param[in] max_i 故障検出できないシミュレーション回数の上限
  /// @param[in] max_pat 最大のパタン数
  void
  operator()(ymuint min_f,
	     ymuint max_i,
	     ymuint max_pat);

  /// @brief 直前の実行結果を得る．
  const RtpgStats&
  stats() const;

  /// @brief パタン生成用の乱数発生器を取り出す．
  RandGen&
  randgen();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // AtpgMgr
  AtpgMgr* mAtpgMgr;

  // パタン生成用の乱数生成器
  RandGen mPatGen;

  // 実行結果
  RtpgStats mStats;

};

END_NAMESPACE_YM_SATPG

#endif // RTPG_H

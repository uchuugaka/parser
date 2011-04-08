#ifndef YM_MVN_MVN2SBJ_H
#define YM_MVN_MVN2SBJ_H

/// @file ym_mvn/Mvn2Sbj.h
/// @brief Mvn2Sbj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"
#include "ym_sbj/sbj_nsdef.h"


BEGIN_NAMESPACE_YM_MVN

class MvnConv;

//////////////////////////////////////////////////////////////////////
/// @class Mvn2Sbj Mvn2Sbj.h "Mvn2Sbj.h"
/// @brief Mvn を SbjGraph に変換するクラス
//////////////////////////////////////////////////////////////////////
class Mvn2Sbj
{
public:

  /// @brief コンストラクタ
  Mvn2Sbj();

  /// @brief デストラクタ
  ~Mvn2Sbj();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief MvnMgr の内容を SbjGraph に変換する．
  /// @param[in] mvmgr 対象の MvNetwork
  /// @param[out] sbjgraph 変換先のサブジェクトグラフ
  /// @param[out] mvnode_map 対応関係を格納するオブジェクト
  void
  operator()(const MvnMgr& mvmgr,
	     SbjGraph& sbjgraph,
	     MvnNodeMap& mvnode_map);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // MvNode の変換関数のリスト
  list<MvnConv*> mConvList;

};

END_NAMESPACE_YM_MVN

#endif // YM_MVN_MVN2SBJ_H

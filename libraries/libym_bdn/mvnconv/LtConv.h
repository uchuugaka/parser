#ifndef LIBYM_BDN_MVNCONV_LTCONV_H
#define LIBYM_BDN_MVNCONV_LTCONV_H

/// @file libym_bdn/mvnconv/LtConv.h
/// @brief LtConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MvnConv.h"


BEGIN_NAMESPACE_YM_BDN

//////////////////////////////////////////////////////////////////////
/// @class LtConv LtConv.h "LtConv.h"
/// @brief LT 演算ノードに対する処理を行う MvnConv の派生クラス
//////////////////////////////////////////////////////////////////////
class LtConv :
  public MvnConv
{
public:

  /// @brief コンストラクタ
  LtConv();

  /// @brief デストラクタ
  virtual
  ~LtConv();


public:
  //////////////////////////////////////////////////////////////////////
  // MvnConv の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief MvNode を BdNetwork に変換する．
  /// @param[in] node ノード
  /// @param[in] bdnetwork 変換結果の BdNetwork
  /// @param[in] nodemap ノードの対応関係を表すマップ
  /// @retval true このクラスで変換処理を行った．
  /// @retval false このクラスでは変換処理を行わなかった．
  virtual
  bool
  operator()(const MvNode* node,
	     BdNetwork& bdnetwork,
	     MvnBdnMap& nodemap);

};

END_NAMESPACE_YM_BDN

#endif // LIBYM_BDN_MVNCONV_LTCONV_H

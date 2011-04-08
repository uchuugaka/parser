#ifndef LIBYM_MVN_MVNINOUT_H
#define LIBYM_MVN_MVNINOUT_H

/// @file libym_mvn/MvnInout.h
/// @brief MvnInout のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnUnaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnInout MvnInout.h "MvnInout.h"
/// @brief 入出力ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnInout :
  public MvnUnaryOp
{
  friend class MvnMgr;

public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width ビット幅
  MvnInout(MvnModule* module,
	   ymuint bit_width);

  /// @brief デストラクタ
  ~MvnInout();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNINOUT_H

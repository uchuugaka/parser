#ifndef YMSLCOMPILER_H
#define YMSLCOMPILER_H

/// @file YmslCompiler.h
/// @brief YmslCompiler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/IDO.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslCompiler YmslCompiler.h "YmslCompiler.h"
/// @brief YMSL 用のコンパイラ
//////////////////////////////////////////////////////////////////////
class YmslCompiler
{
public:

  /// @brief コンストラクタ
  YmslCompiler();

  /// @brief デストラクタ
  ~YmslCompiler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief コンパイルする．
  /// @param[in] ido 入力データ
  /// @return コンパイルしたモジュールを返す．
  ///
  /// エラーが起きたら NULL を返す．
  VsmModule*
  compile(IDO& ido);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_YMSL


#endif // YMSLCOMPILER_H
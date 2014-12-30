
/// @file Ast.cc
/// @brief Ast の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"
#include "AstSymbol.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス Ast
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
Ast::Ast(const FileRegion& loc) :
  mLoc(loc)
{
}

// @brief デストラクタ
Ast::~Ast()
{
}

// @brief ファイル位置を得る．
const FileRegion&
Ast::file_region() const
{
  return mLoc;
}


//////////////////////////////////////////////////////////////////////
// クラス AstSymbol
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] loc ファイル位置
AstSymbol::AstSymbol(ShString val,
		     const FileRegion& loc) :
  Ast(loc),
  mVal(val),
  mNext(NULL)
{
}

// @brief デストラクタ
AstSymbol::~AstSymbol()
{
}

// @brief 次の要素をセットする．
void
AstSymbol::set_next(AstSymbol* next)
{
  ASSERT_COND( mNext == NULL );
  mNext = next;
}

// @brief 文字列型の値を返す．
ShString
AstSymbol::str_val() const
{
  return mVal;
}

// @brief 次の要素を得る．
//
// なければ NULL を返す．
AstSymbol*
AstSymbol::next() const
{
  return mNext;
}

END_NAMESPACE_YM_YMSL

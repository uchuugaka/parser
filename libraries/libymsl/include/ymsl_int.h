#ifndef YMSL_INT_H
#define YMSL_INT_H

/// @file ymsl_int.h
/// @brief ymsl の内部定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


//////////////////////////////////////////////////////////////////////
// 名前空間を定義するためのマクロ
//////////////////////////////////////////////////////////////////////

/// @brief ymsl の名前空間の開始
#define BEGIN_NAMESPACE_YM_YMSL \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsYmsl)

/// @brief ymsl の名前空間の終了
#define END_NAMESPACE_YM_YMSL \
END_NAMESPACE(nsYmsl) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_YMSL

class YmslCodeList;
class YmslDriver;
class YmslEnum;
class YmslExecutable;
class YmslFunction;
class YmslLabel;
class YmslModule;
class YmslObj;
class YmslParser;
class YmslScope;
class YmslSubspace;
class YmslType;
class YmslVar;

class ObjHandle;

class AstCaseItem;
class AstExpr;
class AstFuncDecl;
class AstIfBlock;
class AstMgr;
class AstModule;
class AstParam;
class AstStatement;
class AstStringConst;
class AstSymbol;
class AstToplevel;
class AstType;
class AstVarDecl;

template <typename T>
class AstList;

typedef AstList<AstModule> AstModuleList;
typedef AstList<AstStatement> AstStmtList;
typedef AstList<AstIfBlock> AstIfList;
typedef AstList<AstCaseItem> AstCaseList;
typedef AstList<AstParam> AstParamList;
typedef AstList<AstExpr> AstExprList;

#if 0
/// @brief トークンの値
enum TokenType {
  /// @brief コロン(:)
  COLON,
  /// @brief セミコロン(;)
  SEMI,
  /// @brief コンマ(,)
  COMMA,
  /// @brief ドット(.)
  DOT,

  /// @brief プラス(+)
  PLUS,
  /// @brief マイナス(-)
  MINUS,
  /// @brief かける(*)
  MULT,
  /// @brief わる(/)
  DIV,
  /// @brief 余り(%)
  MOD,
  /// @brief ++
  PLUSPLUS,
  /// @brief --
  MINUSMINUS,

  /// @brief logical not(!)
  LNOT,
  /// @brief logical and(&&)
  LAND,
  /// @brief logical or(||)
  LOR,
  /// @brief 等しい(==)
  EQEQ,
  /// @brief 等しくない(!=)
  NOTEQ,
  /// @brief 小なり(<)
  LT,
  /// @brief 大なり(>)
  GT,
  /// @brief 小なりイコール(<=)
  LE,
  /// @brief 大なりイコール(>=)
  GE,

  /// @brief bitwise not(~)
  BITNOT,
  /// @brief bitwise and(&)
  BITAND,
  /// @brief bitwise or(|)
  BITOR,
  /// @brief bitwise xor(^)
  BITXOR,

  /// @brief 代入 (=)
  EQ,

  /// @brief 左括弧( ( )
  LP,
  /// @brief 右括弧( ) )
  RP,
  /// @brief 左中括弧( { )
  LCB,
  /// @brief 右中括弧( } )
  RCB,
  /// @brief 左角括弧( [ )
  LBK,
  /// @brief 右角括弧( ] )
  RBK,

  /// @brief 'if'
  IF,
  /// @brief 'else'
  ELSE,
  /// @brief 'elif'
  ELIF,
  /// @brief 'for'
  FOR,
  /// @brief 'while'
  WHILE,
  /// @brief 'do'
  DO,
  /// @brief 'goto'
  GOTO,
  /// @brief 'break'
  BREAK,
  /// @brief 'continue'
  CONTINUE,

  /// @brief 'function'
  FUNCTION,
  /// @brief 'return'
  RETURN,
  /// @brief 'var'
  VAR,

  /// @brief シンボル
  SYMBOL,
  /// @brief 整数値
  INT_NUM,
  /// @brief 浮動小数点数値
  FLOAT_NUM,
  /// @brief 文字列リテラル
  STRING,
  /// @brief エラー
  ERROR,
  /// @brief ファイルの末尾
  END
};
#else
typedef int TokenType;
#endif

/// @brief トークン番号から文字列を返す関数
extern
const char*
token2str(TokenType token);


/// @brief 整数型
typedef ymint32 Ymsl_INT;

/// @brief 浮動小数点型
typedef double Ymsl_FLOAT;

/// @brief オブジェクト型
typedef YmslObj* Ymsl_OBJPTR;

/// @brief プログラムのコード
typedef ymuint32 Ymsl_CODE;


//////////////////////////////////////////////////////////////////////
/// @brief 型の種類
//////////////////////////////////////////////////////////////////////
enum TypeId {
  kVoidType,
  kBooleanType,
  kIntType,
  kFloatType,
  kStringType,
  kArrayType,
  kSetType,
  kMapType,
  kFuncType,
  kClassType,
  kEnumType,
  kUserDefType
};

/// @brief ValueType を出力する．
ostream&
operator<<(ostream& s,
	   TypeId vt);


//////////////////////////////////////////////////////////////////////
/// @class 値を表す共用型
///
/// あんまり union は使いたくないんだけど
//////////////////////////////////////////////////////////////////////
union YmslValue {
  Ymsl_INT    int_value;
  Ymsl_FLOAT  float_value;
  Ymsl_OBJPTR obj_value;
};

END_NAMESPACE_YM_YMSL

#endif // YMSL_INT_H
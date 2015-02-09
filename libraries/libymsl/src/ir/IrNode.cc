
/// @file IrNode.cc
/// @brief IrNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opcode オペコード
// @param[in] type 型
IrNode::IrNode(OpCode opcode,
	       const Type* type) :
  mOpCode(opcode),
  mType(type)
{
}

// @brief デストラクタ
IrNode::~IrNode()
{
}

// @brief opcode を返す．
OpCode
IrNode::opcode() const
{
  return mOpCode;
}

// @brief 型を返す．
const Type*
IrNode::type() const
{
  return mType;
}

// @brief 番号を返す．
ymuint
IrNode::id() const
{
  return mId;
}

// @brief 番号をセットする．
// @param[in] id 番号
void
IrNode::set_id(ymuint id)
{
  mId = id;
}

// @brief オペランド数を返す．
//
// 演算子のみ有効
ymuint
IrNode::operand_num() const
{
  return 0;
}

// @brief オペランドを返す．
// @param[in] pos 位置 ( 0 <= pos < operand_num() )
//
// 演算子のみ有効
IrNode*
IrNode::operand(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 整数値を返す．
int
IrNode::int_val() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 実数値を返す．
double
IrNode::float_val() const
{
  ASSERT_NOT_REACHED;
  return 0.0;
}

// @brief 文字列を返す．
const char*
IrNode::string_val() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief ロード/ストア対象のアドレスを得る．
//
// kOpLoad, kOpStore, kOpInc, kOpDec のみ有効
IrHandle*
IrNode::address() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 書き込む値を返す．
//
// kOpStore のみ有効
IrNode*
IrNode::store_val() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 関数アドレスを返す．
//
// kOpFuncCall のみ有効
IrHandle*
IrNode::func_addr() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 関数の引数の数を得る．
//
// kOpFuncCall のみ有効
ymuint
IrNode::arglist_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 関数の引数を得る．
// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
//
// kOpFuncCall のみ有効
IrNode*
IrNode::arglist_elem(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief ジャンプ先のノードを得る．
//
// kOpJump, kOpBranchXXX のみ有効
IrNode*
IrNode::jump_addr() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 分岐条件
IrNode*
IrNode::branch_cond() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 返り値
IrNode*
IrNode::return_val() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 定義済みの時に true を返す．
//
// kOpLabel のみ意味を持つ．
bool
IrNode::is_defined() const
{
  ASSERT_NOT_REACHED;
  return false;
}

// @brief 定義済みにする．
//
// kOpLabel のみ意味を持つ．
void
IrNode::set_defined()
{
  ASSERT_NOT_REACHED;
}


//////////////////////////////////////////////////////////////////////
// その他
//////////////////////////////////////////////////////////////////////

// @brief OpCode を出力する．
// @param[in] s 出力先のストリーム
// @param[in] op オペオード
ostream&
operator<<(ostream& s,
	   OpCode op)
{
  switch ( op ) {
  case kOpTrue:          s << "True"; break;
  case kOpFalse:         s << "False"; break;
  case kOpIntConst:      s << "IntConst"; break;
  case kOpFloatConst:    s << "FloatConst"; break;
  case kOpStringConst:   s << "StringConst"; break;
  case kOpCastBoolean:   s << "CastBoolean"; break;
  case kOpCastInt:       s << "CastInt"; break;
  case kOpCastFloat:     s << "CastFloat"; break;
  case kOpBitNeg:        s << "BitNeg"; break;
  case kOpLogNot:        s << "LogNot"; break;
  case kOpUniMinus:      s << "UniMinus"; break;
  case kOpBitAnd:        s << "BitAnd"; break;
  case kOpBitOr:         s << "BitOr"; break;
  case kOpBitXor:        s << "BitXor"; break;
  case kOpLogAnd:        s << "LogAnd"; break;
  case kOpLogOr:         s << "LogOr"; break;
  case kOpAdd:           s << "Add"; break;
  case kOpSub:           s << "Sub"; break;
  case kOpMul:           s << "Mul"; break;
  case kOpDiv:           s << "Div"; break;
  case kOpMod:           s << "Mod"; break;
  case kOpLshift:        s << "Lshift"; break;
  case kOpRshift:        s << "Rshift"; break;
  case kOpEqual:         s << "Equal"; break;
  case kOpNotEq:         s << "NotEq"; break;
  case kOpLt:            s << "Lt"; break;
  case kOpLe:            s << "Le"; break;
  case kOpIte:           s << "Ite"; break;
  case kOpLoad:          s << "Load"; break;
  case kOpStore:         s << "Store"; break;
  case kOpInc:           s << "Inc"; break;
  case kOpDec:           s << "Dec"; break;
  case kOpInplaceBitAnd: s << "InplaceBitAnd"; break;
  case kOpInplaceBitOr:  s << "InplaceBitOr"; break;
  case kOpInplaceBitXor: s << "InplaceBitXor"; break;
  case kOpInplaceAdd:    s << "InplaceAdd"; break;
  case kOpInplaceSub:    s << "InplaceSub"; break;
  case kOpInplaceMul:    s << "InplaceMul"; break;
  case kOpInplaceDiv:    s << "InplaceDiv"; break;
  case kOpInplaceMod:    s << "InplaceMod"; break;
  case kOpInplaceLshift: s << "InplaceLshift"; break;
  case kOpInplaceRshift: s << "InplaceRshift"; break;
  case kOpFuncCall:      s << "FuncCall"; break;
  case kOpReturn:        s << "Return"; break;
  case kOpJump:          s << "Jump"; break;
  case kOpBranchTrue:    s << "BranchTrue"; break;
  case kOpBranchFalse:   s << "BranchFalse"; break;
  case kOpLabel:         s << "Label"; break;
  case kOpHalt:          s << "Halt"; break;
  default: ASSERT_NOT_REACHED; break;
  }

  return s;
}

END_NAMESPACE_YM_YMSL

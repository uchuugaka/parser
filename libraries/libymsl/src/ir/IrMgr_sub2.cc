
/// @file IrMgr.cc
/// @brief IrMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrMgr.h"
#include "TypeMgr.h"
#include "IrTrue.h"
#include "IrFalse.h"
#include "IrIntConst.h"
#include "IrFloatConst.h"
#include "IrStringConst.h"
#include "IrUniOp.h"
#include "IrBinOp.h"
#include "IrTriOp.h"
#include "IrLoad.h"
#include "IrStore.h"
#include "IrInplaceUniOp.h"
#include "IrInplaceBinOp.h"
#include "IrVarRef.h"
#include "IrFuncRef.h"
#include "IrArrayRef.h"
#include "IrMemberRef.h"
#include "IrFuncCall.h"
#include "IrReturn.h"
#include "IrJump.h"
#include "IrLabel.h"
#include "Var.h"
#include "Function.h"
#include "Type.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrMgr
//////////////////////////////////////////////////////////////////////

// @brief true 定数を生成する．
IrNode*
IrMgr::new_True()
{
  void* p = mAlloc.get_memory(sizeof(IrTrue));
  return new (p) IrTrue(mTypeMgr.boolean_type());
}

// @brief False 定数を生成する．
IrNode*
IrMgr::new_False()
{
  void* p = mAlloc.get_memory(sizeof(IrFalse));
  return new (p) IrFalse(mTypeMgr.boolean_type());
}

// @brief 整数値定数を生成する．
// @param[in] val 値
IrNode*
IrMgr::new_IntConst(int val)
{
  void* p = mAlloc.get_memory(sizeof(IrIntConst));
  return new (p) IrIntConst(mTypeMgr.int_type(), val);
}

// @brief 実数値定数を生成する．
// @param[in] val 値
IrNode*
IrMgr::new_FloatConst(double val)
{
  void* p = mAlloc.get_memory(sizeof(IrFloatConst));
  return new (p) IrFloatConst(mTypeMgr.float_type(), val);
}

// @brief 文字列定数を生成する．
// @param[in] val 値
IrNode*
IrMgr::new_StringConst(const char* val)
{
  void* p = mAlloc.get_memory(sizeof(IrStringConst));
  return new (p) IrStringConst(mTypeMgr.string_type(), val);
}

// @brief 単項演算式を生成する．
// @param[in] opcode オペコード
// @param[in] type 出力の型
// @param[in] opr1 オペランド
IrNode*
IrMgr::new_UniOp(OpCode opcode,
		 const Type* type,
		 IrNode* opr1)
{
  void* p = mAlloc.get_memory(sizeof(IrUniOp));
  return new (p) IrUniOp(opcode, type, opr1);
}

// @brief 二項演算式を生成する．
// @param[in] opcode オペコード
// @param[in] type 出力の型
// @param[in] opr1, opr2 オペランド
IrNode*
IrMgr::new_BinOp(OpCode opcode,
		 const Type* type,
		 IrNode* opr1,
		 IrNode* opr2)
{
  void* p = mAlloc.get_memory(sizeof(IrBinOp));
  return new (p) IrBinOp(opcode, type, opr1, opr2);
}

// @brief 三項演算式を生成する．
// @param[in] opcode オペコード
// @param[in] type 出力の型
// @param[in] opr1, opr2, opr3 オペランド
IrNode*
IrMgr::new_TriOp(OpCode opcode,
		 const Type* type,
		 IrNode* opr1,
		 IrNode* opr2,
		 IrNode* opr3)
{
  void* p = mAlloc.get_memory(sizeof(IrTriOp));
  return new (p) IrTriOp(opcode, type, opr1, opr2, opr3);
}

// @brief load 文を生成する．
// @param[in] addr アドレス
IrNode*
IrMgr::new_Load(IrHandle* addr)
{
  const Type* type = NULL;
  switch ( addr->handle_type() ) {
  case IrHandle::kVar:
  case IrHandle::kMemberRef:
    type = addr->var()->value_type();
    break;

  case IrHandle::kFunction:
  case IrHandle::kMethodRef:
    type = addr->function()->type();
    break;

  case IrHandle::kConstant:
    type = addr->constant()->type();
    break;

  case IrHandle::kArrayRef:
    type = addr->array_expr()->type()->elem_type();
    break;

  default:
    ASSERT_NOT_REACHED;
    break;
  }
  void* p = mAlloc.get_memory(sizeof(IrLoad));
  return new (p) IrLoad(type, addr);
}

// @brief store 文を生成する．
// @param[in] addr アドレス
// @param[in] val 書き込む値
IrNode*
IrMgr::new_Store(IrHandle* addr,
		 IrNode* val)
{
  void* p = mAlloc.get_memory(sizeof(IrStore));
  return new (p) IrStore(addr, val);
}

// @brief 自己代入型の単項演算を生成する．
// @param[in] opcode オペコード
// @param[in] lhs_addr 左辺式
IrNode*
IrMgr::new_InplaceUniOp(OpCode opcode,
			IrHandle* lhs_addr)
{
  void* p = mAlloc.get_memory(sizeof(IrInplaceUniOp));
  return new (p) IrInplaceUniOp(opcode, lhs_addr);
}

// @brief 自己代入型の二項演算を生成する．
// @param[in] opcode オペコード
// @param[in] lhs_addr 左辺式
// @param[in] opr オペランド
IrNode*
IrMgr::new_InplaceBinOp(OpCode opcode,
			IrHandle* lhs_addr,
			IrNode* opr)
{
  void* p = mAlloc.get_memory(sizeof(IrInplaceBinOp));
  return new (p) IrInplaceBinOp(opcode, lhs_addr, opr);
}

#if 0
// @brief 変数参照を生成する．
// @param[in] var 変数
IrNode*
IrMgr::new_VarRef(const Var* var)
{
  void* p = mAlloc.get_memory(sizeof(IrVarRef));
  return new (p) IrVarRef(var);
}

// @brief 関数参照を生成する．
// @param[in] func 関数
IrNode*
IrMgr::new_FuncRef(const Function* func)
{
  void* p = mAlloc.get_memory(sizeof(IrFuncRef));
  return new (p) IrFuncRef(func);
}
#endif

// @brief 配列参照を生成する．
// @param[in] array 配列
// @param[in] index インデックス
IrHandle*
IrMgr::new_ArrayRef(IrNode* array,
		    IrNode* index)
{
  void* p = mAlloc.get_memory(sizeof(IrArrayRef));
  return new (p) IrArrayRef(array, index);
}

// @brief クラスメンバ参照を生成する．
// @param[in] obj オブジェクト
// @param[in] var メンバ変数
IrHandle*
IrMgr::new_MemberRef(IrNode* obj,
		     const Var* var)
{
  void* p = mAlloc.get_memory(sizeof(IrMemberRef));
  return new (p) IrMemberRef(obj, var);
}

// @brief 関数呼び出し式を生成する．
// @param[in] func_addr 関数アドレス
// @param[in] arglist 引数のリスト
IrNode*
IrMgr::new_FuncCall(IrHandle* func_addr,
		    const vector<IrNode*>& arglist)
{
  void* p = mAlloc.get_memory(sizeof(IrFuncCall));
  return new (p) IrFuncCall(func_addr, arglist);
}

// @brief リターン命令を生成する．
// @param[in] ret_val 返り値
IrNode*
IrMgr::new_Return(IrNode* ret_val)
{
  void* p = mAlloc.get_memory(sizeof(IrReturn));
  return new (p) IrReturn(ret_val);
}

// @brief ジャンプ系のノードを生成する．
// @param[in] opcode オペコード
// @param[in] label ジャンプ先のラベル
// @param[in] cond 条件
IrNode*
IrMgr::new_Jump(OpCode opcode,
		IrNode* label,
		IrNode* cond)
{
  void* p = mAlloc.get_memory(sizeof(IrJump));
  return new (p) IrJump(opcode, label, cond);
}

// @brief ラベルノードを生成する．
IrNode*
IrMgr::new_Label()
{
  void* p = mAlloc.get_memory(sizeof(IrLabel));
  return new (p) IrLabel();
}

END_NAMESPACE_YM_YMSL

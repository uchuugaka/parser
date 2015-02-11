
/// @file IrMgr.cc
/// @brief IrMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrMgr.h"
#include "TypeMgr.h"

#include "AstStatement.h"
#include "AstSymbol.h"
#include "AstType.h"

#include "Function.h"
#include "Scope.h"
#include "Type.h"
#include "IrHandle.h"
#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrMgr
//////////////////////////////////////////////////////////////////////

// @brief 要素の生成を行う．
// @param[in] stmt 文
// @param[in] scope 現在のスコープ
// @param[in] start_label ブロックの開始ラベル
// @param[in] end_label ブロックの終了ラベル
// @param[out] node_list 生成されたノードのリスト
void
IrMgr::elab_stmt(const AstStatement* stmt,
		 Scope* scope,
		 IrNode* start_label,
		 IrNode* end_label,
		 vector<IrNode*>& node_list)
{
  switch ( stmt->stmt_type() ) {
  case AstStatement::kBlock:
    {
      Scope* block_scope = new_scope(scope);
      ymuint n = stmt->stmtlist_num();
      for (ymuint i = 0; i < n; ++ i) {
	const AstStatement* stmt1 = stmt->stmtlist_elem(i);
	elab_stmt(stmt1, block_scope, start_label, end_label, node_list);
      }
    }
    break;

  case AstStatement::kBreak:
    {
      if ( end_label == NULL ) {
	// not inside loop
	return;
      }
      IrNode* node = new_Jump(end_label);
      node_list.push_back(node);
    }
    break;

  case AstStatement::kConstDecl:
    reg_const(stmt, scope);
    break;

  case AstStatement::kContinue:
    {
      if ( start_label == NULL ) {
	// not inside loop
	return;
      }
      IrNode* node = new_Jump(start_label);
      node_list.push_back(node);
    }
    break;

  case AstStatement::kDecr:
    {
      IrHandle* lhs_handle = elab_primary(stmt->lhs_expr(), scope);
      if ( lhs_handle == NULL ) {
	// エラー
	return;
      }
      // lhs_handle の指しているものが int 型の変数かチェック

      IrNode* node = new_InplaceUniOp(kOpDec, lhs_handle);
      node_list.push_back(node);
    }
    break;

  case AstStatement::kDoWhile:
    {
      IrNode* start1 = new_Label();
      IrNode* end1 = new_Label();
      node_list.push_back(start1);
      elab_stmt(stmt->stmt(), scope, start1, end1, node_list);
      IrNode* cond = elab_expr(stmt->expr(), scope);
      IrNode* node1 = new_BranchTrue(start1, cond);
      node_list.push_back(node1);
      node_list.push_back(end1);
    }
    break;

  case AstStatement::kEnumDecl:
    reg_enum(stmt, scope);
    break;

  case AstStatement::kAssignment:
    {
      IrNode* rhs = elab_expr(stmt->expr(), scope);
      if ( rhs == NULL ) {
	// エラー
	return;
      }
      IrHandle* lhs_handle = elab_primary(stmt->lhs_expr(), scope);
      if ( lhs_handle == NULL ) {
	// エラー
	return;
      }

      // lhs_handle->type() と rhs->type() のチェック
      // 必要ならキャスト
      IrNode* node = new_Store(lhs_handle, rhs);
      node_list.push_back(node);
    }
    break;

  case AstStatement::kInplaceOp:
    {
      IrNode* rhs = elab_expr(stmt->expr(), scope);
      if ( rhs == NULL ) {
	// エラー
	return;
      }
      IrHandle* lhs_handle = elab_primary(stmt->lhs_expr(), scope);
      if ( lhs_handle == NULL ) {
	// エラー
	return;
      }

      // lhs_handle->type() と rhs->type() のチェック
      // 必要ならキャスト
      OpCode opcode = stmt->opcode();
      IrNode* node = new_InplaceBinOp(opcode, lhs_handle, rhs);
      node_list.push_back(node);
    }
    break;

  case AstStatement::kExpr:
    {
      IrNode* node = elab_expr(stmt->expr(), scope);
      node_list.push_back(node);
    }
    break;

  case AstStatement::kFor:
    {
      Scope* for_scope = new_scope(scope);
      elab_stmt(stmt->init_stmt(), for_scope, NULL, NULL, node_list);
      IrNode* start1 = new_Label();
      node_list.push_back(start1);
      IrNode* end1 = new_Label();
      IrNode* cond = elab_expr(stmt->expr(), for_scope);
      IrNode* node1 = new_BranchFalse(end1, cond);
      node_list.push_back(node1);
      elab_stmt(stmt->stmt(), for_scope, start1, end1, node_list);
      elab_stmt(stmt->next_stmt(), for_scope, start1, end1, node_list);
      IrNode* node2 = new_Jump(start1);
      node_list.push_back(node2);
      node_list.push_back(end1);
    }
    break;

  case AstStatement::kFuncDecl:
    reg_func(stmt, scope);
    break;

  case AstStatement::kGoto:
    {
      const AstSymbol* label_symbol = stmt->label();
      IrHandle* h = scope->find(label_symbol->str_val());
      IrNode* label_node;
      if ( h == NULL ) {
	// ラベルが未定義
	// ノードを作ってしまう．
	label_node = new_Label();
	h = new_LabelHandle(label_symbol->str_val(), label_node);
	scope->add(h);
	mUndefList.push_back(label_node);
      }
      else {
	if ( h->handle_type() != IrHandle::kLabel ) {
	  // label_symbol is not a label
	  return;
	}
      }
      IrNode* node = new_Jump(label_node);
      node_list.push_back(node);
    }
    break;

  case AstStatement::kIf:
    {
      IrNode* cond = elab_expr(stmt->expr(), scope);
      IrNode* label1 = new_Label();
      IrNode* label2 = new_Label();
      IrNode* node1 = new_BranchFalse(label1, cond);
      node_list.push_back(node1);
      elab_stmt(stmt->stmt(), scope, start_label, end_label, node_list);
      IrNode* node2 = new_Jump(label2);
      node_list.push_back(node2);
      node_list.push_back(label1);
      elab_stmt(stmt->else_stmt(), scope, start_label, end_label, node_list);
      node_list.push_back(label2);
    }
    break;

  case AstStatement::kImport:
    break;

  case AstStatement::kIncr:
    {
      IrHandle* lhs_handle = elab_primary(stmt->lhs_expr(), scope);
      if ( lhs_handle == NULL ) {
	// エラー
	return;
      }
      IrNode* node = new_InplaceUniOp(kOpInc, lhs_handle);
      node_list.push_back(node);
    }
    break;

  case AstStatement::kLabel:
    {
      const AstSymbol* label_symbol = stmt->label();
      IrHandle* h = scope->find(label_symbol->str_val());
      IrNode* label_node;
      if ( h == NULL ) {
	label_node = new_Label();
	h = new_LabelHandle(label_symbol->str_val(), label_node);
	scope->add(h);
      }
      else {
	if ( h->handle_type() != IrHandle::kLabel ) {
	  // duplicate definition
	  // というかラベルじゃないものだった．
	  return;
	}
	if ( label_node->is_defined() ) {
	  // 二重定義
	  return;
	}
      }
      label_node->set_defined();
      node_list.push_back(label_node);
    }
    break;

  case AstStatement::kNullStmt:
    break;

  case AstStatement::kReturn:
    {
      const AstExpr* expr = stmt->expr();
      IrNode* ret_val = NULL;
      if ( expr != NULL ) {
	ret_val = elab_expr(expr, scope);
      }
      IrNode* node = new_Return(ret_val);
      node_list.push_back(node);
    }
    break;

  case AstStatement::kSwitch:
    {
      IrNode* cond = elab_expr(stmt->expr(), scope);
      ymuint n = stmt->switch_num();
      for (ymuint i = 0; i < n; ++ i) {
	vector<IrNode*> node_list1;
	elab_stmt(stmt->case_stmt(i), scope, start_label, end_label, node_list1);
      }
    }
    break;

  case AstStatement::kToplevel:
    {
      ymuint n = stmt->stmtlist_num();
      for (ymuint i = 0; i < n; ++ i) {
	const AstStatement* stmt1 = stmt->stmtlist_elem(i);
	elab_stmt(stmt1, scope, NULL, NULL, node_list);
      }
    }
    break;

  case AstStatement::kVarDecl:
    reg_var(stmt, scope);
    break;

  case AstStatement::kWhile:
    {
      IrNode* start1 = new_Label();
      node_list.push_back(start1);
      IrNode* cond = elab_expr(stmt->expr(), scope);
      IrNode* end1 = new_Label();
      IrNode* node1 = new_BranchFalse(end1, cond);
      node_list.push_back(node1);
      elab_stmt(stmt->stmt(), scope, start1, end1, node_list);
      IrNode* node2 = new_Jump(start1);
      node_list.push_back(node2);
      node_list.push_back(end1);
    }
    break;

  }
}

// @brief enum 型の定義を行う．
// @param[in] stmt 文
//
// stmt は kEnumDecl でなければならない．
void
IrMgr::reg_enum(const AstStatement* stmt,
		Scope* scope)
{
  ASSERT_COND( stmt->stmt_type() == AstStatement::kEnumDecl );

  const AstSymbol* name_symbol = stmt->name();
  ShString name = name_symbol->str_val();

  IrHandle* h = scope->find_local(name);
  if ( h != NULL ) {
    // name が重複している．
    // previous definition is h->file_region()
    return;
  }

  ymint n = stmt->enum_num();
  vector<pair<ShString, int> > elem_list(n);
  int next_val = 0;
  for (ymuint i = 0; i < n; ++ i) {
    const AstSymbol* ec_symbol = stmt->enum_const(i);
    ShString elem_name = ec_symbol->str_val();
    const AstExpr* ec_expr = stmt->enum_const_expr(i);
    int v;
    if ( ec_expr != NULL ) {
      IrNode* node = elab_expr(ec_expr, scope);
      if ( node == NULL ) {
	// エラー
	return;
      }
      if ( !node->is_static() ) {
	// ec_expr が定数式ではない．
	return;
      }
      if ( node->value_type() != mTypeMgr.int_type() ) {
	// 整数型ではない．
	return;
      }
      // v = eval(node);
      // v が重複していないかチェック
    }
    else {
      // while ( check(next_val) ) {
      //   ++ next_val;
      // }
      v = next_val;
      ++ next_val;
    }
    elem_list[i] = make_pair(elem_name, v);
  }

  const Type* type = mTypeMgr.enum_type(name, elem_list);
  Scope* enum_scope = new_scope(scope, name);
  for (ymuint i = 0; i < n; ++ i) {
    ShString name = type->enum_elem_name(i);
    int val = type->enum_elem_val(i);
    const ConstVal* const_val = new_IntConst(val);
    IrHandle* h = new_ConstHandle(name, const_val);
    enum_scope->add(h);
  }

  IrHandle* h1 = new_TypeHandle(type, enum_scope);
  scope->add(h1);
}

// @brief 関数の定義を行う．
// @param[in] stmt 文
//
// stmt は kFuncDecl でなければならない．
void
IrMgr::reg_func(const AstStatement* stmt,
		Scope* scope)
{
  ASSERT_COND( stmt->stmt_type() == AstStatement::kFuncDecl );

  const AstSymbol* name_symbol = stmt->name();
  ShString name = name_symbol->str_val();

  IrHandle* h = scope->find_local(name);
  if ( h != NULL ) {
    // name が重複している．
    // previous definition is h->file_region()
    return;
  }

  const AstType* asttype = stmt->type();
  const Type* output_type = resolve_type(asttype, scope);
  if ( output_type == NULL ) {
    // エラーメッセージをどこで出すか考える．
    return;
  }

  ymuint np = stmt->param_num();
  vector<const Type*> input_type_list(np);
  for (ymuint i = 0; i < np; ++ i) {
    const AstType* asttype = stmt->param_type(i);
    const Type* type = resolve_type(asttype, scope);
    if ( type == NULL ) {
      // エラーメッセージをどこで出すか考える．
      return;
    }
    input_type_list[i] = type;
  }

  const Type* ftype = mTypeMgr.function_type(output_type, input_type_list);
  Function* func = new_function(name, ftype);
  IrHandle* h1 = new_FuncHandle(func);
  scope->add(h1);
}

// @brief 変数の定義を行う．
// @param[in] stmt 文
//
// stmt は kVarDecl でなければならない．
void
IrMgr::reg_var(const AstStatement* stmt,
	       Scope* scope)
{
  ASSERT_COND( stmt->stmt_type() == AstStatement::kVarDecl );

  const AstSymbol* name_symbol = stmt->name();
  ShString name = name_symbol->str_val();

  IrHandle* h = scope->find_local(name);
  if ( h != NULL ) {
    // name が重複している．
    // previous definition is h->file_region()
    return;
  }

  const AstType* asttype = stmt->type();
  const Type* type = resolve_type(asttype, scope);
  if ( type == NULL ) {
    // エラーメッセージをどこで出すか考える．
    return;
  }

  Var* var = new_var(name, type);
  IrHandle* h1 = new_VarHandle(var);
  scope->add(h1);
}

// @brief 定数の定義を行う．
// @param[in] stmt 文
//
// stmt は kConstDecl でなければならない．
void
IrMgr::reg_const(const AstStatement* stmt,
		 Scope* scope)
{
  ASSERT_COND( stmt->stmt_type() == AstStatement::kConstDecl );

  const AstSymbol* name_symbol = stmt->name();
  ShString name = name_symbol->str_val();

  IrHandle* h = scope->find_local(name);
  if ( h != NULL ) {
    // name が重複している．
    // previous definition is h->file_region()
    return;
  }

  const AstType* asttype = stmt->type();
  const Type* type = resolve_type(asttype, scope);
  if ( type == NULL ) {
    // エラーメッセージをどこで出すか考える．
    return;
  }

  IrNode* node = elab_expr(stmt->expr(), scope);
  // node->type() が type と互換性があるかをチェック
  // node が定数式かチェック
  const ConstVal* const_val;
  // const_val = eval(node);

  IrHandle* h1 = new_ConstHandle(name, const_val);
  scope->add(h1);
}

// @brief 型の参照を解決する．
// @param[in] asttype 型を表す構文木
//
// 解決できない時には NULL を返す．
const Type*
IrMgr::resolve_type(const AstType* asttype,
		    Scope* scope)
{
  switch ( asttype->type_id() ) {
  case kVoidType:
    return mTypeMgr.void_type();

  case kBooleanType:
    return mTypeMgr.boolean_type();

  case kIntType:
    return mTypeMgr.int_type();

  case kFloatType:
    return mTypeMgr.float_type();

  case kStringType:
    return mTypeMgr.string_type();

  case kArrayType:
    {
      const Type* elem_type = resolve_type(asttype->elem_type(), scope);
      if ( elem_type == NULL ) {
	break;
      }
      return mTypeMgr.array_type(elem_type);
    }
    break;

  case kSetType:
    {
      const Type* elem_type = resolve_type(asttype->elem_type(), scope);
      if ( elem_type == NULL ) {
	break;
      }
      return mTypeMgr.set_type(elem_type);
    }
    break;

  case kMapType:
    {
      const Type* key_type = resolve_type(asttype->key_type(), scope);
      if ( key_type == NULL ) {
	break;
      }
      const Type* elem_type = resolve_type(asttype->elem_type(), scope);
      if ( elem_type == NULL ) {
	break;
      }
      return mTypeMgr.map_type(key_type, elem_type);
    }
    break;

  case kNamedType:
    {
      // スコープから名前の解決を行う
      ymuint n = asttype->scope_num();
      Scope* cur_scope = scope;
      for (ymuint i = 0; i < n; ++ i) {
	const AstSymbol* scope1_symbol = asttype->scope(i);
	ShString scope1_name = scope1_symbol->str_val();
	IrHandle* h = cur_scope->find(scope1_name);
	if ( h == NULL ) {
	  // scope1_name is not found
	  return NULL;
	}
	cur_scope = h->scope();
	if ( cur_scope == NULL ) {
	  // scope1_name is not a scope
	  return NULL;
	}
      }
      const AstSymbol* name_symbol = asttype->name();
      ShString name = name_symbol->str_val();
      IrHandle* h = cur_scope->find(name);
      if ( h == NULL ) {
	// name is not defined
	return NULL;
      }
      const Type* type = h->named_type();
      if ( type == NULL ) {
	// name is not a type;
      }
      return type;
    }
    break;

  case kFuncType:
  case kEnumType:
  case kClassType:
  case kUserDefType:
    // AstType でこれらの型はありえない．
    ASSERT_NOT_REACHED;
    break;

  }
  return NULL;
}

// @brief 式から関数の解決を行う．
// @param[in] expr 式
// @param[in] scope 現在のスコープ
// @param[in] node 関数呼び出しノード
bool
IrMgr::resolve_func(const AstExpr* expr,
		    Scope* scope,
		    IrNode* node)
{
  IrHandle* h = elab_primary(expr, scope);
  if ( h == NULL ) {
    // expr not found
    return false;
  }
  if ( h->handle_type() != IrHandle::kFunction ) {
    // expr is not a function;
    return false;
  }
  const Function* func = h->function();
  // func の型と node の arglist の型をチェック

  // node に func をセット
  return true;
}

END_NAMESPACE_YM_YMSL

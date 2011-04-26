#ifndef LIBYM_CELL_DOTLIB_PTNODEIMPL_H
#define LIBYM_CELL_DOTLIB_PTNODEIMPL_H

/// @file libym_cell/dotlib/PtNodeImpl.h
/// @brief PtNode の継承クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtSimpleNode PtNodeImpl.h "PtNodeImpl.h"
/// @brief simple attribute を表す PtNode の継承クラス
//////////////////////////////////////////////////////////////////////
class PtSimpleNode :
  public PtNode
{
  friend class PtMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] value 値を表すトークン
  PtSimpleNode(const PtValue* value);

  /// @brief デストラクタ
  virtual
  ~PtSimpleNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の数を返す．
  /// @note simple attribute なら常に1
  virtual
  ymuint
  value_num() const;

  /// @brief 値を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < value_num() )
  virtual
  const PtValue*
  value(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  const PtValue* mValue;

};


//////////////////////////////////////////////////////////////////////
/// @class PtComplexNode PtNodeImpl.h "PtNodeImpl.h"
/// @brief complex attribute を表す PtNode の継承クラス
//////////////////////////////////////////////////////////////////////
class PtComplexNode :
  public PtNode
{
public:

  /// @brief 用のコンストラクタ
  /// @param[in] value_list 値を表すトークンのリスト
  PtComplexNode(const vector<const PtValue*>& value_list);

  /// @brief デストラクタ
  virtual
  ~PtComplexNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の数を返す．
  virtual
  ymuint
  value_num() const;

  /// @brief 値を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < value_num() )
  virtual
  const PtValue*
  value(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値のリスト
  vector<const PtValue*> mValueList;

};


//////////////////////////////////////////////////////////////////////
/// @class PtGroupNode PtNodeImpl.h "PtNodeImpl.h"
/// @brief group statement を表す PtNode の継承クラス
//////////////////////////////////////////////////////////////////////
class PtGroupNode :
  public PtComplexNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] value_list 値を表すトークンのリスト
  PtGroupNode(const vector<const PtValue*>& value_list);

  /// @brief デストラクタ
  virtual
  ~PtGroupNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供を追加する．
  /// @param[in] attr_name 属性名
  /// @param[in] node 追加する子供のノード
  virtual
  void
  add_child(const ShString& attr_name,
	    PtNode* node);


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供の属性名の個数を返す．
  /// @note デフォルトの実装は 0 を返す．
  virtual
  ymuint
  child_attr_num() const;

  /// @brief 子供の属性名を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < child_attr_num() )
  virtual
  ShString
  child_attr_name(ymuint pos) const;

  /// @brief 属性に対応した子供の要素数を返す．
  /// @param[in] attr_name 子供の属性名
  /// @note デフォルトの実装は 0 を返す．
  virtual
  ymuint
  child_num(const ShString& attr_name) const;

  /// @brief 属性に対応した子供を返す．
  /// @param[in] attr_name 子供の属性名
  /// @param[in] pos 位置番号 ( 0 <= pos < child_num(attr_name) )
  virtual
  const PtNode*
  child(const ShString& attr_name,
	ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 属性名のリスト
  vector<ShString> mAttrList;

#if 0
  // 属性名をキーにして子供のリストを格納する連想配列
  hash_map<ShString, vector<PtNode*> > mChildMap;
#endif
};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTNODE_H
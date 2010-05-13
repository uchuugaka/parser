#ifndef VLPTNODE_ITEM_H
#define VLPTNODE_ITEM_H

/// @file libym_verilog/tests/vlview/VlPtNode_item.h
/// @brief VlPtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlPtNode_item.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VlPtNode.h"
#include <ym_verilog/verilog.h>
#include <ym_verilog/pt/PtP.h>
#include <ym_verilog/pt/PtArray.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ItemListNode VlPtNode_item.h
/// @brief item リストを表すノード
//////////////////////////////////////////////////////////////////////
class ItemListNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] item_array item の配列
  ItemListNode(const QString& label,
	       const PtItemArray& item_array);

  /// @brief デストラクタ
  virtual
  ~ItemListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ラベル
  QString mLabel;
  
  // item の配列
  PtItemArray mItemArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class ItemNode VlPtNode_item.h
/// @brief item を表すノード
//////////////////////////////////////////////////////////////////////
class ItemNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] item item
  ItemNode(const PtItem* item);

  /// @brief デストラクタ
  virtual
  ~ItemNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ラベル
  QString mLabel;
  
  // item
  const PtItem* mItem;
  
};


//////////////////////////////////////////////////////////////////////
/// @class DefParamNode VlPtNode_item.h
/// @brief defparam 文を表すノード
//////////////////////////////////////////////////////////////////////
class DefParamNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] defparam
  DefParamNode(const PtDefParam* defparam);

  /// @brief デストラクタ
  virtual
  ~DefParamNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // defparam 文を表すパース木
  const PtDefParam* mDefParam;
  
};


//////////////////////////////////////////////////////////////////////
/// @class ContAssignNode VlPtNode_item.h
/// @brief continuous assign 文を表すノード
//////////////////////////////////////////////////////////////////////
class ContAssignNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] cont_assign
  ContAssignNode(const PtContAssign* cont_assign);

  /// @brief デストラクタ
  virtual
  ~ContAssignNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // continuous assign 文を表すパース木
  const PtContAssign* mContAssign;
  
};


//////////////////////////////////////////////////////////////////////
/// @class InstListNode VlPtNode_item.h
/// @brief instance 文のリストを表すノード
//////////////////////////////////////////////////////////////////////
class InstListNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] item 親の item
  InstListNode(const PtItem* item);

  /// @brief デストラクタ
  virtual
  ~InstListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 親の item
  const PtItem* mItem;
  
};


//////////////////////////////////////////////////////////////////////
/// @class InstNode VlPtNode_item.h
/// @brief instance 文を表すノード
//////////////////////////////////////////////////////////////////////
class InstNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] inst インスタンス
  InstNode(const PtInst* inst);

  /// @brief デストラクタ
  virtual
  ~InstNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // instance 文を表すパース木
  const PtInst* mInst;
  
};


//////////////////////////////////////////////////////////////////////
/// @class InstPortListNode VlPtNode_item.h
/// @brief instance 文のポート割り当てリストを表すノード
//////////////////////////////////////////////////////////////////////
class InstPortListNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] inst 親のインスタンス
  InstPortListNode(const PtInst* inst);

  /// @brief デストラクタ
  virtual
  ~InstPortListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // instance 文を表すパース木
  const PtInst* mInst;
  
};


//////////////////////////////////////////////////////////////////////
/// @class GenCaseItemListNode VlPtNode_item.h
/// @brief generate case item 文のリストを表すノード
//////////////////////////////////////////////////////////////////////
class GenCaseItemListNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] item 親の GenCase
  GenCaseItemListNode(const PtItem* item);

  /// @brief デストラクタ
  virtual
  ~GenCaseItemListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 親の generate case 文を表すパース木
  const PtItem* mItem;
  
};


//////////////////////////////////////////////////////////////////////
/// @class GenCaseItem VlPtNode_item.h
/// @brief generate case item 文を表すノード
//////////////////////////////////////////////////////////////////////
class GenCaseItemNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] item generate case item
  GenCaseItemNode(const PtGenCaseItem* item);

  /// @brief デストラクタ
  virtual
  ~GenCaseItemNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // generate case item 文を表すパース木
  const PtGenCaseItem* mItem;
  
};

END_NAMESPACE_YM_VERILOG

#endif // VLPTNODE_STMT_H

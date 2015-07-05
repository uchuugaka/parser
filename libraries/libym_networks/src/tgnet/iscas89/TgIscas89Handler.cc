﻿
/// @file TgIscas89Handler.cc
/// @brief TgIscas89Handler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "TgIscas89Handler.h"
#include "YmNetworks/TgNetwork.h"
#include "YmNetworks/TgNode.h"
#include "../LogicMgr.h"


BEGIN_NAMESPACE_YM_ISCAS89

// @brief コンストラクタ
TgIscas89Handler::TgIscas89Handler()
{
}

// @brief デストラクタ
TgIscas89Handler::~TgIscas89Handler()
{
}

// @brief ネットワークをセットする．
void
TgIscas89Handler::set_network(TgNetwork& network)
{
  mNetwork = &network;
}

// @brief 初期化
bool
TgIscas89Handler::init()
{
  mNetwork->clear();
  mConList.clear();
  return true;
}

// @brief INPUT 文を読み込む．
// @param[in] loc ファイル位置
// @param[in] name_id 入力ピン名のID番号
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TgIscas89Handler::read_input(const FileRegion& loc,
			     ymuint32 name_id)
{
  const char* name = id2str(name_id);
  TgNode* node = mNetwork->find_node(name, true);
  if ( !node->is_undef() ) {
    // 二重定義
    return false;
  }
  mNetwork->set_to_input(node);
  return true;
}

// @brief OUTPUT 文を読み込む．
// @param[in] loc ファイル位置
// @param[in] name_id 出力ピン名のID番号
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TgIscas89Handler::read_output(const FileRegion& loc,
			      ymuint32 name_id)
{
  const char* name = id2str(name_id);
  TgNode* onode = mNetwork->new_node();
  mNetwork->set_to_output(onode);
  TgNode* node = mNetwork->find_node(name, true);
  connect(node, onode, 0);
  return true;
}

// @brief ゲート文を読み込む．
// @param[in] loc ファイル位置
// @param[in] type ゲートの型
// @param[in] oname 出力名
// @param[in] iname_list 入力名のリスト
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TgIscas89Handler::read_gate(const FileRegion& loc,
			    tIscas89GateType type,
			    ymuint32 oname_id,
			    const vector<ymuint32>& iname_list)
{
  const char* oname = id2str(oname_id);
  TgNode* node = mNetwork->find_node(oname, true);
  if ( !node->is_undef() ) {
    // 二重定義
    return false;
  }

  size_t ni = iname_list.size();
  switch ( type ) {
  case kGt89DFF:
    {
      TgNode* ffin = mNetwork->new_node();
      mNetwork->set_to_ff(ffin, node);
      node = ffin;
    }
    break;

  case kGt89BUFF:
    ASSERT_COND(ni == 1 );
    mNetwork->set_to_builtin_logic(node, kTgGateBuff, 1);
    break;

  case kGt89NOT:
    ASSERT_COND(ni == 1 );
    mNetwork->set_to_builtin_logic(node, kTgGateNot, 1);
    break;

  case kGt89AND:
    switch ( ni ) {
    case 0:
    case 1:
      ASSERT_NOT_REACHED;
      break;
    default:
      mNetwork->set_to_builtin_logic(node, kTgGateAnd, ni);
      break;
    }
    break;

  case kGt89NAND:
    switch ( ni ) {
    case 0:
    case 1:
      ASSERT_NOT_REACHED;
      break;
    default:
      mNetwork->set_to_builtin_logic(node, kTgGateNand, ni);
      break;
    }
    break;

  case kGt89OR:
    switch ( ni ) {
    case 0:
    case 1:
      ASSERT_NOT_REACHED;
      break;
    default:
      mNetwork->set_to_builtin_logic(node, kTgGateOr, ni);
      break;
    }
    break;

  case kGt89NOR:
    switch ( ni ) {
    case 0:
    case 1:
      ASSERT_NOT_REACHED;
      break;
    default:
      mNetwork->set_to_builtin_logic(node, kTgGateNor, ni);
      break;
    }
    break;

  case kGt89XOR:
    switch ( ni ) {
    case 0:
    case 1:
      ASSERT_NOT_REACHED;
      break;
    default:
      mNetwork->set_to_builtin_logic(node, kTgGateXor, ni);
      break;
    }
    break;

  case kGt89XNOR:
    switch ( ni ) {
    case 0:
    case 1:
      ASSERT_NOT_REACHED;
      break;
    default:
      mNetwork->set_to_builtin_logic(node, kTgGateXnor, ni);
      break;
    }
    break;
  }

  for (size_t i = 0; i < ni; ++ i) {
    const char* name = id2str(iname_list[i]);
    TgNode* inode = mNetwork->find_node(name, true);
    connect(inode, node, i);
  }

  return true;
}

// @brief 終了処理
void
TgIscas89Handler::normal_exit()
{
  for (vector<Connection>::iterator p = mConList.begin();
       p != mConList.end(); ++ p) {
    const Connection& con = *p;
    mNetwork->connect(con.mFrom, con.mTo, con.mPos);
  }
  mConList.clear();

  mNetwork->wrap_up();
}

// @brief エラー終了時の処理
void
TgIscas89Handler::error_exit()
{
  mNetwork->clear();
}

// 可能なら接続する．
// できれなれば mConList に積む
void
TgIscas89Handler::connect(TgNode* from,
			  TgNode* to,
			  size_t pos)
{
  if ( from->is_undef() ) {
    mConList.push_back(Connection(from, to, pos));
  }
  else {
    mNetwork->connect(from, to, pos);
  }
}

END_NAMESPACE_YM_ISCAS89

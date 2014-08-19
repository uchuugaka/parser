﻿
/// @file atpg/src/matpg/ReadIscas89Cmd.cc
/// @brief ランダムパタン生成を行うコマンド
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ReadIscas89Cmd.cc 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "matpg_config.h"
#endif


#include "ReadIscas89Cmd.h"
#include "MatpgMsgHandler.h"
#include "ym_YmNetworks/TgNetwork.h"
#include "ym_YmTclpp/TclPopt.h"
#include "ym_YmUtils/MsgMgr.h"
#include "ym_YmUtils/StopWatch.h"
#include "matpg.h"
#include "network.h"


BEGIN_NAMESPACE_YM_MATPG

extern
MStopWatch sw_timer;

//////////////////////////////////////////////////////////////////////
// 情報を表示するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ReadIscas89Cmd::ReadIscas89Cmd()
{
  set_usage_string("<ISCAS89(.bench)-filename>");
}

// @brief デストラクタ
ReadIscas89Cmd::~ReadIscas89Cmd()
{
}

// コマンド処理関数
int
ReadIscas89Cmd::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc != 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  int old_tm_id = sw_timer.change(TM_READ);

  string filename = objv[1];
  // ファイル名の展開を行う．
  string ex_filename;
  bool stat1 = tilde_subst(filename, ex_filename);
  if ( !stat1 ) {
    // ファイル名の文字列に誤りがあった．
    return TCL_ERROR;
  }

  MatpgMsgHandler mh;
  MsgMgr::reg_handler(&mh);
  bool stat = mReader(ex_filename, mNetwork);
  sw_timer.change(old_tm_id);

  if ( !stat ) {
    set_result(mh.msg_obj());
    return TCL_ERROR;
  }

  gn_gen(mNetwork);

  return TCL_OK;
}

END_NAMESPACE_YM_MATPG

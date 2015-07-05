﻿
/// @file PrintFaultCmd.cc
/// @brief PrintFaultCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "PrintFaultCmd.h"
#include "AtpgMgr.h"
#include "TpgFault.h"
#include "FaultMgr.h"
#include "YmTclpp/TclPopt.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// 情報を表示するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PrintFaultCmd::PrintFaultCmd(AtpgMgr* mgr) :
  AtpgCmd(mgr)
{
  mPoptType = new TclPoptStr(this, "type",
			     "spefify fault type "
			     "(detected, untestable, remain)");
  set_usage_string("?filename?");
}

// @brief デストラクタ
PrintFaultCmd::~PrintFaultCmd()
{
}

// コマンド処理関数
int
PrintFaultCmd::cmd_proc(TclObjVector& objv)
{
  // このコマンドはファイル名のみを引数に取る．
  // 引数がなければ標準出力に出す．
  ymuint objc = objv.size();
  if ( objc > 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  // 出力先のストリームを開く
  ostream* osp = &cout;
  ofstream ofs;
  if ( objc == 2 ) {
    string filename = objv[1];
    if ( !open_ofile(ofs, filename) ) {
      // ファイルが開けなかった．
      return TCL_ERROR;
    }
    osp = &ofs;
  }

  // 参照を使いたいのでめんどくさいことをやっている．
  ostream& out = *osp;

  FaultMgr& fmgr = _fault_mgr();

  vector<const TpgFault*>::const_iterator b = fmgr.remain_list().begin();
  vector<const TpgFault*>::const_iterator e = fmgr.remain_list().end();

  if ( mPoptType->is_specified() ) {
    string tmp = mPoptType->val();
    if ( tmp == "detected" ) {
      b = fmgr.det_list().begin();
      e = fmgr.det_list().end();
    }
    else if ( tmp == "untestable" ) {
      b = fmgr.untest_list().begin();
      e = fmgr.untest_list().end();
    }
    else if ( tmp == "remain" ) {
      b = fmgr.remain_list().begin();
      e = fmgr.remain_list().end();
    }
    else {
      print_usage();
    }
  }

  for (vector<const TpgFault*>::const_iterator p = b; p != e; ++ p) {
    const TpgFault* f = *p;
    out << f->str() << endl;
  }

  return TCL_OK;
}

END_NAMESPACE_YM_SATPG

﻿
/// @file src/atpg_tclsh/PrintPatCmd.cc
/// @brief ランダムパタン生成を行うコマンド
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "PrintPatCmd.h"
#include "AtpgMgr.h"
#include "TestVector.h"
#include "ym_YmTclpp/TclPopt.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// 情報を表示するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PrintPatCmd::PrintPatCmd(AtpgMgr* mgr) :
  AtpgCmd(mgr)
{
  mPoptHex = new TclPopt(this, "hex",
			 "print in hex mode");
  mPoptNum = new TclPopt(this, "num",
			 "print with index numbering");
}

// @brief デストラクタ
PrintPatCmd::~PrintPatCmd()
{
}

// コマンド処理関数
int
PrintPatCmd::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  bool hex_flag = mPoptHex->is_specified();
  bool num_flag = mPoptNum->is_specified();

  vector<TestVector*>& tvlist = mgr()._tv_list();
  ymuint n = tvlist.size();
  for (ymuint i = 0; i < n; ++ i) {
    TestVector* tv = tvlist[i];
    if ( num_flag ) {
      cout << setw(5) << setfill('0') << (i + 1) << ": ";
    }
    if ( hex_flag ) {
      tv->dump_hex(cout);
    }
    else {
      tv->dump_bin(cout);
    }
    cout << endl;
  }

  return TCL_OK;
}

END_NAMESPACE_YM_SATPG

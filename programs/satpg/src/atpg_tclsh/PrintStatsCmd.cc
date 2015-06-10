﻿
/// @file src/atpg_tclsh/PrintStatsCmd.cc
/// @brief ランダムパタン生成を行うコマンド
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "PrintStatsCmd.h"
#include "TpgNetwork.h"
#include "FaultMgr.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// 情報を表示するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PrintStatsCmd::PrintStatsCmd(AtpgMgr* mgr) :
  AtpgCmd(mgr)
{
  set_usage_string("?filename?");
  mStopWatch.start();
}

// @brief デストラクタ
PrintStatsCmd::~PrintStatsCmd()
{
}

// コマンド処理関数
int
PrintStatsCmd::cmd_proc(TclObjVector& objv)
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

#warning "TODO: ostream を使うようにする．"

  USTime lap = mStopWatch.time();

  USTime r_time = read_time();
  USTime d_time = dtpg_time();
  USTime f_time = fsim_time();
  USTime s_time = sat_time();
  USTime m_time = misc_time();

  FaultMgr& fmgr = _fault_mgr();

  fprintf(stdout, "#A: # of total faults       = %7lu\n", _network().rep_faults().size());
  fprintf(stdout, "#B: # of detected faults    = %7lu\n", fmgr.det_list().size());
  fprintf(stdout, "#C: # of redundant faults   = %7lu\n", fmgr.untest_list().size());
  fprintf(stdout, "#D: # of undetected faults  = %7lu\n", fmgr.remain_list().size());
  fprintf(stdout, "#E: # of generated patterns = %7lu\n", _tv_list().size());
  fprintf(stdout, "#F:   Total CPU time        = %7.2fu %7.2fs\n",
	  lap.usr_time(), lap.sys_time());
  fprintf(stdout, "#G:    (read time           = %7.2f)\n",
	  r_time.usr_time());
  fprintf(stdout, "#H:    (dtpg time           = %7.2f)\n",
	  d_time.usr_time());
  fprintf(stdout, "#I:    (fsim time           = %7.2f)\n",
	  f_time.usr_time());
  fprintf(stdout, "#J:    (sat  time           = %7.2f)\n",
	  s_time.usr_time());
  fprintf(stdout, "#K:    (misc time           = %7.2f)\n",
	  m_time.usr_time());

  return TCL_OK;
}

END_NAMESPACE_YM_SATPG

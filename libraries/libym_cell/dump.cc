
/// @file libym_cell/dump.cc
/// @brief CellLibrary の内容をダンプする関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/Cell.h"
#include "ym_cell/CellLibrary.h"
#include "ym_cell/CellPin.h"
#include "ym_cell/CellTiming.h"
#include "ym_lexp/LogExpr.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELL

BEGIN_NONAMESPACE

/// @brief タイミング情報を書き出す．
/// @param[in] s 出力先のストリーム
/// @param[in] timing タイミング情報
void
dump_timing(ostream& s,
	    const CellTiming* timing)
{
  BinIO::write_double(s, timing->intrinsic_rise().value());
  BinIO::write_double(s, timing->intrinsic_fall().value());
  BinIO::write_double(s, timing->slope_rise().value());
  BinIO::write_double(s, timing->slope_fall().value());
  BinIO::write_double(s, timing->rise_resistance().value());
  BinIO::write_double(s, timing->fall_resistance().value());
}

END_NONAMESPACE

/// @brief 内容をバイナリダンプする．
/// @param[in] s 出力先のストリーム
/// @param[in] library セルライブラリ
void
dump_library(ostream& s,
	     const CellLibrary& library)
{
  // 名前
  BinIO::write_str(s, library.name());

  // セル数
  ymuint nc = library.cell_num();
  BinIO::write_32(s, nc);
  for (ymuint i = 0; i < nc; ++ i) {
    const Cell* cell = library.cell(i);
    BinIO::write_str(s, cell->name());
    BinIO::write_double(s, cell->area().value());

    ymuint np = cell->pin_num();
    BinIO::write_32(s, np);

    ymuint nb = cell->bus_num();
    BinIO::write_32(s, nb);

    ymuint nc = cell->bundle_num();
    BinIO::write_32(s, nc);

    // タイミング情報のID -> 通し番号のマップ
    hash_map<ymuint, ymuint> timing_map;
    // タイミング情報のリスト
    vector<const CellTiming*> timing_list;
    for (ymuint j = 0; j < np; ++ j) {
      const CellPin* pin = cell->pin(j);
      if ( pin->direction() != kDirOutput &&
	   pin->direction() != kDirInout ) {
	continue;
      }
      for (ymuint k = 0; k < np; ++ k) {
	const CellTiming* timing_p = pin->timing(k, kSensePosiUnate);
	if ( timing_p ) {
	  if ( timing_map.count(timing_p->id()) == 0 ) {
	    ymuint pos = timing_list.size();
	    timing_map.insert(make_pair(timing_p->id(), pos));
	    timing_list.push_back(timing_p);
	  }
	}
	const CellTiming* timing_n = pin->timing(k, kSenseNegaUnate);
	if ( timing_n ) {
	  if ( timing_map.count(timing_n->id()) == 0 ) {
	    ymuint pos = timing_list.size();
	    timing_map.insert(make_pair(timing_n->id(), pos));
	    timing_list.push_back(timing_n);
	  }
	}
      }
    }
    ymuint nt = timing_list.size();
    BinIO::write_32(s, nt);
    for (ymuint j = 0; j < nt; ++ j) {
      const CellTiming* timing = timing_list[j];
      dump_timing(s, timing);
    }

    for (ymuint j = 0; j < np; ++ j) {
      const CellPin* pin = cell->pin(j);
      BinIO::write_str(s, pin->name());
      switch ( pin->direction() ) {
      case kDirInput:
	// Input のつもり
	BinIO::write_32(s, 1);
	BinIO::write_double(s, pin->capacitance().value());
	BinIO::write_double(s, pin->rise_capacitance().value());
	BinIO::write_double(s, pin->fall_capacitance().value());
	break;

      case kDirOutput:
	// Output のつもり
	BinIO::write_32(s, 2);
	BinIO::write_double(s, pin->max_fanout().value());
	BinIO::write_double(s, pin->min_fanout().value());
	BinIO::write_double(s, pin->max_capacitance().value());
	BinIO::write_double(s, pin->min_capacitance().value());
	BinIO::write_double(s, pin->max_transition().value());
	BinIO::write_double(s, pin->min_transition().value());
	for (ymuint k = 0; k < np; ++ k) {
	  const CellTiming* timing_p = pin->timing(k, kSensePosiUnate);
	  if ( timing_p ) {
	    hash_map<ymuint, ymuint>::iterator p = timing_map.find(timing_p->id());
	    assert_cond( p != timing_map.end(), __FILE__, __LINE__);
	    BinIO::write_32(s, 1);
	    BinIO::write_32(s, k);
	    BinIO::write_32(s, p->second);
	  }
	  const CellTiming* timing_n = pin->timing(k, kSenseNegaUnate);
	  if ( timing_n ) {
	    hash_map<ymuint, ymuint>::iterator p = timing_map.find(timing_n->id());
	    assert_cond( p != timing_map.end(), __FILE__, __LINE__);
	    BinIO::write_32(s, 2);
	    BinIO::write_32(s, k);
	    BinIO::write_32(s, p->second);
	  }
	}
	BinIO::write_32(s, 0); // timing 情報が終わった印
	break;

      case kDirInout:
	// InOut のつもり
	BinIO::write_32(s, 3);
	BinIO::write_double(s, pin->capacitance().value());
	BinIO::write_double(s, pin->rise_capacitance().value());
	BinIO::write_double(s, pin->fall_capacitance().value());
	BinIO::write_double(s, pin->max_fanout().value());
	BinIO::write_double(s, pin->min_fanout().value());
	BinIO::write_double(s, pin->max_capacitance().value());
	BinIO::write_double(s, pin->min_capacitance().value());
	BinIO::write_double(s, pin->max_transition().value());
	BinIO::write_double(s, pin->min_transition().value());
	for (ymuint k = 0; k < np; ++ k) {
	  const CellTiming* timing_p = pin->timing(k, kSensePosiUnate);
	  if ( timing_p ) {
	    hash_map<ymuint, ymuint>::iterator p = timing_map.find(timing_p->id());
	    assert_cond( p != timing_map.end(), __FILE__, __LINE__);
	    BinIO::write_32(s, 1);
	    BinIO::write_32(s, k);
	    BinIO::write_32(s, p->second);
	  }
	  const CellTiming* timing_n = pin->timing(k, kSenseNegaUnate);
	  if ( timing_n ) {
	    hash_map<ymuint, ymuint>::iterator p = timing_map.find(timing_n->id());
	    assert_cond( p != timing_map.end(), __FILE__, __LINE__);
	    BinIO::write_32(s, 2);
	    BinIO::write_32(s, k);
	    BinIO::write_32(s, p->second);
	  }
	}
	BinIO::write_32(s, 0); // timing 情報が終わった印
	break;

      case kDirInternal:
	// Internal のつもり
	BinIO::write_32(s, 4);
      }
    }
  }
}


BEGIN_NONAMESPACE

// タイミング情報を出力する．
void
display_timing(ostream& s,
	       const Cell* cell,
	       const CellPin* opin,
	       ymuint rpin_id,
	       tCellTimingSense sense)
{
  const CellTiming* timing = opin->timing(rpin_id, sense);
  if ( timing ) {
    s << "    Timing:" << endl
      << "      Related Pin     = " << cell->pin(rpin_id)->name() << endl
      << "      Sense           = ";
    if ( sense == kSensePosiUnate ) {
      s << "positive unate";
    }
    else if ( sense == kSenseNegaUnate ) {
      s << "negative unate";
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
    s << endl
      << "      Rise Intrinsic  = " << timing->intrinsic_rise() << endl
      << "      Rise Resistance = " << timing->rise_resistance() << endl
      << "      Fall Intrinsic  = " << timing->intrinsic_fall() << endl
      << "      Fall Resistance = " << timing->fall_resistance() << endl;
  }
}

END_NONAMESPACE

void
display_library(ostream& s,
		const CellLibrary& library)
{
  ymuint n = library.cell_num();
  for (ymuint i = 0; i < n; ++ i) {
    const Cell* cell = library.cell(i);
    s << "Cell (" << cell->name() << ")" << endl;
    s << "  area = " << cell->area() << endl;

    ymuint np = cell->pin_num();
    for (ymuint j = 0; j < np; ++ j) {
      const CellPin* pin = cell->pin(j);
      s << "  Pin#" << pin->id() << ":" << endl
	<< "    Name             = " << pin->name() << endl;

      switch ( pin->direction() ) {
      case kDirInput:
	s << "    Direction        = INPUT" << endl
	  << "    Capacitance      = " << pin->capacitance() << endl
	  << "    Rise Capacitance = " << pin->rise_capacitance() << endl
	  << "    Fall Capacitance = " << pin->fall_capacitance() << endl;
	break;

      case kDirOutput:
	s << "    Direction        = OUTPUT" << endl;
	if ( pin->has_function() ) {
	  s << "    Function         = " << pin->function() << endl;
	}
	s << "    Max Fanout       = " << pin->max_fanout() << endl
	  << "    Min Fanout       = " << pin->min_fanout() << endl
	  << "    Max Capacitance  = " << pin->max_capacitance() << endl
	  << "    Min Capacitance  = " << pin->min_capacitance() << endl
	  << "    Max Transition   = " << pin->max_transition() << endl
	  << "    Min Transition   = " << pin->min_transition() << endl;
	for (ymuint k = 0; k < np; ++ k) {
	  display_timing(s, cell, pin, k, kSensePosiUnate);
	  display_timing(s, cell, pin, k, kSenseNegaUnate);
	}
	break;

      case kDirInout:
	s << "    Direction        = INOUT" << endl;
	if ( pin->has_function() ) {
	  s << "    Function         = " << pin->function() << endl;
	}
	s << "    Function         = " << pin->function() << endl
	  << "    Capacitance      = " << pin->capacitance() << endl
	  << "    Rise Capacitance = " << pin->rise_capacitance() << endl
	  << "    Fall Capacitance = " << pin->fall_capacitance() << endl
	  << "    Max Fanout       = " << pin->max_fanout() << endl
	  << "    Min Fanout       = " << pin->min_fanout() << endl
	  << "    Max Capacitance  = " << pin->max_capacitance() << endl
	  << "    Min Capacitance  = " << pin->min_capacitance() << endl
	  << "    Max Transition   = " << pin->max_transition() << endl
	  << "    Min Transition   = " << pin->min_transition() << endl;
	for (ymuint k = 0; k < np; ++ k) {
	  display_timing(s, cell, pin, k, kSensePosiUnate);
	  display_timing(s, cell, pin, k, kSenseNegaUnate);
	}
	break;

      case kDirInternal:
	s << "    Direction        = INTERNAL" << endl;
	break;

      default:
	assert_not_reached(__FILE__, __LINE__);
      }
    }
    s << endl;
  }
}

END_NAMESPACE_YM_CELL



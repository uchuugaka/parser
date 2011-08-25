
/// @file dump.cc
/// @brief CellLibrary の内容をダンプする関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/Cell.h"
#include "ym_cell/CellLibrary.h"
#include "ym_cell/CellPin.h"
#include "ym_cell/CellTiming.h"

#include "ym_logic/LogExpr.h"
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
    ymuint tid = 0;
    switch ( cell->type() ) {
    case Cell::kLogic: tid = 0; break;
    case Cell::kFF:    tid = 1; break;
    case Cell::kLatch: tid = 2; break;
    case Cell::kFSM:   tid = 3; break;
    default: assert_not_reached(__FILE__, __LINE__); break;
    }
    BinIO::write_8(s, tid);
    BinIO::write_str(s, cell->name());
    BinIO::write_double(s, cell->area().value());

    ymuint ni = cell->input_num();
    BinIO::write_32(s, ni);

    ymuint no = cell->output_num();
    BinIO::write_32(s, no);

    ymuint nio = cell->output_num();
    BinIO::write_32(s, nio);

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
      if ( !pin->is_output() && !pin->is_inout() ) {
	continue;
      }
      for (ymuint k = 0; k < np; ++ k) {
	const CellTiming* timing_p = pin->timing(k, kCellPosiUnate);
	if ( timing_p ) {
	  if ( timing_map.count(timing_p->id()) == 0 ) {
	    ymuint pos = timing_list.size();
	    timing_map.insert(make_pair(timing_p->id(), pos));
	    timing_list.push_back(timing_p);
	  }
	}
	const CellTiming* timing_n = pin->timing(k, kCellNegaUnate);
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

    // 入力ピンのダンプ
    for (ymuint ipin = 0; ipin < ni; ++ ipin) {
      const CellPin* pin = cell->input(ipin);
      BinIO::write_str(s, pin->name());
      // Input のつもり
      BinIO::write_8(s, 1);
      BinIO::write_double(s, pin->capacitance().value());
      BinIO::write_double(s, pin->rise_capacitance().value());
      BinIO::write_double(s, pin->fall_capacitance().value());
    }
    // 出力ピンのダンプ
    for (ymuint opin = 0; opin < no; ++ opin) {
      const CellPin* pin = cell->output(opin);
      BinIO::write_str(s, pin->name());
      // Output のつもり
      BinIO::write_8(s, 2);
      BinIO::write_double(s, pin->max_fanout().value());
      BinIO::write_double(s, pin->min_fanout().value());
      BinIO::write_double(s, pin->max_capacitance().value());
      BinIO::write_double(s, pin->min_capacitance().value());
      BinIO::write_double(s, pin->max_transition().value());
      BinIO::write_double(s, pin->min_transition().value());
    }
    // 入出力ピンのダンプ
    for (ymuint iopin = 0; iopin < nio; ++ iopin) {
      const CellPin* pin = cell->output(iopin);
      BinIO::write_str(s, pin->name());
      // Inout のつもり
      BinIO::write_8(s, 3);
      BinIO::write_double(s, pin->capacitance().value());
      BinIO::write_double(s, pin->rise_capacitance().value());
      BinIO::write_double(s, pin->fall_capacitance().value());
      BinIO::write_double(s, pin->max_fanout().value());
      BinIO::write_double(s, pin->min_fanout().value());
      BinIO::write_double(s, pin->max_capacitance().value());
      BinIO::write_double(s, pin->min_capacitance().value());
      BinIO::write_double(s, pin->max_transition().value());
      BinIO::write_double(s, pin->min_transition().value());
    }
    // タイミング情報のダンプ
    for (ymuint ipin = 0; ipin < ni + nio; ++ ipin) {
      for (ymuint opin = 0; opin < no + nio; ++ opin) {
	const CellTiming* timing_p = cell->timing(ipin, opin, kCellPosiUnate);
	if ( timing_p ) {
	  hash_map<ymuint, ymuint>::iterator p = timing_map.find(timing_p->id());
	  assert_cond( p != timing_map.end(), __FILE__, __LINE__);
	  BinIO::write_8(s, 1);
	  BinIO::write_32(s, ipin);
	  BinIO::write_32(s, opin);
	  BinIO::write_32(s, p->second);
	}
	const CellTiming* timing_n = cell->timing(ipin, opin, kCellNegaUnate);
	if ( timing_n ) {
	  hash_map<ymuint, ymuint>::iterator p = timing_map.find(timing_n->id());
	  assert_cond( p != timing_map.end(), __FILE__, __LINE__);
	  BinIO::write_8(s, 1);
	  BinIO::write_32(s, ipin);
	  BinIO::write_32(s, opin);
	  BinIO::write_32(s, p->second);
	}
      }
    }
    BinIO::write_32(s, 0); // timing 情報が終わった印
  }
}


BEGIN_NONAMESPACE

// タイミング情報を出力する．
void
display_timing(ostream& s,
	       const Cell* cell,
	       const CellPin* opin,
	       ymuint rpin_id,
	       CellPin::tTimingSense sense)
{
  const CellTiming* timing = opin->timing(rpin_id, sense);
  if ( timing ) {
    s << "    Timing:" << endl
      << "      Related Pin     = " << cell->pin(rpin_id)->name() << endl
      << "      Sense           = ";
    if ( sense == kCellPosiUnate ) {
      s << "positive unate";
    }
    else if ( sense == kCellNegaUnate ) {
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
    s << "Cell#" << cell->id() << " (" << cell->name() << ")" << endl;
    s << "  area = " << cell->area() << endl;

    if ( cell->is_ff() ) {
      s << "  ff ( " << cell->var1_name()
	<< ", " << cell->var2_name() << " )" << endl
	<< "    next_state        = " << cell->next_state() << endl
	<< "    clocked_on        = " << cell->clocked_on() << endl;
      if ( !cell->clocked_on_also().is_zero() ) {
	s << "    clocked_on_also   = " << cell->clocked_on_also() << endl;
      }
      if ( !cell->clear().is_zero() ) {
	s << "    clear             = " << cell->clear() << endl;
      }
      if ( !cell->preset().is_zero() ) {
	s << "    preset            = " << cell->preset() << endl;
      }
      if ( !cell->clear().is_zero() && !cell->preset().is_zero() ) {
	s << "    clear_preset_var1 = " << cell->clear_preset_var1() << endl
	  << "    clear_preset_var2 = " << cell->clear_preset_var2() << endl;
      }
    }
    else if ( cell->is_latch() ) {
      s << "  latch ( " << cell->var1_name()
	<< ", " << cell->var2_name() << " )" << endl
	<< "    data_in           = " << cell->data_in() << endl
	<< "    enable            = " << cell->enable() << endl;
      if ( !cell->enable().is_zero() ) {
	s << "    enable_also       = " << cell->enable_also() << endl;
      }
      if ( !cell->clear().is_zero() ) {
	s << "    clear             = " << cell->clear() << endl;
      }
      if ( !cell->preset().is_zero() ) {
	s << "    preset            = " << cell->preset() << endl;
      }
      if ( !cell->clear().is_zero() && !cell->preset().is_zero() ) {
	s << "    clear_preset_var1 = " << cell->clear_preset_var1() << endl
	  << "    clear_preset_var2 = " << cell->clear_preset_var2() << endl;
      }
    }

    ymuint np = cell->pin_num();
    for (ymuint j = 0; j < np; ++ j) {
      const CellPin* pin = cell->pin(j);
      s << "  Pin#" << pin->id() << ":" << endl
	<< "    Name             = " << pin->name() << endl;

      switch ( pin->direction() ) {
      case CellPin::kDirInput:
	s << "    Direction        = INPUT" << endl
	  << "    Capacitance      = " << pin->capacitance() << endl
	  << "    Rise Capacitance = " << pin->rise_capacitance() << endl
	  << "    Fall Capacitance = " << pin->fall_capacitance() << endl;
	break;

      case CellPin::kDirOutput:
	s << "    Direction        = OUTPUT" << endl;
	if ( pin->has_function() ) {
	  s << "    Function         = " << pin->function() << endl;
	}
	if ( pin->has_three_state() ) {
	  s << "    Three State      = " << pin->three_state() << endl;
	}
	s << "    Max Fanout       = " << pin->max_fanout() << endl
	  << "    Min Fanout       = " << pin->min_fanout() << endl
	  << "    Max Capacitance  = " << pin->max_capacitance() << endl
	  << "    Min Capacitance  = " << pin->min_capacitance() << endl
	  << "    Max Transition   = " << pin->max_transition() << endl
	  << "    Min Transition   = " << pin->min_transition() << endl;
	for (ymuint k = 0; k < np; ++ k) {
	  display_timing(s, cell, pin, k, kCellPosiUnate);
	  display_timing(s, cell, pin, k, kCellNegaUnate);
	}
	break;

      case CellPin::kDirInout:
	s << "    Direction        = INOUT" << endl;
	if ( pin->has_function() ) {
	  s << "    Function         = " << pin->function() << endl;
	}
	if ( pin->has_three_state() ) {
	  s << "    Three State      = " << pin->three_state() << endl;
	}
	s << "    Capacitance      = " << pin->capacitance() << endl
	  << "    Rise Capacitance = " << pin->rise_capacitance() << endl
	  << "    Fall Capacitance = " << pin->fall_capacitance() << endl
	  << "    Max Fanout       = " << pin->max_fanout() << endl
	  << "    Min Fanout       = " << pin->min_fanout() << endl
	  << "    Max Capacitance  = " << pin->max_capacitance() << endl
	  << "    Min Capacitance  = " << pin->min_capacitance() << endl
	  << "    Max Transition   = " << pin->max_transition() << endl
	  << "    Min Transition   = " << pin->min_transition() << endl;
	for (ymuint k = 0; k < np; ++ k) {
	  display_timing(s, cell, pin, k, kCellPosiUnate);
	  display_timing(s, cell, pin, k, kCellNegaUnate);
	}
	break;

      case CellPin::kDirInternal:
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

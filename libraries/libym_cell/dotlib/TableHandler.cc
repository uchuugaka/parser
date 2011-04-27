
/// @file libym_cell/dotlib/TableHandler.cc
/// @brief TableHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "TableHandler.h"

#include "SimpleHandler.h"
#include "ComplexHandler.h"
#include "GroupHandler.h"
#include "TimingHandler.h"

#include "PtMgr.h"
#include "PtNode.h"
#include "PtPin.h"
#include "PtTable.h"
#include "PtValue.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

BEGIN_NONAMESPACE

inline
SimpleHandler*
new_simple(GroupHandler* parent)
{
  return new SimpleHandler(parent);
}

inline
ComplexHandler*
new_complex(GroupHandler* parent)
{
  return new ComplexHandler(parent);
}

inline
GroupHandler*
new_group(GroupHandler* parent)
{
  return new GenGroupHandler(parent);
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス TableHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
TableHandler::TableHandler(GroupHandler* parent) :
  GroupHandler(parent),
  mTable(NULL)
{
  // simple attributes

  // complex attribute
  DotlibHandler* complex = new_complex(this);
  reg_handler("index_1", complex);
  reg_handler("index_2", complex);
  reg_handler("index_3", complex);
  reg_handler("values", complex);

  // group statements
  reg_handler("domain", new_group(this));
}

// @brief デストラクタ
TableHandler::~TableHandler()
{
}

// @brief index_x 属性をセットする．
// @param[in] attr_name 属性名
// @param[in] value_list 値のリスト
bool
TableHandler::add_index_x(const ShString& attr_name,
			  const vector<const PtValue*>& value_list)
{
  return true;
}

// @brief values 属性をセットする．
// @param[in] attr_name 属性名
// @param[in] value_list 値のリスト
bool
TableHandler::add_values(const vector<const PtValue*>& value_list)
{
  return true;
}

// @brief domain グループをセットする．
// @param[in] domain ドメイン
bool
TableHandler::add_domain(PtDomain* domain)
{
  return true;
}

// @brief group statement の最初に呼ばれる関数
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value_list 値を表すトークンのリスト
bool
TableHandler::begin_group(const ShString& attr_name,
			  const FileRegion& attr_loc,
			  const vector<const PtValue*>& value_list)
{
  if ( value_list.size() != 1 ) {
    FileRegion loc;
    if ( value_list.size() > 1 ) {
      loc = value_list[1]->loc();
    }
    else {
      loc = attr_loc;
    }
    ostringstream buf;
    buf << attr_name << " group requires just one string as a parameter.";
    put_msg(__FILE__, __LINE__, loc,
	    kMsgError,
	    "DOTLIBPARSER",
	    buf.str());
    return false;
  }

  mTable = ptmgr().new_pttable(value_list[0]);
  parent()->add_table(attr_name, mTable);

  return true;
}

// @brief group statement の最後に呼ばれる関数
bool
TableHandler::end_group()
{
  mTable = NULL;
  return true;
}

END_NAMESPACE_YM_CELL_DOTLIB
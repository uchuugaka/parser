
/// @file libym_logic/tests/dimacstest.cc
/// @brief DIMACS CNF ファイルの読み込みテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: dimacstest.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/DimacsParser.h"
#include "ym_logic/DimacsHandler.h"

#include "ym_utils/FileIDO.h"


BEGIN_NAMESPACE_YM
BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
/// @class TestDimacsHandler parsertest.cc
/// @brief テスト用の DimacsHandler
//////////////////////////////////////////////////////////////////////
class TestDimacsHandler :
  public DimacsHandler
{
public:

  /// @brief コンストラクタ
  TestDimacsHandler();

  /// @brief デストラクタ
  virtual
  ~TestDimacsHandler();


public:

  /// @brief 初期化
  virtual
  bool
  init();

  /// @brief p 行の読込み
  /// @param[in] loc ファイル上の位置情報
  /// @param[in] nv 変数の数
  /// @param[in] nc 節の数
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_p(const FileRegion& loc,
	 ymuint nv,
	 ymuint nc);

  /// @brief clause 行の読込み
  /// @param[in] loc ファイル上の位置情報
  /// @param[in] lits リテラルの配列．最後の0は含まない
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_clause(const FileRegion& loc,
	      const vector<int>& lits);

  /// @brief 終了処理
  virtual
  bool
  end();

  /// @brief エラー終了時の処理
  virtual
  void
  error_exit();

};


#if 0
//////////////////////////////////////////////////////////////////////
// テスト用の DimacsMsgHandler
//////////////////////////////////////////////////////////////////////
class TestDimacsMsgHandler :
  public DimacsMsgHandler
{
public:

  /// @brief コンストラクタ
  TestDimacsMsgHandler();

  /// @brief デストラクタ
  virtual
  ~TestDimacsMsgHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスは下記の仮想関数を上書きすること
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] lineno ファイル中の行番号
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  virtual
  void operator()(const char* src_file,
		  int src_line,
		  int lineno,
		  const string& label,
		  const string& body);
};
#endif

// @brief コンストラクタ
TestDimacsHandler::TestDimacsHandler()
{
}

// @brief デストラクタ
TestDimacsHandler::~TestDimacsHandler()
{
}

// @brief 初期化
bool
TestDimacsHandler::init()
{
  return true;
}

// @brief p 行の読込み
// @param[in] loc ファイル上の位置情報
// @param[in] nv 変数の数
// @param[in] nc 節の数
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TestDimacsHandler::read_p(const FileRegion& loc,
			  ymuint nv,
			  ymuint nc)
{
  cout << "read_p(" << nv << ", " << nc << ")@" << loc << endl;
  return true;
}

// @brief clause 行の読込み
// @param[in] loc ファイル上の位置情報
// @param[in] lits リテラルの配列．最後の0は含まない
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TestDimacsHandler::read_clause(const FileRegion& loc,
			       const vector<int>& lits)
{
  cout << "read_clause(";
  const char* comma = "";
  for (vector<int>::const_iterator p = lits.begin();
       p != lits.end(); ++ p) {
    cout << comma << *p;
    comma = ", ";
  }
  cout << ")@" << loc << endl;
  return true;
}

// @brief 終了処理
// @param[in] loc 位置情報
bool
TestDimacsHandler::end()
{
  cout << "end()" << endl;
  return true;
}

// @brief エラー終了時の処理
void
TestDimacsHandler::error_exit()
{
}

#if 0
// @brief コンストラクタ
TestDimacsMsgHandler::TestDimacsMsgHandler()
{
}

// @brief デストラクタ
TestDimacsMsgHandler::~TestDimacsMsgHandler()
{
}

// @brief メッセージが登録されるたびに呼ばれる仮想関数
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] lineno ファイル中の行番号
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
TestDimacsMsgHandler::operator()(const char* src_file,
				 int src_line,
				 int lineno,
				 const string& label,
				 const string& body)
{
  cerr << "Line " << lineno << ": [" << label << "]: " << body << endl;
}
#endif

END_NONAMESPACE
END_NAMESPACE_YM

int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  if ( argc != 2 ) {
    cerr << "USAGE : " << argv[0] << " cnf-file" << endl;
    return 2;
  }

  try {
    DimacsParser parser;
    TestDimacsHandler handler;
#if 0
    TestDimacsMsgHandler msg_handler;
#endif
    parser.add_handler(&handler);
#if 0
    parser.add_msg_handler(&msg_handler);
#endif

    FileIDO ido;
    if ( !ido.open(argv[1]) ) {
      cerr << "Could not open " << argv[1] << endl;
      return -1;
    }
    if ( !parser.read(ido) ) {
      cerr << "Error in reading " << argv[1] << endl;
      return 4;
    }
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}

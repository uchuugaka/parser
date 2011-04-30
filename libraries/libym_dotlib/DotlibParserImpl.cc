
/// @file libym_dotlib/DotlibParserImpl.cc
/// @brief DotlibParserImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DotlibParser.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibParserImpl.h"
#include "DotlibHandler.h"
#include "HandlerFactory.h"
#include "ym_utils/ShString.h"
#include "PtNodeImpl.h"


BEGIN_NAMESPACE_YM_DOTLIB

// @brief コンストラクタ
// @param[in] msg_mgr メッセージを管理するオブジェクト
DotlibParserImpl::DotlibParserImpl(MsgMgr& msg_mgr) :
  mMsgMgr(msg_mgr),
  mLibraryHandler( HandlerFactory::new_library(*this) )
{
  init();
}

// デストラクタ
DotlibParserImpl::~DotlibParserImpl()
{
  close_file();

  delete mLibraryHandler;
}

// @brief ファイルを読み込む．
// @param[in] filename ファイル名
// @param[in] debug デバッグモード
// @param[in] allow_no_semi 行末のセミコロンなしを許すかどうか
// @return パース木の根のノードを返す．
// @note エラーが起きたら NULL を返す．
const DotlibNode*
DotlibParserImpl::read_file(const string& filename,
			    bool debug,
			    bool allow_no_semi)
{
  mDebug = debug;
  mAllowNoSemi = allow_no_semi;

  if ( !open_file(filename) ) {
    return NULL;
  }

  bool error = false;
  tTokenType type;
  // 空行を読み飛ばす．
  for (type = read_token(); type == NL; type = read_token()) { }
  ShString name(cur_string());
  FileRegion loc = cur_loc();
  if ( type != SYMBOL || name != "library" ) {
    put_msg(__FILE__, __LINE__, loc,
	    kMsgError,
	    "DOTLIB_PARSER",
	    "'library' keyword is expected "
	    "on the top of the structure");
    error = true;
    goto last;
  }

  if ( !mLibraryHandler->read_attr(name, loc) ) {
    error = true;
    goto last;
  }

  if ( !expect_nl() ) {
    error = true;
    goto last;
  }
  for ( ; ; ) {
    tTokenType type = read_token();
    if ( type == END ) {
      break;
    }
    put_msg(__FILE__, __LINE__, cur_loc(),
	    kMsgWarning,
	    "DOTLIB_PARSER",
	    "contents after library group are ignored.");
  }

last:
  close_file();

  if ( error ) {
    return NULL;
  }

  const DotlibNode* library = mLibraryHandler->pt_node();
  const DotlibNode* root = mPtMgr.new_attr(ShString("library"), library, library->loc());
  return root;
}

// @brief 直前の read_file() で確保したパース木を解放する．
void
DotlibParserImpl::clear_node()
{
  mPtMgr.clear();
}

// @brief メモリ使用量のサマリを出力する．
// @param[in] s 出力先のストリーム
void
DotlibParserImpl::show_stats(ostream& s)
{
  mPtMgr.show_stats(s);
}

// @brief 引数の種類のトークンでなければエラーメッセージを出力する．
bool
DotlibParserImpl::expect(tTokenType req_type)
{
  tTokenType type = read_token();
  if ( type == req_type ) {
    return true;
  }

  const char* type_str = NULL;
  switch ( req_type ) {
  case COLON:      type_str = "':'"; break;
  case SEMI:       type_str = "';'"; break;
  case COMMA:      type_str = "','"; break;
  case PLUS:       type_str = "'+'"; break;
  case MINUS:      type_str = "'-'"; break;
  case MULT:       type_str = "'*'"; break;
  case DIV:        type_str = "'/'"; break;
  case LP:         type_str = "'('"; break;
  case RP:         type_str = "')'"; break;
  case LCB:        type_str = "'{'"; break;
  case RCB:        type_str = "'}'"; break;
  case SYMBOL:     type_str = "STR"; break;
  case INT_NUM:    type_str = "INT"; break;
  case FLOAT_NUM:  type_str = "FLOAT"; break;
  case EXPRESSION: type_str = "EXPRESSION"; break;
  case NL:         type_str = "new-line"; break;
  case ERROR:      assert_not_reached(__FILE__, __LINE__);
  case END:        assert_not_reached(__FILE__, __LINE__);
  }
  ostringstream buf;
  buf << "syntax error. " << type_str << " is expected.";
  put_msg(__FILE__, __LINE__, cur_loc(),
	  kMsgError,
	  "DOTLIB_PARSER",
	  buf.str());
  return false;
}

// @brief 行末まで読み込む．
bool
DotlibParserImpl::expect_nl()
{
  if ( mAllowNoSemi ) {
    tTokenType type = read_token();
    if ( type == SEMI ) {
      type = read_token();
    }
    if ( type == NL || type == END ) {
      return true;
    }
    ostringstream buf;
    put_msg(__FILE__, __LINE__, cur_loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "Syntax error. Semicolon is expected.");
    return false;
  }
  else {
    if ( !expect(SEMI) ) {
      return false;
    }
    if ( !expect(NL) ) {
      return false;
    }
  }
  return true;
}

// @brief 直前の read_token() に対応する整数値を返す．
// @note 型が INT_NUM でなかったときの値は不定
int
DotlibParserImpl::cur_int() const
{
  return strtol(cur_string(), NULL, 10);
}

// @brief 直前の read_token() に対応する実数値を返す．
// @note 型が FLOAT_NUM/INT_NUM でなかったときの値は不定
double
DotlibParserImpl::cur_float() const
{
  return strtod(cur_string(), NULL);
}

// @brief メッセージ出力管理オブジェクトを返す．
MsgMgr&
DotlibParserImpl::msg_mgr()
{
  return mMsgMgr;
}

// @brief メッセージを出力する．
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
DotlibParserImpl::put_msg(const char* src_file,
			  int src_line,
			  const FileRegion& file_loc,
			  tMsgType type,
			  const char* label,
			  const char* msg)
{
  mMsgMgr.put_msg(src_file, src_line, file_loc, type, label, msg);
}

// @brief メッセージを出力する．
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
DotlibParserImpl::put_msg(const char* src_file,
			  int src_line,
			  const FileRegion& file_loc,
			  tMsgType type,
			  const char* label,
			  const string& msg)
{
  mMsgMgr.put_msg(src_file, src_line, file_loc, type, label, msg);
}

// @brief パース木を管理するオブジェクトを返す．
PtMgr&
DotlibParserImpl::pt_mgr()
{
  return mPtMgr;
}

// @brief デバッグモードの時 true を返す．
bool
DotlibParserImpl::debug()
{
  return mDebug;
}

// ファイルをセットする．
// 自動的にに clear() を呼ぶ．
// ファイルのオープンに失敗したら false を返す．
bool
DotlibParserImpl::open_file(const string& filename)
{
  init();
  mInput.open(filename.c_str());
  if ( !mInput ) {
    return false;
  }
  mCurFileDesc = mFileDescMgr.new_file_desc(filename);
  return true;
}

// ファイルをクローズする．
void
DotlibParserImpl::close_file()
{
  mInput.close();
  mCurFileDesc = NULL;
}

// 初期化
void
DotlibParserImpl::init()
{
  mCR = false;
  mCurChar = 0;
  mCurLine = 1;
  mCurColumn = 0;
  mNeedUpdate = true;
}

// @brief トークンを一つとってくる．
// @param[in] symbol_mode 数字も文字とみなすモード
tTokenType
DotlibParserImpl::read_token(bool symbol_mode)
{
  mSymbolMode = symbol_mode;

  int c;

  mCurString.clear();

 ST_INIT: // 初期状態
  c = get();
  mFirstColumn = cur_column();

  if ( is_symbol(c) ) {
    accept(c);
    goto ST_ID;
  }
  if ( isdigit(c) ) {
    accept(c);
    goto ST_NUM1;
  }

  switch (c) {
  case '.':
    accept(c);
    goto ST_DOT;

  case EOF:
    return END;

  case ' ':
  case '\t':
    goto ST_INIT; // 最初の空白は読み飛ばす．

  case '\n':
    return NL;

  case '\"':
    goto ST_DQ;

  case '\\':
    c = get();
    if ( c == '\n' ) {
      // 無視する．
      goto ST_INIT;
    }
    // それ以外はバックスラッシュがなかったことにする．
    unget();
    goto ST_INIT;

  case '/':
    goto ST_COMMENT1;

  case ':':
    return COLON;

  case ';':
    return SEMI;

  case ',':
    return COMMA;

  case '+':
    return PLUS;

  case '-':
    goto ST_MINUS;

  case '*':
    return MULT;

  case '(':
    return LP;

  case ')':
    return RP;

  case '{':
    return LCB;

  case '}':
    return RCB;

  default:
    // それ以外はエラーなんじゃない？
    put_msg(__FILE__, __LINE__, cur_loc(),
	    kMsgError,
	    "DOTLIB_LEX",
	    "syntax error");
    return ERROR;
  }
  assert_not_reached(__FILE__, __LINE__);

 ST_MINUS: // '-' を読み込んだ時
  c = get();
  if ( isdigit(c) ) {
    mCurString.put_char('-');
    accept(c);
    goto ST_NUM1;
  }
  unget();
  return MINUS;

 ST_NUM1: // 一文字目が[0-9]の時
  c = get();
  if ( isdigit(c) ) {
    accept(c);
    goto ST_NUM1;
  }
  if ( c == '.' ) {
    accept(c);
    goto ST_DOT;
  }
  unget();
  return INT_NUM;

 ST_DOT: // [0-9]*'.' を読み込んだ時
  c = get();
  if ( isdigit(c) ) {
    accept(c);
    goto ST_NUM2;
  }
  { // '.' の直後はかならず数字
    ostringstream buf;
    buf << "digit number expected after dot";
    put_msg(__FILE__, __LINE__, cur_loc(),
	    kMsgError,
	    "DOTLIB_LEX",
	    buf.str());
    return ERROR;
  }

 ST_NUM2: // [0-9]*'.'[0-9]* を読み込んだ時
  c = get();
  if ( isdigit(c) ) {
    accept(c);
    goto ST_NUM2;
  }
  if ( c == 'e' || c == 'E' ) {
    accept(c);
    goto ST_NUM3;
  }
  unget();
  return FLOAT_NUM;

 ST_NUM3: // [0-9]*'.'[0-9]*(e|E)を読み込んだ時
  c = get();
  if ( isdigit(c) ) {
    accept(c);
    goto ST_NUM4;
  }
  if ( c == '+' || c == '-' ) {
    accept(c);
    goto ST_NUM4;
  }
  { // (e|E) の直後はかならず数字か符号
    ostringstream buf;
    buf << "exponent value expected";
    put_msg(__FILE__, __LINE__, cur_loc(),
	    kMsgError,
	    "DOTLIB_LEX",
	    buf.str());
    return ERROR;
  }

 ST_NUM4: // [0-9]*'.'[0-9]*(e|E)(+|-)?[0-9]*を読み込んだ直後
  c = get();
  if ( isdigit(c) ) {
    accept(c);
    goto ST_NUM4;
  }
  unget();
  return FLOAT_NUM;

 ST_ID: // 一文字目が[a-zA-Z_]の時
  c = get();
  if ( is_symbol(c) || isdigit(c) ) {
    accept(c);
    goto ST_ID;
  }
  unget();
  return SYMBOL;

 ST_DQ: // "があったら次の"までを強制的に文字列だと思う．
  c = get();
  if ( c == '\"' ) {
    return SYMBOL;
  }
  if ( c == '\\' ) {
    c = get();
    if ( c == '\n' ) {
      accept(c);
    }
    // それ以外はバックスラッシュを無視する．
    accept(c);
  }
  else if ( c == '\n' ) {
    ostringstream buf;
    buf << "unexpected newline in quoted string.";
    put_msg(__FILE__, __LINE__, cur_loc(),
	    kMsgError,
	    "DOTLIB_LEX",
	    buf.str());
    return ERROR;
  }
  if ( c == EOF ) {
    ostringstream buf;
    buf << "unexpected end-of-file in quoted string.";
    mMsgMgr.put_msg(__FILE__, __LINE__, cur_loc(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
    return ERROR;
  }
  accept(c);
  goto ST_DQ;

 ST_COMMENT1: // '/' を読み込んだ直後
  c = get();
  if ( c == '/' ) { // C++ スタイルのコメント
    goto ST_COMMENT2;
  }
  if ( c == '*' ) { // C スタイルのコメント
    goto ST_COMMENT3;
  }
  unget();
  return DIV;

 ST_COMMENT2: // 改行まで読み飛ばす．
  c = get();
  if ( c == '\n' ) {
    goto ST_INIT;
  }
  if ( c == EOF ) {
    return END;
  }
  goto ST_COMMENT2;

 ST_COMMENT3: // "/*" を読み込んだ直後
  c = get();
  if ( c == EOF ) {
    goto ST_EOF;
  }
  if ( c == '*' ) {
    goto ST_COMMENT4;
  }
  goto ST_COMMENT3;

 ST_COMMENT4: // "/* 〜 *" まで読み込んだ直後
  c = get();
  if ( c == EOF ) {
    goto ST_EOF;
  }
  if ( c == '/' ) {
    goto ST_INIT;
  }
  if ( c == '*' ) {
    goto ST_COMMENT4;
  }
  goto ST_COMMENT3;

 ST_EOF:
  {
    ostringstream buf;
    buf << "Unexpected end-of-file in comment block.";
    put_msg(__FILE__, __LINE__, cur_loc(),
	    kMsgError,
	    "DOTLIB_LEX",
	    buf.str());
  }
  return ERROR;
}

// @brief c が文字の時に true を返す．
// @note mSymbolMode が true なら数字も文字とみなす．
bool
DotlibParserImpl::is_symbol(int c)
{
  if ( isalpha(c) || c == '_' ) {
    return true;
  }
  if ( mSymbolMode ) {
    if ( isdigit(c) || c == '.' ) {
      return true;
    }
  }
  return false;
}

// @brief 文字を受け入れる．
void
DotlibParserImpl::accept(int c)
{
  mCurString.put_char(c);
  mLastColumn = cur_column();
}

// 一文字読み出す．
int
DotlibParserImpl::get()
{
  if ( mNeedUpdate ) {
    int c = 0;
    for ( ; ; ) {
      c = mInput.get();
      if ( c == EOF ) {
	break;
      }
      // Windows(DOS)/Mac/UNIX の間で改行コードの扱いが異なるのでここで
      // 強制的に '\n' に書き換えてしまう．
      // Windows : '\r', '\n'
      // Mac     : '\r'
      // UNIX    : '\n'
      // なので '\r' を '\n' に書き換えてしまう．
      // ただし次に本当の '\n' が来たときには無視するために
      // mCR を true にしておく．
      if ( c == '\r' ) {
	mCR = true;
	c = '\n';
	break;
      }
      if ( c == '\n' ) {
	if ( mCR ) {
	  // 直前に '\r' を読んで '\n' を返していたので今の '\n' を
	  // 無視する．これが唯一ループを回る条件
	  mCR = false;
	  continue;
	}
	break;
      }
      // 普通の文字の時はそのまま返す．
      mCR = false;
      break;
    }
    if ( mCurChar == '\n' ) {
      ++ mCurLine;
      mCurColumn = 1;
    }
    else {
      ++ mCurColumn;
    }
    mCurChar = c;
  }
  mNeedUpdate = true;
  return mCurChar;
}

// 一文字読み戻す．
void
DotlibParserImpl::unget()
{
  mNeedUpdate = false;
}

END_NAMESPACE_YM_DOTLIB
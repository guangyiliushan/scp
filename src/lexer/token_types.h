// 标记类型定义头文件

#ifndef TOKEN_TYPES_H
#define TOKEN_TYPES_H

// 标记类型枚举
typedef enum {
    // 基本标记
    TOKEN_UNKNOWN,       // 未知标记
    TOKEN_EOF,           // 文件结束
    TOKEN_IDENTIFIER,    // 标识符
    TOKEN_NUMBER,        // 数字
    TOKEN_FLOAT,         // 浮点数
    TOKEN_STRING,        // 字符串
    TOKEN_CHAR,          // 字符
    
    // 硬关键字 (Hard Keywords)
    TOKEN_KEYWORD_FUN,    // fun
    TOKEN_KEYWORD_IF,     // if
    TOKEN_KEYWORD_ELSE,   // else
    TOKEN_KEYWORD_WHILE,  // while
    TOKEN_KEYWORD_FOR,    // for
    TOKEN_KEYWORD_BREAK,  // break
    TOKEN_KEYWORD_CONTINUE,// continue
    TOKEN_KEYWORD_RETURN, // return
    TOKEN_KEYWORD_INCLUDE,// include
    TOKEN_KEYWORD_CLASS,  // class
    TOKEN_KEYWORD_ENUM,   // enum
    TOKEN_KEYWORD_STRUCT, // struct
    TOKEN_KEYWORD_OBJ,    // obj
    TOKEN_KEYWORD_WHEN,   // when
    TOKEN_KEYWORD_MATCH,  // match
    TOKEN_KEYWORD_DO,     // do
    TOKEN_KEYWORD_YIELD,  // yield
    TOKEN_KEYWORD_TRY,    // try
    TOKEN_KEYWORD_CATCH,  // catch
    TOKEN_KEYWORD_FINALLY,// finally
    TOKEN_KEYWORD_THROW,  // throw
    TOKEN_KEYWORD_IS,     // is
    TOKEN_KEYWORD_AS,     // as
    TOKEN_KEYWORD_IN,     // in
    TOKEN_KEYWORD_THIS,   // this
    TOKEN_KEYWORD_SUPER,  // super
    TOKEN_KEYWORD_OPERATOR,// operator
    TOKEN_KEYWORD_NULL,   // null
    TOKEN_KEYWORD_TRUE,   // true
    TOKEN_KEYWORD_FALSE,  // false
    
    // 软关键字 (Soft Keywords)
    TOKEN_KEYWORD_ANNOTATION, // annotation
    TOKEN_KEYWORD_SEALED,   // sealed
    TOKEN_KEYWORD_DATA,     // data
    TOKEN_KEYWORD_COMPANION,// companion
    TOKEN_KEYWORD_WHERE,    // where
    TOKEN_KEYWORD_ASYNC,    // async
    TOKEN_KEYWORD_AWAIT,    // await
    TOKEN_KEYWORD_BY,       // by
    TOKEN_KEYWORD_MACRO,    // macro
    TOKEN_KEYWORD_UNSAFE,   // unsafe
    TOKEN_KEYWORD_ACTUAL,   // actual
    TOKEN_KEYWORD_CRATE,    // crate
    TOKEN_KEYWORD_USE,      // use
    TOKEN_KEYWORD_MOD,      // mod
    TOKEN_KEYWORD_PACKAGE,  // package
    TOKEN_KEYWORD_DYNAMIC,  // dynamic
    TOKEN_KEYWORD_UNSIZED,  // unsized
    TOKEN_KEYWORD_TYPE,     // type
    
    // 修饰符关键字 (Modifier Keywords)
    TOKEN_KEYWORD_OPEN,     // open
    TOKEN_KEYWORD_FINAL,    // final
    TOKEN_KEYWORD_IMPL,     // impl
    TOKEN_KEYWORD_VAR,      // var
    TOKEN_KEYWORD_VAL,      // val
    TOKEN_KEYWORD_CONST,    // const
    TOKEN_KEYWORD_LATEINIT, // lateinit
    TOKEN_KEYWORD_PUBLIC,   // public
    TOKEN_KEYWORD_PRIVATE,  // private
    TOKEN_KEYWORD_PUB,      // pub
    TOKEN_KEYWORD_PRIV,     // priv
    TOKEN_KEYWORD_PROT,     // prot
    TOKEN_KEYWORD_INTER,    // inter
    TOKEN_KEYWORD_OVERRIDE, // override
    TOKEN_KEYWORD_TAILREC,  // tailrec
    TOKEN_KEYWORD_CROSSINLINE, // crossinline
    
    // 类型标记
    TOKEN_TYPE_INT,       // int
    TOKEN_TYPE_STR,       // str
    TOKEN_TYPE_FLOAT,     // float
    TOKEN_TYPE_I8,        // i8
    TOKEN_TYPE_U8,        // u8
    TOKEN_TYPE_I16,       // i16
    TOKEN_TYPE_U16,       // u16
    TOKEN_TYPE_I32,       // i32
    TOKEN_TYPE_U32,       // u32
    TOKEN_TYPE_I64,       // i64
    TOKEN_TYPE_U64,       // u64
    TOKEN_TYPE_I128,      // i128
    TOKEN_TYPE_U128,      // u128
    TOKEN_TYPE_ISIZE,     // isize
    TOKEN_TYPE_USIZE,     // usize
    TOKEN_TYPE_F32,       // f32
    TOKEN_TYPE_F64,       // f64
    TOKEN_TYPE_BOOL,      // bool
    TOKEN_TYPE_CHAR,      // char
    TOKEN_TYPE_STRING,    // String
    TOKEN_TYPE_TUPLE,     // tuple
    TOKEN_TYPE_ARRAY,     // array
    TOKEN_TYPE_SLICE,     // slice
    TOKEN_TYPE_UNIT,      // ()
    TOKEN_TYPE_NEVER,     // !
    TOKEN_TYPE_NULLABLE,  // ?
    
    // 运算符
    TOKEN_PLUS,          // +
    TOKEN_MINUS,         // -
    TOKEN_MULTIPLY,      // *
    TOKEN_DIVIDE,        // /
    TOKEN_MOD,           // %
    TOKEN_ASSIGN,        // =
    TOKEN_EQUAL,         // ==
    TOKEN_NOT_EQUAL,     // !=
    TOKEN_LESS,          // <
    TOKEN_LESS_EQUAL,    // <=
    TOKEN_GREATER,       // >
    TOKEN_GREATER_EQUAL, // >=
    TOKEN_NOT,           // !
    TOKEN_AND,           // &&
    TOKEN_OR,            // ||
    TOKEN_BIT_AND,       // &
    TOKEN_BIT_OR,        // |
    TOKEN_BIT_XOR,       // ^
    TOKEN_BIT_NOT,       // ~
    TOKEN_SHIFT_LEFT,    // <<
    TOKEN_SHIFT_RIGHT,   // >>
    
    // 复合赋值运算符
    TOKEN_PLUS_ASSIGN,    // +=
    TOKEN_MINUS_ASSIGN,   // -=
    TOKEN_MULTIPLY_ASSIGN,// *=
    TOKEN_DIVIDE_ASSIGN,  // /=
    TOKEN_MOD_ASSIGN,     // %=
    TOKEN_AND_ASSIGN,     // &=
    TOKEN_OR_ASSIGN,      // |=
    TOKEN_XOR_ASSIGN,     // ^=
    TOKEN_SHIFT_LEFT_ASSIGN,  // <<=
    TOKEN_SHIFT_RIGHT_ASSIGN, // >>=
    
    // 递增递减运算符
    TOKEN_INCREMENT,      // ++
    TOKEN_DECREMENT,      // --
    
    // 指针相关运算符
    TOKEN_ADDRESS_OF,     // & (取址操作，区别于位与)
    TOKEN_DEREFERENCE,    // * (指针解引用，区别于乘法)
    
    // 附加箭头操作符
    TOKEN_ARROW_MATCH,    // => (用于模式匹配)
    
    // 特殊辅助符号
    TOKEN_AT,             // @ (模式绑定)
    TOKEN_DOLLAR,         // $ (模板变量)
    TOKEN_UNDERSCORE,     // _ (参数占位符)
    
    // 标点符号
    TOKEN_LPAREN,        // (
    TOKEN_RPAREN,        // )
    TOKEN_LBRACE,        // {
    TOKEN_RBRACE,        // }
    TOKEN_LBRACKET,      // [
    TOKEN_RBRACKET,      // ]
    TOKEN_COMMA,         // ,
    TOKEN_DOT,           // .
    TOKEN_SEMICOLON,     // ;
    TOKEN_COLON,         // :
    TOKEN_HASH,          // #
    TOKEN_QUOTE,         // "
    TOKEN_SINGLE_QUOTE,  // '
    TOKEN_ARROW,         // ->
    TOKEN_DOUBLE_COLON,  // ::
    TOKEN_RANGE,         // ..
    TOKEN_RANGE_TO,      // ..<
    TOKEN_RANGE_INCL,    // ..=
    TOKEN_SAFE_ACCESS,   // ?.
    TOKEN_ELVIS,         // ?:
    TOKEN_NOTNULL,       // !!
    
    // 注释
    TOKEN_COMMENT_SINGLE, // 单行注释
    TOKEN_COMMENT_MULTI   // 多行注释
} TokenType;

#endif // TOKEN_TYPES_H
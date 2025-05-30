grammar SCP;

// 程序入口
program: declarationList EOF;

// 声明列表
declarationList: declaration*;

// 声明
declaration: variableDeclaration
           | functionDeclaration
           | classDeclaration
           | enumDeclaration
           | structDeclaration
           | moduleImport
           | objectDeclaration;

// 变量声明 - 支持 val/var/const/lateinit
variableDeclaration: ('val' | 'var' | 'const' | 'lateinit') 
                    IDENTIFIER (':' type)? ('=' expression)? ';'?;

// 函数声明 - 支持泛型和async
functionDeclaration: ('async' | 'tailrec')? 'fun' IDENTIFIER 
                    genericParameterList? '(' parameterList? ')' 
                    (':' type)? functionBody;

// 类声明 - 支持open/sealed/data修饰符
classDeclaration: ('open' | 'sealed' | 'data')? 'class' IDENTIFIER 
                 genericParameterList? (':' interfaceList)? classBody;

// 枚举声明
enumDeclaration: 'enum' IDENTIFIER genericParameterList? enumBody;

// 结构体声明
structDeclaration: 'struct' IDENTIFIER genericParameterList? structBody;

// 单例对象声明
objectDeclaration: 'obj' IDENTIFIER? classBody;

// 模块导入
moduleImport: 'use' modulePath ('as' IDENTIFIER)? ';'?;

// 类型系统
type: basicType
    | compositeType
    | genericType
    | nullableType
    | dynamicType
    | unitType
    | neverType;

// 基本类型
basicType: 'i8' | 'i16' | 'i32' | 'i64' | 'i128'
         | 'u8' | 'u16' | 'u32' | 'u64' | 'u128'
         | 'f32' | 'f64' | 'f128'
         | 'bool' | 'char';

// 复合类型
compositeType: tupleType | arrayType | sliceType | stringType;

// 元组类型
tupleType: '(' (type (',' type)*)? ')';

// 数组类型
arrayType: '[' type ';' INTEGER_LITERAL ']';

// 切片类型
sliceType: 'slice' '(' type ')';

// 字符串类型
stringType: 'str';

// 泛型类型
genericType: IDENTIFIER '<' typeParameterList '>';

// 可空类型
nullableType: type '?';

// 动态类型
dynamicType: 'int' | 'flo' | 'str' | 'obj';

// 单元类型
unitType: '(' ')';

// 不可达类型
neverType: 'never';

// 表达式
expression: conditionalExpression;

conditionalExpression: logicalOrExpression 
                      ('if' '(' expression ')' expression ('else' expression)?)?;

logicalOrExpression: logicalAndExpression ('||' logicalAndExpression)*;
logicalAndExpression: equalityExpression ('&&' equalityExpression)*;
equalityExpression: relationalExpression (('==' | '!=' | '===' | '!==') relationalExpression)*;
relationalExpression: additiveExpression (('<' | '>' | '<=' | '>=') additiveExpression)*;
additiveExpression: multiplicativeExpression (('+' | '-') multiplicativeExpression)*;
multiplicativeExpression: unaryExpression (('*' | '/' | '%') unaryExpression)*;
bitwiseExpression: shiftExpression (('&' | '|' | '^') shiftExpression)*;
shiftExpression: additiveExpression (('<<' | '>>') additiveExpression)*;

unaryExpression: ('+' | '-' | '!' | '~' | '++' | '--')? postfixExpression;

postfixExpression: primaryExpression 
                  (memberAccess | functionCall | arrayAccess | '++' | '--')*;

primaryExpression: literal
                 | IDENTIFIER
                 | parenthesizedExpression
                 | 'this'
                 | 'super'
                 | typeConversionExpression
                 | whenExpression;

// 成员访问
memberAccess: '.' IDENTIFIER | '::' IDENTIFIER | '?.' IDENTIFIER;

// 函数调用
functionCall: '(' argumentList? ')';

// 数组访问
arrayAccess: '[' expression ']';

// 括号表达式
parenthesizedExpression: '(' expression ')';

// 类型转换
typeConversionExpression: expression ('as' | 'as?') type;

// when表达式
whenExpression: 'when' '(' expression ')' '{' whenBranch* '}';
whenBranch: (expression (',' expression)* | 'else') '->' expression ';'?;

// 字面量
literal: INTEGER_LITERAL
       | FLOAT_LITERAL
       | STRING_LITERAL
       | BOOLEAN_LITERAL
       | 'null'
       | CHAR_LITERAL;

// 函数体
functionBody: '{' statement* '}'
            | '->' expression;

// 语句
statement: expression ';'?
         | variableDeclaration
         | ifStatement
         | forStatement
         | whileStatement
         | doWhileStatement
         | tryStatement
         | 'break' ';'?
         | 'continue' ';'?
         | 'return' expression? ';'?
         | 'yield' expression ';'?;

// 控制流语句
ifStatement: 'if' '(' expression ')' statement ('else' statement)?;
forStatement: 'for' '(' IDENTIFIER 'in' expression ')' statement;
whileStatement: 'while' '(' expression ')' statement;
doWhileStatement: 'do' statement 'while' '(' expression ')' ';'?;
tryStatement: 'try' statement ('catch' '(' IDENTIFIER ':' type ')' statement)* ('finally' statement)?;

// 类体
classBody: '{' classMember* '}';
classMember: variableDeclaration
           | functionDeclaration
           | constructorDeclaration
           | companionObjectDeclaration;

// 构造函数
constructorDeclaration: 'init' '(' parameterList? ')' functionBody?;

// 伴生对象
companionObjectDeclaration: 'companion' 'object' IDENTIFIER? classBody;

// 枚举体
enumBody: '{' enumMember (',' enumMember)* '}';
enumMember: IDENTIFIER ('(' argumentList? ')')?;

// 结构体
structBody: '{' structMember* '}';
structMember: IDENTIFIER ':' type ';'?;

// 其他辅助规则
modulePath: IDENTIFIER ('::' IDENTIFIER)*;
genericParameterList: '<' typeParameter (',' typeParameter)* '>';
typeParameter: IDENTIFIER ('where' typeConstraint)?;
typeConstraint: IDENTIFIER ':' type (',' type)*;
typeParameterList: type (',' type)*;
parameterList: parameter (',' parameter)*;
parameter: IDENTIFIER ':' type ('=' expression)?;
argumentList: expression (',' expression)*;
interfaceList: IDENTIFIER (',' IDENTIFIER)*;

// 词法单元定义
IDENTIFIER: [a-zA-Z_][a-zA-Z0-9_]*;
INTEGER_LITERAL: [0-9]+ ('i8'|'i16'|'i32'|'i64'|'i128'|'u8'|'u16'|'u32'|'u64'|'u128')?;
FLOAT_LITERAL: ([0-9]* '.' [0-9]+ | [0-9]+ '.') ('f32'|'f64'|'f128')?;
STRING_LITERAL: '"' (~["\\] | '\\' .)* '"';
BOOLEAN_LITERAL: 'true' | 'false';
CHAR_LITERAL: '\'' (~['\\\r\n] | '\\' [ntrbf'\\]) '\'';

// 空白字符与注释
WS: [ \t\r\n]+ -> skip;
COMMENT: '//' ~[\r\n]* -> skip;
BLOCK_COMMENT: '/*' .*? '*/' -> skip;

// 安全调用表达式
nullSafeExpression
  : expression '?.' IDENTIFIER
  | expression '?:' expression
  | expression '!!'
  ;
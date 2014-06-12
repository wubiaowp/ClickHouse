#pragma once

#include <DB/Parsers/IParserBase.h>
#include <DB/Parsers/ExpressionElementParsers.h>


namespace DB
{

/** Запрос типа такого:
  * RENAME TABLE [db.]name TO [db.]name, [db.]name TO [db.]name, ...
  * (Переименовываться может произвольное количество таблиц.)
  */
class ParserRenameQuery : public IParserBase
{
protected:
	const char * getName() const { return "RENAME query"; }
	bool parseImpl(Pos & pos, Pos end, ASTPtr & node, Expected & expected);
};

}

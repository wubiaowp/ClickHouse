#pragma once

#include <DB/Parsers/IAST.h>

namespace DB
{


/** [GLOBAL] ANY|ALL INNER|LEFT JOIN (subquery) USING (tuple)
  * Замечание: ARRAY JOIN к этому не относится.
  */
class ASTJoin : public IAST
{
public:
	/// Алгоритм работы при распределённой обработке запроса.
	enum Locality
	{
		Local,	/// Выполнить JOIN, используя соответствующие данные в пределах каждого удалённого сервера.
		Global	/// Собрать соединяемые данные со всех удалённых серверов, объединить их, затем отправить на каждый удалённый сервер.
	};

	/// Оптимизация JOIN-а для большинства простых случаев.
	enum Strictness
	{
		Any,	/// Если в "правой" таблице несколько подходящих строк - выбрать из них одну, любую.
		All		/// Если в "правой" таблице несколько подходящих строк - размножить строки по ним (обычная семантика JOIN-а).
	};

	/// Способ соединения.
	enum Kind
	{
		Inner,	/// Оставить только записи, для которых в "правой" таблице есть соответствующая.
		Left	/// Если в "правой" таблице нет соответствующих записей, заполнить столбцы значениями "по-умолчанию".
	};

	Locality locality = Local;
	Strictness strictness = Any;
	Kind kind = Inner;

	ASTPtr subquery;			/// Подзапрос - "правая" таблица для соединения.
	ASTPtr using_expr_list;		/// По каким столбцам выполнять соединение.

	ASTJoin() {}
	ASTJoin(StringRange range_) : IAST(range_) {}
	
	/** Получить текст, который идентифицирует этот элемент. */
	String getID() const { return "Join"; };

	ASTPtr clone() const
	{
		ASTJoin * res = new ASTJoin(*this);
		res->children.clear();

		if (subquery) 			{ res->subquery 		= subquery->clone(); 		res->children.push_back(res->subquery); }
		if (using_expr_list) 	{ res->using_expr_list 	= using_expr_list->clone(); res->children.push_back(res->using_expr_list); }

		return res;
	}
};

}

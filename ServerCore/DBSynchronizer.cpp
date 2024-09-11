#include "pch.h"
#include "DBSynchronizer.h"
#include "DBBind.h"
#include "CoreGlobal.h"
#include "XmlParser.h"
#include <regex>

/*---------------------
	StoredProcedures
----------------------*/

namespace SP
{
	// SQL Server
	// const WCHAR* QTablesAndColumns =
	// 	L"	SELECT c.object_id, t.name AS tableName, c.name AS columnName, c.column_id, c.user_type_id, c.max_length,"
	// 	"		c.is_nullable, c.is_identity, CAST(ic.seed_value AS BIGINT) AS seedValue, CAST(ic.increment_value AS BIGINT) AS incValue,"
	// 	"		c.default_object_id, dc.definition as defaultDefinition, dc.name as defaultConstraintName"
	// 	"	FROM sys.columns AS c"
	// 	"	JOIN sys.tables AS t"
	// 	"		ON c.object_id = t.object_id"
	// 	"	LEFT JOIN sys.default_constraints AS dc"
	// 	"		ON c.default_object_id = dc.object_id"
	// 	"	LEFT JOIN sys.identity_columns AS ic"
	// 	"		ON c.object_id = ic.object_id AND c.column_id = ic.column_id"
	// 	"	WHERE t.type = 'U'"
	// 	"	ORDER BY object_id ASC, column_id ASC;";

	// MySql
	const WCHAR* QTablesAndColumns =
		L"SELECT \
			c.TABLE_NAME AS tableName,\
			c.COLUMN_NAME AS columnName,\
			c.ORDINAL_POSITION AS columnPosition,\
			c.COLUMN_TYPE AS columnType,\
			c.IS_NULLABLE AS isNullable,\
			c.COLUMN_DEFAULT AS defaultValue,\
		IF( c.EXTRA LIKE \'%auto_increment%\', 1, 0 ) AS isIdentity\
		FROM\
			INFORMATION_SCHEMA.COLUMNS AS c\
			JOIN INFORMATION_SCHEMA.TABLES AS t\
			ON c.TABLE_SCHEMA = t.TABLE_SCHEMA\
			AND c.TABLE_NAME = t.TABLE_NAME\
		WHERE\
			t.TABLE_TYPE = 'BASE TABLE'\
				ORDER BY\
				c.TABLE_NAME ASC,\
				c.ORDINAL_POSITION ASC;";

	class GetDBTables : public DBBind<0, 13>
	{
	public:
		GetDBTables(DBConnection& conn) : DBBind(conn, QTablesAndColumns) {}

		void Out_ObjectId(OUT int32& value) { BindCol(0, value); }
		template<int32 N> void Out_TableName(OUT WCHAR(&value)[N]) { BindCol(1, value); }
		template<int32 N> void Out_ColumnName(OUT WCHAR(&value)[N]) { BindCol(2, value); }
		void Out_ColumnId(OUT int32& value) { BindCol(3, value); }
		void Out_UserType(OUT int32& value) { BindCol(4, value); }
		void Out_MaxLength(OUT int32& value) { BindCol(5, value); }
		void Out_IsNullable(OUT bool& value) { BindCol(6, value); }
		void Out_IsIdentity(OUT bool& value) { BindCol(7, value); }
		void Out_SeedValue(OUT int64& value) { BindCol(8, value); }
		void Out_IncrementValue(OUT int64& value) { BindCol(9, value); }
		void Out_DefaultObjectId(OUT int32& value) { BindCol(10, value); }
		template<int32 N> void Out_DefaultDefinition(OUT WCHAR(&value)[N]) { BindCol(11, value); }
		template<int32 N> void Out_DefaultConstraintName(OUT WCHAR(&value)[N]) { BindCol(12, value); }
	};

	// SQL Server
	// const WCHAR* QIndexes =
	// 	L"	SELECT i.object_id, i.name as indexName, i.index_id, i.type, i.is_primary_key,"
	// 	"		i.is_unique_constraint, ic.column_id, COL_NAME(ic.object_id, ic.column_id) as columnName"
	// 	"	FROM sys.indexes AS i"
	// 	"	JOIN sys.index_columns AS ic"
	// 	"		ON i.object_id = ic.object_id AND i.index_id = ic.index_id"
	// 	"	WHERE i.type > 0 AND i.object_id IN(SELECT object_id FROM sys.tables WHERE type = 'U')"
	// 	"	ORDER BY i.object_id ASC, i.index_id ASC;";

	// MySql
	const WCHAR* QIndexes = 
		L"SELECT\
			s.TABLE_NAME AS tableName,\
			s.INDEX_NAME AS indexName,\
			s.INDEX_SCHEMA AS indexSchema,\
			s.SEQ_IN_INDEX AS indexSeq,\
			s.COLUMN_NAME AS columnName,\
			s.NON_UNIQUE AS isUnique,\
			t.CONSTRAINT_TYPE AS isPrimaryKey\
		FROM\
			INFORMATION_SCHEMA.STATISTICS AS s\
				LEFT JOIN INFORMATION_SCHEMA.TABLE_CONSTRAINTS AS t\
				ON s.TABLE_NAME = t.TABLE_NAME\
				AND s.INDEX_NAME = t.CONSTRAINT_NAME\
				AND s.TABLE_SCHEMA = t.TABLE_SCHEMA\
		WHERE\
			s.TABLE_SCHEMA = DATABASE()\
			AND s.INDEX_NAME != 'PRIMARY'\
		ORDER BY\
			s.TABLE_NAME ASC,\
			s.INDEX_NAME ASC,\
			s.SEQ_IN_INDEX ASC;";

	class GetDBIndexes : public DBBind<0, 8>
	{
	public:
		GetDBIndexes(DBConnection& conn) : DBBind(conn, QIndexes) {}

		void Out_ObjectId(OUT int32& value) { BindCol(0, value); }
		template<int32 N> void Out_IndexName(OUT WCHAR(&value)[N]) { BindCol(1, value); }
		void Out_IndexId(OUT int32& value) { BindCol(2, value); }
		void Out_IndexType(OUT int32& value) { BindCol(3, value); }
		void Out_IsPrimaryKey(OUT bool& value) { BindCol(4, value); }
		void Out_IsUniqueConstraint(OUT bool& value) { BindCol(5, value); }
		void Out_ColumnId(OUT int32& value) { BindCol(6, value); }
		template<int32 N> void Out_ColumnName(OUT WCHAR(&value)[N]) { BindCol(7, value); }
	};

	// SQL Server
	// const WCHAR* QStoredProcedures =
	// 	L"	SELECT name, OBJECT_DEFINITION(object_id) AS body FROM sys.procedures;";

	// MySql
	const WCHAR* QStoredProcedures =
		L"SELECT \
			ROUTINE_NAME AS name,\
			ROUTINE_DEFINITION AS body\
		FROM\
			INFORMATION_SCHEMA.ROUTINES\
		WHERE\
			ROUTINE_TYPE = 'PROCEDURE'\
			AND ROUTINE_SCHEMA = DATABASE();";

	class GetDBStoredProcedures : public DBBind<0, 2>
	{
	public:
		GetDBStoredProcedures(DBConnection& conn) : DBBind(conn, QStoredProcedures) {}

		template<int32 N> void Out_Name(OUT WCHAR(&value)[N]) { BindCol(0, value); }
		void Out_Body(OUT WCHAR* value, int32 len) { BindCol(1, value, len); }
	};
}

/*--------------------
	DBSynchronizer
---------------------*/

DBSynchronizer::~DBSynchronizer()
{
}

bool DBSynchronizer::Synchronize(const WCHAR* path)
{
	ParseXmlDB(path);

	GatherDBTables();
	GatherDBIndexes();
	GatherDBStoredProcedures();

	CompareDBModel();
	ExecuteUpdateQueries();

	return true;
}

void DBSynchronizer::ParseXmlDB(const WCHAR* path)
{
	XmlNode root;
	XmlParser parser;
	ASSERT_CRASH(parser.ParseFromFile(path, OUT root));

	Vector<XmlNode> tables = root.FindChildren(L"Table");
	for (XmlNode& table : tables)
	{
		DBModel::TablePtr t = MakeShared<DBModel::Table>();
		t->_name = table.GetStringAttr(L"name");

		Vector<XmlNode> columns = table.FindChildren(L"Column");
		for (XmlNode& column : columns)
		{
			DBModel::ColumnPtr c = MakeShared<DBModel::Column>();
			c->_name = column.GetStringAttr(L"name");
			c->_typeText = column.GetStringAttr(L"type");
			c->_type = DBModel::Helpers::String2DataType(c->_typeText.c_str(), OUT c->_maxLength);
			ASSERT_CRASH(c->_type != DBModel::DataType::None);
			c->_nullable = !column.GetBoolAttr(L"notnull", false);

			const WCHAR* identityStr = column.GetStringAttr(L"identity");
			if (::wcslen(identityStr) > 0)
			{
				std::wregex pt(L"(\\d+),(\\d+)");
				std::wcmatch match;
				ASSERT_CRASH(std::regex_match(identityStr, OUT match, pt));
				c->_identity = true;
				c->_seedValue = _wtoi(match[1].str().c_str());
				c->_incrementValue = _wtoi(match[2].str().c_str());
			}

			c->_default = column.GetStringAttr(L"default");
			t->_columns.push_back(c);
		}

		Vector<XmlNode> indexes = table.FindChildren(L"Index");
		for (XmlNode& index : indexes)
		{
			DBModel::IndexPtr i = MakeShared<DBModel::Index>();
			const WCHAR* typeStr = index.GetStringAttr(L"type");
			if (::_wcsicmp(typeStr, L"clustered") == 0)
				i->_type = DBModel::IndexType::Clustered;
			else if (::_wcsicmp(typeStr, L"nonclustered") == 0)
				i->_type = DBModel::IndexType::NonClustered;
			else
				CRASH("Invalid Index Type");

			i->_primaryKey = index.FindChild(L"PrimaryKey").IsValid();
			i->_uniqueConstraint = index.FindChild(L"UniqueKey").IsValid();

			Vector<XmlNode> columns = index.FindChildren(L"Column");
			for (XmlNode& column : columns)
			{
				const WCHAR* nameStr = column.GetStringAttr(L"name");
				DBModel::ColumnPtr c = t->FindColumn(nameStr);
				ASSERT_CRASH(c != nullptr);
				i->_columns.push_back(c);
			}

			t->_indexes.push_back(i);
		}

		_xmlTables.push_back(t);
	}

	Vector<XmlNode> procedures = root.FindChildren(L"Procedure");
	for (XmlNode& procedure : procedures)
	{
		DBModel::ProcedurePtr p = MakeShared<DBModel::Procedure>();
		p->_name = procedure.GetStringAttr(L"name");
		p->_body = procedure.FindChild(L"Body").GetStringValue();

		Vector<XmlNode> params = procedure.FindChildren(L"Param");
		for (XmlNode& paramNode : params)
		{
			DBModel::Param param;
			param._name = paramNode.GetStringAttr(L"name");
			param._type = paramNode.GetStringAttr(L"type");
			p->_parameters.push_back(param);
		}

		_xmlProcedures.push_back(p);
	}

	Vector<XmlNode> removedTables = root.FindChildren(L"RemovedTable");
	for (XmlNode& removedTable : removedTables)
	{
		_xmlRemovedTables.insert(removedTable.GetStringAttr(L"name"));
	}
}

bool DBSynchronizer::GatherDBTables()
{
	int32 objectId = 0;
	WCHAR tableName[101] = { 0 };
	WCHAR columnName[101] = { 0 };
	int32 columnId = 0;
	int32 userTypeId = 0;
	int32 maxLength = 0;
	bool isNullable = false;
	bool isIdentity = false;
	int64 seedValue = 0;
	int64 incValue = 0;
	int32 defaultObjectId = 0;
	WCHAR defaultDefinition[101] = { 0 };
	WCHAR defaultConstraintName[101] = { 0 };

	SP::GetDBTables getDBTables(_dbConn);
	getDBTables.Out_ObjectId(OUT objectId);
	getDBTables.Out_TableName(OUT tableName);
	getDBTables.Out_ColumnName(OUT columnName);
	getDBTables.Out_ColumnId(OUT columnId);
	getDBTables.Out_UserType(OUT userTypeId);
	getDBTables.Out_MaxLength(OUT maxLength);
	getDBTables.Out_IsNullable(OUT isNullable);
	getDBTables.Out_IsIdentity(OUT isIdentity);
	getDBTables.Out_SeedValue(OUT seedValue);
	getDBTables.Out_IncrementValue(OUT incValue);
	getDBTables.Out_DefaultObjectId(OUT defaultObjectId);
	getDBTables.Out_DefaultDefinition(OUT defaultDefinition);
	getDBTables.Out_DefaultConstraintName(OUT defaultConstraintName);

	if (getDBTables.Execute() == false)
		return false;

	while (getDBTables.Fetch())
	{
		DBModel::TablePtr table;

		auto findTable = std::find_if(_dbTables.begin(), _dbTables.end(), [=](const DBModel::TablePtr& table) { return table->_objectId == objectId; });
		if (findTable == _dbTables.end())
		{
			table = MakeShared<DBModel::Table>();
			table->_objectId = objectId;
			table->_name = tableName;
			_dbTables.push_back(table);
		}
		else
		{
			table = *findTable;
		}

		DBModel::ColumnPtr column = MakeShared<DBModel::Column>();
		{
			column->_name = columnName;
			column->_columnId = columnId;
			column->_type = static_cast<DBModel::DataType>(userTypeId);
			column->_typeText = DBModel::Helpers::DataType2String(column->_type);
			column->_maxLength = (column->_type == DBModel::DataType::NVarChar ? maxLength / 2 : maxLength);
			column->_nullable = isNullable;
			column->_identity = isIdentity;
			column->_seedValue = (isIdentity ? seedValue : 0);
			column->_incrementValue = (isIdentity ? incValue : 0);

			if (defaultObjectId > 0)
			{
				column->_default = defaultDefinition;
				uint64 p = column->_default.find_first_not_of('(');
				column->_default = column->_default.substr(p, column->_default.size() - p * 2);
				column->_defaultConstraintName = defaultConstraintName;
			}
		}

		table->_columns.push_back(column);
	}

	return true;
}

bool DBSynchronizer::GatherDBIndexes()
{
	int32 objectId;
	WCHAR indexName[101] = { 0 };
	int32 indexId;
	int32 indexType;
	bool isPrimaryKey;
	bool isUniqueConstraint;
	int32 columnId;
	WCHAR columnName[101] = { 0 };

	SP::GetDBIndexes getDBIndexes(_dbConn);
	getDBIndexes.Out_ObjectId(OUT objectId);
	getDBIndexes.Out_IndexName(OUT indexName);
	getDBIndexes.Out_IndexId(OUT indexId);
	getDBIndexes.Out_IndexType(OUT indexType);
	getDBIndexes.Out_IsPrimaryKey(OUT isPrimaryKey);
	getDBIndexes.Out_IsUniqueConstraint(OUT isUniqueConstraint);
	getDBIndexes.Out_ColumnId(OUT columnId);
	getDBIndexes.Out_ColumnName(OUT columnName);

	if (getDBIndexes.Execute() == false)
		return false;

	while (getDBIndexes.Fetch())
	{
		auto findTable = std::find_if(_dbTables.begin(), _dbTables.end(), [=](const DBModel::TablePtr& table) { return table->_objectId == objectId; });
		ASSERT_CRASH(findTable != _dbTables.end());
		Vector<DBModel::IndexPtr>& indexes = (*findTable)->_indexes;
		auto findIndex = std::find_if(indexes.begin(), indexes.end(), [indexId](DBModel::IndexPtr& index) { return index->_indexId == indexId; });
		if (findIndex == indexes.end())
		{
			DBModel::IndexPtr index = MakeShared<DBModel::Index>();
			{
				index->_name = indexName;
				index->_indexId = indexId;
				index->_type = static_cast<DBModel::IndexType>(indexType);
				index->_primaryKey = isPrimaryKey;
				index->_uniqueConstraint = isUniqueConstraint;
			}
			indexes.push_back(index);
			findIndex = indexes.end() - 1;
		}

		// 인덱스가 걸린 column 찾아서 매핑해준다.
		Vector<DBModel::ColumnPtr>& columns = (*findTable)->_columns;
		auto findColumn = std::find_if(columns.begin(), columns.end(), [columnId](DBModel::ColumnPtr& column) { return column->_columnId == columnId; });
		ASSERT_CRASH(findColumn != columns.end());
		(*findIndex)->_columns.push_back(*findColumn);
	}

	return true;
}

bool DBSynchronizer::GatherDBStoredProcedures()
{
	WCHAR name[101] = { 0 };
	Vector<WCHAR> body(PROCEDURE_MAX_LEN);

	SP::GetDBStoredProcedures getDBStoredProcedures(_dbConn);
	getDBStoredProcedures.Out_Name(OUT name);
	getDBStoredProcedures.Out_Body(OUT & body[0], PROCEDURE_MAX_LEN);

	if (getDBStoredProcedures.Execute() == false)
		return false;

	while (getDBStoredProcedures.Fetch())
	{
		DBModel::ProcedurePtr proc = MakeShared<DBModel::Procedure>();
		{
			proc->_name = name;
			proc->_fullBody = String(body.begin(), std::find(body.begin(), body.end(), 0));
		}
		_dbProcedures.push_back(proc);
	}

	return true;
}

void DBSynchronizer::CompareDBModel()
{
	// 업데이트 목록 초기화.
	_dependentIndexes.clear();
	for (Vector<String>& queries : _updateQueries)
		queries.clear();

	// XML에 있는 목록을 우선 갖고 온다.
	Map<String, DBModel::TablePtr> xmlTableMap;
	for (DBModel::TablePtr& xmlTable : _xmlTables)
		xmlTableMap[xmlTable->_name] = xmlTable;

	// DB에 실존하는 테이블들을 돌면서 XML에 정의된 테이블들과 비교한다.
	for (DBModel::TablePtr& dbTable : _dbTables)
	{
		auto findTable = xmlTableMap.find(dbTable->_name);
		if (findTable != xmlTableMap.end())
		{
			DBModel::TablePtr xmlTable = findTable->second;
			CompareTables(dbTable, xmlTable);
			xmlTableMap.erase(findTable);
		}
		else
		{
			if (_xmlRemovedTables.find(dbTable->_name) != _xmlRemovedTables.end())
			{
				GConsoleLogger->WriteStdOut(Color::YELLOW, L"Removing Table : [dbo].[%s]\n", dbTable->_name.c_str());
				_updateQueries[UpdateStep::DropTable].push_back(DBModel::Helpers::Format(L"DROP TABLE [dbo].[%s]", dbTable->_name.c_str()));
			}
		}
	}

	// 맵에서 제거되지 않은 XML 테이블 정의는 새로 추가.
	for (auto& mapIt : xmlTableMap)
	{
		DBModel::TablePtr& xmlTable = mapIt.second;

		String columnsStr;
		const int32 size = static_cast<int32>(xmlTable->_columns.size());
		for (int32 i = 0; i < size; i++)
		{
			if (i != 0)
				columnsStr += L",";
			columnsStr += L"\n\t";
			columnsStr += xmlTable->_columns[i]->CreateText();
		}

		// SQL Server
		// GConsoleLogger->WriteStdOut(Color::YELLOW, L"Creating Table : [dbo].[%s]\n", xmlTable->_name.c_str());
		// _updateQueries[UpdateStep::CreateTable].push_back(DBModel::Helpers::Format(L"CREATE TABLE [dbo].[%s] (%s)", xmlTable->_name.c_str(), columnsStr.c_str()));

		// MySql
		GConsoleLogger->WriteStdOut( Color::YELLOW, L"Creating Table : %s\n", xmlTable->_name.c_str() );
		_updateQueries[UpdateStep::CreateTable].push_back(DBModel::Helpers::Format(L"CREATE TABLE %s (%s);", xmlTable->_name.c_str(), columnsStr.c_str()));

		for (DBModel::ColumnPtr& xmlColumn : xmlTable->_columns)
		{
			if (xmlColumn->_default.empty())
				continue;

			_updateQueries[UpdateStep::DefaultConstraint].push_back(DBModel::Helpers::Format(L"ALTER TABLE [dbo].[%s] ADD CONSTRAINT [%s] DEFAULT (%s) FOR [%s]",
				xmlTable->_name.c_str(),
				DBModel::Helpers::Format(L"DF_%s_%s", xmlTable->_name.c_str(), xmlColumn->_name.c_str()).c_str(),
				xmlColumn->_default.c_str(),
				xmlColumn->_name.c_str()));
		}

		for (DBModel::IndexPtr& xmlIndex : xmlTable->_indexes)
		{
			GConsoleLogger->WriteStdOut(Color::YELLOW, L"Creating Index : [%s] %s %s [%s]\n", xmlTable->_name.c_str(), xmlIndex->GetKeyText().c_str(), xmlIndex->GetTypeText().c_str(), xmlIndex->GetUniqueName().c_str());
			if (xmlIndex->_primaryKey || xmlIndex->_uniqueConstraint)
			{
				// SQL Server
				// _updateQueries[UpdateStep::CreateIndex].push_back(DBModel::Helpers::Format(
				// 	L"ALTER TABLE [dbo].[%s] ADD CONSTRAINT [%s] %s %s (%s)",
				// 	xmlTable->_name.c_str(),
				// 	xmlIndex->CreateName(xmlTable->_name).c_str(),
				// 	xmlIndex->GetKeyText().c_str(),
				// 	xmlIndex->GetTypeText().c_str(),
				// 	xmlIndex->CreateColumnsText().c_str()));

				_updateQueries[ UpdateStep::CreateIndex ].push_back( DBModel::Helpers::Format(
					L"ALTER TABLE %s ADD CONSTRAINT %s %s (%s);",
					xmlTable->_name.c_str(),
					xmlIndex->CreateName( xmlTable->_name ).c_str(),
					xmlIndex->GetKeyText().c_str(),
					xmlIndex->CreateColumnsText().c_str() ) );
			}
			else
			{
				_updateQueries[UpdateStep::CreateIndex].push_back(DBModel::Helpers::Format(
					L"CREATE %s INDEX [%s] ON [dbo].[%s] (%s)",
					xmlIndex->GetTypeText().c_str(),
					xmlIndex->CreateName(xmlTable->_name).c_str(),
					xmlTable->_name.c_str(),
					xmlIndex->CreateColumnsText().c_str()));
			}
		}
	}

	CompareStoredProcedures();
}

void DBSynchronizer::ExecuteUpdateQueries()
{
	for (int32 step = 0; step < UpdateStep::Max; step++)
	{
		for (String& query : _updateQueries[step])
		{
			_dbConn.Unbind();
			ASSERT_CRASH(_dbConn.Execute(query.c_str()));
		}
	}
}

void DBSynchronizer::CompareTables(DBModel::TablePtr dbTable, DBModel::TablePtr xmlTable)
{
	// XML에 있는 컬럼 목록을 갖고 온다.
	Map<String, DBModel::ColumnPtr> xmlColumnMap;
	for (DBModel::ColumnPtr& xmlColumn : xmlTable->_columns)
		xmlColumnMap[xmlColumn->_name] = xmlColumn;

	// DB에 실존하는 테이블 컬럼들을 돌면서 XML에 정의된 컬럼들과 비교한다.
	for (DBModel::ColumnPtr& dbColumn : dbTable->_columns)
	{
		auto findColumn = xmlColumnMap.find(dbColumn->_name);
		if (findColumn != xmlColumnMap.end())
		{
			DBModel::ColumnPtr& xmlColumn = findColumn->second;
			CompareColumns(dbTable, dbColumn, xmlColumn);
			xmlColumnMap.erase(findColumn);
		}
		else
		{
			GConsoleLogger->WriteStdOut(Color::YELLOW, L"Dropping Column : [%s].[%s]\n", dbTable->_name.c_str(), dbColumn->_name.c_str());
			if (dbColumn->_defaultConstraintName.empty() == false)
				_updateQueries[UpdateStep::DropColumn].push_back(DBModel::Helpers::Format(L"ALTER TABLE [dbo].[%s] DROP CONSTRAINT [%s]", dbTable->_name.c_str(), dbColumn->_defaultConstraintName.c_str()));

			_updateQueries[UpdateStep::DropColumn].push_back(DBModel::Helpers::Format(L"ALTER TABLE [dbo].[%s] DROP COLUMN [%s]", dbTable->_name.c_str(), dbColumn->_name.c_str()));
		}
	}

	// 맵에서 제거되지 않은 XML 컬럼 정의는 새로 추가.
	for (auto& mapIt : xmlColumnMap)
	{
		DBModel::ColumnPtr& xmlColumn = mapIt.second;
		DBModel::Column newColumn = *xmlColumn;
		newColumn._nullable = true;

		GConsoleLogger->WriteStdOut(Color::YELLOW, L"Adding Column : [%s].[%s]\n", dbTable->_name.c_str(), xmlColumn->_name.c_str());
		_updateQueries[UpdateStep::AddColumn].push_back(DBModel::Helpers::Format(L"ALTER TABLE [dbo].[%s] ADD %s %s",
			dbTable->_name.c_str(), xmlColumn->_name.c_str(), xmlColumn->_typeText.c_str()));

		if (xmlColumn->_nullable == false && xmlColumn->_default.empty() == false)
		{
			_updateQueries[UpdateStep::AddColumn].push_back(DBModel::Helpers::Format(L"SET NOCOUNT ON; UPDATE [dbo].[%s] SET [%s] = %s WHERE [%s] IS NULL",
				dbTable->_name.c_str(), xmlColumn->_name.c_str(), xmlColumn->_default.c_str(), xmlColumn->_name.c_str()));
		}

		if (xmlColumn->_nullable == false)
		{
			_updateQueries[UpdateStep::AddColumn].push_back(DBModel::Helpers::Format(L"ALTER TABLE [dbo].[%s] ALTER COLUMN %s",
				dbTable->_name.c_str(), xmlColumn->CreateText().c_str()));
		}

		if (xmlColumn->_default.empty() == false)
		{
			_updateQueries[UpdateStep::AddColumn].push_back(DBModel::Helpers::Format(L"ALTER TABLE [dbo].[%s] ADD CONSTRAINT [DF_%s_%s] DEFAULT (%s) FOR [%s]",
				dbTable->_name.c_str(), dbTable->_name.c_str(), xmlColumn->_name.c_str(), xmlColumn->_default.c_str(), xmlColumn->_name.c_str()));
		}
	}

	// XML에 있는 인덱스 목록을 갖고 온다.
	Map<String, DBModel::IndexPtr> xmlIndexMap;
	for (DBModel::IndexPtr& xmlIndex : xmlTable->_indexes)
		xmlIndexMap[xmlIndex->GetUniqueName()] = xmlIndex;

	// DB에 실존하는 테이블 인덱스들을 돌면서 XML에 정의된 인덱스들과 비교한다.
	for (DBModel::IndexPtr& dbIndex : dbTable->_indexes)
	{
		auto findIndex = xmlIndexMap.find(dbIndex->GetUniqueName());
		if (findIndex != xmlIndexMap.end() && _dependentIndexes.find(dbIndex->GetUniqueName()) == _dependentIndexes.end())
		{
			DBModel::IndexPtr xmlIndex = findIndex->second;
			xmlIndexMap.erase(findIndex);
		}
		else
		{
			GConsoleLogger->WriteStdOut(Color::YELLOW, L"Dropping Index : [%s] [%s] %s %s\n", dbTable->_name.c_str(), dbIndex->_name.c_str(), dbIndex->GetKeyText().c_str(), dbIndex->GetTypeText().c_str());
			if (dbIndex->_primaryKey || dbIndex->_uniqueConstraint)
				_updateQueries[UpdateStep::DropIndex].push_back(DBModel::Helpers::Format(L"ALTER TABLE [dbo].[%s] DROP CONSTRAINT [%s]", dbTable->_name.c_str(), dbIndex->_name.c_str()));
			else
				_updateQueries[UpdateStep::DropIndex].push_back(DBModel::Helpers::Format(L"DROP INDEX [%s] ON [dbo].[%s]", dbIndex->_name.c_str(), dbTable->_name.c_str()));
		}
	}

	// 맵에서 제거되지 않은 XML 인덱스 정의는 새로 추가.
	for (auto& mapIt : xmlIndexMap)
	{
		DBModel::IndexPtr xmlIndex = mapIt.second;
		GConsoleLogger->WriteStdOut(Color::YELLOW, L"Creating Index : [%s] %s %s [%s]\n", dbTable->_name.c_str(), xmlIndex->GetKeyText().c_str(), xmlIndex->GetTypeText().c_str(), xmlIndex->GetUniqueName().c_str());
		if (xmlIndex->_primaryKey || xmlIndex->_uniqueConstraint)
		{
			_updateQueries[UpdateStep::CreateIndex].push_back(DBModel::Helpers::Format(L"ALTER TABLE [dbo].[%s] ADD CONSTRAINT [%s] %s %s (%s)",
				dbTable->_name.c_str(), xmlIndex->CreateName(dbTable->_name).c_str(), xmlIndex->GetKeyText().c_str(), xmlIndex->GetTypeText().c_str(), xmlIndex->CreateColumnsText().c_str()));
		}
		else
		{
			_updateQueries[UpdateStep::CreateIndex].push_back(DBModel::Helpers::Format(L"CREATE %s INDEX [%s] ON [dbo].[%s] (%s)",
				xmlIndex->GetTypeText(), xmlIndex->CreateName(dbTable->_name).c_str(), dbTable->_name.c_str(), xmlIndex->CreateColumnsText().c_str()));
		}
	}
}

void DBSynchronizer::CompareColumns(DBModel::TablePtr dbTable, DBModel::ColumnPtr dbColumn, DBModel::ColumnPtr xmlColumn)
{
	uint8 flag = 0;

	if (dbColumn->_type != xmlColumn->_type)
		flag |= ColumnFlag::Type;
	if (dbColumn->_maxLength != xmlColumn->_maxLength && xmlColumn->_maxLength > 0)
		flag |= ColumnFlag::Length;
	if (dbColumn->_nullable != xmlColumn->_nullable)
		flag |= ColumnFlag::Nullable;
	if (dbColumn->_identity != xmlColumn->_identity || (dbColumn->_identity && dbColumn->_incrementValue != xmlColumn->_incrementValue))
		flag |= ColumnFlag::Identity;
	if (dbColumn->_default != xmlColumn->_default)
		flag |= ColumnFlag::Default;

	if (flag)
	{
		GConsoleLogger->WriteStdOut(Color::YELLOW, L"Updating Column [%s] : (%s) -> (%s)\n", dbTable->_name.c_str(), dbColumn->CreateText().c_str(), xmlColumn->CreateText().c_str());
	}

	// 연관된 인덱스가 있으면 나중에 삭제하기 위해 기록한다.
	if (flag & (ColumnFlag::Type | ColumnFlag::Length | ColumnFlag::Nullable))
	{
		for (DBModel::IndexPtr& dbIndex : dbTable->_indexes)
			if (dbIndex->DependsOn(dbColumn->_name))
				_dependentIndexes.insert(dbIndex->GetUniqueName());

		flag |= ColumnFlag::Default;
	}

	if (flag & ColumnFlag::Default)
	{
		if (dbColumn->_defaultConstraintName.empty() == false)
		{
			_updateQueries[UpdateStep::AlterColumn].push_back(DBModel::Helpers::Format(
				L"ALTER TABLE [dbo].[%s] DROP CONSTRAINT [%s]",
				dbTable->_name.c_str(),
				dbColumn->_defaultConstraintName.c_str()));
		}
	}

	DBModel::Column newColumn = *dbColumn;
	newColumn._default = L"";
	newColumn._type = xmlColumn->_type;
	newColumn._maxLength = xmlColumn->_maxLength;
	newColumn._typeText = xmlColumn->_typeText;
	newColumn._seedValue = xmlColumn->_seedValue;
	newColumn._incrementValue = xmlColumn->_incrementValue;

	if (flag & (ColumnFlag::Type | ColumnFlag::Length | ColumnFlag::Identity))
	{
		_updateQueries[UpdateStep::AlterColumn].push_back(DBModel::Helpers::Format(
			L"ALTER TABLE [dbo].[%s] ALTER COLUMN %s",
			dbTable->_name.c_str(),
			newColumn.CreateText().c_str()));
	}

	newColumn._nullable = xmlColumn->_nullable;
	if (flag & ColumnFlag::Nullable)
	{
		if (xmlColumn->_default.empty() == false)
		{
			_updateQueries[UpdateStep::AlterColumn].push_back(DBModel::Helpers::Format(
				L"SET NOCOUNT ON; UPDATE [dbo].[%s] SET [%s] = %s WHERE [%s] IS NULL",
				dbTable->_name.c_str(),
				xmlColumn->_name.c_str(),
				xmlColumn->_name.c_str(),
				xmlColumn->_name.c_str()));
		}

		_updateQueries[UpdateStep::AlterColumn].push_back(DBModel::Helpers::Format(
			L"ALTER TABLE [dbo].[%s] ALTER COLUMN %s",
			dbTable->_name.c_str(),
			newColumn.CreateText().c_str()));
	}

	if (flag & ColumnFlag::Default)
	{
		if (dbColumn->_defaultConstraintName.empty() == false)
		{
			_updateQueries[UpdateStep::AlterColumn].push_back(DBModel::Helpers::Format(
				L"ALTER TABLE [dbo].[%s] ADD CONSTRAINT [%s] DEFAULT (%s) FOR [%s]",
				dbTable->_name.c_str(),
				DBModel::Helpers::Format(L"DF_%s_%s", dbTable->_name.c_str(), dbColumn->_name.c_str()).c_str(),
				dbColumn->_default.c_str(), dbColumn->_name.c_str()));
		}
	}
}

void DBSynchronizer::CompareStoredProcedures()
{
	// XML에 있는 프로시저 목록을 갖고 온다.
	Map<String, DBModel::ProcedurePtr> xmlProceduresMap;
	for (DBModel::ProcedurePtr& xmlProcedure : _xmlProcedures)
		xmlProceduresMap[xmlProcedure->_name] = xmlProcedure;

	// DB에 실존하는 테이블 프로시저들을 돌면서 XML에 정의된 프로시저들과 비교한다.
	for (DBModel::ProcedurePtr& dbProcedure : _dbProcedures)
	{
		auto findProcedure = xmlProceduresMap.find(dbProcedure->_name);
		if (findProcedure != xmlProceduresMap.end())
		{
			DBModel::ProcedurePtr xmlProcedure = findProcedure->second;
			String xmlBody = xmlProcedure->GenerateCreateQuery();
			if (DBModel::Helpers::RemoveWhiteSpace(dbProcedure->_fullBody) != DBModel::Helpers::RemoveWhiteSpace(xmlBody))
			{
				GConsoleLogger->WriteStdOut(Color::YELLOW, L"Updating Procedure : %s\n", dbProcedure->_name.c_str());
				_updateQueries[UpdateStep::StoredProcecure].push_back(xmlProcedure->GenerateAlterQuery());
			}
			xmlProceduresMap.erase(findProcedure);
		}
	}

	// 맵에서 제거되지 않은 XML 프로시저 정의는 새로 추가.
	for (auto& mapIt : xmlProceduresMap)
	{
		GConsoleLogger->WriteStdOut(Color::YELLOW, L"Updating Procedure : %s\n", mapIt.first.c_str());
		_updateQueries[UpdateStep::StoredProcecure].push_back(mapIt.second->GenerateCreateQuery());
	}
}
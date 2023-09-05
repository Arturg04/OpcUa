#include <sqlite3.h>
#include <stdio.h>

sqlite3	*db;

typedef struct {
	char *name;
	char *node;
	char *type;
	__int32_t value;
} xmlTag;

void	db_Comm(void)
{
	int		rc;
	char	*sql;
	char	*errMsg = 0;

	rc = sqlite3_open("values.db", &db);
	if (SQLITE_OK != rc)
	{
		printf("ERROR DB : %s\n", sqlite3_errmsg(db));
		return ;
	}
	sql = "CREATE TABLE IF NOT EXISTS Tags ("
			"ID INT PRIMARY KEY NOT NULL,"
			"name TEXT NOT NULL,"
			"node TEXT NOT NULL,"
			"type TEXT NOT NULL,"
			"value INT);";

	rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

	if (SQLITE_OK != rc)
	{
		printf("ERROR DB : %s\n", errMsg);
	}
	else
	{
		printf("Table created successfully\n");
	}
}

void addTags(xmlTag Tags[], int size)
{
	int		rc;
	char	*sql;

	rc = sqlite3_open("values.db", &db);
	sqlite3_stmt *stmt;
	sql = "INSERT INTO Tags (name, node, type, value) VALUES (?, ?, ?, ?);";
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		printf("ERROR DB : %s\n", sqlite3_errmsg(db));
		return;
	}

    for (int i = 0; i < size; i++) {
		sqlite3_bind_text(stmt, 1, Tags[i].name, -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 2, Tags[i].node, -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 3, Tags[i].type, -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt, 4, Tags[i].value);
		rc = sqlite3_step(stmt);
		if (rc != SQLITE_DONE) {
			printf("ERROR DB : %s\n", sqlite3_errmsg(db));
			return;
		}
		sqlite3_reset(stmt);
	}
	sqlite3_finalize(stmt);
}

void updateValue(char *node, int value) {
	int		rc;
	char	*sql;

	sql = "UPDATE Tags SET value = ? WHERE node = ?;";
	sqlite3_stmt *stmt;
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		printf("ERROR DB : %s\n", sqlite3_errmsg(db));
		return;
	}
	sqlite3_bind_int(stmt, 1, value);
	sqlite3_bind_int(stmt, 2, (node));
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE)
	{
		printf("ERROR DB : %s\n", sqlite3_errmsg(db));
		return;
	}
	sqlite3_finalize(stmt);
}

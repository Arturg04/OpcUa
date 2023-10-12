/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Database.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-pinh <artur.13.goncalves@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 15:23:44 by ade-pinh          #+#    #+#             */
/*   Updated: 2023/10/12 11:55:16 by ade-pinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/OpcLib.h"

static sqlite3	*g_db;

int	open_db(char *filepath)
{
	sqlite3	*db;
	if (sqlite3_open(filepath, &db) != SQLITE_OK) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(g_db));
		return (0);
	}
	g_db = db;
	return (1);
}

int	create_table(void)
{
	int		rc;
	char	*sql;
	char	*zErrMsg;

	zErrMsg = ft_calloc(1, sizeof(char));
	sql = ft_strdup("CREATE TABLE IF NOT EXISTS Tags (Name TEXT, NodeIndex INTEGER, NodeIdentifier INTEGER, Type TEXT, Value INTEGER)");
	rc = sqlite3_exec(g_db, "DROP TABLE IF EXISTS Tags", 0, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
			fprintf(stderr, "Error deleting existing table: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
			free(sql);
			return (0);
	}
	rc = sqlite3_exec(g_db, sql, 0, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Can't create table: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		free(sql);
		return (0);
	}
	free(sql);
	free(zErrMsg);
	return (1);
}

int	insert_data(t_TagInfo *tag, int value)
{
	char *zErrMsg = 0;
	int rc;
	char *sql = "INSERT INTO Tags (Name , NodeIndex, NodeIdentifier, Type, Value) VALUES (?, ?, ?, ?, ?)";

	sqlite3_stmt *stmt;
	rc = sqlite3_prepare(g_db, sql, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Can't prepare statement: %s\n", sqlite3_errmsg(g_db));
		return 0;
	}
	rc = sqlite3_bind_text(stmt, 1, tag->name, -1, 0);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Can't bind text: %s\n", sqlite3_errmsg(g_db));
		sqlite3_finalize(stmt);
		return 0;
	}
	rc = sqlite3_bind_int(stmt, 2, tag->node->index);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Can't bind int: %s\n", sqlite3_errmsg(g_db));
		sqlite3_finalize(stmt);
		return 0;
	}
	rc = sqlite3_bind_int(stmt, 3, tag->node->identifier);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Can't bind int: %s\n", sqlite3_errmsg(g_db));
		sqlite3_finalize(stmt);
		return 0;
	}
	rc = sqlite3_bind_text(stmt, 4, tag->tag_type, -1, 0);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Can't bind int: %s\n", sqlite3_errmsg(g_db));
		sqlite3_finalize(stmt);
		return 0;
	}
	rc = sqlite3_bind_int(stmt, 5, value);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Can't bind int: %s\n", sqlite3_errmsg(g_db));
		sqlite3_finalize(stmt);
		return 0;
	}
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		fprintf(stderr, "Can't insert data: %s\n", sqlite3_errmsg(g_db));
		sqlite3_finalize(stmt);
		return 0;
	}
	sqlite3_finalize(stmt);
	return 1;
}

int	update_data(t_TagInfo *tag, int value)
{
	char *zErrMsg = 0;
	int rc;
	char *sql = "UPDATE Tags SET Value = ? WHERE NodeIndex = ? AND NodeIdentifier = ?";
	sqlite3_stmt *stmt;
	rc = sqlite3_prepare(g_db, sql, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Can't prepare statement: %s\n", sqlite3_errmsg(g_db));
		return 0;
	}
	rc = sqlite3_bind_int(stmt, 1, value);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Can't bind text: %s\n", sqlite3_errmsg(g_db));
		sqlite3_finalize(stmt);
		return 0;
	}
	rc = sqlite3_bind_int(stmt, 2, tag->node->index);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Can't bind int: %s\n", sqlite3_errmsg(g_db));
		sqlite3_finalize(stmt);
		return 0;
	}
	rc = sqlite3_bind_int(stmt, 3, tag->node->identifier);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Can't bind int: %s\n", sqlite3_errmsg(g_db));
		sqlite3_finalize(stmt);
		return 0;
	}
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		fprintf(stderr, "Can't update data: %s\n", sqlite3_errmsg(g_db));
		sqlite3_finalize(stmt);
		return 0;
	}
	sqlite3_finalize(stmt);
	return 1;
}

int	delete_data(t_TagInfo *tag)
{
    char *zErrMsg = 0;
    int rc;
    char *sql = "DELETE FROM Tags WHERE NodeIndex = ? AND NodeIdentifier = ?";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare(g_db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't prepare statement: %s\n", sqlite3_errmsg(g_db));
        return 0;
    }
    rc = sqlite3_bind_int(stmt, 1, tag->node->index);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't bind int: %s\n", sqlite3_errmsg(g_db));
        sqlite3_finalize(stmt);
        return 0;
    }
	rc = sqlite3_bind_int(stmt, 1, tag->node->identifier);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't bind int: %s\n", sqlite3_errmsg(g_db));
        sqlite3_finalize(stmt);
        return 0;
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Can't delete data: %s\n", sqlite3_errmsg(g_db));
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);
    return 1;
}

void	close_db(void)
{
    sqlite3_close(g_db);
}
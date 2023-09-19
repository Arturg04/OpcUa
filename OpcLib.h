#ifndef OPCUA_H
# define OPCUA_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>

# include <open62541/types.h>
# include <open62541/client.h>
# include <open62541/server.h>
# include <open62541/client_subscriptions.h>
# include <open62541/client_highlevel_async.h>

# include <libxml/parser.h>
# include <libxml/tree.h>

# include <sqlite3.h>

struct	TagInfo {
	char name[50];
	char node[50];
	char tag_type[50];
};


// Server Comm
int		isServerReachable(const char *serverAddress);
int		connectToOpcServer(UA_Client **clientPtr, const char *serverUrl);
void	disconnectFromOpcServer(UA_Client *client);

// db
//void	addTags(struct	TagInfo Tags[], int size);
//void	updateValue(char *node, int value);

//void	addSubscriptions(UA_Client *client, struct TagInfo *tags, size_t numTags);

// xml
int		parseXmlFile(const char *filename, struct TagInfo tags[], int maxTags);
void	printTags(struct TagInfo tags[], int tagCount);

#endif

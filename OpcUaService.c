#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <client_highlevel_async.h>

typedef struct {
	char *name;
	char *node;
	char *type;
	int32_t value;
} xmlTag;

int		connectToOpcServer(UA_Client **clientPtr, const char *serverUrl);
void	disconnectFromOpcServer(UA_Client *client);
int		isServerReachable(const char *serverAddress);
void	addSubscription(UA_Client *client, xmlTag *tags, size_t size);
xmlTag*	parseXML(char* xmlFilePath, int* numTags);
void	addTags(xmlTag Tags[], int size);


void	*threadFunction(void *arg)
{
	char *url = (char *)arg;

	while (isServerReachable(url))
	{
		printf("Thread is running.\n");
		sleep(10);
	}
	return NULL;
}

int		main()
{
	int			threadId = 1;
	int			countTags;
	char		*url = "opc.tcp://192.168.10.1:4840";
	pthread_t	thread;
	UA_Client	*client;
	xmlTag		*Tags;

	countTags = 0;
	connectToOpcServer(&client, url);
	//Start thread just to check my comm
	if (pthread_create(&thread, NULL, threadFunction, "192.168.10.1") != 0) {
		fprintf(stderr, "Failed to create a new thread.\n");
		UA_Client_delete(client);
		return 1;
	}

	Tags = parseXML( "tags.xml", &countTags);
	addTags(Tags, countTags);
	addSubscription(client, Tags, countTags);

	while (1)
	{
		UA_Client_run_iterate(client, 100); // run the client for 100 ms
	}

	//join to my main program the thread
	if (pthread_join(thread, NULL) != 0) {
		fprintf(stderr, "Failed to join the thread.\n");
		UA_Client_delete(client);
		return 1;
	}

	UA_Client_delete(client);
	printf("Main thread is done.\n");

	return 0;
}

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <client_highlevel_async.h>

int		connectToOpcServer(UA_Client **clientPtr, const char *serverUrl);
void	disconnectFromOpcServer(UA_Client *client);
int		isOpcServerConnected(UA_Client *client);
int		isServerReachable(const char *serverAddress);

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
	char		*url = "opc.tcp://192.168.10.1:4840";
	pthread_t	thread;
	UA_Client	*client;

	connectToOpcServer(&client, url);
	//Start thread just to check my comm
	if (pthread_create(&thread, NULL, threadFunction, "192.168.10.1") != 0) {
		fprintf(stderr, "Failed to create a new thread.\n");
		UA_Client_delete(client);
		return 1;
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

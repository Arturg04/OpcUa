#include "OpcLib.h"

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
	char		*filename = "tags.xml";
	char		*url = "opc.tcp://192.168.10.1:4840";
	pthread_t	thread;
	UA_Client	*client;
	struct TagInfo	Tags[100];


	int tagCount = parseXmlFile(filename, Tags, sizeof(Tags) / sizeof(Tags[0]));
	if (tagCount > 0) {
        printTags(Tags, tagCount);
    } else {
        fprintf(stderr, "No tags found or error occurred during parsing.\n");
    }

	connectToOpcServer(&client, url);
	//Start thread just to check my comm
	if (pthread_create(&thread, NULL, threadFunction, "192.168.10.1") != 0) {
		fprintf(stderr, "Failed to create a new thread.\n");
		UA_Client_delete(client);
		return 1;
	}

    if (createSubscriptions(client, Tags, tagCount) != UA_STATUSCODE_GOOD) {
        printf("Failed to create subscriptions.\n");
        return 1;
    }
	//addSubscriptions(client, Tags, countTags);

	// while (1)
	// {
	// 	UA_Client_run_iterate(client, 100); // run the client for 100 ms
	// }

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

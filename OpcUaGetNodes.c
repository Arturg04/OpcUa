#include <stdio.h>
//#include "open62541/include/open62541/client.h"
//#include "open62541/include/open62541/client_highlevel.h"
#include <client_highlevel_async.h>
//#include "open62541/include/open62541/client_subscriptions.h"

int	main(void)
{
    UA_Client *client = UA_Client_new();
	if (client == NULL)
	{
		fprintf(stderr, "Failed to create client\n");
		return 1;
	}
	UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://192.168.10.1:4840");

	if(retval != UA_STATUSCODE_GOOD)
	{
		fprintf(stderr, "Failed to connect to the server, StatusCode %s\n", UA_StatusCode_name(retval));
		UA_Client_delete(client);
		return 1;
	}
	printf("Connected to the server!\n");
	// Browse the root node to find all available nodes
	UA_BrowseRequest request;
	UA_BrowseRequest_init(&request);
	request.nodesToBrowseSize = 1;
	request.nodesToBrowse = UA_BrowseDescription_new();
	request.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
	request.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL; // Retrieve all available nodes
	UA_BrowseResponse response = UA_Client_Service_browse(client, request);
	for (size_t i = 0; i < response.resultsSize; ++i)
	{
		for (size_t j = 0; j < response.results[i].referencesSize; ++j)
		{
			UA_ReferenceDescription *ref = &response.results[i].references[j];
			printf("NodeID: %u:%u\n", ref->nodeId.nodeId.namespaceIndex, ref->nodeId.nodeId.identifier.numeric);
		}
	}
	UA_BrowseRequest_clear(&request);
	UA_BrowseResponse_clear(&response);
	UA_Client_disconnect(client);
	UA_Client_delete(client);
	return 0;
}

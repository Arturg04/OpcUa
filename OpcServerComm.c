#include "OpcLib.h"

// Function to connect to the OPC UA server
int		connectToOpcServer(UA_Client **clientPtr,const char *serverUrl)
{
	*clientPtr = UA_Client_new(); // Update the pointer through clientPtr
	UA_Client *client = *clientPtr; // Get the updated pointer

	if (client == NULL)
	{
		fprintf(stderr, "Failed to create client\n");
		return 0;
	}
	UA_StatusCode retval = UA_Client_connect(client, serverUrl);
	if (retval != UA_STATUSCODE_GOOD)
	{
		fprintf(stderr, "Failed to connect to the server, StatusCode %s\n", UA_StatusCode_name(retval));
		UA_Client_delete(client);
		return 0;
	}
	printf("Connected to the server!\n");
	return 1;
}

// Function to disconnect from the OPC UA server
void disconnectFromOpcServer(UA_Client *client)
{
	if (client != NULL)
	{
		UA_Client_disconnect(client);
		UA_Client_delete(client);
		printf("Disconnected from the server.\n");
	}
}

// Function to check the OPC UA connection status
int isServerReachable(const char *serverAddress)
{
    char command[100];
    snprintf(command, sizeof(command), "ping -c 4 -t 2 %s", serverAddress);

    // Execute the ping command and capture the result
    int result = system(command);

    // Check the return value to determine reachability
    if (result == 0) {
        // Ping was successful, server is reachable
        return 1;
    } else {
        // Ping failed, server is not reachable
        return 0;
    }
}


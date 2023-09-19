#include <stdio.h>
#include <stdlib.h>
#include <open62541/client.h>

// Define an OPC server endpoint URL
#define SERVER_ENDPOINT "opc.tcp://localhost:4840"

// Function to connect to an OPC server
UA_Client *connectToOPCServer(const char *endpointUrl) {
    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));

    UA_StatusCode connectStatus = UA_Client_connect(client, endpointUrl);
    if (connectStatus != UA_STATUSCODE_GOOD) {
        printf("Failed to connect to OPC server. Error code: %x\n", connectStatus);
        UA_Client_delete(client);
        return NULL;
    }

    printf("Connected to OPC server: %s\n", endpointUrl);
    return client;
}

// Function to disconnect from the OPC server
void disconnectFromOPCServer(UA_Client *client) {
    UA_Client_disconnect(client);
    UA_Client_delete(client);
    printf("Disconnected from OPC server.\n");
}

// Function to read data from an OPC server node
void readOPCData(UA_Client *client, const char *nodeId) {
    UA_ReadRequest request;
    UA_ReadRequest_init(&request);

    UA_ReadValueId itemsToRead[1];
    UA_ReadValueId_init(&itemsToRead[0]);
    itemsToRead[0].nodeId = UA_NODEID_STRING_ALLOC(1, nodeId); // Replace with your node ID
    itemsToRead[0].attributeId = UA_ATTRIBUTEID_VALUE;

    request.nodesToRead = itemsToRead;
    request.nodesToReadSize = 1;

    UA_ReadResponse response = UA_Client_Service_read(client, request);

    if (response.responseHeader.serviceResult == UA_STATUSCODE_GOOD) {
        UA_Variant *value = &response.results[0].value;
        printf("Read value: ");
        UA_Variant_print(value);
        printf("\n");
    } else {
        printf("Read failed. Error code: %x\n", response.responseHeader.serviceResult);
    }

    UA_ReadRequest_deleteMembers(&request);
    UA_ReadResponse_deleteMembers(&response);
}

int main() {
    UA_Client *client = connectToOPCServer(SERVER_ENDPOINT);

    if (client != NULL) {
        // Replace "YourNodeID" with the actual Node ID you want to read
        readOPCData(client, "YourNodeID");

        disconnectFromOPCServer(client);
    }

    return 0;
}

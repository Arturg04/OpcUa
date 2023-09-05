#include <stdio.h>
#include <client_highlevel_async.h>

// Function to read the value of a node
UA_StatusCode readNodeValue(UA_Client *client, const UA_NodeId nodeId, UA_Variant *value) {
    UA_ReadRequest request;
    UA_ReadRequest_init(&request);
    request.nodesToReadSize = 1;
    request.nodesToRead = UA_ReadValueId_new();
    request.nodesToRead[0].nodeId = nodeId;
    request.nodesToRead[0].attributeId = UA_ATTRIBUTEID_VALUE; // Read the value attribute

    UA_ReadResponse response = UA_Client_Service_read(client, request);

    if (response.responseHeader.serviceResult == UA_STATUSCODE_GOOD) {
        *value = response.results[0].value;
    }

    UA_ReadResponse_clear(&response); // Use UA_ReadResponse_clear to replace deprecated function

    return response.responseHeader.serviceResult;
}

int main(void) {
    UA_Client *client = UA_Client_new();
    if (client == NULL) {
        fprintf(stderr, "Failed to create client\n");
        return 1;
    }

    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://192.168.10.1:4840");

    if (retval != UA_STATUSCODE_GOOD) {
        fprintf(stderr, "Failed to connect to the server, StatusCode %s\n", UA_StatusCode_name(retval));
        UA_Client_delete(client);
        return 1;
    }

    printf("Connected to the server!\n");

    // Specify the NodeId of the node you want to read
    UA_NodeId nodeIdToRead = UA_NODEID_NUMERIC(4, 3); // Example NodeId, replace with the NodeId you want to read

    UA_Variant value;
    UA_StatusCode readStatus = readNodeValue(client, nodeIdToRead, &value);

    if (readStatus == UA_STATUSCODE_GOOD) {
        printf("Value of Node %u:%u: ", nodeIdToRead.namespaceIndex, nodeIdToRead.identifier.numeric);

        // Check if the data type is "Dword" (UInt32)
        if (value.type == &UA_TYPES[UA_TYPES_UINT32]) {
            printf("%" PRIu32 "\n", *(UA_UInt32 *)value.data); // Use PRIu32 for uint32_t
        }
		else if (value.type == &UA_TYPES[UA_TYPES_UINT16])
		{
            printf("%" PRIu16 "\n", *(UA_UInt16 *)value.data);
		}
		else {
            printf("Unsupported data type\n");
        }
    } else {
        printf("Read failed with StatusCode %s\n", UA_StatusCode_name(readStatus));
    }

    UA_Client_disconnect(client);
    UA_Client_delete(client);
    return 0;
}

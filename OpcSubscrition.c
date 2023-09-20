#include "OpcLib.h"
#include "open62541/client.h" // Include the main client header
#include <open62541/client_subscriptions.h>

// Callback function to handle data changes
static void dataChangeCallback(UA_Client *client, UA_UInt32 monitoredItemId, UA_DataValue *value, void *context) {
    // Suppress "unused parameter" warnings for client, monitoredItemId, and context
    (void)client;
    (void)monitoredItemId;
    (void)context;

    if (value->status == UA_STATUSCODE_GOOD) {
        UA_Variant *variant = &(value->value);

        // Extract and process the value (assuming it's a double)
        double newValue = *(UA_Double *)variant->data;

        // You can access the struct TagInfo using context (passed when adding the callback)
        struct TagInfo *tagInfo = (struct TagInfo *)context;

        // Print the entire struct and the updated value
        printf("Received value update for node %s:\n", tagInfo->node);
        printf("  Name: %s\n", tagInfo->name);
        printf("  Node: %s\n", tagInfo->node);
        printf("  Tag Type: %s\n", tagInfo->tag_type);
        printf("  New Value: %f\n", newValue);

        UA_Variant_clear(variant);
    }
}

UA_StatusCode createSubscriptions(UA_Client *client, struct TagInfo tags[], int tagCount) {
    UA_Client_Subscriptions_new(client);

    for (int i = 0; i < tagCount; i++) {
        UA_Client_Subscriptions_add(client, UA_CreateSubscriptionRequest_default(), NULL, NULL, NULL);
        UA_NodeId nodeId = UA_NODEID_STRING(1, tags[i].node);
        UA_MonitoredItemCreateRequest itemRequest = UA_MonitoredItemCreateRequest_default(nodeId);
        itemRequest.requestedParameters.samplingInterval = 500.0; // Milliseconds
        itemRequest.requestedParameters.queueSize = 10;
        itemRequest.requestedParameters.discardOldest = true;

        UA_MonitoredItemCreateResult itemResult;
        UA_UInt32 subscriptionId;
        UA_StatusCode result = UA_Client_Subscriptions_addMonitoredItem(client, subscriptionId, itemRequest, NULL, NULL, &itemResult);
        if (result != UA_STATUSCODE_GOOD) {
            printf("Failed to create subscription for node %s. Error code: %s\n", tags[i].node, UA_StatusCode_name(result));
            return result;
        }

        // Set up a callback for data changes
        UA_Client_Subscriptions_addDataChangeCallback(client, itemResult.monitoredItemId, dataChangeCallback, &tags[i]);
    }

    UA_Client_Subscriptions_manuallySendPublishRequest(client);
    return UA_STATUSCODE_GOOD;
}

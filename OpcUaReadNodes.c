#include "OpcLib.h"

void addSubscriptions(UA_Client *client, struct Tagsinfo *tags, size_t numTags) {
    // Depending on your library version, you might need different settings
    UA_CreateSubscriptionRequest request = UA_CreateSubscriptionRequest_default();
    request.requestedPublishingInterval = 100.0; // Set your desired publishing interval in milliseconds
    UA_CreateSubscriptionResponse response = UA_Client_Subscriptions_create(client, request, NULL, NULL, NULL);

    for (size_t i = 0; i < numTags; i++) {
        if (i % 5 == 0) { // Create a new subscription every 5 elements
            UA_NodeId monitoredNodeId = UA_NODEID_STRING(1, tags[i].node);

            UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(monitoredNodeId);
            monRequest.requestedParameters.samplingInterval = 100.0; // Set your desired sampling interval in milliseconds
            monRequest.requestedParameters.queueSize = 10; // Set your desired queue size

            UA_Client_Subscriptions_addMonitoredItem(client, response.subscriptionId, UA_TIMESTAMPSTORETURN_BOTH, monRequest, &tags[i], NULL, NULL);

            printf("Added subscription for Tag: %s, Node: %s\n", tags[i].name, tags[i].node);
        }
    }
}

void dataChangeHandler(UA_Client *client, UA_UInt32 subId, void *subContext, UA_UInt32 monId, void *monContext, UA_DataValue *value) {
    struct Tagsinfo *tag = (struct Tagsinfo *)monContext;

    // Check if the value is valid and contains data
    if (value->hasValue) {
        int32_t *dataPtr = (int32_t *)(value->value.data);

        if (dataPtr) {
            printf("Received Data Change for Tag: %s, Node: %s\n", tag->name, tag->node);
            printf("New Value: %d\n", *dataPtr);

            // Update the database with the new value
            updateValue(tag->name, *dataPtr);
        } else {
            printf("Received Data Change for Tag: %s, Node: %s (Data is null)\n", tag->name, tag->node);
        }
    } else {
        printf("Received Data Change for Tag: %s, Node: %s (No data)\n", tag->name, tag->node);
    }
}


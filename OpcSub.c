/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpcSub.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-pinh <artur.13.goncalves@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 13:51:06 by ade-pinh          #+#    #+#             */
/*   Updated: 2023/10/11 14:36:38 by ade-pinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpcLib.h"

static void	handler_TheAnswerChanged(UA_Client *client, UA_UInt32 subId, void *subContext,
						UA_UInt32 monId, void *monContext, UA_DataValue *value)
{
	client = client;
	subId = subId;
	subContext = subContext;
	monId = monId;
	monContext = monContext;
	if(value->hasValue)
	{
		UA_Int32 printValue = *(UA_Int32*)value->value.data;
		printf("the value is: %i\n", printValue);
	} else {
		printf("The value is not valid.\n");
	}
}

static void	subscriptionInactivityCallback(UA_Client *client, UA_UInt32 subId, void *subContext)
{
	(void)client;
	(void)subContext;
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
				"Subscription %u went inactive.", subId);
}

void	CreateSubscription(UA_Client *client, t_TagInfo *Tag)
{
    UA_CreateSubscriptionRequest request = UA_CreateSubscriptionRequest_default();
	request.requestedPublishingInterval = 100;

	UA_CreateSubscriptionResponse response = UA_Client_Subscriptions_create(client, request, NULL, NULL, NULL);
	if(response.responseHeader.serviceResult != UA_STATUSCODE_GOOD)
	{
		UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
					"Failed to create a subscription. Error code %s", UA_StatusCode_name(response.responseHeader.serviceResult));
		return ;
	}
	// Tag->subid = response.subscriptionId;
	// subscriptionInactivityCallback(client, Tag->subid, NULL);

	UA_NodeId nodeId = UA_NODEID_NUMERIC(Tag->node->index, Tag->node->identifier);
	UA_MonitoredItemCreateRequest monRequest =
					UA_MonitoredItemCreateRequest_default(nodeId);

	UA_MonitoredItemCreateResult monResponse =
					UA_Client_MonitoredItems_createDataChange(client, response.subscriptionId,
													UA_TIMESTAMPSTORETURN_BOTH,
													monRequest, NULL, handler_TheAnswerChanged, NULL);

	if(monResponse.statusCode != UA_STATUSCODE_GOOD) {
		UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
					"Failed to create a monitored item. Error code %s", UA_StatusCode_name(monResponse.statusCode));
		return ;
	}
}
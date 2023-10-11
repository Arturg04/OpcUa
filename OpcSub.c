/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpcSub.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-pinh <artur.13.goncalves@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 13:51:06 by ade-pinh          #+#    #+#             */
/*   Updated: 2023/10/11 15:15:07 by ade-pinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpcLib.h"

static void	handler_the_answer_changed(UA_Client *client,
						UA_UInt32 subId, void *subContext,
						UA_UInt32 monId, void *monContext,
						UA_DataValue *value)
{
	UA_Int32	print_value;

	client = client;
	subId = subId;
	subContext = subContext;
	monId = monId;
	monContext = monContext;
	if (value->hasValue)
	{
		print_value = *(UA_Int32 *)value->value.data;
		printf("the value is: %i\n", print_value);
		return ;
	}
	printf("The value is not valid.\n");
}

static void	subscription_inactivity_callback(UA_Client *client,
						UA_UInt32 subId,
						void *subContext)
{
	(void)client;
	(void)subContext;
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
		"Subscription %u went inactive.", subId);
}

void	create_subscription(UA_Client *client, t_TagInfo *Tag)
{
	UA_CreateSubscriptionRequest		request;
	UA_CreateSubscriptionResponse		response;
	UA_MonitoredItemCreateRequest		mon_request;
	UA_MonitoredItemCreateResult		mon_response;
	UA_NodeId							node_id;

	request = UA_CreateSubscriptionRequest_default();
	request.requestedPublishingInterval = 100;
	request.requestedLifetimeCount = 0;
	response = UA_Client_Subscriptions_create(client,
			request,
			NULL, NULL, NULL);
	if (response.responseHeader.serviceResult != UA_STATUSCODE_GOOD)
		return ;
	node_id = UA_NODEID_NUMERIC(Tag->node->index, Tag->node->identifier);
	mon_request = UA_MonitoredItemCreateRequest_default(node_id);
	mon_response = UA_Client_MonitoredItems_createDataChange(client,
			response.subscriptionId,
			UA_TIMESTAMPSTORETURN_BOTH,
			mon_request, NULL, handler_the_answer_changed, NULL);
	if (mon_response.statusCode != UA_STATUSCODE_GOOD)
		return ;
}
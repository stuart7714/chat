#pragma once

#include <grpcpp/grpcpp.h>

#include "MessageQueue.h"

/*
 * The implementation of the gRPC service.
 * Clients may subscribe to the service to receive notifications.
 * Clients may also send messages to the service which will then
 * be broadcast to all subscribed clients.
 */
class Service final : public chat::ChatService::Service
{
public:
    Service(const std::string& url);

private:
    // A client has subscribed to the service.
    // We can send notifications back to this client.
    grpc::Status Subscribe(
        grpc::ServerContext* context,
        const chat::SubscribeRequest* request,
        grpc::ServerWriter<chat::Notification>* writer) override;

    // A client has sent a message to the service.
    // This message should be broadcast back to all subscribed clients.
    grpc::Status SendMessage(
        grpc::ServerContext* context,
        const chat::SendMessageRequest* request,
        chat::SendMessageResponse* response) override;

    // Each client who has subscribed has an individual message queue.
    std::vector<std::shared_ptr<MessageQueue>> m_messageQueues;
};

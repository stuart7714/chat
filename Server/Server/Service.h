#pragma once

#include <grpcpp/grpcpp.h>

#include "MessageQueue.h"

class Service final : public chat::ChatService::Service
{
public:
    Service(const std::string& url);

private:
    grpc::Status Subscribe(
        grpc::ServerContext* context,
        const chat::SubscribeRequest* request,
        grpc::ServerWriter<chat::Notification>* writer) override;

    grpc::Status SendMessage(
        grpc::ServerContext* context,
        const chat::SendMessageRequest* request,
        chat::SendMessageResponse* response) override;

    std::vector<std::shared_ptr<MessageQueue>> m_messageQueues;
};

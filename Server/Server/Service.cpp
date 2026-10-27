#include "Message.h"
#include "Service.h"

Service::Service(const std::string& url)
{
    grpc::ServerBuilder serverBuilder;
    serverBuilder.AddListeningPort(url, grpc::InsecureServerCredentials());
    serverBuilder.RegisterService(this);

    std::unique_ptr<grpc::Server> server = serverBuilder.BuildAndStart();
    if (!server)
    {
        throw std::runtime_error("Failed to start gRPC server");
    }

    std::cout << "Service listening on " << url << std::endl;
    server->Wait();
}

grpc::Status Service::Subscribe(
    grpc::ServerContext* context,
    const chat::SubscribeRequest* request,
    grpc::ServerWriter<chat::Notification>* writer)
{
    std::shared_ptr<MessageQueue> messageQueue = std::make_shared<MessageQueue>();
    m_messageQueues.emplace_back(messageQueue);

    while (!context->IsCancelled())
    {
        std::shared_ptr<Message> message = messageQueue->WaitAndPop(std::chrono::milliseconds(100));
        if (message)
        {
            message->Write(writer);
        }
    }

    std::erase(m_messageQueues, messageQueue);

    return grpc::Status::OK;
}

grpc::Status Service::SendMessage(
    grpc::ServerContext* context,
    const chat::SendMessageRequest* request,
    chat::SendMessageResponse* response)
{
    const std::string& clientId = request->client_id();
    const std::string& text = request->text();

    std::shared_ptr<Message> message = std::make_shared<Message>(clientId, text);
    for (std::shared_ptr<MessageQueue>& messageQueue : m_messageQueues)
    {
        messageQueue->Push(message);
    }

    return grpc::Status::OK;
}

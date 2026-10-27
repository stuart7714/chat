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
    // Create a queue of messages which can be sent back
    // to this client via the given writer.
    std::shared_ptr<MessageQueue> messageQueue = std::make_shared<MessageQueue>();
    m_messageQueues.push_back(messageQueue);

    // Wait for the message queue to see if there is a message which
    // needs to be sent back to the client.
    // Also, periodically check to see if the gRPC connection is broken.
    while (!context->IsCancelled())
    {
        std::shared_ptr<Message> message = messageQueue->WaitAndPop(std::chrono::milliseconds(100));
        if (message)
        {
            message->Write(writer);
        }
    }

    // The gRPC connection is broken so forget the client subscription.
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

    // A client has sent a message which needs to be broadcast
    // to all subscribed clients. We add this message to all message
    // queues (there is a queue for each subscribed client).
    std::shared_ptr<Message> message = std::make_shared<Message>(clientId, text);
    for (std::shared_ptr<MessageQueue>& messageQueue : m_messageQueues)
    {
        messageQueue->Push(message);
    }

    return grpc::Status::OK;
}

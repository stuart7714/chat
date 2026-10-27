#include "Message.h"

Message::Message(const std::string& clientId, const std::string& text)
    : m_clientId(clientId)
    , m_text(text)
{
}

void Message::Write(grpc::ServerWriter<chat::Notification>* writer)
{
    chat::Notification notification;
    chat::ClientMessage* clientMessage = notification.mutable_client_message();
    clientMessage->set_client_id(m_clientId);
    clientMessage->set_text(m_text);
    writer->Write(notification);
}

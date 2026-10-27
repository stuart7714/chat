#pragma once

#include "chat.grpc.pb.h"

/*
 * A message is a piece of text sent by a client.
 * The message may then be broadcast to subscribed clients.
 */
class Message
{
public:
	Message(const std::string& clientId, const std::string& text);
	
	// Send the message to a client as a notification.
	void Write(grpc::ServerWriter<chat::Notification>* writer);

private:
	// The identifier of the client who sent the message.
	std::string m_clientId;
	
	// The text of the message.
	std::string m_text;
};

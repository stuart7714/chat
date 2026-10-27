#pragma once

#include "chat.grpc.pb.h"

class Message
{
public:
	Message(const std::string& clientId, const std::string& text);
	void Write(grpc::ServerWriter<chat::Notification>* writer);
private:
	std::string m_clientId;
	std::string m_text;
};

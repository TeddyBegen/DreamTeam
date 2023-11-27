#if !defined(MARCH_MESSAGE_H_)
#define MARCH_MESSAGE_H_

#include <string>

#include "information.hpp"

// TODO: Implement strategy pattern for message types, that will be used
// in the execute() method of the message-related actions. The goal is to let the message itsself
// form a JSON string/object which will be parsed by the receiver.

namespace march
{
    enum MESSAGE_TYPE
    {
        REQUEST,
        INFORM,
        RESPONSE,
        BROADCAST
    };

    class message
    {
    public:
        message(MESSAGE_TYPE type, std::string body) : m_type(type), m_body(body){};
        virtual ~message() = default;

    public:
        MESSAGE_TYPE m_type;
        std::string m_body;
    };

    class global_message : public message
    {
    public:
        global_message(std::string body, MESSAGE_TYPE type, march::information sender_information)
            : message(type, body), m_sender_information(sender_information){};

        virtual ~global_message() = default;

    public:
        march::information m_sender_information;
    };

    class direct_message : public message
    {
    public:
        direct_message(std::string body, MESSAGE_TYPE type, march::information sender_information, march::information receiver_information)
            : message(type, body), m_sender_information(sender_information), m_receiver_information(receiver_information){};

        virtual ~direct_message() = default;

    public:
        march::information m_sender_information;
        march::information m_receiver_information;
    };

}

#endif
#if !defined(COMMS_HPP_)
#define COMMS_HPP_

#include <memory>
#include <queue>

#ifdef ARDUINO

#include "painlessMesh.h"
extern painlessMesh mesh;
extern Scheduler userScheduler;

#define MESH_PREFIX "dreamteamnetwork"
#define MESH_PASSWORD "inyourdreams"
#define MESH_PORT 5555

#else
extern int id_counter;
#endif

namespace dteam
{
    class node;

    enum class message_scope
    {
        broadcast,
        unicast
    };

    class comms
    {
    public:
        comms() = default;
        virtual ~comms() = default;

    public:
        virtual auto is_connected() -> bool = 0;

#ifdef ARDUINO
        virtual auto send(int to, std::string message, std::string query) -> void = 0;
        virtual auto send_broadcast(std::string message, std::string query) -> void = 0;
        virtual auto make_json(std::string message_body, std::string query) -> JsonObject = 0;
        virtual auto parse_message(String &message) -> JsonObject = 0;
#else
        virtual auto send(std::string message) -> void = 0;
        virtual auto send_broadcast(std::string message) -> void = 0;
#endif
        virtual auto receive(int from_who, std::string message) -> void = 0;
        virtual auto update() -> void = 0;
        virtual auto get_ID() -> int = 0;

    public:
        std::queue<DynamicJsonDocument> m_messages;

    public:
        virtual auto initialize(std::shared_ptr<dteam::node> node) -> void = 0;

    protected:
        auto set_node(std::shared_ptr<dteam::node> node) -> void { m_node = node; }
        bool m_connected = false;

        std::shared_ptr<dteam::node> m_node;
    private:
    };

#ifdef ARDUINO

    class real_comms : public comms
    {
    public:
        real_comms() = default;
        virtual ~real_comms() = default;

    public:
        auto initialize(std::shared_ptr<dteam::node> node) -> void override;

    public:
        auto is_connected() -> bool override { return m_connected; }

        auto send(int to, std::string message, std::string query) -> void override;
        // auto send_broadcast(JsonObject message) -> void override;
        auto send_broadcast(std::string message, std::string query) -> void override;

        auto receive(int from_who, std::string message) -> void override;
        auto update() -> void override;
        auto get_ID() -> int override;

        auto make_json(std::string message_body, std::string query) -> JsonObject override;
        auto parse_message(String &message) -> JsonObject override;
    };

#else

    class fake_comms : public comms
    {
    public:
        fake_comms(std::shared_ptr<dteam::node> node) : comms(node) { id_counter++ }
        virtual ~fake_comms() = default;

    public:
        auto is_connected() -> bool override { return true; }

        auto send() -> void override;
        auto receive() -> void override;
        auto update() -> void override;
        auto get_ID() -> int override { return id_counter; }
    };

#endif
};

#endif // COMMS_HPP_

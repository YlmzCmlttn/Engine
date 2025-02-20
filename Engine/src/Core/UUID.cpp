#include "Core/UUID.h"

namespace Engine
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<uint64_t> distrib;
    std::unordered_set<uint64_t> UUID::s_UUIDSet;

    UUID::UUID()
    {
        m_UUID = Generate();
    }

    UUID::UUID(uint64_t uuid)
    {
        m_UUID = uuid;
    }

    UUID UUID::Generate()
    {
        auto uuid = distrib(gen);
        while (UUID::s_UUIDSet.find(uuid) != UUID::s_UUIDSet.end())
        {
            uuid = distrib(gen);
        }
        UUID::s_UUIDSet.insert(uuid);
        return UUID(uuid);
    }
    
    

    
} // namespace Engine

#include "Core/UUID.h"

namespace Engine
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<uint64_t> distrib;
    //std::unordered_set<uint64_t> UUID::s_UUIDSet;

    UUID::UUID()
    {
        m_UUID = Generate();
    }

    UUID::UUID(uint64_t uuid)
    {
        m_UUID = uuid;
    }

    UUID::UUID(const UUID& other)
    {
        m_UUID = other.m_UUID;
    }

    UUID& UUID::operator=(const UUID& other)
    {
        if(this != &other)
        {
            m_UUID = other.m_UUID;
        }
        return *this;
    }

    UUID UUID::Generate()
    {
        auto uuid = distrib(gen);
        return UUID(uuid);
    }
    
    

    
} // namespace Engine

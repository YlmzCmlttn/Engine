#pragma once

namespace Engine
{
    class UUID
	{
	public:
		UUID();

		UUID(uint64_t uuid);
		UUID(const UUID& other);

		operator uint64_t () { return m_UUID; }
		operator const uint64_t () const { return m_UUID; }

		static UUID Generate();
	private:
		uint64_t m_UUID;
		//static std::unordered_set<uint64_t> s_UUIDSet;
	};
} // namespace Engine

namespace std {

	template <>
	struct hash<Engine::UUID>
	{
		std::size_t operator()(const Engine::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}
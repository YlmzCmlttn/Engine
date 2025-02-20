#pragma once

namespace Engine
{
    class UUID
	{
	public:
		UUID();

		operator uint64_t () { return m_UUID; }
		operator const uint64_t () const { return m_UUID; }

		static UUID Generate();
	private:
		UUID(uint64_t uuid);
		uint64_t m_UUID;
		static std::unordered_set<uint64_t> s_UUIDSet;
	};
} // namespace Engine

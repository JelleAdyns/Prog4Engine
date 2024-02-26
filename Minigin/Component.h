#pragma once
#include <memory>


namespace dae
{
	class GameObject;
	class Component
	{
	public:

		virtual ~Component() = default;

		Component(const Component&) = delete;
		Component(Component&&) noexcept = delete;
		Component& operator= (const Component&) = delete;
		Component& operator= (Component&&) noexcept = delete;

		virtual void Update() = 0;

	protected:
		Component(const std::shared_ptr<GameObject>& pOwner): m_pOwner{pOwner}
		{}
		std::shared_ptr<GameObject> GetOwner() const { return m_pOwner.lock();}
	private:
		std::weak_ptr<GameObject> m_pOwner;
	};
}


#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <glm/vec2.hpp>
#include "GameTime.h"

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

		//Idea from Wout Firlefyn
		virtual void Start() = 0;

		virtual void Update() = 0;
		virtual void PrepareImGuiRender() = 0;

	protected:
		explicit Component(GameObject* pOwner): m_pOwner{pOwner}
		{}
		GameObject* GetOwner() const { return m_pOwner;}
	private:
		GameObject* m_pOwner;
	};
}

#endif // !COMPONENT_H
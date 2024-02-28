#pragma once
#include <memory>
#include <map>
#include <typeindex>
#include <stdexcept>
#include <cassert>
#include "Component.h"
#include "TransformComponent.h"


namespace dae
{
	class Texture2D;

	class GameObject final
	{
	public:

		void Update();
		void Render() const;

		bool IsDead() const;
		TransformComponent* GetTransform() const;

		template <typename T, typename ...Args>
		void AddComponent(Args&&... args)
		{
			assert((typeid(T) != typeid(TransformComponent)) && "Type passed to 'AddComponent' was a TransformComponent");
			if (!HasComponent<T>()) m_pMapComponents[typeid(T)] = std::move(std::make_unique<T>(this, args...));
			else throw std::runtime_error(std::string{ "Object already owns a reference to an instance of the passed component" });
		}

		template <typename T>
		void RemoveComponent()
		{
			if (HasComponent<T>()) m_pMapComponents.erase(typeid(T));
		}

		template <typename T>
		T* GetComponent() const
		{
			if (HasComponent<T>()) return dynamic_cast<T*>(m_pMapComponents.at(typeid(T)).get());
			else throw std::runtime_error(std::string{ "Object doesn't own a reference to an instance of the passed component" });
		}

		template <typename T>
		bool HasComponent() const
		{
			return m_pMapComponents.contains(typeid(T));
		}


		GameObject() :
			m_IsDead{},
			m_pTransform{std::make_unique<TransformComponent>(this)},
			m_pMapComponents{}
		{}
		GameObject(float x, float y) :
			m_IsDead{},
			m_pTransform{ std::make_unique<TransformComponent>(this) },
			m_pMapComponents{}
		{
			m_pTransform->SetPosition(x, y);
		}
		virtual ~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		
		bool m_IsDead;

		std::unique_ptr<TransformComponent> m_pTransform;
		std::map<std::type_index, std::unique_ptr<Component>> m_pMapComponents;	
	};
}

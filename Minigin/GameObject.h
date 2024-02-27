#pragma once
#include <memory>
#include <map>
#include <typeindex>
#include <stdexcept>
#include "Component.h"


namespace dae
{
	class Texture2D;

	class GameObject final
	{
	public:

		void Update();
		void Render() const;

		
		template <typename T, typename ...Args>
		void AddComponent(/*const std::shared_ptr<GameObject>& pOwner,*/ Args&&... args)
		{
			if (!HasComponent<T>()) m_pMapComponents[typeid(T)] = std::move(std::make_unique<T>(this, args...));
			else throw std::runtime_error(std::string{ "Object already owns a reference to the passed component" });
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
			else throw std::runtime_error(std::string{ "Object already owns a reference to the passed component" });
		}

		template <typename T>
		bool HasComponent() const
		{
			return m_pMapComponents.contains(typeid(T));
		}


		GameObject() = default;

		virtual ~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::map<std::type_index, std::unique_ptr<Component>> m_pMapComponents;	
	};
}

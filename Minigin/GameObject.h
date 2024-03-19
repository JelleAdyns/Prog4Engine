#pragma once
#include <memory>
#include <map>
#include <vector>
#include <typeindex>
#include <stdexcept>
#include <cassert>
#include <algorithm>
#include "Component.h"
#include "TransformComponent.h"
#include "RenderComponent.h"

namespace dae
{
	class Texture2D;

	class GameObject final
	{
	public:

		GameObject();
		GameObject(float x, float y);

		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update();
		void PrepareImGuiRender();
		void Render() const;

		bool IsDead() const;

		void SetParent(const std::unique_ptr<GameObject>& pParent, bool keepWorldPosition);
		bool IsChild(const std::unique_ptr<GameObject>& pGameObject) const;
		void SetLocalPos(const glm::vec2& newLocalPos);
		void SetPosDirty();
		const glm::vec2& GetWorldPosition();
		const glm::vec2& GetLocalPosition();
	

		template <typename T, typename ...Args>
		void AddComponent(Args&&... args)
		{
			assert((typeid(T) != typeid(TransformComponent)) && "Type passed to 'AddComponent' was a TransformComponent");
			assert((typeid(T) != typeid(RenderComponent)) && "Type passed to 'AddComponent' was a RenderComponent, use AddRenderComponent instead!");
			if (!HasComponent<T>()) m_pMapComponents[typeid(T)] = std::move(std::make_unique<T>(this, args...));
			else throw std::runtime_error("Object already owns a reference to an instance of the passed component" );
		}

		void AddRenderComponent(bool useMiddleOfTexture = false)
		{
			auto pComp = std::make_unique<RenderComponent>(this, useMiddleOfTexture);
			m_pRenderComponent = pComp.get();
			m_pMapComponents[typeid(RenderComponent)] = std::move(pComp);
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
			else throw std::runtime_error("Object doesn't own a reference to an instance of the passed component");
		}

		template <typename T>
		bool HasComponent() const
		{
			return m_pMapComponents.contains(typeid(T));
		}

	private:
		
		bool m_IsDead;
		bool m_IsPosDirty;
		std::unique_ptr<TransformComponent> m_LocalTransform;
		std::unique_ptr<TransformComponent> m_WorldTransform;

		//Tree
		GameObject* m_pParent;
		std::vector<GameObject*> m_pVecChildren;

		//Components
		RenderComponent* m_pRenderComponent;
		std::map<std::type_index, std::unique_ptr<Component>> m_pMapComponents;	

		void UpdateWorldPosition();
	};

}

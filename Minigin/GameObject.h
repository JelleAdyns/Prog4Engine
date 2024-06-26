#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H


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
#include "PhysicsComponent.h"

namespace dae
{
	class Texture2D;

	class GameObject final
	{
	public:

		GameObject();
		GameObject(int x, int y);
		GameObject(float x, float y);
		GameObject(const glm::vec2& pos);

		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) noexcept = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) noexcept = delete;

		void Start();
		void Update();
		void PrepareImGuiRender();
		void Render() const;
		void FixedUpdate();

		void MarkDead();
		bool IsDead() const;

		void SetParent(const std::unique_ptr<GameObject>& pParent, bool keepWorldPosition);
		void SetParent(GameObject* pParent, bool keepWorldPosition);
		bool IsChild(const std::unique_ptr<GameObject>& pGameObject) const;
		bool IsChild(GameObject* pGameObject) const;
		void SetLocalPos(float x, float y);
		void SetLocalPos(const glm::vec2& newLocalPos);
		void SetPosDirty();
		const glm::vec2& GetWorldPosition();
		const glm::vec2& GetLocalPosition();
	

		template <typename T, typename ...Args>
			requires std::derived_from<T, Component>
		void AddComponent(Args&&... args)
		{
			assert((typeid(T) != typeid(TransformComponent)) && "Type passed to 'AddComponent' was a TransformComponent");
			assert((typeid(T) != typeid(RenderComponent)) && "Type passed to 'AddComponent' was a RenderComponent, use AddRenderComponent instead!");
			assert((typeid(T) != typeid(PhysicsComponent)) && "Type passed to 'AddComponent' was a PhysicsComponent, use AddPhysicsComponent instead!");
			if (!HasComponent<T>()) m_pMapComponents[typeid(T)] = std::move(std::make_unique<T>(this, args...));
			else throw std::runtime_error("Object already owns a reference to an instance of the passed component" );
		}

		void AddRenderComponent(bool useMiddleOfTexture = false)
		{
			auto pComp = std::make_unique<RenderComponent>(this, useMiddleOfTexture);
			m_pRenderComponent = pComp.get();
			m_pMapComponents[typeid(RenderComponent)] = std::move(pComp);
		}

		void AddPhysicsComponent()
		{
			auto pComp = std::make_unique<PhysicsComponent>(this);
			m_pPhysicsComponent = pComp.get();
			m_pMapComponents[typeid(PhysicsComponent)] = std::move(pComp);
		}

		template <typename T>
			requires std::derived_from<T, Component>
		void RemoveComponent()
		{
			if (HasComponent<T>()) m_pMapComponents.erase(typeid(T));
		}

		template <typename T>
			requires std::derived_from<T, Component>
		T* GetComponent() const
		{
			if (HasComponent<T>()) return dynamic_cast<T*>(m_pMapComponents.at(typeid(T)).get());
			else throw std::runtime_error("Object doesn't own a reference to an instance of the passed component");
		}

		template <typename T>
			requires std::derived_from<T, Component>
		bool HasComponent() const
		{
			return m_pMapComponents.contains(typeid(T));
		}

	private:
		
		bool m_IsDead;
		bool m_IsPosDirty;
		std::unique_ptr<TransformComponent> m_pLocalTransform;
		std::unique_ptr<TransformComponent> m_pWorldTransform;

		//Tree
		GameObject* m_pParent;
		std::vector<GameObject*> m_pVecChildren;

		//Components
		RenderComponent* m_pRenderComponent;
		PhysicsComponent* m_pPhysicsComponent;
		std::map<std::type_index, std::unique_ptr<Component>> m_pMapComponents;	

		void UpdateWorldPosition();
	};

}
#endif // !GAMEOBJECT_H
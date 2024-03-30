#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "RenderComponent.h"
#include "Textcomponent.h"

namespace dae
{
	GameObject::GameObject() :
		m_IsDead{},
		m_IsPosDirty{true},
		m_pRenderComponent{nullptr},
		m_pMapComponents{},
		m_pParent{ nullptr },
		m_pVecChildren{},
		m_LocalTransform{ std::make_unique<TransformComponent>(this) },
		m_WorldTransform{ std::make_unique<TransformComponent>(this) }
	{}
	GameObject::GameObject(float x, float y) :
		m_IsDead{},
		m_IsPosDirty{true},
		m_pRenderComponent{ nullptr },
		m_pMapComponents{},
		m_pParent{ nullptr },
		m_pVecChildren{},
		m_LocalTransform{ std::make_unique<TransformComponent>(this) },
		m_WorldTransform{ std::make_unique<TransformComponent>(this) }
	{
		m_LocalTransform->SetPosition(glm::vec2{x,y});
		m_WorldTransform->SetPosition(glm::vec2{ x,y });
	}

	void GameObject::Update()
	{
		for (auto& component: m_pMapComponents)
		{
			component.second->Update();
		}
	}
	void GameObject::PrepareImGuiRender()
	{
		for (auto& component : m_pMapComponents)
		{
			component.second->PrepareImGuiRender();
		}

	}
	void GameObject::Render() const
	{
		if (m_pRenderComponent) m_pRenderComponent->Render();
	}

	bool GameObject::IsDead() const
	{
		return m_IsDead;
	}

	void GameObject::SetParent(const std::unique_ptr<GameObject>& pParent, bool keepWorldPosition)
	{
		if (pParent.get() == this || pParent.get() == m_pParent || IsChild(pParent))
		{
			throw std::runtime_error{ "New Parent is not valid" };
		}

		/*if (pParent == nullptr || !keepWorldPosition) SetLocalPos(GetWorldPosition());
		else if (keepWorldPosition) SetLocalPos(GetWorldPosition() - pParent->GetWorldPosition());*/
		if (pParent == nullptr) SetLocalPos(GetWorldPosition());
		else
		{
			if (keepWorldPosition) SetLocalPos(GetWorldPosition() - pParent->GetWorldPosition());
			else SetPosDirty();
		}

		if (m_pParent) m_pParent->m_pVecChildren.erase(std::remove(m_pParent->m_pVecChildren.begin(), m_pParent->m_pVecChildren.end(), this));
		m_pParent = pParent.get();
		if (m_pParent) m_pParent->m_pVecChildren.emplace_back(this);
	}

	bool GameObject::IsChild(const std::unique_ptr<GameObject>& pGameObject) const
	{
		return std::find(m_pVecChildren.cbegin(), m_pVecChildren.cend(), pGameObject.get()) != m_pVecChildren.cend();
	}

	void GameObject::SetLocalPos(const glm::vec2& newLocalPos)
	{
		m_LocalTransform->SetPosition(newLocalPos);
		SetPosDirty();
	}
	void GameObject::SetPosDirty()
	{
		m_IsPosDirty = true;
		std::for_each(m_pVecChildren.begin(), m_pVecChildren.end(), [&](GameObject* pChild)
			{
				pChild->SetPosDirty();
			});
	}

	const glm::vec2& GameObject::GetLocalPosition()
	{
		return m_LocalTransform->GetPosition();
	}
	const glm::vec2& GameObject::GetWorldPosition()
	{
		if (m_IsPosDirty)
			UpdateWorldPosition();
		return m_WorldTransform->GetPosition();
	}
	void GameObject::UpdateWorldPosition()
	{
		if (m_IsPosDirty)
		{
			if (m_pParent == nullptr) m_WorldTransform->SetPosition(m_LocalTransform->GetPosition());
			else m_WorldTransform->SetPosition(m_pParent->GetWorldPosition() + m_LocalTransform->GetPosition());

			m_IsPosDirty = false;
		}
		
	}
}


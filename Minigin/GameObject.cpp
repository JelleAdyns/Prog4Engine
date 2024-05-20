#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "RenderComponent.h"
#include "Textcomponent.h"

namespace dae
{
	GameObject::GameObject() :
		GameObject(0,0)
	{}

	GameObject::GameObject(int x, int y) :
		GameObject(static_cast<float>(x), static_cast<float>(y))
	{}

	GameObject::GameObject(float x, float y) :
		m_IsDead{},
		m_IsPosDirty{ true },
		m_pLocalTransform{ std::make_unique<TransformComponent>(this, glm::vec2{ x, y }) },
		m_pWorldTransform{ std::make_unique<TransformComponent>(this, glm::vec2{ x, y }) },
		m_pParent{ nullptr },
		m_pVecChildren{},
		m_pRenderComponent{ nullptr },
		m_pPhysicsComponent{ nullptr },
		m_pMapComponents{}
	{
	}

	void GameObject::Update()
	{
		m_pWorldTransform->Update();
		m_pLocalTransform->Update();
		for (auto& component: m_pMapComponents)
		{
			component.second->Update();
		}
	}
	void GameObject::Start()
	{
		m_pWorldTransform->Start();
		m_pLocalTransform->Start();
		for (auto& component : m_pMapComponents)
		{
			component.second->Start();
		}
	}
	void GameObject::PrepareImGuiRender()
	{
		m_pWorldTransform->PrepareImGuiRender();
		m_pLocalTransform->PrepareImGuiRender();
		for (auto& component : m_pMapComponents)
		{
			component.second->PrepareImGuiRender();
		}

	}
	void GameObject::Render() const
	{
		if (m_pRenderComponent) m_pRenderComponent->Render();
	}
	void GameObject::FixedUpdate()
	{
		if (m_pPhysicsComponent) m_pPhysicsComponent->FixedUpdate();
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

	void GameObject::SetLocalPos(float x, float y)
	{
		SetLocalPos(glm::vec2{ x,y });
	}
	void GameObject::SetLocalPos(const glm::vec2& newLocalPos)
	{
		m_pLocalTransform->SetPosition(newLocalPos);
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
		return m_pLocalTransform->GetPosition();
	}
	const glm::vec2& GameObject::GetWorldPosition()
	{
		if (m_IsPosDirty)
			UpdateWorldPosition();
		return m_pWorldTransform->GetPosition();
	}
	void GameObject::UpdateWorldPosition()
	{
		if (m_IsPosDirty)
		{
			if (m_pParent == nullptr) m_pWorldTransform->SetPosition(m_pLocalTransform->GetPosition());
			else m_pWorldTransform->SetPosition(m_pParent->GetWorldPosition() + m_pLocalTransform->GetPosition());

			m_IsPosDirty = false;
		}
		
	}
}


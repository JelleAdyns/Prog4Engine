#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <Component.h>
#include <string>

namespace dae
{
	class Texture2D;
	class RenderComponent;
	class SpriteComponent final : public Component
	{
	public:
		explicit SpriteComponent(GameObject* pOwner, const std::string& texturePath, int nrCols, int nrFrames, float frameTime, bool needsToUpdate = true);
		virtual ~SpriteComponent() = default;


		SpriteComponent(const SpriteComponent& other) = delete;
		SpriteComponent(SpriteComponent&& other) noexcept = delete;
		SpriteComponent& operator=(const SpriteComponent& other) = delete;
		SpriteComponent& operator=(SpriteComponent&& other) noexcept = delete;

		virtual void Update() override;
		virtual void PrepareImGuiRender() override;

		//virtual void SetCol(int col = 0);
		//void SetRow(int row = 0);
		//virtual void SetIsLookingLeft(bool isLookingLeft);
		//void GetDestinationRect() const;

		void SetUpdate(bool needsToUpdate);
		void SetFrame(int col, int row);

	private:
		
		//void SrcRect() const;

		bool m_NeedsUpdate;
		bool m_IsLookingLeft;

		int	m_CurrentCol;
		int	m_CurrentRow;

		const int m_NrOfCols;
		const int m_NrOfFrames;
		const int m_NrOfRows;
		const float	m_FrameTime;

		float m_PassedTime;

		RenderComponent* m_pRenderComponent;

		std::unique_ptr<Texture2D> m_pTexture;
	};
}
#endif // !SPRITECOMPONENT_H
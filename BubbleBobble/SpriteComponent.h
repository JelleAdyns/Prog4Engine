#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <Component.h>
#include <string>
#include <Subject.h>


namespace dae
{
	class Texture2D;
	class RenderComponent;
}

class PlayerComponent;
class SpriteComponent final : public dae::Component
{
public:
	explicit SpriteComponent(dae::GameObject* pOwner, const std::string& texturePath, int nrCols, int nrRows, float frameTime, bool needsUpdate = true, bool needsRowUpdate = false, PlayerComponent* pObserver = nullptr);
	explicit SpriteComponent(dae::GameObject* pOwner, std::unique_ptr<dae::Texture2D>&& pTexture, int nrCols, int nrRows, float frameTime, bool needsUpdate = true, bool needsRowUpdate = false, PlayerComponent* pObserver = nullptr);
	virtual ~SpriteComponent() = default;


	SpriteComponent(const SpriteComponent& other) = delete;
	SpriteComponent(SpriteComponent&& other) noexcept = delete;
	SpriteComponent& operator=(const SpriteComponent& other) = delete;
	SpriteComponent& operator=(SpriteComponent&& other) noexcept = delete;

	virtual void Update() override;
	virtual void PrepareImGuiRender() override;

	void SetCol(int col);
	void SetRow(int row);
	void SetHeightMarkers(float startHeight, float endHeight);
	void SetNrOfRows(int nrOfRows);
	void SetRowUpdate(bool rowNeedsToUpdate);
	void SetStartRow(int startRow);
	void SetFrameTime(float frameTime);
	void LookLeft(bool isLookingLeft);

	glm::ivec2 GetTextureSize() const;
	glm::ivec2 GetDestRectSize() const;



private:
		
	void UpdateSrcRect();

	bool m_SpriteIsDirty;
	bool m_NeedsUpdate;
	bool m_NeedsRowUpdate;
	bool m_IsLookingLeft;

	int	m_StartRow;

	int	m_CurrentCol;
	int	m_CurrentRow;

	int m_NrOfRows;
	const int m_NrOfCols;
	float m_FrameTime;

	float m_PassedTime;

	float m_StartHeightMarker;
	float m_EndHeightMarker;

	dae::RenderComponent* m_pRenderComponent;

	std::unique_ptr<dae::Texture2D> m_pTexture;

	std::unique_ptr<dae::Subject<SpriteComponent>> m_pRowFinished;
};

#endif // !SPRITECOMPONENT_H
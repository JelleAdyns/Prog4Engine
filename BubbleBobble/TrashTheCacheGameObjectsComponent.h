#ifndef TRASHTHECACHEGAMEOBJECTCOMPONENT_H
#define TRASHTHECACHEGAMEOBJECTCOMPONENT_H

#include "Component.h"
#include <string>
#include <vector>
#include <imgui_plot.h>

namespace dae
{
	class GameObject;
}

class CacheExperimentComponent;
class TrashTheCacheGameObjectsComponent final : public dae::Component
{
public:
	explicit TrashTheCacheGameObjectsComponent(dae::GameObject* pOwner, int nrOfSamples, const std::string& nameOfGuiWindow);
	virtual ~TrashTheCacheGameObjectsComponent();
	TrashTheCacheGameObjectsComponent(const TrashTheCacheGameObjectsComponent& other) = delete;
	TrashTheCacheGameObjectsComponent(TrashTheCacheGameObjectsComponent&& other) = delete;
	TrashTheCacheGameObjectsComponent& operator=(const TrashTheCacheGameObjectsComponent& other) = delete;
	TrashTheCacheGameObjectsComponent& operator=(TrashTheCacheGameObjectsComponent&& other) = delete;

	virtual void Update() override;
	virtual void PrepareImGuiRender() override;
private:

	void TestGameObject3D();
	void TestGameObject3DAlt();
	CacheExperimentComponent* m_Experiment;

	bool m_GameObject3DStarted;
	bool m_GameObject3DAltStarted;
	int m_NrOfSamples;
	ImColor m_ColorGameObject3D;
	ImColor m_ColorGameObject3DAlt;
	std::string m_Title;
	ImGui::PlotConfig m_PlotGameObject3D;
	ImGui::PlotConfig m_PlotGameObject3DAlt;
	ImGui::PlotConfig m_PlotCombined;
	std::vector<float> m_ResultsGameObject3D;
	std::vector<float> m_ResultsGameObject3DAlt;

	std::vector<float> m_PlotsData;
};

#endif // !TRASHTHECACHEGAMEOBJECTCOMPONENT_H
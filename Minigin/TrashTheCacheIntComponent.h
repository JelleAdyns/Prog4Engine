#pragma once
#include "Component.h"
#include <string>
#include <imgui_plot.h>
namespace dae
{
	class GameObject;
	class CacheExperimentComponent;
	class TrashTheCacheIntComponent final : public Component
	{
	public:
		explicit TrashTheCacheIntComponent(GameObject* pOwner, int nrOfSamples, const std::string& nameOfGuiWindow);
		virtual ~TrashTheCacheIntComponent() = default;
		TrashTheCacheIntComponent(const TrashTheCacheIntComponent& other) = delete;
		TrashTheCacheIntComponent(TrashTheCacheIntComponent&& other) = delete;
		TrashTheCacheIntComponent& operator=(const TrashTheCacheIntComponent& other) = delete;
		TrashTheCacheIntComponent& operator=(TrashTheCacheIntComponent&& other) = delete;

		virtual void Update() override;
		virtual void PrepareImGuiRender() override;
	private:
		CacheExperimentComponent* m_Experiment;
		
		bool m_ExperimentStarted;
		int m_NrOfSamples;
		std::string m_Title;
		ImGui::PlotConfig m_PlotConf;
	};

}

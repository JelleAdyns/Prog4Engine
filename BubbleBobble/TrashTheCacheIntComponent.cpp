#include "TrashTheCacheIntComponent.h"
#include "CacheExperimentComponent.h"
#include "GameObject.h"


TrashTheCacheIntComponent::TrashTheCacheIntComponent(dae::GameObject* pOwner, int nrOfSamples, const std::string& nameOfGuiWindow) :
	dae::Component{ pOwner },
	m_Experiment{},
	m_ExperimentStarted{false},
	m_NrOfSamples{ nrOfSamples },
	m_Title{ nameOfGuiWindow }
{
}

void TrashTheCacheIntComponent::Start()
{
	if (!m_Experiment) m_Experiment = GetOwner()->GetComponent<CacheExperimentComponent>();
}

void TrashTheCacheIntComponent::Update()
{
	
}

void TrashTheCacheIntComponent::PrepareImGuiRender()
{

	static uint32_t selection_start = 0, selection_length = 0;



	ImGui::Begin(m_Title.c_str());
	ImGui::InputInt("# samples", &m_NrOfSamples);
	if (!m_ExperimentStarted)
	{
		if (ImGui::Button("Trash the cache"))
		{
			m_ExperimentStarted = true;
			ImGui::Text("Wait for it ...");
		}

	}
	else
	{
		using Type = int;
		m_Experiment->StartExperiment<Type>(m_NrOfSamples, [&](Type* arr, int i) { arr[i] += 2; });

		auto& results = m_Experiment->GetResults();
		m_PlotConf.values.xs = m_Experiment->GetXValues().data();
		m_PlotConf.values.ys = results.data();
		m_PlotConf.values.count = int(results.size());

		m_PlotConf.values.ys_count = 1;
		m_PlotConf.values.color = ImColor(255,0,0);
		m_PlotConf.scale.min = 0;
		m_PlotConf.scale.max = float(results[0]);
		m_PlotConf.tooltip.show = true;
	
		m_PlotConf.selection.show = true;
		m_PlotConf.selection.start = &selection_start;
		m_PlotConf.selection.length = &selection_length;
		m_PlotConf.frame_size = ImVec2(250, 100);
			
		m_ExperimentStarted = false;
	}

	if (!m_Experiment->GetResults().empty())
	{
		ImGui::Plot((m_Title + "plot").c_str(), m_PlotConf);
	}

	ImGui::End();
}

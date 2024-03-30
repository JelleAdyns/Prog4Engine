#include "TrashTheCacheGameObjectsComponent.h"
#include "GameObject.h"
#include "CacheExperimentComponent.h"


namespace dae
{
	struct transform
	{
		float matrix[16]{
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};
	};

	class GameObject3D {
	public:
		transform local{};
		int id{};
	};

	class GameObject3DAlt {
	public:
		transform* local{};
		int id{};
	};


	TrashTheCacheGameObjectsComponent::TrashTheCacheGameObjectsComponent(GameObject* pOwner, int nrOfSamples, const std::string& nameOfGuiWindow):
		Component{pOwner},
		m_Experiment{},
		m_GameObject3DStarted{false},
		m_GameObject3DAltStarted{false},
		m_NrOfSamples{nrOfSamples},
		m_ColorGameObject3D{ImColor(0,255,0)},
		m_ColorGameObject3DAlt{ImColor(0,255,255)},
		m_Title{nameOfGuiWindow},
		m_PlotGameObject3D{},
		m_PlotGameObject3DAlt{},
		m_PlotCombined{},
		m_ResultsGameObject3D{},
		m_ResultsGameObject3DAlt{}
	{
	}

	TrashTheCacheGameObjectsComponent::~TrashTheCacheGameObjectsComponent()
	{

	}

	void TrashTheCacheGameObjectsComponent::Update()
	{
		if (!m_Experiment) m_Experiment = GetOwner()->GetComponent<CacheExperimentComponent>();
	}

	void TrashTheCacheGameObjectsComponent::PrepareImGuiRender()
	{
		
		ImGui::Begin(m_Title.c_str());
		ImGui::InputInt("# samples", &m_NrOfSamples);
		TestGameObject3D();
		TestGameObject3DAlt();

		if (!m_ResultsGameObject3D.empty() && !m_ResultsGameObject3DAlt.empty())
		{
			
			ImGui::Text("Combined:");
			m_PlotCombined.values.xs = m_Experiment->GetXValues().data();
			//m_PlotCombined.values.ys = m_ResultsGameObject3D.data();
			m_PlotCombined.values.count = int(m_ResultsGameObject3D.size());

			static const float* y_data[] = { m_ResultsGameObject3D.data(), m_ResultsGameObject3DAlt.data()};
			static ImU32 colors[2] = { m_ColorGameObject3D, m_ColorGameObject3DAlt };

			m_PlotCombined.values.ys_list = y_data; // use ys_list to draw several lines simultaneously
			m_PlotCombined.values.ys_count = 2;
			m_PlotCombined.values.colors = colors;
			m_PlotCombined.scale.min = 0;
			m_PlotCombined.scale.max = float(m_ResultsGameObject3D[0]);
			m_PlotCombined.tooltip.show = true;

			m_PlotCombined.frame_size = ImVec2(250, 100);

			ImGui::Plot("Plot3", m_PlotCombined);
		}
		ImGui::End();
	}

	void TrashTheCacheGameObjectsComponent::TestGameObject3D()
	{
		static uint32_t selection_start = 0, selection_length = 0;

		if (!m_GameObject3DStarted)
		{
			if (ImGui::Button("Trash the cache with GameObject3D"))
			{
				m_GameObject3DStarted = true;
				ImGui::Text("Wait for it ...");
			}

		}
		else
		{
			using Type = GameObject3D;
			m_Experiment->StartExperiment<Type>(m_NrOfSamples, [&](Type* arr, int i) { arr[i].id += 2; });
			m_ResultsGameObject3D = m_Experiment->GetResults();

			m_PlotGameObject3D.values.xs = m_Experiment->GetXValues().data();
			m_PlotGameObject3D.values.ys = m_ResultsGameObject3D.data();
			m_PlotGameObject3D.values.count = int(m_ResultsGameObject3D.size());
			//m_PlotGameObject3D.values.ys_list = y_data; // use ys_list to draw several lines simultaneously
			//m_PlotGameObject3D.values.ys_count = 1;
			m_PlotGameObject3D.values.color = m_ColorGameObject3D;
			m_PlotGameObject3D.scale.min = 0;
			m_PlotGameObject3D.scale.max = float(m_ResultsGameObject3D[0]);
			m_PlotGameObject3D.tooltip.show = true;

			m_PlotGameObject3D.selection.show = true;
			m_PlotGameObject3D.selection.start = &selection_start;
			m_PlotGameObject3D.selection.length = &selection_length;
			m_PlotGameObject3D.frame_size = ImVec2(250, 100);

			
			for (const auto& value : m_ResultsGameObject3D)
			{
				m_PlotsData.push_back(value);
			}

			m_GameObject3DStarted = false;
		}

		if (!m_Experiment->GetResults().empty())
		{
			ImGui::Plot("plot1", m_PlotGameObject3D);
		}
	}

	void TrashTheCacheGameObjectsComponent::TestGameObject3DAlt()
	{
		static uint32_t selection_start = 0, selection_length = 0;

		
		if (!m_GameObject3DAltStarted)
		{
			if (ImGui::Button("Trash the cache with GameObject3DAlt"))
			{
				m_GameObject3DAltStarted = true;
				ImGui::Text("Wait for it ...");
			}

		}
		else
		{
			using Type = GameObject3DAlt;
			m_Experiment->StartExperiment<Type>(m_NrOfSamples, [&](Type* arr, int i) { arr[i].id += 2; });
			m_ResultsGameObject3DAlt = m_Experiment->GetResults();

			m_PlotGameObject3DAlt.values.xs = m_Experiment->GetXValues().data();
			m_PlotGameObject3DAlt.values.ys = m_ResultsGameObject3DAlt.data();
			m_PlotGameObject3DAlt.values.count = int(m_ResultsGameObject3DAlt.size());
			//m_PlotGameObject3DAlt.values.ys_list = y_data; // use ys_list to draw several lines simultaneously
			//m_PlotGameObject3DAlt.values.ys_count = 1;
			m_PlotGameObject3DAlt.values.color = m_ColorGameObject3DAlt;
			m_PlotGameObject3DAlt.scale.min = 0;
			m_PlotGameObject3DAlt.scale.max = float(m_ResultsGameObject3DAlt[0]);
			m_PlotGameObject3DAlt.tooltip.show = true;

			m_PlotGameObject3DAlt.selection.show = true;
			m_PlotGameObject3DAlt.selection.start = &selection_start;
			m_PlotGameObject3DAlt.selection.length = &selection_length;
			m_PlotGameObject3DAlt.frame_size = ImVec2(250, 100);

			for (const auto& value : m_ResultsGameObject3DAlt)
			{
				m_PlotsData.push_back(value);
			}

			m_GameObject3DAltStarted = false;
		}

		if (!m_Experiment->GetResults().empty())
		{
			ImGui::Plot("plot2", m_PlotGameObject3DAlt);
		}
	}

}

#include "CacheExperimentComponent.h"

namespace dae
{

    CacheExperimentComponent::CacheExperimentComponent(GameObject* pOwner) :
        Component{ pOwner },
        m_Averages{},
        m_Tests{},
        m_XValues{}
    {
    }

    void CacheExperimentComponent::Update()
    {
    }

    void CacheExperimentComponent::PrepareImGuiRender()
    {
    }

    const std::vector<float>& CacheExperimentComponent::GetResults()
    {
        return m_Averages;
    }

    const std::vector<float>& CacheExperimentComponent::GetXValues()
    {
        return m_XValues;
    }
}

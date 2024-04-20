#include "CacheExperimentComponent.h"


CacheExperimentComponent::CacheExperimentComponent(dae::GameObject* pOwner) :
    dae::Component{ pOwner },
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
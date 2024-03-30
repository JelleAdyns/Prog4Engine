#ifndef CACHEEXPERIMENTCOMPONENT_H
#define CACHEEXPERIMENTCOMPONENT_H
#include "Component.h"
#include <vector>
#include <deque>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <chrono>
#include <imgui_plot.h>
namespace dae
{
	class CacheExperimentComponent final : public Component
	{
	public:
		explicit CacheExperimentComponent(GameObject* pOwner);
		virtual ~CacheExperimentComponent() = default;

		CacheExperimentComponent(const CacheExperimentComponent&) = delete;
		CacheExperimentComponent(CacheExperimentComponent&&) noexcept = delete;
		CacheExperimentComponent& operator= (const CacheExperimentComponent&) = delete;
		CacheExperimentComponent& operator= (CacheExperimentComponent&&) noexcept = delete;

		virtual void Update() override;
		virtual void PrepareImGuiRender() override;

		const std::vector<float>& GetResults();
		const std::vector<float>& GetXValues();


		template <typename T>
		void StartExperiment(int nrOfSamples, const std::function<void(T*, int)>& func)
		{
			m_Tests.clear();
			m_XValues.clear();

			for (int testNr = 0; testNr < nrOfSamples; testNr++)
			{
				TestCache<T>(testNr, func);
			}

			//std::cout << "\nAverages\n";

			//calculate averages of every deque
			
			m_Averages.clear();
			for (auto& deq : m_Tests)
			{
				if (nrOfSamples > 2)
				{
					std::sort(deq.begin(), deq.end());

					deq.pop_back();
					deq.pop_front();
				}

				float average = std::accumulate(deq.cbegin(), deq.cend(), 0.f) / deq.size();
				m_Averages.push_back(average);

				//std::cout << average << '\n';
			}

			
			
			

		}

	private:
		template <typename T>
		void TestCache(int testNr, const std::function<void(T*, int)>& func)
		{
			constexpr int stepsizeMultiplier{ 2 };

			//std::cout << "\nTest " << (testNr + 1) << "\n";
			std::deque<int> times{};

			const int nrOfObjects{ int(std::pow(2, 26)) };
			T* arr = new T[nrOfObjects] {};

			for (int stepsize = 1; stepsize <= 1024; stepsize *= stepsizeMultiplier)
			{
				m_XValues.push_back(float(stepsize));

				//make the 'tests' vector the same size as the amount of steps;
				if (testNr == 0) m_Tests.push_back(std::deque<int>{});

				//start measuring
				auto t1 = std::chrono::high_resolution_clock::now();

				for (int i = 0; i < nrOfObjects; i += stepsize) func(arr, i);

				auto t2 = std::chrono::high_resolution_clock::now();
				//end measuring

				const auto total = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

				//std::cout << stepsize << ": " << total << '\n';
				times.push_back(int(total));

			}

			delete[] arr;

			// Push all the values of the same step together into their corresponding deque
			// For example: all the samples of the first step, go into the first deque. All the samples of the second step, go into the second deque. ... 
			for (size_t i = 0; i < times.size(); ++i)
			{
				m_Tests[i].push_back(times[i]);
			}
		}

		std::vector<float> m_Averages;
		std::vector<std::deque<int>> m_Tests;
		std::vector<float> m_XValues;

		//static float* m_XValues;
	};
}
#endif // !CACHEEXPERIMENTCOMPONENT_H
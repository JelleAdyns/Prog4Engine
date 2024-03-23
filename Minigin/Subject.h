#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include "Observer.h"
#include <exception>
#include <iostream>

namespace dae
{
	template <typename T>
	class Subject
	{
	public:
		Subject() = default;
		virtual ~Subject() = default;

		Subject(const Subject&) = delete;
		Subject(Subject&&) noexcept = delete;
		Subject& operator= (const Subject&) = delete;
		Subject& operator= (Subject&&) noexcept = delete;

		void AddObserver(Observer<T>* pObserver)
		{
			auto pos = std::find(m_pVecObservers.cbegin(), m_pVecObservers.cend(), pObserver);
			if (pos == m_pVecObservers.cend()) m_pVecObservers.push_back(pObserver);
			else throw std::runtime_error("Observer already subscribed to Subject");
		}
		void RemoveObserver(Observer<T>* pObserver)
		{
			auto pos = std::find(m_pVecObservers.cbegin(), m_pVecObservers.cend(), pObserver);
			if (pos != m_pVecObservers.cend()) m_pVecObservers.erase(pos);
			else std::cout << "Couldn't find Observer to remove in the vector. Continuing.\n";
		}

		void NotifyObservers()
		{

			for (auto& pObserver : m_pVecObservers)
			{
				pObserver->Notify(static_cast<T*>(this));
			}

		}
	private:
		std::vector<Observer<T>*> m_pVecObservers;

	};

}
#endif // !SUBJECT_H
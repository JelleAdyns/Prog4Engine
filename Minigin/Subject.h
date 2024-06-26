#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include "Observer.h"
#include <exception>
#include <iostream>

namespace dae
{
	template <typename T>
	class Subject final
	{
	public:
		Subject() = default;
		~Subject()
		{
			for (Observer<T>* pObserver : m_pVecObservers)
			{
				pObserver->SetSubjectPointersInvalid(this);
			}
		}

		Subject(const Subject&) = delete;
		Subject(Subject&&) noexcept = delete;
		Subject& operator= (const Subject&) = delete;
		Subject& operator= (Subject&&) noexcept = delete;

		void AddObserver(Observer<T>* pObserver)
		{
			if(pObserver)
			{
				auto pos = std::find(m_pVecObservers.cbegin(), m_pVecObservers.cend(), pObserver);
				if (pos == m_pVecObservers.cend())
				{
					m_pVecObservers.push_back(pObserver);
					if (pObserver) pObserver->AddSubjectPointer(this);
				}
				else throw std::runtime_error("Observer already subscribed to Subject");
			}
		}
		void RemoveObserver(Observer<T>* pObserver)
		{
			if(m_pVecObservers.size() > 0)
			{
				auto pos = std::find(m_pVecObservers.cbegin(), m_pVecObservers.cend(), pObserver);
				if (pos != m_pVecObservers.cend()) m_pVecObservers.erase(pos);
#ifndef NDEBUG
				else std::cout << "Couldn't find Observer to remove in the vector. Continuing.\n";
#endif // !NDEBUG
			}
		}

		void NotifyObservers(T* pSubjectOwner)
		{
			for (auto& pObserver : m_pVecObservers)
			{
				pObserver->Notify(pSubjectOwner);
			}
		}
	private:
		std::vector<Observer<T>*> m_pVecObservers;

	};

}
#endif // !SUBJECT_H
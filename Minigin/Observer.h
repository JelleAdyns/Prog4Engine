#ifndef OBSERVER_H
#define OBSERVER_H

#include <memory>

namespace dae
{
	template <typename T>
	class Subject;
	template <typename T>
	class Observer
	{
	public:

		virtual ~Observer() { m_pSubject->RemoveObserver(this); };

		Observer(const Observer&) = delete;
		Observer(Observer&&) noexcept = delete;
		Observer& operator= (const Observer&) = delete;
		Observer& operator= (Observer&&) noexcept = delete;

		virtual void Notify(T* pSubjectOwner) = 0;
		void SetSubject(Subject<T>* pSubject) { m_pSubject = pSubject; }
		Subject<T>* GetSubject() { return m_pSubject; }

	protected:
		Observer() = default;
	private:
		Subject<T>* m_pSubject;
	};

}
#endif // !OBSERVER_H
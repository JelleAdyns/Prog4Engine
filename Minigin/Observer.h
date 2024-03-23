#ifndef OBSERVER_H
#define OBSERVER_H

#include <memory>

namespace dae
{
	template <typename T>
	class Observer
	{
	public:

		virtual ~Observer() = default;

		Observer(const Observer&) = delete;
		Observer(Observer&&) noexcept = delete;
		Observer& operator= (const Observer&) = delete;
		Observer& operator= (Observer&&) noexcept = delete;

		virtual void Notify(T* pSubject) = 0;

	protected:
		Observer() = default;
	};

}
#endif // !OBSERVER_H
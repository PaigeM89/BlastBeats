#pragma once
#include <vector>
#include <memory>

namespace Observers {

	template<typename T>
	class Observer
	{
	public:
		virtual void Update(std::shared_ptr<T> t) = 0;
	};

	template<typename T>
	class Subject
	{
	private:
		std::vector<std::shared_ptr<Observer<T>>> m_Observers;
	public:
		void Attach(std::shared_ptr<Observer<T>> observer)
		{
			m_Observers.push_back(observer);
		}
		void Detach(std::shared_ptr<Observer<T>> observer)
		{
			m_Observers.erase(observer);
		}
		void Notify(std::shared_ptr<T> t)
		{
			for (auto& o : m_Observers)
				o->Update(t);
		}
	};

}
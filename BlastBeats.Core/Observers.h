#pragma once
#include <vector>
#include <memory>
#include "Messages.h"

namespace Observers {

	// do i need this file any more?

	/// <summary>
	/// Observes a music directory for any changes.
	/// </summary>
	class MusicDirectoryObserver
	{
	public:
		virtual void Update(std::shared_ptr<Messages::MusicDirectoryChanged> msg) = 0;
	};

	/// <summary>
	/// Updates observers when the music directory changes.
	/// </summary>
	class MusicDirectorySubject
	{
	private:
		std::vector<std::shared_ptr<MusicDirectoryObserver>> m_Observers;
	public:
		void Attach(std::shared_ptr<MusicDirectoryObserver> observer)
		{
			m_Observers.push_back(observer);
		}
		void Detach(std::shared_ptr<MusicDirectoryObserver> observer)
		{
			auto it = std::find_if(m_Observers.begin(), m_Observers.end(),
				[&observer](const std::shared_ptr<MusicDirectoryObserver>& p) {
					return p.get() == observer.get();
				});
			if (it != m_Observers.end())
			{
				m_Observers.erase(it);
			}
		}
		void Notify(std::shared_ptr<Messages::MusicDirectoryChanged> msg)
		{
			for (auto& o : m_Observers)
			{
				printf("Updating observer with message\n");
				o->Update(msg);
			}
		}
	};

}
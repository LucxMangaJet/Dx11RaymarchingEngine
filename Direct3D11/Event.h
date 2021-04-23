#pragma once

#include <functional>               // std::function
#include <unordered_map>            // std::unordered_map

typedef std::function<void()> Action;

enum ObserverID : int {};

class ActionEvent
{
private:
	std::unordered_map<int, Action> _callbacks;

	static int& id_value()
	{
		static int the_id;
		return the_id;
	}

public:
	ObserverID AddListener(Action cb)
	{
		const ObserverID id = ObserverID(++id_value());
		_callbacks.emplace(id, move(cb));
		return id;
	}

	bool RemoveListener(const ObserverID id)
	{
		const auto it = _callbacks.find(id);
		if (it == _callbacks.end())
		{
			return false;
		}
		_callbacks.erase(it);
		return true;
	}

	void Invoke() const
	{
		for (const auto& pair : _callbacks)
		{
			pair.second();
		}
	}
};

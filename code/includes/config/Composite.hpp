#pragma once

#include <iostream>
#include <vector>

template <class T> class Composite
{
	public:
		void addChild(const T &newChild)
		{
			_children.push_back(newChild);
		}

		const std::vector<T> &getChildren() const
		{
			return _children;
		}

	protected:
		virtual ~Composite()
		{
			delete _children;
		}

	private:
		std::vector<T> _children;
};

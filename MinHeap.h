#pragma once

// Carey build this for A* but it was not necessary...

#include <vector>
#include <algorithm>

template <class foo>
class MinHeap
{
public:
	MinHeap()
	{
	}

	~MinHeap()
	{
	}

	void push(const foo &f)
	{
		// now sift up
		size_t curIndex = addItem(f);
		while (curIndex > 0)
		{
			size_t parentIndex = getParentIndex(curIndex);
			if (m_items[curIndex] < m_items[parentIndex])
				std::swap(m_items[curIndex], m_items[parentIndex]);
			else
				break;

			curIndex = parentIndex;
		}
	}

	bool pop(foo &f)
	{
		if (m_items.empty())
			return false;

		f = m_items[getIndexOfRoot()];

		if (m_items.size() == 1)
		{
			m_items.clear();
			return true;
		}

		size_t curIndex = getIndexOfRoot();
		m_items[curIndex] = getAndRemoveLowestRight();	// move at top

		for (;;)
		{
			size_t lci = getLeftChildIndex(curIndex);
			size_t rci = getRightChildIndex(curIndex);
			bool leftExists = doesItemExist(lci);
			bool rightExists = doesItemExist(rci);
			if (!leftExists && !rightExists)
				return true;		// done sifting down

			bool possibleLeftSwap = false, possibleRightSwap = false;

			if (leftExists)
			{
				if (m_items[lci] < m_items[curIndex])
					possibleLeftSwap = true;
			}
			if (rightExists)
			{
				if (m_items[rci] < m_items[curIndex])
					possibleRightSwap = true;
			}

			if (!possibleLeftSwap && !possibleRightSwap)
				return true;

			if (possibleLeftSwap && possibleRightSwap)
			{
				if (m_items[lci] < m_items[rci])
					possibleRightSwap = false;
				else
					possibleLeftSwap = false;	// left and right children could be equal... who cares which we swap with in this case
			}

			if (possibleLeftSwap)
			{
				std::swap(m_items[curIndex], m_items[lci]);
				curIndex = lci;
			}
			else 
			{
				std::swap(m_items[curIndex], m_items[rci]);
				curIndex = rci;
			}
		}

		return true;
	}

	bool empty() const
	{
		return m_items.empty();
	}

	size_t size() const
	{
		return m_items.size();
	}

private:

	size_t getIndexOfRoot() const
	{
		return 0; 
	}

	size_t getParentIndex(size_t curIndex) const
	{
		return (curIndex - 1) / 2;
	}

	size_t getLeftChildIndex(size_t curIndex) const
	{
		return curIndex * 2 + 1;
	}

	size_t getRightChildIndex(size_t curIndex) const
	{
		return curIndex * 2 + 2;
	}

	foo getAndRemoveLowestRight() 
	{
		foo item = m_items[m_items.size() - 1];
		m_items.pop_back();
		return item;
	}

	size_t addItem(const foo &item)
	{
		m_items.push_back(item);		// add to lower right corner
		return m_items.size() - 1;
	}

	bool doesItemExist(size_t index) const
	{
		return index < m_items.size();
	}

private:
	std::vector<foo>	m_items;
};
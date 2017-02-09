#ifndef _MYMAP_H_
#define _MYMAP_H_

// BST holding map<key, value>

#include <queue>

template<class KeyType, class ValueType>
class MyMap
{
public:
	// constuctor
	MyMap()
		:m_nodeCount(0), m_root(nullptr)
	{
	}

	// destructor
	~MyMap()
	{
		this->clear();
	}

	// deletes all the trees nodes -> empty tree
	void clear()
	{
		if (nullptr == m_root)
			return;

		std::queue<Node*> treeNodes;
		Node* curNode;

		treeNodes.push(m_root);
		while (0 < treeNodes.size())
		{
			curNode = treeNodes.front();

			treeNodes.pop();
			if (nullptr != curNode->left)
				treeNodes.push(curNode->left);
			if (nullptr != curNode->right)
				treeNodes.push(curNode->right);

			delete curNode;
		}

		m_root = nullptr;
		m_nodeCount = 0;
	}

	// return the number of associations in the map
	int size() const
	{
		return m_nodeCount;
	}

	// The associate method associates one item (key) with another (value).
	// If no association currently exists with that key, this method inserts
	// a new association into the tree with that key/value pair.  If there is
	// already an association with that key in the tree, then the item
	// associated with that key is replaced by the second parameter (value).
	// Thus, the tree contains no duplicate keys.
	void associate(const KeyType& key, const ValueType& value)
	{
		//if (0 == m_nodeCount) // empty tree
		if (nullptr == m_root)
		{
			m_root = new Node(key, value);
			m_nodeCount++;
			return;
		}

		ValueType* valPtr = this->find(key); // find pre-existing node with key
		if (nullptr != valPtr)
		{
			// valPtr points to ValueType of associated KeyType in Node
			*valPtr = value;
			return;
		}

		// else nullptr == valPtr: associate new key
		Node* newAssociation = new Node(key, value);
		Node* cur = m_root;
		while (nullptr != cur)
		{
			Node* leftNode = cur->left;
			Node* rightNode = cur->right;

			// if key is smaller than current key, move left
			if (cur->key > key)
			{
				if (nullptr == leftNode || leftNode->key < key)
				{
					// Appropriate position for the new association is found
					insertNode(newAssociation, cur, LEFT);
					m_nodeCount++;
					return;
				}
				else
				{
					cur = leftNode;
					continue; // continue loop
				}
			}
			else // key is larger than current key, move right
			{
				if (nullptr == rightNode || rightNode->key > key)
				{
					// Appropriate position for the new association is found
					insertNode(newAssociation, cur, RIGHT);
					m_nodeCount++;
					return;
				}
				else
				{
					cur = rightNode;
					continue; // continue loop
				}
			}
		}
	}

	// If no association exists with the given key, return NULL; otherwise,
	// return a pointer to value associated with that key.  This pointer can be
	// used to examine that value, and if the map is allowed to be modified, to
	// modify that value directly within the map.  (The second overload enables
	// this.  Using a little C++ magic, we have implemented it in terms of the
	// first overload, which you must implement.
	const ValueType* find(const KeyType& key) const;

	ValueType* find(const KeyType& key)
	{
		Node* cur = m_root;
		while (nullptr != cur)
		{
			if (cur->key == key)
			{
				//std::cerr << cur->key << endl;
				return &(cur->value);
			}

			if (cur->key > key)
			{
				//std::cerr << cur->key << endl;
				// move left
				cur = cur->left;
			}
			else
			{
				//std::cerr << cur->key << endl;
				// move right
				cur = cur->right;
			}
		}

		return nullptr;
	}

	// Testing purposes only
	void printBST_BreadthFirst()
	{
		if (nullptr == m_root)
		{
			cout << "Empty tree" << endl;
			return;
		}

		cout << "Tree size: " << this->size() << endl;

		queue<Node*> traversal;
		queue<Node*> printThis;

		Node* cur = nullptr;
		printThis.push(m_root);
		while (0 < printThis.size())
		{
			while (0 < printThis.size())
			{
				cur = printThis.front();
				printThis.pop();

				// Used to test MyMap
				// Key = int, ValueType = int
				//cout << cur->key;
				// end use-case

				// Used to test AttractionMapperImpl
				// Key = string, ValueType = GeoCoord
				//cout << cur->key;

				//const GeoCoord curGCValue = cur->value;
				//cout << " -> " << curGCValue.sLatitude
				//	<< ", " << curGCValue.sLongitude;
				// end use-case

				// Used to test SegmentMapperImpl
				// Key = GeoCoord, ValueType = vector<StreetSegment>
				//const GeoCoord gcKey = cur->key;
				//vector<StreetSegment>& vecValue = cur->value;
				//if (2 < vecValue.size()) // Are certain GeoCoords associated with more than 2 StreetSegments?
				//{
				//	cout << gcKey.sLatitude << ", " << gcKey.sLongitude;
				//	cout << " -> ";
				//	cout << vecValue.size() << " StreetSegments stored";
				//}
				// end use-case

				if (nullptr != cur->left)
					traversal.push(cur->left);
				if (nullptr != cur->right)
					traversal.push(cur->right);

				if (0 < printThis.size())
					cout << " || ";
			}
			cout << endl;

			while (0 < traversal.size())
			{
				printThis.push(traversal.front());
				traversal.pop();
			}
		}
	}

// Node declaration
private:
	struct Node
	{
		Node(const KeyType k, ValueType v) // Q: why can't I declare arguments k and v as references?
			:key(k), value(v), left(nullptr), right(nullptr)
		{
		}

		Node* left;
		Node* right;
		
		const KeyType key;
		ValueType value;
	};

	Node* getRoot()
	{
		return m_root;
	}

// Member variables
private:
	Node* m_root;
	int m_nodeCount;

	enum m_child {
		LEFT,
		RIGHT
	};

// Functions
private:
	void insertNode(Node* newAssociation, Node* parent, int side)
	{
		Node* originalChild = nullptr;
		switch (side)
		{
		case LEFT:
			if (nullptr != parent->left)
				originalChild = parent->left;
			newAssociation->left = originalChild;
			parent->left = newAssociation;
			return;
		case RIGHT:
			if (nullptr != parent->right)
				originalChild = parent->right;
			newAssociation->right = originalChild;
			parent->right = newAssociation;
			return;
		}
	}

	MyMap(const MyMap &other);
	MyMap &operator=(const MyMap &other);
};

#endif
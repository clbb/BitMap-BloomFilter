#include<vector>
#include<iostream>
using namespace std;

template <class K,class V>
struct HashNode
{
    pair<K,V> _kv;
    HashNode<K,V>* _next;

	HashNode(pair<K,V> kv)
		:_kv(kv)
		,_next(NULL)
	{}
};

template<class K,class V>
class HashTable
{
    typedef HashNode<K,V> Node;
public:
	HashTable()
		:_size(0)
	{
		_table.resize(_GetPrimeSize());
	}
	
	~HashTable()
	{}

	pair<Node*,bool> Insert(const pair<K,V>& kv)
	{
		//满
		_CheckCapacity();
		//直接插入
		int index = _HashFunc(kv.first);
		Node* cur = _table[index];
		while(cur)
		{
			if(cur->_kv.first == kv.first)//防止重复插入
				return make_pair(cur,false);
			cur = cur->_next;
		}
		//头插
		Node* tmp = new Node(kv);
		tmp->_next = _table[index];//注意不要操控临时变量
		_table[index] = tmp;
		_size++;
		return make_pair(tmp,true);
	}

	void Swap(HashTable<K,V> ht)
	{
		_table.swap(ht._table);
		swap(ht._size,_size);	
	}
	
	Node* Find(const K& key)
	{
		size_t index = _HashFunc(key);
		Node* cur = _table[index];
		while(cur)
		{
			if(cur->_kv.first == key)
				return cur;
			cur = cur->_next;
		}
		cout<<"没有这个值"<<endl;
		return NULL;
	}

	bool Erase(const K& key)
	{
		size_t index = _HashFunc(key);
		Node* prev = NULL;	
		Node* cur = _table[index];
		while(cur)
		{
			//头删
			if(key == cur->_kv.first)
			{
				//只有一个节点
				if(prev == NULL)
				{
					_table[index] = cur->_next;
				}
				//有n个节点
				else
				{
					prev->_next = cur->_next;
				}	
				delete cur;
				cur = NULL;
				_size--;
				return true;
			}
			prev = cur;
			cur = cur->_next;
		}
		return false;
	}
protected:
	size_t _GetPrimeSize()
	{
		const int _PrimeSize= 28;
		static const unsigned long _PrimeList[_PrimeSize] =
		{
			53ul,         97ul,         193ul,		 389ul,		  769ul,
			1543ul,       3079ul,       6151ul,		 12289ul,	  24593ul,
			49157ul,      98317ul,      196613ul,	 393241ul,	  786433ul,
			1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
			50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
			1610612741ul, 3221225473ul, 4294967291ul
		};
		// 10 < 53 return 53  
		//  54 < 53 ++ 54 < 97 return 97
		for(size_t i = 0; i<_PrimeSize; ++i)
		{
			if(_table.size() <  _PrimeList[i])
			{
				return _PrimeList[i];
			}
		}
		return 0;
	}
protected:
	void _CheckCapacity()
	{
		//负载因子＝1
		if(_size == _table.size())
		{
			size_t newcapacity = _GetPrimeSize();
			HashTable<K,V> ht;
			for(int i = 0; i<_table.size(); i++)
			{
				Node* cur = _table[i];
				while(cur)
				{
					//取节点
					Node* tmp = cur;
					cur = cur->_next;
					//头插
					size_t newIndex = _HashFunc(cur->_kv.first);
					tmp->_next = ht._table[newIndex];
					ht._table[newIndex] = tmp;
				}
				_table[i] = NULL;//取走了
			}
			this->Swap(ht);
		}
	}
	size_t _HashFunc(const K& key)
	{
		return key % _table.size();
	}	

 protected:
    vector<Node*> _table;
    size_t _size;
};

void Test()
{
	HashTable<int,int> ht;
	ht.Insert(make_pair(0,2));
	ht.Insert(make_pair(53,2));
	ht.Insert(make_pair(106,2));
	ht.Insert(make_pair(2,2));
	ht.Insert(make_pair(55,2));
	ht.Insert(make_pair(3,2));
	ht.Insert(make_pair(52,2));
	ht.Insert(make_pair(105,2));
	ht.Insert(make_pair(51,2));
	
	
	cout<<ht.Find(3)->_kv.first<<endl;
	ht.Find(22);

	cout<<ht.Erase(51)<<endl;
	cout<<ht.Erase(1)<<endl;
	cout<<ht.Find(52)->_kv.first<<endl;

}

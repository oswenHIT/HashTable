#ifndef HASHTABLE2ND_H
#define HASHTABLE2ND_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename HashObj>
class HashTable2nd{
public:
	explicit HashTable2nd(int size = 101) : ar(nextPrime(size)), curSize(0) { makeEmpty(); }

	bool contains(const HashObj& x) const;
	
	void makeEmpty()
	{
		curPos = 0;
		for (int i = 0; i < ar.size(); i++)
		{
			ar[i].info = EMPTY;
		}
	}
	bool insert(const HashObj& x);
	bool remove(const HashObj& x);

	enum EntryType { ACTIVE, EMPTY, DELETED };

private:
	struct HashEntry
	{
		HashObj ele;
		EntryType info;
		HashEntry(const HashObj & x, EntryType i = EMPTY)
			: ele(x), info(i) {}
	};

	vector<HashEntry> ar;
	int curSize;

	bool isActive(int pos) const;
	int findPos(const HashObj& x) const;
	void rehash();
	int myHash(const HashObj & x) const;
};

template <typename HashObj>
int HashTable2nd<HashObj>::findPos(const HashObj & x) const
{
	int offset = 1;
	int pos = myHash(x);

	while (ar[pos].info != EMPTY && ar[pos].ele != x)
	{
		pos += offset;
		offset += 2;
		if (pos >= ar.size())
			pos -= ar.size();
	}

	return pos;
}

template <typename HashObj>
bool HashTable2nd<HashObj>::isActive(int pos) const
{
	return ar[pos].info == ACTIVE;
}

template <typename HashObj>
bool HashTable2nd<HashObj>::contains(const HashObj & x) const
{
	return isActive(findPos(x));
}

template <typename HashObj>
bool HashTable2nd<HashObj>::insert(const HashObj & x)
{
	int pos = findPos(x);
	if (isActive(pos))
		return false;

	ar[pos] = HashEntry(x, ACTIVE);

	if (++curSize > ar.size() / 2)
		rehash();

	return true;
}

template <typename HashObj>
bool HashTable2nd<HashObj>::remove(const HashObj & x)
{
	int pos = find(x);
	if (!isActive(pos))
		return false;

	ar[pos].info = DELETED;

	return true;
}

int hash(const string & key)
{
	int hashVal = 0;

	for (unsigned int i = 0; i < key.length(); i++)
		hashVal = 37 * hashVal + key[i];

	return hashVal;
}

template <typename HashObj>
int HashTable2nd<HashObj>::myHash(const HashObj & x) const
{
	string key;
	int hashVal = hash(x >> key);

	hashVal %= ar.size();
	if (hashVal < 0)
		hashVal += ar.size();

	return hashVal;
}

template <typename HashObj>
void HashTable2nd<HashObj>::rehash()
{
	vector<HashObj> oldTable;
	memcpy(&oldTable[0], &ar[0], ar.size()*sizeof(HashObj));

	ar.resize(nextPrime(ar.size()) * 2);
	for (int i = 0; i < ar.size(); i++)
	{
		ar[i].info = EMPTY;
	}

	curSize = 0;
	for (int i = 0; i < oldTable.size(); i++)
	{
		if (oldTable[i].info == ACTIVE)
			insert(oldTable[i].ele);
	}
}

bool isPrime(const int & x)
{
	if (x < 2)
		return false;
	for (int i = 2; i < x / 2; i++)
	{
		if (x%i == 0)
		{
			return 0;
		}
	}

	return true;
}

int nextPrime(int a)
{
	while (1)
	{
		if (isPrime(a))
			break;

		++a;
	}

	return a;
}

#endif
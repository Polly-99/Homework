#pragma once
#include <iostream>
#include <fstream>

struct TNode {
	int Data;
	int Key;
	int Height;
	TNode * Left;
	TNode * Right;
	TNode * Parent;

	TNode()
		:Key(0)
		, Data(0)
		, Height(0)
		, Left(nullptr)
		, Right(nullptr)
		, Parent(nullptr)
	{}

	TNode(const int & data, const int & key)
		:Key(key)
		, Data(data)
		, Height(1)
		, Left(nullptr)
		, Right(nullptr)
		, Parent(nullptr)
	{}

	~TNode()
	{
		if (Left != nullptr)
			delete Left;
		if (Right != nullptr)
			delete Right;
	}

	int is_balanced() {
		if (Left) {
			if (Right)
				return (Left->Height - Right->Height);
			else
				return Left->Height;
		}
		else {
			if (Right)
				return -(Right->Height);
			else
				return 0;
		}
	}

	void fixHeight()
	{
		if (is_balanced() > 0)
			Height = Left->Height + 1;
		if (is_balanced() < 0)
			Height = Right->Height + 1;
		if (Left == nullptr && Right == nullptr)
			Height = 1;
	}

	void print(std::ofstream & file_out)
	{
		std::cout << "(" Key << ", " << Data << ") ";
		if (Left != nullptr)
			Left->print();
		if (Right != nullptr)
			Right->print();
	}
};

class TTree
{
	TNode * Root;

	TNode* rotateRight(TNode * cur) 
	{
		TNode* node = cur->Left;
		cur->Left = node->Right;
		if (node->Right)
			node->Right->Parent = cur;
		node->Parent = cur->Parent;
		cur->Parent = node;
		node->Right = cur;
		cur->fixHeight();
		node->fixHeight();
		if (node->Parent->Key > node->Key)
			node->Parent->Left = node;
		else
			node->Parent->Right = node;
		return node;
	}

	TNode* rotateLeft(TNode * cur) 
	{
		TNode* node = cur->Right;
		cur->Right = node->Left;
		if (node->Left)
			node->Left->Parent = cur;
		node->Parent = cur->Parent;
		cur->Parent = node;
		node->Left = cur;
		cur->fixHeight();
		node->fixHeight();
		if (node->Parent->Key > node->Key)
			node->Parent->Left = node;
		else
			node->Parent->Right = node;
		return node;
	}

	TNode* rotateBigLeft(TNode * cur) 
	{
		cur->Right = rotateRight(cur->Right);
		TNode * node = rotateLeft(cur);
		cur->fixHeight();
		node->fixHeight();
		return node;
	}

	TNode* rotateBigRight(TNode * cur) 
	{
		cur->Left = rotateLeft(cur->Left);
		TNode * node = rotateRight(cur);
		cur->fixHeight();
		node->fixHeight();
		return node;
	}

	TNode * balance(TNode * cur) 
	{
		TNode * node = cur;
		if (cur->is_balanced() == 2)
		{
			if (cur->Left->is_balanced() < 0)
				node = rotateBigRight(cur);
			else
				node = rotateRight(cur);
		}
		if (cur->is_balanced() == -2)
		{
			if (cur->Right->is_balanced() > 0)
				node = rotateBigLeft(cur);
			else
				node = rotateLeft(cur);
		}
		return node;
	}

	TNode * insert(const int & key, const int & data) 
	{
		if (Root == nullptr) {
			Root = new TNode(data, key);
			return Root;
		}
		TNode * cur = Root;
		TNode * node = new TNode(data, key);
		while (cur != nullptr)
		{
			if (cur->Key > key)
			{
				if (cur->Left == nullptr) {
					cur->Left = node;
					node->Parent = cur;
					return node;
				}
				cur = cur->Left;
			}
			else if (cur->Key < key) {
				if (cur->Right == nullptr) {
					cur->Right = node;
					node->Parent = cur;
					return node;
				}
				cur = cur->Right;
			}
			else
				return nullptr;
		}
	}

public:

	TTree()
		:Root(nullptr)
	{}

	~TTree()
	{
		delete Root;
	}

	TNode *  add(const int & key, const int & data) 
	{
		TNode * cur = insert(key, data);
		while (cur && cur != Root)
		{
			cur = cur->Parent;
			cur->fixHeight();
			if (abs(cur->is_balanced()) > 1) {
				cur = balance(cur);
			}
		}
		return cur;
	}

	TNode * search(const int & key)
	{
		TNode * cur = Root;
		while (cur != nullptr && cur->Key != key) {
			if (key < cur->Key)
				cur = cur->Left;
			else 
				cur = cur->Right;
		}
		return cur;
	}

	int min() {
		TNode * cur = Root;
		while (cur->Left != nullptr) {
			cur = cur->Left;
		}
		return cur->Key;
	}

	int max() {
		TNode * cur = Root;
		while (cur->Right != nullptr) {
			cur = cur->Right;
		}
		return cur->Key;
	}

	TNode * remove(const int & key) {
		TNode * del = search(key);
		if (del == nullptr)
			return nullptr;
		TNode * cur = nullptr;
		if (del->Left || del->Right) { //если есть потомки
			if (del->Left) { //если есть левый потомок
				cur = del->Left;
				while (cur->Right) {
					cur = cur->Right;
				}
				if (del->Right)
					del->Right->Parent = cur;
			}
			else { // если нет левого, но есть правый потомок
				cur = del->Right;
				while (cur->Left) {
					cur = cur->Left;
				}
			}
			if (cur->Parent->Key > cur->Key)
				cur->Parent->Left = nullptr;
			else
				cur->Parent->Right = nullptr;
			cur->Parent = del->Parent;
			cur->Right = del->Right;
			cur->Left = del->Left;
		}
		if (del == Root)
			Root = cur;
		else {
			if (del->Parent->Key > del->Key)
				del->Parent->Left = cur;
			else
				del->Parent->Right = cur;
		}
		if (!cur)
			cur = del->Parent;
		del->Parent = nullptr;
		del->Left = nullptr;
		del->Right = nullptr;
		delete del;
		while (cur) // балансировка
		{
			cur->fixHeight();
			if (abs(cur->is_balanced()) > 1) {
				cur = balance(cur);
			}
			cur = cur->Parent;
		}
		return cur;
	}

	void print(std::ofstream & file_out) {
		TNode * cur = Root;
		cur->print(file_out);
	}
};
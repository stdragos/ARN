#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

template<class _keyType,
	class _valueType,
	class _compare = std::less<_keyType>,
	class _equal = std::equal_to<_keyType>>
	class ARN
{
private:
	class _node
	{
	public:
		_keyType key;
		_valueType value;
		_node* left, * right, * parent;
		char color;
		char originalColor;
		_node(_keyType key = 0, _valueType value = 0);
	};
	_node* root;
	_node* sentinel;
	void recolorInsert(_node*);
	void rotate_left(_node*);
	void rotate_right(_node*);
	void transplant(_node*, _node*);
	bool _nodeDelete(_node*);
	void recolorDelete(_node*);
	void printLayers();
	void printPreorder(_node*);
	void printInorder(_node*);
	void printPostorder(_node*);
	void print_node(_node*);
	void delete_node(_node*);
public:
	ARN(std::vector<std::pair<_keyType, _valueType>> arr = std::vector<std::pair<_keyType, _valueType>>());
	~ARN();

	_node* find(_keyType);
	void insert(_keyType, _valueType);
	void printTree(const short&);
	void construct(const std::vector<std::pair<_keyType, _valueType>>&);
	_node* min(_node*);
	_node* max(_node*);
	void erase(const _keyType&);
	void clear();
	_node* successor(_node*);
	_node* predecessor(_node*);
	_node* getRoot();

	_valueType& operator[](const _keyType);
};

template<class _keyType, class _valueType, class _compare, class _equal>
inline ARN<_keyType, _valueType, _compare, _equal>::_node::_node(_keyType key, _valueType value)
{
	this->key = key;
	this->value = value;

	this->left = this->right = this->parent = nullptr;
	this->color = 'r';
	this->originalColor = 'b';
}

template<class _keyType, class _valueType, class _compare, class _equal>
void ARN<_keyType, _valueType, _compare, _equal>::recolorInsert(_node* z)
{
	while (z->parent->color == 'r')
	{
		if (z->parent == z->parent->parent->left)
		{
			_node* aux = z->parent->parent->right;

			//case 1
			if (aux->color == 'r')
			{
				z->parent->color = 'b';
				aux->color = 'b';

				z->parent->parent->color = 'r';
				z = z->parent->parent;
			}
			else
			{
				//case 2
				if (z == z->parent->right)
				{
					z = z->parent;
					rotate_left(z);
				}
				//case3
				z->parent->color = 'b';
				z->parent->parent->color = 'r';
				rotate_right(z->parent->parent);
			}
		}
		else
		{
			_node* aux = z->parent->parent->left;
			if (aux->color == 'r')
			{
				z->parent->color = 'b';
				aux->color = 'b';
				z->parent->parent->color = 'r';
				z = z->parent->parent;
			}
			else
			{
				if (z == z->parent->left)
				{
					z = z->parent;
					rotate_right(z);
				}
				z->parent->color = 'b';
				z->parent->parent->color = 'r';
				rotate_left(z->parent->parent);
			}
		}
	}
	this->root->color = 'b';
	//this->sentinel->color = 'b'
}

template<class _keyType, class _valueType, class _compare, class _equal>
void ARN<_keyType, _valueType, _compare, _equal>::rotate_left(_node* point)
{
	_node* right = point->right;
	point->right = right->left;
	if (right->left != sentinel)
		right->left->parent = point;
	right->parent = point->parent;
	if (point->parent == sentinel)
		this->root = right;
	else
	{
		if (point == point->parent->left)
			point->parent->left = right;
		else
			point->parent->right = right;
	}
	right->left = point;
	point->parent = right;
}

template<class _keyType, class _valueType, class _compare, class _equal>
void ARN<_keyType, _valueType, _compare, _equal>::rotate_right(_node* point)
{
	_node* left = point->left;
	point->left = left->right;
	if (left->right != sentinel)
		left->right->parent = point;
	left->parent = point->parent;
	if (point->parent == sentinel)
		this->root = left;
	else
	{
		if (point == point->parent->left)
			point->parent->left = left;
		else
			point->parent->right = left;
	}
	left->right = point;
	point->parent = left;
}

template<class _keyType, class _valueType, class _compare, class _equal>
void ARN<_keyType, _valueType, _compare, _equal>::transplant(_node* u, _node* v)
{
	if (u->parent == sentinel)
		this->root = v;
	else
	{
		if (u == u->parent->left)
			u->parent->left = v;
		else
			u->parent->right = v;
	}
	v->parent = u->parent;
}

template<class _keyType, class _valueType, class _compare, class _equal>
typename ARN<_keyType, _valueType, _compare, _equal>::_node* ARN<_keyType, _valueType, _compare, _equal>::successor(_node* p)
{
	_node* y;
	if (p->right != sentinel)
		y = min(p->right);
	else
	{
		y = p->parent;
		while (y != sentinel && p == y->right)
		{
			p = y;
			y = y->parent;
		}
	}
	return y;

}

template<class _keyType, class _valueType, class _compare, class _equal>
bool ARN<_keyType, _valueType, _compare, _equal>::_nodeDelete(_node* point)
{
	if (point != this->sentinel)
	{
		_node* y = point;
		_node* x;
		y->originalColor = y->color;
		if (point->left == this->sentinel)
		{
			x = point->right;
			transplant(point, point->right);
		}
		else
		{
			y = min(point->right);
			y->originalColor = y->color;
			x = y->right;
			if (y->parent == point)
				x->parent = y;
			else
			{
				transplant(y, y->right);
				y->right = point->right;
				y->right->parent = y;
			}
			transplant(point, y);
			y->left = point->left;
			y->left->parent = y;
			y->color = point->color;
		}
		if (y->originalColor == 'b')
			recolorDelete(x);

		point->parent = point->left = point->right = nullptr;
		delete point;

		return true;
	}
	return false;
}

template<class _keyType, class _valueType, class _compare, class _equal>
void ARN<_keyType, _valueType, _compare, _equal>::recolorDelete(_node* x)
{
	while (x != this->root && x->color == 'b')
	{
		if (x == x->parent->left)
		{
			_node* w = x->parent->right;
			if (w->color = 'r')
			{
				w->color = 'b';
				x->parent->color = 'r';
				rotate_left(x->parent);
				w = x->parent->right;
			}
			if (w->left->color == 'b' && w->right->color == 'b')
			{
				w->color = 'r';
				x = x->parent;
			}
			else if (w->right->color == 'b')
			{
				w->left->color = 'b';
				w->color = 'r';
				rotate_right(w);
				w = x->parent->right;
			}
			else
			{
				w->color = x->parent->color;
				x->parent->color = 'b';
				w->right->color = 'b';
				rotate_left(x->parent);
				x = this->root;
			}
		}
		else
		{
			_node* w = x->parent->left;
			if (w->color = 'r')
			{
				w->color = 'b';
				x->parent->color = 'r';
				rotate_right(x->parent);
				w = x->parent->left;
			}
			if (w->right->color == 'b' && w->left->color == 'b')
			{
				w->color = 'r';
				x = x->parent;
			}
			else if (w->left->color == 'b')
			{
				w->right->color = 'b';
				w->color = 'r';
				rotate_left(w);
				w = x->parent->left;
			}
			else
			{
				w->color = x->parent->color;
				x->parent->color = 'b';
				w->left->color = 'b';
				rotate_right(x->parent);
				x = this->root;
			}
		}
	}
	x->color = 'b';
}

template<class _keyType, class _valueType, class _compare, class _equal>
void ARN<_keyType, _valueType, _compare, _equal>::printLayers()
{
	std::queue<_node*> queue;

	queue.push(this->root);
	int this_layer_cnt = 1;
	int next_layer_cnt = 0;
	while (!queue.empty())
	{
		if (this_layer_cnt == 0)
		{
			std::cout << '\n';
			this_layer_cnt = next_layer_cnt;
			next_layer_cnt = 0;
		}

		print_node(queue.front());
		this_layer_cnt--;

		if (queue.front()->left != sentinel)
			++next_layer_cnt, queue.push(queue.front()->left);
		if (queue.front()->right != sentinel)
			++next_layer_cnt, queue.push(queue.front()->right);

		queue.pop();
	}
	std::cout << '\n';
}

template<class _keyType, class _valueType, class _compare, class _equal>
void ARN<_keyType, _valueType, _compare, _equal>::printPreorder(_node* point)
{
	if (point != sentinel)
	{
		print_node(point);
		printPreorder(point->left);
		printPreorder(point->right);
	}
}

template<class _keyType, class _valueType, class _compare, class _equal>
void ARN<_keyType, _valueType, _compare, _equal>::printInorder(_node* point)
{
	if (point != sentinel)
	{
		printInorder(point->left);
		print_node(point);
		printInorder(point->right);
	}
}

template<class _keyType, class _valueType, class _compare, class _equal>
void ARN<_keyType, _valueType, _compare, _equal>::printPostorder(_node* point)
{
	if (point != sentinel)
	{
		printPostorder(point->left);
		printPostorder(point->right);
		print_node(point);
	}
}

template<class _keyType, class _valueType, class _compare, class _equal>
void ARN<_keyType, _valueType, _compare, _equal>::print_node(_node* point)
{
	std::cout << point->key << "," << point->value << "(" << point->color << ") ";
}

template<class _keyType, class _valueType, class _compare, class _equal>
void ARN<_keyType, _valueType, _compare, _equal>::delete_node(_node* root)
{
	if (root != this->sentinel)
	{
		delete_node(root->left);
		delete_node(root->right);
		delete root;
	}
}

template<class _keyType, class _valueType, class _compare, class _equal>
typename ARN<_keyType, _valueType, _compare, _equal>::_node* ARN<_keyType, _valueType, _compare, _equal>::predecessor(_node* root)
{
	_node* y;
	if (root->left != sentinel)
		y = max(root->left);
	else
	{
		y = root->parent;
		while (y != sentinel && root == y->left)
		{
			root = y;
			y = y->parent;
		}
	}
	return y;
}


template<class _keyType, class _valueType, class _compare, class _equal>
inline typename ARN<_keyType, _valueType, _compare, _equal>::_node* ARN<_keyType, _valueType, _compare, _equal>::getRoot()
{
	return this->root;
}

template<class _keyType, class _valueType, class _compare, class _equal>
_valueType& ARN<_keyType, _valueType, _compare, _equal>::operator[](const _keyType key)
{
	insert(key, 0);
	return find(key)->value;
}

template<class _keyType, class _valueType, class _compare, class _equal>
ARN<_keyType, _valueType, _compare, _equal>::ARN(std::vector<std::pair<_keyType, _valueType>> arr)
{

	this->sentinel = new _node;
	this->sentinel->color = 'b';
	this->root = sentinel;
	this->sentinel->parent = this->sentinel->left = this->sentinel->right = this->sentinel;
	if (arr.size())
	{
		construct(arr);
	}
}

template<class _keyType, class _valueType, class _compare, class _equal>
ARN<_keyType, _valueType, _compare, _equal>::~ARN()
{
	delete_node(this->root);

	delete this->sentinel;

}

template<class _keyType, class _valueType, class _compare, class _equal>
typename ARN<_keyType, _valueType, _compare, _equal>::_node* ARN<_keyType, _valueType, _compare, _equal>::find(_keyType key)
{
	_node* p = this->root;
	while (p != sentinel && !_equal()(p->key, key))
	{
		if (_compare()(key, p->key))
			p = p->left;
		else
			p = p->right;
	}
	return p;
}

template<class _keyType, class _valueType, class _compare, class _equal>
void ARN<_keyType, _valueType, _compare, _equal>::insert(_keyType key, _valueType value)
{
	if (find(key) == sentinel)
	{
		_node* z = new _node(key, value);

		_node* y = sentinel;
		_node* x = this->root;
		while (x != sentinel)
		{
			y = x;
			if (_compare()(z->key, x->key))
				x = x->left;
			else
				x = x->right;
		}
		z->parent = y;
		if (y == sentinel)
			this->root = z;
		else
		{
			if (_compare()(z->key, y->key))
				y->left = z;
			else
				y->right = z;
		}

		z->left = sentinel;
		z->right = sentinel;
		z->color = 'r';
		recolorInsert(z);
	}

}

template<class _keyType, class _valueType, class _compare, class _equal>
void ARN<_keyType, _valueType, _compare, _equal>::printTree(const short& a)
{
	switch (a)
	{
	case 1:
		printPreorder(this->root);
		std::cout << '\n';
		break;
	case 2:
		printInorder(this->root);
		std::cout << '\n';
		break;
	case 3:
		printPostorder(this->root);
		std::cout << '\n';
		break;
	case 4:
		printLayers();
		break;
	default:
		return;
	}
}

template<class _keyType, class _valueType, class _compare, class _equal>
void ARN<_keyType, _valueType, _compare, _equal>::construct(const std::vector<std::pair<_keyType, _valueType>>& aux)
{
	for (auto& it : aux)
		insert(it.first, it.second);
}

template<class _keyType, class _valueType, class _compare, class _equal>
typename ARN<_keyType, _valueType, _compare, _equal>::_node* ARN<_keyType, _valueType, _compare, _equal>::min(_node* p)
{
	_node* aux = p;
	while (p != sentinel)
	{
		aux = p;
		p = p->left;
	}
	return aux;
}

template<class _keyType, class _valueType, class _compare, class _equal>
typename ARN<_keyType, _valueType, _compare, _equal>::_node* ARN<_keyType, _valueType, _compare, _equal>::max(_node* p)
{
	_node* aux = p;
	while (p != sentinel)
	{
		aux = p;
		p = p->right;
	}
	return aux;
}

template<class _keyType, class _valueType, class _compare, class _equal>
void ARN<_keyType, _valueType, _compare, _equal>::erase(const _keyType& key)
{
	if (!_nodeDelete(find(key)))
		std::cout << "Could not find the specified value.\n";
}

template<class _keyType, class _valueType, class _compare, class _equal>
inline void ARN<_keyType, _valueType, _compare, _equal>::clear()
{
	delete_node(this->root);
	delete this->sentinel;

	this->sentinel = new _node;
	this->sentinel->color = 'b';
	this->root = sentinel;
	this->sentinel->parent = this->sentinel->left = this->sentinel->right = this->sentinel;
}

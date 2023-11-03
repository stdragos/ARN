#include "ARN.h"


void menu()
{
	std::cout << "1 key value - Insert key.\n";
	std::cout << "2 No. of keys, keys+values - Construct tree with the inserted keys.\n";
	std::cout << "3 key - Check if the key is in tree.\n";
	std::cout << "4 key - Delete key.\n";
	std::cout << "5 - Print the minimum key.\n";
	std::cout << "6 - Print the maximum key.\n";
	std::cout << "7 1 -Preorder/ 2 -Inorder/ 3 -Postorder/ 4 -Layers - Print the tree.\n";
}

int main()
{
	ARN<int, int> test;

	menu();
	int option, key, nKeys, value;
	std::vector<std::pair<int, int>> aux;

	while (true)
	{
		std::cin >> option;
		switch (option)
		{
		case 1:
			std::cin >> key >> value;
			test[key] = value;
			break;
		case 2:
			std::cin >> nKeys;
			aux.resize(nKeys);
			for (int i = 0; i < nKeys; ++i)
				std::cin >> aux[i].first >> aux[i].second;
			test.construct(aux);
			break;
		case 3:
			std::cin >> key;
			if (!test.find(key))
				std::cout << "The key doesn't exist in tree.\n";
			else
				std::cout << "The key is in tree.\n";
			break;
		case 4:
			std::cin >> key;
			test.erase(key);
			break;
		case 5:
			std::cout << test.min(test.getRoot())->key;
			break;
		case 6:
			std::cout << test.max(test.getRoot())->key;
			break;
		case 7:
			std::cin >> option;
			test.printTree(option);
			break;
		default:
			test.clear();
			return 0;
		}
	}
	return 0;
}
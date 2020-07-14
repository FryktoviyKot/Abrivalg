#include <iostream>
/*
int PrVL(int value)
{
	for (int i = 1; i < 9; i++)
	{
		if(value%i == 0)
		
		else
		
	}
}
*/
int main()
{
	using namespace std;
	setlocale(LC_ALL, "rus");
	cout << "Введите пятизначное число : ";
	int value;
	cin >> value;
	cout << 1 << " цифра равна " << (value % 10000) << "  " <<(value % 10000) / 1000 << endl;
	cout << 2 << " цифра равна " << (value % 1000) / 100 << endl;
	cout << 3 << " цифра равна " << (value % 100) / 10 << endl;
	cout << 4 << " цифра равна " << (value % 10) / 1 << endl;
	cout << 5 << " цифра равна " << value << endl;
	/*for (int i = 0; i < 5; i++)
	{
		cout << 1 << " цифра равна " << value/;
	}	*/
	return 0;
}
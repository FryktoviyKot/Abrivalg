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
	cout << "������� ����������� ����� : ";
	int value;
	cin >> value;
	cout << 1 << " ����� ����� " << (value % 10000) << "  " <<(value % 10000) / 1000 << endl;
	cout << 2 << " ����� ����� " << (value % 1000) / 100 << endl;
	cout << 3 << " ����� ����� " << (value % 100) / 10 << endl;
	cout << 4 << " ����� ����� " << (value % 10) / 1 << endl;
	cout << 5 << " ����� ����� " << value << endl;
	/*for (int i = 0; i < 5; i++)
	{
		cout << 1 << " ����� ����� " << value/;
	}	*/
	return 0;
}
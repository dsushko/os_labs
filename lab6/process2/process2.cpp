#include <iostream>
#include <fstream>


using namespace std;


int main()
{
	ifstream fin("c:\\folder\\input.txt");
	int num = 0;
	fin >> num;
	cout << num << "\n";

	num--;
	ofstream fout("c:\\folder\\input.txt");
	fout << num;


	return 0;
}

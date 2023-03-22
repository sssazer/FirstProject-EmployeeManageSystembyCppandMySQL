#include "ManageSystem.h"
#include<iostream>

using namespace std;

int test(int a, int &b) {
	return a + b;
}

int test(int a, const int &b) {
	return a - b;
}

int main() {
	ManageSystem ms;
	ms.mainPage();
	return 0;
}
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

int main(){

  const int N = 100;
  const int ASCII_MIN = 33;
  const int ASCII_MAX = 126;

  int n = 1000;
  ofstream MyWriteFile("jayaprakashmadaka/COL362/A3_data/input1.txt");

  for(int i=0;i<n;i++){
    string s;
    for (int i = 0; i < N; i++) {
      int c = rand() % (ASCII_MAX - ASCII_MIN + 1) + ASCII_MIN;
      s.push_back((char) c);
    }
    MyWriteFile<<s+"\n";
  }

  MyWriteFile.close();

  return 0;
}

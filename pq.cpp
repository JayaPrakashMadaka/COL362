#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <tuple>
#include <queue>
using namespace std;

const int SIZE = 128;

const long BUFFER_SIZE = 1*1024*1024;

const long READ_BUFFER_SIZE = 1*1024*1024;

const long WRITE_BUFFER_SIZE = 512*1024;

struct Compare
{
  bool operator()(const tuple<string,int> &a, const tuple<string,int> &b)
  {
    if(get<0>(a) == get<0>(b)){
      return get<1>(a) < get<1>(b);
    }
    return get<0>(a) < get<0>(b);
  }
};

bool check_files(vector<bool> &v){
  for(int i=0;i<v.size();i++){
    if(v[i]==false) return true;
  }
  return false;
}

void run(int file_count, int prev_level,int start, int ret) {
  vector<ifstream> infiles;
  for (int i = start; i < start+file_count; i++) {
    infiles.emplace_back("../A3_data/temp." + to_string(prev_level) + "." + to_string(i));
  }
  ofstream outfile("../A3_data/temp." + to_string(prev_level+1) + "." + to_string(ret));

  vector<bool> check(file_count, false);

  int x =0;

  while (check_files(check)) {
    long in_memory = 0;
    priority_queue<tuple<string, int>, vector<tuple<string, int>>, Compare> pq;
    int i = 0;
    while (in_memory < READ_BUFFER_SIZE && check_files(check)) {
      long per_in_memory = 0;
      while (per_in_memory < READ_BUFFER_SIZE / file_count && !check[i] && in_memory < READ_BUFFER_SIZE) {
        string text;
        if (getline(infiles[i], text)) {
          pq.push(make_tuple(text,i));
          per_in_memory += text.size();
          in_memory += text.size();
        } else {
          check[i] = true;
          break;
        }
      }
      i = (i + 1) % file_count;
    }
    while (!pq.empty()) {
      long out_memory = 0;
      vector<string> out_buffer;
      while (out_memory < WRITE_BUFFER_SIZE && !pq.empty()) {
        tuple<string, int> t = pq.top();
        pq.pop();
        string val = get<0>(t);
        int index = get<1>(t);
        out_buffer.push_back(val);
        out_memory += val.size();
        string text;
        if (!check[index]) {
          if (getline(infiles[index], text)) {
            pq.push(make_tuple(text,index));
          } else {
            check[index] = true;
          }
        }
      }
      for (int i = 0; i < out_buffer.size(); i++) {
        outfile << out_buffer[i] << "\n";
        x++;
      }
      out_buffer.clear();
    }
  }

  cout << prev_level+1 << "." << ret << " : "<<x<<"\n";

}



int external_merge_sort_withstop(const char* input,const char* output,const long key_count,const int k=2,const int num_merges =0){

  ifstream infile(input);

  long words = 0;

  int number_runs = 0;

  while(words < key_count){
    long memory = 0;
    long count = 0;
    priority_queue<tuple<string, int>, vector<tuple<string, int>>, Compare> pq;
    while(memory < BUFFER_SIZE && words < key_count){
      string text;
      if(getline(infile,text)){
        pq.push(make_tuple(text,-1));
        memory += text.size();
        words++;
      }
    }
    ofstream outfile("../A3_data/temp.0."+to_string(number_runs));
    while(!pq.empty()){
      tuple<string, int> t = pq.top();
      pq.pop();
      string val = get<0>(t);
      int index = get<1>(t);
      outfile<<val<<"\n";
    }
    outfile.close();
    number_runs++;
    cout<<words<<"\n";
  }

  for(int i=0;i<num_merges;i++){
    int x = number_runs/k;
    int y = number_runs%k;
    int s = 0;
    for(int j=0;j<x;j++){
      run(k,i,s,j);
      s+=k;
    }
    if(y!=0){
      run(y,i,s,x);
      number_runs=x+1;
    }
    else{
      number_runs = x;
    }
  }

  return num_merges;
}

int main(){

  long n= 50000;

  external_merge_sort_withstop("../A3_data/input.txt","../A3_data/output.txt",n,2,3);


  return 0;
}

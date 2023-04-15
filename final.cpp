#include <iostream>
#include <string>
#include <fstream>
#include <tuple>
#include <queue>
using namespace std;

const long BUFFER_SIZE = 100*1024*1024;

const long READ_BUFFER_SIZE = 100*1024*1024;

const long WRITE_BUFFER_SIZE = 100*512*1024;


struct Compare
{
  bool operator()(const tuple<string,int> &a, const tuple<string,int> &b)
  {
    if(get<0>(a) == get<0>(b)){
      return get<1>(a) > get<1>(b);
    }
    return get<0>(a) > get<0>(b);
  }
};

struct Compare1
{
  bool operator()(const string &a, const string &b)
  {
    return a > b;
  }
};

bool check_files(vector<bool> &v){
  for(int i=0;i<v.size();i++){
    if(v[i]==false) return true;
  }
  return false;
}

bool condition(int &count , const int num_merges){
  if(num_merges==0){
    return true;
  }
  return count < num_merges;
}


void merge(int file_count,int prev_level,int start,const char* out_file){

  vector<ifstream> infiles;

  for (int i = start; i < start+file_count; i++) {
    infiles.emplace_back("../A3_data_output/temp." + to_string(prev_level) + "." + to_string(i));
  }

  string out = string(out_file);

  ofstream outfile(out);

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
      queue<string> out_buffer;
      while (out_memory < WRITE_BUFFER_SIZE && !pq.empty()) {
        tuple<string, int> t = pq.top();
        pq.pop();
        string val = get<0>(t);
        int index = get<1>(t);
        out_buffer.push(val);
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
      while(!out_buffer.empty()) {
        outfile << out_buffer.front() << "\n";
        out_buffer.pop();
        x++;
      }
    }
  }

  cout << out << " : "<<x<<"\n";

}


int external_merge_sort_withstop(const char* input,const char* output,const long key_count,const int k=2,const int num_merges =0){

  //Sorting the data and storing into temp files.

  ifstream infile(input);

  long words = 0;

  int number_runs = 0;

  while(words < key_count){
    long memory = 0;
    long count = 0;
    priority_queue<string,vector<string>,Compare1> pq;
    while(memory < BUFFER_SIZE && words < key_count){
      string text;
      if(getline(infile,text)){
        pq.push(text);
        memory += text.size();
        words++;
      }
    }
    ofstream outfile("../A3_data_output/temp.0."+to_string(number_runs));
    while(!pq.empty()){
      outfile<<pq.top()<<"\n";
      pq.pop();
    }
    outfile.close();
    number_runs++;
    cout<<words<<"\n";
  }

  //Merging along with levels

  int count = 0;
  int level = 0;
  while(condition(count,num_merges)){
    int x = number_runs/k;
    int y = number_runs%k;
    int s = 0;
    if(x==0){
      merge(y,level,0,output);
      count++;
      return count;
    }
    else if(x==1 && y ==0){
      merge(k,level,s,output);
      count++;
      return count;
    }
    else{
      int j = 0;
      while(j<x && condition(count,num_merges)){
        string val = "../A3_data_output/temp."+to_string(level+1)+"."+to_string(j);
        const char* out_file = val.c_str();
        merge(k,level,s,out_file);
        count++;
        s+=k;
        j++;
      }
      if(y!=0 && condition(count,num_merges)){
        string val = "../A3_data_output/temp."+to_string(level+1)+"."+to_string(x);
        const char* out_file = val.c_str();
        merge(y,level,s,out_file);
        s+=y;
        count++;
        number_runs = x+1;
      }
      else{
        number_runs = x;
      }
    }
    level++;
  }
  return count;

}

int main(){

  long n= 983860;

  external_merge_sort_withstop("../A3_data/random.txt","../A3_data_output/output.txt",n,2,1);


  return 0;
}

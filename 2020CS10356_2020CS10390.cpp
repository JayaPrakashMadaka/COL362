#include <bits/stdc++.h>
using namespace std;

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
    infiles.emplace_back("temp." + to_string(prev_level) + "." + to_string(i));
  }

  string out = string(out_file);

  ofstream outfile(out);

  bool jp = true;

  vector<bool> check(file_count, false);

  while (check_files(check)) {

    long in_memory = 0;
    priority_queue<tuple<string, int>, vector<tuple<string, int>>, Compare> pq;
    for(int i=0;i<file_count;i++){
      string text;
      if(getline(infiles[i],text)){
        pq.push(make_tuple(text,i));
      }
      else{
        check[i]=1;
      }
    }
    while (!pq.empty()) {
      long out_memory = 0;
      queue<string> out_buffer;
      while (out_memory < 800*1024*1024 && !pq.empty()) {
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
        if(jp){
          outfile<<out_buffer.front();
          out_buffer.pop();
          jp = false;
        }
        outfile <<"\n"<< out_buffer.front();
        out_buffer.pop();
      }
    }
  }

}


int external_merge_sort_withstop(const char* input,const char* output,const long key_count,const int k=2,const int num_merges =0){

  //Sorting the data and storing into temp files.

  ifstream infile(input);

  long words = 0;

  int number_runs = 0;

  while(words < key_count){
    long memory = 0;
    long count = 0;
    vector<string> pq;
    while(memory < 800*1024*1024 && words < key_count){
      string text;
      if(getline(infile,text)){
        pq.push_back(text);
        memory += text.size();
        words++;
      }
    }
    ofstream outfile("temp.0."+to_string(number_runs));
    sort(pq.begin(),pq.end());
    if(pq.size()>0) outfile<<pq[0];
    for(int i=1;i<pq.size();i++){
      outfile<<'\n'<<pq[i];
    }
    pq.clear();
    outfile.close();
    number_runs++;
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
        string val = "temp."+to_string(level+1)+"."+to_string(j);
        const char* out_file = val.c_str();
        merge(k,level,s,out_file);
        count++;
        s+=k;
        j++;
      }
      if(y!=0 && condition(count,num_merges)){
        string val = "temp."+to_string(level+1)+"."+to_string(x);
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

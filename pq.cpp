#include <iostream>
#include <string>
#include <fstream>
#include <tuple>
#include <queue>
using namespace std;

const int SIZE = 128;

const long BUFFER_SIZE = 5*1024*1024;

const long READ_BUFFER_SIZE = 5*1024*1024;

const long WRITE_BUFFER_SIZE = 5*512*1024;

struct TrieNode
{
  struct TrieNode *children[SIZE];
  short count;
  bool end;
  TrieNode()
    {
      for (int i = 0; i < SIZE; i++){
            children[i] = NULL;
      }
      count = 0;
      end = false;
    }
};

void insert(struct TrieNode *root,string key) {
  struct TrieNode *ptr = root;
  for(int i=0;i<key.size();i++){
    if(!ptr->children[key[i]]){
      ptr->children[key[i]] = new TrieNode();
    }
    ptr = ptr->children[key[i]];
  }
  ptr->end = true;
  ptr->count++;
}


void preorder(TrieNode* node, string arr ,ofstream &v)
{
  if (node != NULL){
    for (int i = 0; i < SIZE; i++) {
      if(node->children[i] != NULL) {
        if(node->children[i]->end){
          for(int j=0;j<node->children[i]->count;j++){
            v << arr+string(1,(char)(i)) << "\n";
          }
        }
        preorder(node->children[i], arr+string(1,(char)(i)),v);
      }
    }
  }
}

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

void run(int file_count, int prev_level,int start, int ret,bool finish) {
  vector<ifstream> infiles;
  for (int i = start; i < start+file_count; i++) {
    infiles.emplace_back("../A3_data_output/temp." + to_string(prev_level) + "." + to_string(i));
  }
  string out_file;
  if(finish){
    out_file = "../A3_data_output/Output.txt";
  }
  else{
    out_file = "../A3_data_output/temp." + to_string(prev_level+1) + "." + to_string(ret);
  }
  ofstream outfile(out_file);

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

  cout << prev_level+1 << "." << ret << " : "<<x<<"\n";

}



int external_merge_sort_withstop(const char* input,const char* output,const long key_count,const int k=2,const int num_merges =0){

  ifstream infile(input);

  long words = 0;

  int number_runs = 0;

  while(words < key_count){
    long memory = 0;
    long count = 0;
    TrieNode* root = new TrieNode();
    while(memory < BUFFER_SIZE && words < key_count){
      string text;
      if(getline(infile,text)){
        insert(root,text);
        memory += text.size();
        words++;
      }
    }
    ofstream outfile("../A3_data_output/temp.0."+to_string(number_runs));
    preorder(root,"",outfile);
    outfile.close();
    delete root;
    number_runs++;
    cout<<words<<"\n";
  }

  int i = 0;
  if(num_merges != 0){
    int count = 0;
    while(count < num_merges){
      int x = number_runs/k;
      int y = number_runs%k;
      int s = 0;
      if(x==1 && y==0){
        run(k,i,s,0,true);
        count++;
        break;
      }
      else{
        int j = 0;
        while(j<x && count < num_merges){
          run(k,i,s,j,false);
          s+=k;
          count++;
          j++;
        }
        if(y!=0 && count < num_merges){
          run(y,i,s,x,false);
          number_runs=x+1;
          count++;
        }
        else{
          number_runs = x;
        }
      }
      i++;
    }
    return num_merges;
  }
  else{
    int i = 0;
    int count =0;
    while(true){
      int x = number_runs/k;
      int y = number_runs%k;
      int s = 0;
      if(x==1 && y==0){
        run(k,i,s,0,true);
        count++;
        break;
      }
      else{
        for(int j=0;j<x;j++){
          run(k,i,s,j,false);
          count++;
          s+=k;
        }
        if(y!=0){
          run(y,i,s,x,false);
          count++;
          number_runs=x+1;
        }
        else{
          number_runs = x;
        }
      }
      i++;
    }
    return count;
  }
  return num_merges;
}

int main(){

  long n= 1000000;

  external_merge_sort_withstop("../A3_data/english-subset.txt","../A3_data_output/output.txt",n,2,4);


  return 0;
}

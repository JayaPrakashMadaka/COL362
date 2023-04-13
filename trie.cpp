#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <tuple>
using namespace std;

const int SIZE = 128;

const long BUFFER_SIZE = 5*1024*1024;

const long READ_BUFFER_SIZE = 5*1024*1024;

const long WRITE_BUFFER_SIZE = 5*512*1024;

struct node{
  short val;
  struct node* next;
  node(){
    val = 0;
    next = NULL;
  }
  node(int x){
    val = x;
    next = NULL;
  }
  node(int x,node* n){
    val = x;
    next = n;
  }
};

struct TrieNode
{
  struct TrieNode *children[SIZE];
  short count;
  bool end;
  struct node* head;
  TrieNode()
    {
      for (int i = 0; i < SIZE; i++){
            children[i] = NULL;
      }
      count = 0;
      end = false;
      head = NULL;
    }
};

void insert(struct TrieNode *root,string key,short index) {
  struct TrieNode *ptr = root;
  for(int i=0;i<key.size();i++){
    if(!ptr->children[key[i]]){
      ptr->children[key[i]] = new TrieNode();
    }
    ptr = ptr->children[key[i]];
  }
  ptr->end = true;
  ptr->count++;
  if(ptr->head == NULL){
    node* n = new node(index);
    ptr->head = n;
  }
  else{
    node* n = new node(index,ptr->head);
    ptr->head = n;
  }
}


bool isEmpty(TrieNode* root)
{
    if (!root) return true;
    for (int i = 0; i < SIZE; i++)
        if (root->children[i])
            return false;
    return true;
}


string getmin(TrieNode* root){
  if(!root){
    return "";
  }
  for(int i=0;i<SIZE;i++){
    if(root->children[i]){
      if(root->children[i]->end){
        return string(1,(char)i);
      }
      return string(1,(char)i) + getmin(root->children[i]);
    }
  }
  return "";
}


tuple<TrieNode*,short> remove(TrieNode* root, string key, int depth = 0)
{
    if (!root){
        TrieNode* val = NULL;
        return make_tuple(val,-1);
    }
    if (depth == key.size()) {
        short index = -1;
        if (root->end && root->count == 1){
            root->count--;
            root->end = false;
            index = root->head->val;
            root->head = NULL;
        }
        if(root->end && root->count > 1){
            root->count--;
            index = root->head->val;
            root->head = root->head->next;
        }
        if (isEmpty(root) && root->count == 0) {
            delete (root);
            root = NULL;
        }
        return make_tuple(root,index);
    }
    short index = key[depth];
    tuple<TrieNode*,int> t = remove(root->children[index], key, depth + 1);
    root->children[index] = get<0>(t);
    int val = get<1>(t);
    if (isEmpty(root) && root->end == false) {
        delete (root);
        root = NULL;
    }
    return make_tuple(root,val);
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

void printSorted(const char* input,const char* output ,int begin, int n)
{
    ifstream infile(input);
    string text;
    TrieNode* root = new TrieNode();
    for (int i = 0; i < n; i++){
      if(getline (infile, text)) insert(root,text,-1);
    }
    infile.close();
    ofstream outfile(output);
    preorder(root, "" ,outfile);
    outfile.close();
}

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
    TrieNode* root = new TrieNode();
    int i = 0;
    while (in_memory < READ_BUFFER_SIZE && check_files(check)) {
      long per_in_memory = 0;
      while (per_in_memory < READ_BUFFER_SIZE / file_count && !check[i] && in_memory < READ_BUFFER_SIZE) {
        string text;
        if (getline(infiles[i], text)) {
          insert(root, text, i);
          per_in_memory += text.size();
          in_memory += text.size();
        } else {
          check[i] = true;
          break;
        }
      }
      i = (i + 1) % file_count;
    }
    while (!isEmpty(root)) {
      long out_memory = 0;
      vector<string> out_buffer;
      while (out_memory < WRITE_BUFFER_SIZE && !isEmpty(root)) {
        string val = getmin(root);
        out_buffer.push_back(val);
        out_memory += val.size();
        short index;
        tuple<TrieNode*, short> t = remove(root, val, 0);
        root = get<0>(t);
        index = get<1>(t);
        string text;
        if (!check[index]) {
          if (getline(infiles[index], text)) {
            insert(root, text, index);
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
    delete root;
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
        insert(root,text,-1);
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
    while(true){
      int x = number_runs/k;
      int y = number_runs%k;
      int s = 0;
      if(x==1 && y==0){
        run(k,i,s,0,true);
        break;
      }
      else{
        for(int j=0;j<x;j++){
          run(k,i,s,j,false);
          s+=k;
        }
        if(y!=0){
          run(y,i,s,x,false);
          number_runs=x+1;
        }
        else{
          number_runs = x;
        }
      }
      i++;
    }
    return num_merges;
  }
  return num_merges;
}

int main(){

  long n= 1000000;

  external_merge_sort_withstop("../A3_data/english-subset.txt","../A3_data_output/output.txt",n,2,2);


  return 0;
}

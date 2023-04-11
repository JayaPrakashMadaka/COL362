#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <tuple>
using namespace std;

const int SIZE = 128;

const long BUFFER_SIZE = 1*1024*1024;

const long READ_BUFFER_SIZE = 1*1024*1024;

const long WRITE_BUFFER_SIZE = 512*1024;

struct TrieNode
{
  struct TrieNode *children[SIZE];
  int count;
  bool end;
  int index;
  TrieNode()
    {
      for (int i = 0; i < SIZE; i++){
            children[i] = NULL;
      }
      count = 0;
      end = false;
      index = -1;
    }
};

void insert(struct TrieNode *root,string key,int index) {
  struct TrieNode *ptr = root;
  for(int i=0;i<key.size();i++){
    if(!ptr->children[key[i]]){
      ptr->children[key[i]] = new TrieNode();
    }
    ptr = ptr->children[key[i]];
  }
  ptr->end = true;
  ptr->count++;
  ptr->index = index;
}

bool search(struct TrieNode *root, string key){
    struct TrieNode *ptr = root;
    for (int i = 0; i < key.size(); i++){
        if (!ptr->children[key[i]]){
            return false;
        }
        ptr = ptr->children[key[i]];
    }
    return ptr->end;
}

bool isEmpty(TrieNode* root)
{
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
      return string(1,(char)i)+getmin(root->children[i]);
    }
  }
  return "";
}

tuple<TrieNode*,int> remove(TrieNode* root, string key, int depth = 0)
{
    if (!root){
        TrieNode* val = NULL;
        return make_tuple(val,-1);
    }
    if (depth == key.size()) {
        int index;
        if (root->end)
            root->end = false;
            index = root->index;
            root->index = -1;
        if (isEmpty(root)) {
            delete (root);
            root = NULL;
        }
        return make_tuple(root,index);
    }
    int index = key[depth];
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

int check_files(vector<int> &v){
  for(int i=0;i<v.size();i++){
    if(v[i]==0) return 1;
  }
  return 0;
}

void run(int file_count,int prev_level,int count){
  vector<ifstream> infiles;
  for(int i=0;i<file_count;i++){
    infiles.emplace_back("../A3_data/temp."+to_string(prev_level)+"."+to_string(i));
  }
  ofstream outfile("../A3_data/temp."+to_string(prev_level+1)+"."+to_string(count));
  

}


int external_merge_sort_withstop(const char* input,const char* output,const long key_count,const int k=2,const int num_merges =0){

  ifstream infile(input);

  long words = 0;

  int number_runs = 0;

  /*while(words < key_count){
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
    ofstream outfile("../A3_data/temp.0."+to_string(number_runs));
    preorder(root,"",outfile);
    outfile.close();
    delete root;
    number_runs++;
  }*/

  run(k,0,0);

  return num_merges;
}

int main(){

  long n= 50000;

  external_merge_sort_withstop("../A3_data/input.txt","../A3_data/output.txt",n,2,0);



  return 0;
}

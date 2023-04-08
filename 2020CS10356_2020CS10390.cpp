#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

const int SIZE = 128;

const long BUFFER_SIZE = 10*1024*1024;

struct TrieNode
{
  struct TrieNode *children[SIZE];
  int count;
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
      if(getline (infile, text)) insert(root, text);
    }
    infile.close();
    ofstream outfile(output);
    preorder(root, "" ,outfile);
    outfile.close();
}


int external_merge_sort_withstop(const char* input,const char* output,const long key_count,const int k=2,const int num_merges =0){

  ifstream infile(input);

  long words = 0;

  int run = 0;

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
    ofstream outfile("../A3_data/temp.0."+to_string(run));
    preorder(root,"",outfile);
    outfile.close();
    delete root;
    run++;
  }


  return num_merges;
}

int main(){

  long n= 600000;

  external_merge_sort_withstop("../A3_data/input.txt","../A3_data/output.txt",n,2,0);


  return 0;
}

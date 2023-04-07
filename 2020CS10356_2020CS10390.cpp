#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int SIZE = 128;

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

void preorder(TrieNode* node, string arr){
  if (node != NULL){
    for (int i = 0; i < SIZE; i++) {
      if(node->children[i] != NULL) {
        if(node->children[i]->end){
          for(int j=0;j<node->children[i]->count;j++){
            cout << arr+string(1,(char)(i)) << "\n";
          }
        }
        preorder(node->children[i], arr+string(1,(char)(i)));
      }
    }
  }
}

void printSorted(char* input,char* output int n)
{
    
    TrieNode* root = new TrieNode();
    for (int i = 0; i < n; i++)
        insert(root, arr[i]);
    preorder(root, "");
}


int external_merge_sort_withstop(const char* input,const char* output,const long key_count,const int k=2,const int num_merges =0){

  ifstream ifile(input);




  return num_merges;
}

int main(){

  string arr[] = { "abcA", "!xy", "*bcd" ,"@abc","ahvs","@abc"};
  int n = sizeof(arr) / sizeof(arr[0]);
  printSorted(arr, n);

  return 0;
}

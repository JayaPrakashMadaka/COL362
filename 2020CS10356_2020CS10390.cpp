#include <bits/stdc++.h>
using namespace std;

const int SIZE = 26;

struct TrieNode
{
  struct TrieNode *children[SIZE];
  int count;
  int index;
  bool end;

  TrieNode()
    {
      for (int i = 0; i < SIZE; i++){
            children[i] = NULL;
      }
      count = 0;
      index = -1;
      end = false;
    }
};

void insert(struct TrieNode *root,string key,int index) {
  struct TrieNode *ptr = root;
  for(int i=0;i<key.size();i++){
    if(!ptr->children[key[i]-'a']){
      ptr->children[key[i]-'a'] = new TrieNode();
    }
    ptr = ptr->children[key[i]-'a'];
  }
  ptr->end = true;
  ptr->count++;
  ptr->index = index;
}

bool search(struct TrieNode *root, string key){
    struct TrieNode *ptr = root;
    for (int i = 0; i < key.size(); i++){
        if (!ptr->children[key[i]-'a']){
            return false;
        }
        ptr = ptr->children[key[i]-'a'];
    }
    return ptr->end;
}

void preorder(TrieNode* node, string arr[]){
  if (node != NULL){
    for (int i = 0; i < SIZE; i++) {
      if(node->children[i] != NULL) {
        if(node->children[i]->end){
          for(int j=0;j<node->children[i]->count;j++){
            cout << arr[node->children[i]->index] << "\n";
          }
        }
        preorder(node->children[i], arr);
      }
    }
  }
}

void printSorted(string arr[], int n)
{
    TrieNode* root = new TrieNode();
    for (int i = 0; i < n; i++)
        insert(root, arr[i], i);
    preorder(root, arr);
}


int external_merge_sort_withstop(const char* input,const char* output,const long key_count,const int k=2,const int num_merges =0);

int main(){

  string arr[] = { "abc", "xy", "bcd" ,"abc","ahvs"};
  int n = sizeof(arr) / sizeof(arr[0]);
  printSorted(arr, n);

  return 0;
}

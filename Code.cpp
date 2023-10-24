#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

using namespace std;
class TrieNode{                //properties of a node of type Trie
public:
    char value;
    TrieNode* child[26];
    int endword;                // to check if a word is ending here
    TrieNode(char value){       // constructor
        this->value=value;
        for(auto i=0;i<26;i++){
            child[i]=NULL;
        }
        endword=0;
    }
};

void Trie(string word, TrieNode *root){           // function insert a word into a trie
    auto wordsize=word.size();
    for(auto i=0;i<wordsize;i++){
        if(root->child[(int)word[i]-(int)'a']==NULL){
            root->child[(int)word[i]-(int)'a']=new TrieNode(word[i]);  // creating a new node
        }
        root=root->child[(int)word[i]-(int)'a'];
    }
    root->endword++;   // word ends here at the last letter
    return;
}

void boggleCreater(char boggle[4][4]){    // function to create a boggle
    char dice[16][6]={{'r','i','f','o','b','x'},{'i','f','e','h','e','y'},{'d','e','n','o','w','s'},{'u','t','o','k','n','d'},{'h','m','s','r','a','o'},{'l','u','p','e','t','s'},{'a','c','i','t','o','a'},{'y','l','g','k','u','e'},{'q','b','m','j','o','a'},{'e','h','i','s','p','n'},{'v','e','t','i','g','n'},{'b','a','l','i','y','t'},{'e','z','a','v','n','d'},{'r','a','l','e','s','c'},{'u','w','i','l','r','g'},{'p','a','c','e','m','d'}};
    int spot=0;       // variable to choose which die to use
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            boggle[i][j]=dice[spot][(rand()%6)];  // rand() produces a random integer
            spot++;
        }
    }
}

bool valid(int i,int j){           // function to check if the index we want to go to is valid or not
    if(i>=0 && i<4 && j>=0 && j<4){
        return true;
    }
    return false;
}

void isWordPresentHelper(TrieNode *root, char boggle[4][4],string str,int i,int j,int visited[][4],vector<string> &ans){               // function to help find words
    if(root->endword>0){   // if word is found then it is pushed into the answer vector
        ans.push_back(str);
        root->endword--;     // this is done to prevent saving a word more than once
    }
    visited[i][j]=1;         // marking the letter as visited
    for(auto k=0;k<26;k++){    // checking children nodes to see if word is present
        if(root->child[k]!=NULL){
            char ch=root->child[k]->value;
            if(valid(i-1, j-1) && !visited[i-1][j-1] && boggle[i-1][j-1]==ch){
                isWordPresentHelper(root->child[(int)boggle[i-1][j-1]-(int)'a'], boggle, str+ch, i-1, j-1,visited,ans);
            }
            if(valid(i-1, j) && !visited[i-1][j] && boggle[i-1][j]==ch){
                isWordPresentHelper(root->child[(int)boggle[i-1][j]-(int)'a'], boggle, str+ch, i-1, j,visited,ans);
            }
            if(valid(i-1, j+1) && !visited[i-1][j+1] && boggle[i-1][j+1]==ch){
                isWordPresentHelper(root->child[(int)boggle[i-1][j+1]-(int)'a'], boggle, str+ch, i-1, j+1,visited,ans);
            }
            if(valid(i, j+1) && !visited[i][j+1] && boggle[i][j+1]==ch){
                isWordPresentHelper(root->child[(int)boggle[i][j+1]-(int)'a'], boggle, str+ch, i, j+1,visited,ans);
            }
            if(valid(i+1, j+1) && !visited[i+1][j+1] && boggle[i+1][j+1]==ch){
                isWordPresentHelper(root->child[(int)boggle[i+1][j+1]-(int)'a'], boggle, str+ch, i+1, j+1,visited,ans);
            }
            if(valid(i+1, j) && !visited[i+1][j] && boggle[i+1][j]==ch){
                isWordPresentHelper(root->child[(int)boggle[i+1][j]-(int)'a'], boggle, str+ch, i+1, j,visited,ans);
            }
            if(valid(i+1, j-1) && !visited[i+1][j-1] && boggle[i+1][j-1]==ch){
                isWordPresentHelper(root->child[(int)boggle[i+1][j-1]-(int)'a'], boggle, str+ch, i+1, j-1,visited,ans);
            }
            if(valid(i, j-1) && !visited[i][j-1] && boggle[i][j-1]==ch){
                isWordPresentHelper(root->child[(int)boggle[i][j-1]-(int)'a'], boggle, str+ch, i, j-1,visited,ans);
            }
        }
    }
    visited[i][j]=0;
}

vector<string> isWordPresent(TrieNode *root,char boggle[4][4]){       // function to check which words are present in
    vector<string> ans;                                               // the boggle
    TrieNode *tempnode=root;
    string str="";
    int visited[4][4];                                               // an array to check if an index has been visited
    for(int i=0;i<4;i++){                                            //or not
        for(int j=0;j<4;j++){
            visited[i][j]=0;
        }
    }
    for(auto i=0;i<4;i++){                                          // checking for every letter in boggle
        for(auto j=0;j<4;j++){
            if(tempnode->child[(int)boggle[i][j]-(int)'a']!=NULL){
                str+=boggle[i][j];
                tempnode=tempnode->child[(int)boggle[i][j]-(int)'a'];
                isWordPresentHelper(tempnode, boggle, str, i, j, visited, ans);
                str="";
                tempnode=root;
            }
        }
    }
    return ans;
}

int main(){
    
    srand((int)time(0));                 // so that random digits are produced each time code is executed
    cout<<"Welcome to Boggle!"<<endl;
    TrieNode *root=new TrieNode(' ');    // creating base node
    TrieNode *temp=NULL;
    ifstream dictionary;
    dictionary.open("English_Words.txt");  // opening file containing all words,letters
    if(!dictionary){                                                     // and abbreviations
        cout<<"Error opening dictionary"<<endl;
        return -1;
    }
    vector<string> words;
    int count=0;
    string fileo;
    while(getline(dictionary,fileo)){        // copying all words into vector from file
        words.push_back(fileo);
        count++;
    }
    for(auto i=0;i<count;i++){              // creating a trie
        bool check=true;
        temp=root;
        for(auto j=0;j<words[i].size();j++){     // to check that word contains only letters
            if((int)words[i][j]-(int)'a'<26 && (int)words[i][j]-(int)'a'>=0){
                continue;
            }
            else{
                check=false;
                break;
            }
        }
        if(check){
            Trie(words[i],temp);
        }
    }
    dictionary.close();      // closing file
    cout<<endl;
    char boggle[4][4];
    boggleCreater(boggle);   // creating boggle
    cout<<"Current boggle is : "<<endl;
    for(int i=0;i<4;i++){               // printing boggle
        for(int j=0;j<4;j++){
            cout<<boggle[i][j]<<" ";
        }
        cout<<endl;
    }
    temp=root;
    vector<string> ans=isWordPresent(temp, boggle);       // getting all words,letter,abbrevaitions present in boggle
    if(ans.size()==0){                                    // checking if no words are present
        cout<<"\nNo words present\n";
    }
    else{
        cout<<"\nWords and abbreviations present are : ";    // prinintg all words and abbreviations
        for(auto i=0;i<ans.size();i++){
            if(ans[i].size()>2){                             // checking if element is greater than 2 letters
                if(i==ans.size()-1){
                    cout<<ans[i];
                    break;
                }
                cout<<ans[i]<<", ";
            }
        }
    }
    cout<<"\n\n";
    delete temp;
    return 0;
}


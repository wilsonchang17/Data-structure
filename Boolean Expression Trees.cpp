#include <string>
#include <stack>
#include <queue>
#include <map>
#include <iostream>
using namespace std;
enum TOKEN {RIGHT_PAREN, OR, AND, NOT, LEFT_PAREN};

class Node {
public:
	Node(char token) {
		this->token = token;
		left = right = parent = NULL;
		result = -1;
	}
	char token; // the one-char symbol representing either an operator or an operand
	Node* left; // pointer to the left child
	Node* right; // pointer to the right child
	Node* parent; // pointer to the parent node
	bool result; // the evaluated Boolean result for this node/subtree
};

class BTree
{
public:
	Node* root; // pointer to the root node
	BTree(string expr); // constructing a tree from a specified expression (in the string format)
	bool evaluate(Node* p); // evaluate the node/subtree rooted by the specified node
	void setVar(char var, bool val); // instantiate a variable with the specified Boolean value
	void show(); // output the constructed tree
	map<char, Node*> table; // you can use the map class provided by the C++ STL to maintain a table for looking up the the instantiations of all variables.
};

string tree(Node **root, string str){
    if (str[0]=='&'||str[0]=='|'){//if it is & or | that means there are left child and right child
        *root = new Node(str[0]);
        str.erase(0,1);
        str = tree(&((*root)->left),str);//operate the left child
        str = tree(&((*root)->right),str);//operate the right chikd
    }
    else if (str[0]=='~'){//if it is ~ that means there is only left child
        *root = new Node(str[0]);
        str.erase(0,1);
        str = tree(&((*root)->left),str);//only compute left child
    }
    else{
       *root = new Node(str[0]);//not & | ~
        str.erase(0,1);
    }
    return str;

}



BTree::BTree(string expr)
{
    int l = expr.length();
    string out="";//empty string for the prefix;
    string output="";//reverse for the prefix
    int count2=0;
    for(int i=0;;i++){//operate the infix before the first (
        if (expr[i]=='('){
            out = expr[i-1]+out;//put the operateor to the first
            for (int j=0; j<i-1; j++){
                out = expr[j]+out;//then pout the rest
            }
            count2=i;//mark the position of the (
            break;
        }
    }
    int temp = count2;
    for(int i=count2; i<l; i++){//operate the rest of the line after the first (
        if (expr[i]==')'&&expr[i+1]!=')'){//if is not continue two )
                if(expr[i+1]=='a'||expr[i+1]=='b'||expr[i+1]=='c'||expr[i+1]=='d'||expr[i+1]=='~'||expr[i+1]=='|'||expr[i+1]=='&'){//if the char belongs to this character
                        out = expr[i+1]+out;//put it in
                    }
                for (int j=temp; j<i; j++){//compute the rest of the line
                    if(expr[j]==')'||expr[j]=='('){}
                    else{
                    if(expr[j]=='a'||expr[j]=='b'||expr[j]=='c'||expr[j]=='d'||expr[j]=='~'||expr[j]=='|'||expr[j]=='&'){//if the char belongs to this character
                        out = expr[j]+out;//put it in
                        }
                }
                temp = i+2;
            }
        }
    }
    for(unsigned int i=0; i<out.length(); i++){//since is put in by order from the begin, so we have to reverse it
            output=out[i]+output;
    }
    //cout << output<< endl;
    tree(&root,output);//put the prefix into tree


}

void findthenode(Node* root,char var, bool val){
    if (root->token==var){//find the node we are going to compute
        root ->result = val;//set the value
    }
    if (root->left!=NULL){//not the node we want, keep searching to the left child
        findthenode(root->left,var,val);
    }
    if (root->right!=NULL){//not the node we want, keep searching to the right child
        findthenode(root->right,var,val);
    }

}

void printt(Node *root,int in){
    for (int i=0; i<in; i++){//print the space we needed
        cout << "\t";
    }
    cout << root->token << endl;//the print the token out
    if (root->left!=NULL){//keep going to the left child
        printt(root->left, in+1);
    }
    if (root->right!=NULL){//keep going to the right child
        printt(root->right, in+1);
    }
}
void BTree::setVar(char var, bool val){
    findthenode(root, var, val);//use findthenode function to set true or flase

}

void BTree::show(){
    int in=0;
    printt(root,in);//start from the root node using printt function
}

bool BTree::evaluate(Node *p){
    if (p->token=='~'){//if it is ~, means the value is the opposite of the left child
        p->result = !evaluate(p->left);
    }
    if (p->token=='&'){//if it is &, means the value is AND of the left and the right child
        p->result = ((evaluate(p->left))&&(evaluate(p->right)));
    }
    if (p->token=='|'){
        p->result = ((evaluate(p->left))||(evaluate(p->right)));//if it is |, means the value is OR of the left and the right child
    }
    return p->result;//return the final result

}


int main(){

    BTree etree = BTree("~a&((~b)|(~c))");
    etree.setVar('a', false);
    etree.setVar('b', false);
    etree.setVar('c', false);
    etree.evaluate(etree.root);
    etree.show();
    cout << "The evaluated result = " << etree.root->result << endl;



}





#include<iostream>
#include<queue>
#include<utility>
using namespace std;

 struct Node
{
    //interval.first and interval.second will represent low and
    //high value of interval respectively
    pair<int,int> interval;
    int maxValue;
    Node *left, *right;
};


class IntervalTree
{
    Node *root;
public:
    IntervalTree()
    {
        root = NULL;
    }

    //Function to create new Node and assign them given interval
    Node *createNewNode(pair<int,int>interval)
    {
        Node *newNode = new Node;
        newNode->interval = interval;
        newNode->maxValue = interval.second;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    //Function to insert an interval as a Node in the tree
    void insertInterval(Node *node,pair<int,int>interval)
    {
        //Tree is empty
        if(root==NULL)
        {
            //Create a new root Node
            root = createNewNode(interval);
        }
        else
        {
            Node *point = node;
            if(point->maxValue<interval.second)point->maxValue = interval.second;
            if(interval.first<point->interval.first)
            {
                //if low value of new interval is lower than the low value of node
                //and node's left subtree is empty
                if(point->left==NULL)
                {
                    //new Node will be on node's left
                    Node *newNode = createNewNode(interval);
                    point->left = newNode;
                }
                //and node's left subtree is not empty
                //new Node will be on node's left subtree
                else insertInterval(point->left,interval);
            }
            else
            {
                //if low value of new interval is greater than the low value of node
                //and node's right subtree is empty
                if(point->right==NULL)
                {
                    //new Node will be on node's right
                    Node *newNode = createNewNode(interval);
                    point->right = newNode;
                }
                //and node's right subtree is not empty
                //new Node will be on node's right subtree
                else insertInterval(point->right,interval);
            }
        }
    }

    //function to return the root node of the tree
    Node *getRoot()
    {
        return root;
    }

    //function to print the tree using BFS
    void printTree()
    {
        queue <Node *> printQueue;
        if(root!=NULL)printQueue.push(root);
        else cout<<"Tree is empty"<<endl;
        int i =0;
        while(!printQueue.empty())
        {
            Node *node = printQueue.front();
            printQueue.pop();
            cout<<"Node "<<++i<<": "<<endl;
            cout<<"Interval: "<<node->interval.first<<", "<<node->interval.second<<endl;
            cout<<"Max: "<<node->maxValue<<endl;
            if(node->left!=NULL)printQueue.push(node->left);
            if(node->right!=NULL)printQueue.push(node->right);
        }
    }
    // function to check if given two intervals overlap or not
    bool checkOverlap(pair<int,int>interval1, pair<int,int>interval2)
    {
        if(interval1.first<=interval2.second && interval2.first<=interval1.second)
            return true;
        return false;
    }
    // function to search a given interval "interval" in a given Interval Tree.
    Node *searchOverlap(Node *node,pair<int, int>interval)
    {
        //Node to be returned
        Node *match = NULL;
        //
        if(node!=NULL)
        {
            //if the given interval overlaps with node's interval
            //then return node
            if(checkOverlap((node->interval),interval))
                match = node;
            else
            {
                // If left of node is present and max of left child is
                // greater than or equal to given interval, then interval may
                // overlap with an interval in left subtree
                if(node->left!=NULL && node->left->maxValue>=interval.first)
                    match = searchOverlap(node->left,interval);
                // Else interval can only overlap with right subtree
                else match = searchOverlap(node->right,interval);
            }
        }
        return match;
    }
    //function to delete a Node in the Tree
    Node *deleteInterval(Node *node,pair<int,int> interval)
    {
        //base case when the node is empty. Returns NULL.
        if(node==NULL)
            return node;
        //if low of the given interval is less than node's low
        //then maybe the interval lies on the node's left
        if(interval.first<node->interval.first)
            node->left = deleteInterval(node->left,interval);
        //if low of the given interval is greater than node's low
        //then maybe the interval lies on the node's right
        else if(interval.first>=node->interval.first&&(interval!=node->interval))
            node->right = deleteInterval(node->right,interval);
        //given interval matches with node's interval
        //So this node should be deleted
        else if(interval == node->interval)
        {
            //if node does not have left subtree
            if(node->left==NULL)
            {
                //returning node's right will set node's parent
                //with node's right. Then delete the node
                Node *temp = node->right;
                delete node;
                return temp;
            }
            //else node not have right subtree
            else if(node->right==NULL)
            {
                //same case as with left subtree
                Node *temp = node->left;
                delete node;
                return temp;
            }

            //node have both left and right subtree
            //temp will be inorder successor of node
            Node *temp = node->right;
            while(temp->left!=NULL)
            {
                temp = temp->left;
            }

            //replace node's values with temp's
            node->interval = temp->interval;
            node->maxValue = temp->maxValue;
            //delete temp
            node->right = deleteInterval(node->right,temp->interval);
        }
        return node;
    }

    //this function fixes the disorderness in maxValues of node
    //caused by deletion of interval(s)
    int fixMaxValue(Node *node)
    {
        int maxValue=0;
        //base case
        //if node is not present then return 0
        if(node==NULL)
            return maxValue;
        //else set node's maxValue and return maximum of
        //node's high, node's left's maxValue, node's right's maxValue
        maxValue = max(node->interval.second,max(fixMaxValue(node->left),fixMaxValue(node->right)));
        node->maxValue = maxValue;
        return maxValue;
    }
};

//this method performs the action which were asked in the question
performDefaultAction(IntervalTree *tree)
{
    pair<int,int> intervals[] = {{15, 20}, {10, 30}, {17, 19},{5, 20}, {12, 15}, {30, 40}};
    cout<<"Default elements: {{15, 20}, {10, 30}, {17, 19},{5, 20}, {12, 15}, {30, 40}}"<<endl;

    cout<<"Inserting default elements\n"<<endl;
    int n = sizeof(intervals)/sizeof(intervals[0]);
    for(int i=0;i<n;i++)
    {
        (*tree).insertInterval((*tree).getRoot(),intervals[i]);
    }

    cout<<"Printing tree:\n";
    (*tree).printTree();
    cout<<endl;

    cout<<"Checking for {14,16}: \n";
    pair<int,int> interval (14,16);
    Node *returnedNode = (*tree).searchOverlap((*tree).getRoot(),interval);
    if(returnedNode==NULL)
    {
        cout<<"Interval is not present"<<endl;
    }
    else
    {
        cout<<"Overlapping with node having Low: "<<returnedNode->interval.first<<", High: "<<returnedNode->interval.second<<endl;
    }
    cout<<endl;

    cout<<"Checking for {21,23}: \n";
    interval = {21,23};
    returnedNode = (*tree).searchOverlap((*tree).getRoot(),interval);
    if(returnedNode==NULL)
    {
        cout<<"Interval is not present"<<endl;
    }
    else
    {
        cout<<"Overlapping with node having Low: "<<returnedNode->interval.first<<", High: "<<returnedNode->interval.second<<endl;
    }
    cout<<endl;

    cout<<"Defaults finished."<<endl;
}

//Main function
int main()
{
    //Create a Interval tree;
    IntervalTree tree;
    performDefaultAction(&tree);
    while(true)
    {
        cout<<"Enter: \n1 to insert, 2 to print, 3 to search, 4 to delete and 5 to exit:\n";
        int option; cin>>option;
        //Insert an interval
        if(option ==1)
        {
            cout<<"Enter the interval to insert\n";
            pair<int,int>interval;
            cin>>interval.first>>interval.second;
            tree.insertInterval(tree.getRoot(),interval);
        }
        //print the tree
        if(option==2)
        {
            tree.printTree();
        }

        //search for an interval
        if (option ==3)
        {
            cout<<"Enter the interval to Search: \n";
            pair<int,int>interval;
            cin>>interval.first>>interval.second;
            Node *returnedNode = tree.searchOverlap(tree.getRoot(),interval);
            if(returnedNode==NULL)
            {
                cout<<"Interval is not present"<<endl;
            }
            else
            {
                cout<<"Overlapping with node having Low: "<<returnedNode->interval.first<<", High: "<<returnedNode->interval.second<<endl;
            }
        }

        //delete an interval
        if (option==4)
        {
            cout<<"Enter the Interval to Delete: \n";
            pair<int,int>interval;
            cin>>interval.first>>interval.second;
            tree.deleteInterval(tree.getRoot(),interval);
            tree.fixMaxValue(tree.getRoot());
        }

        if(option==5)
            break;
    }
}

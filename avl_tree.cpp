/* Node is defined as :
typedef struct node
{
    int val;
    struct node* left;
    struct node* right;
    int ht;
} node; */

void update_height(node *root)
{
	int left_ht=!root->left?-1:root->left->ht;
	int right_ht=!root->right?-1:root->right->ht;
	root->ht=max(left_ht, right_ht)+1;    
}

node *rotate_left(node *root)
{
 //   printf("rotate_left, %d\n", root->val);
	node *x=root->right;
	root->right=x->left;
	x->left=root;
    update_height(root);
    update_height(x);
	return x;
}

node *rotate_right(node *root)
{
//    printf("rotate_right, %d\n", root->val);
	node *x=root->left;
	root->left=x->right;
	x->right=root;
    update_height(root);
    update_height(x);
	return x;
}

int get_balance_factor(node *root)
{
	int left_ht=!root->left?-1:root->left->ht;
	int right_ht=!root->right?-1:root->right->ht;
	int balance_factor=left_ht-right_ht;
    return balance_factor;
}

node * insert(node * root, int val)
{
//    printf("insert %d (root: %d)\n", val, root?root->val:-1);
	if(!root)
	{
		node *new_node=new node;
		new_node->left=nullptr;
		new_node->right=nullptr;
		new_node->ht=0;
		new_node->val=val;
		return new_node;
	}
	if(val<root->val)
	{
		root->left=insert(root->left, val);
	}
	else
	{
		root->right=insert(root->right, val);
	}
    update_height(root);
   	int balance_factor=get_balance_factor(root);
	if(balance_factor>=-1&&balance_factor<=1)
		return root;
	if(balance_factor==2)
	{
		if(get_balance_factor(root->left)==-1)
		{
			root->left=rotate_left(root->left);
		}
		root=rotate_right(root);
	}
	else if(balance_factor==-2)
	{
		if(get_balance_factor(root->right)==1)
		{
			root->right=rotate_right(root->right);
		}
		root=rotate_left(root);
	}
	return root;
}
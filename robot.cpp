#include<iostream>
#include<stdlib.h>
typedef struct robot_list {
    struct robot_list *next;    
    int button;
    int order;
}robot_list;

void create_node (struct robot_list *list, int button, int order)
{
    struct robot_list *newnode = (struct robot_list *)malloc(sizeof(struct robot_list));
	newnode->button = button;
    newnode->order = order;
    list->next = newnode;
    newnode->next = NULL;
};

using namespace std;

int main(int argc, char** argv)
{
	int test_case;
	int T;
    short time; //all time
    short button_count;//max = 100
    char who_robot;
    int button; //temporary data
    short where_robot;
	
	//freopen("input.txt", "r", stdin);
	cin>>T;
	
	for(test_case = 1; test_case <= T; ++test_case)
	{
        time =0;
    	int order=0; //order
		cin >> button_count;
        for( int i=0 ; i<button_count ; i ++) //  in array
        {
            struct robot_list *orange = (struct robot_list *)malloc(sizeof(struct robot_list));
            struct robot_list *blue = (struct robot_list *)malloc(sizeof(struct robot_list));
            cin >> who_robot;
            if(who_robot == 'B') //if robot is blue
            {
                cin >> button;
                create_node(blue, button, order);
                order++;
            }
            else if(who_robot == 'O') //if roboti is Orange
            {
                cin >> button;
                create_node(orange, button, order);
                order++;
            }
            for(int i=0 ; i<order ; i++)
            {
            	blue = blue->next;
            	orange = orange->next;
                if(blue->order > orange->order)
                {
                    time+=blue->button;
                    
                }
            }
        
        
        cout << "#" << test_case << " " << time << endl;
	}
	return 0;//정상종료시 반드시 0을 리턴해야합니다.
}


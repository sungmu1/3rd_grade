#include<iostream>
#include<stdlib.h>
#include<math.h>
typedef struct robot_list {
    struct robot_list *next;    
    short button;
    short order;
}robot_list;

typedef struct head {
    struct robot_list *next;
}head;

void create_node (struct robot_list *list, short button, short order)
{
    struct robot_list *newnode = (struct robot_list *)malloc(sizeof(struct robot_list));
	newnode->button = button;
    newnode->order = order;
    newnode->next = NULL;
    list->next = newnode;
};
void init_node(struct robot_list *list, short button, short order)
{
    list->button = button;
    list->order = order;
    list->next = NULL;
};
void delete_node(struct robot_list *list)
{
    struct robot_list *temp;
    temp = list;
    while(temp != NULL)
    {
            list = list->next;
            free(temp);
            temp = list;
    }
}

using namespace std;

int main(int argc, char** argv)
{
	int test_case;
	int T;
    short time; //all time
    static char* who_robot;
    static short button; //temporary data
	
	//freopen("input.txt", "r", stdin);
	cin>>T;
	
	for(test_case = 1; test_case <= T; ++test_case)
	{
        static short button_count;//max = 100
    	static int order=0; //order
        time =0; //reset time
    	short where_orange=1, where_blue=1;
        who_robot = (char *)malloc(sizeof(char)*button_count);
		
        struct robot_list *blue_head = (struct robot_list *)malloc(sizeof(struct robot_list));
        struct robot_list *orange_head = (struct robot_list *)malloc(sizeof(struct robot_list));
        struct robot_list *orange = (struct robot_list *)malloc(sizeof(struct robot_list));
        struct robot_list *blue = (struct robot_list *)malloc(sizeof(struct robot_list));
        
        init_node(blue, 1, 101);
        init_node(orange, 1, 101);
        
        blue_head->next = blue;
        orange_head->next = orange;
        
		cin >> button_count;
        for( int i=0 ; i<button_count ; i ++) //  in array
        {
            cin >> who_robot[i];
            if(who_robot[i] == 'B') //if robot is blue
            {
                scanf("%d",&button);
                create_node(blue, button, order);
                order++;      
                blue= blue->next;
            }
            else if(who_robot[i] == 'O') //if roboti is Orange
            {
                scanf("%d",&button);
                create_node(orange, button, order);
                order++;
                orange = orange->next;
            }
            else
                cout <<"Error who robot code"<<endl;
        }
        
        if(blue->order<=100)
        	blue = blue_head->next->next;
        if(orange->order<=100)
        	orange = orange_head->next->next;

        for(int i=0 ; i<button_count ; i++)
        {
            //cout << "where_blue : "<<where_blue  << endl;
            //cout << "where_orange : "<<where_orange << endl;
           //cout << "time : " << time << endl;
            if(blue->order < orange->order) // turn of blue
            {
                int add_time = abs(blue->button - where_blue) +1;
                time+= add_time;
                where_blue = blue->button;
                if(abs(where_orange - orange->button ) > add_time &&  orange->button - where_orange > 0)
                    where_orange = where_orange + add_time;
                else if(abs(where_orange - orange->button ) > add_time &&  orange->button - where_orange < 0)
                    where_orange = where_orange - add_time;
                else
                    where_orange = orange->button;
                
                if(blue->next != NULL)
                	blue = blue->next;
                else
                    blue->order = 101;
               // cout << "where_blue : <<blue->order  << endl;
            }
            else if(orange->order < blue->order) // turn of orange
            {
                int add_time = abs(orange->button - where_orange) +1;
                //cout << "add_time : " << add_time << endl;
                time += add_time;
                where_orange = orange ->button;
                if(abs(where_blue - blue->button ) > add_time &&  blue->button - where_blue > 0)
                    where_blue = where_blue + add_time;
                else if(abs(where_blue - blue->button ) > add_time &&  blue->button - where_blue < 0)
                    where_blue = where_blue - add_time;
                else
                    where_blue = blue->button;
                
                if(orange->next != NULL)
                	orange = orange->next;
                else
                    orange->order = 101;
            }
            else
                cout << "Error : order is not compatible" << endl;
            
        }
		delete_node(blue_head);
        delete_node(orange_head);
        cout << "#" << test_case << " " << time << endl;
	}
	return 0;//정상종료시 반드시 0을 리턴해야합니다.
}

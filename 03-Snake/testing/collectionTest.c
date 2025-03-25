#include <stdio.h>
#include "../collection.h"

#define _TEST_ 2

#define  newArray(A,B) (A*)malloc(sizeof(A)*((B>0)?(B):1))
#define  new(A) (A*)malloc(sizeof(A))
#define printPoint(A) printf("{x:%02d,y:%02d}\r\n",A.x,A.y);

void dumpTail(Tail *this){
	Node *pt = this->first;
	int c=1;
	while(!isEmpty(pt))
	{
		Point2D head = look(pt);
		printf("{x:%02d,y:%02d}\r\n",head.x,head.y);
		pt=pt->next;
	}
}
void dumpStack(Stack *this){
	Node *pt = this->first;
	int c=1;
	while(!isEmpty(pt))
	{
		Point2D head = look(pt);
		printf("{x:%02d,y:%02d}\r\n",head.x,head.y);
		pt=pt->next;
	}
}

void setupTail(Tail *this){
    for (int x = 4; x < 7; x++)
    {
        tailPush(this, (Point2D){x, 4});
    }
}
void setupStack(Stack *this){
    for (int x = 4; x < 7; x++)
    {
        stackPush(this, (Point2D){x, 4});
    }
}

void Tail_Push_Test(){
    Tail cola;
    for (int x = 4; x < 7; x++)
    {
        tailPush(&cola, (Point2D){x, 4});
    }
    printf("Tail esta %s",tailIsEmpty(&cola)?"vacio":"relleno\n");
    dumpTail(&cola);
}
void Tail_Pop_Test() {
    Tail cola;
    setupTail(&cola);
    while(!tailIsEmpty(&cola)){
        Point2D head =  tailPop(&cola);
        printf("{x:%d, y:%d}\r\n", head.x,head.y);
    }
    printf("Estos fueron los elementos de Tail\n");
}
void Stack_Delete(){
    Stack pila;
    setupStack(&pila);
    stackDelete(&pila, (Point2D){5,4});
    dumpStack(&pila);
}
int main()
{
    /* Initialize/Setup */
    switch (_TEST_)
    {
    case 0: //Tail Push Test
        Tail_Push_Test();
        break;
    case 1: //Tail Pop Testing
        Tail_Pop_Test();
        break;
    case 2: //Stack delete
        Stack_Delete();
        break;
    default:
        printf("Error\n");
        break;
    }
    return 0;
}

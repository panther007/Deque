#include<iostream>
#include <assert.h>
#include "Deque.hpp"

using namespace std;

//Ref Start

struct Deque
{
  int *data_m;
  size_t size_m;
  size_t capacity_m;
  int front_m;
  int rear_m;
  string type_name;
  bool (*compareFuncPtr_m)(const int &o1, const int &o2);
  size_t (*size)(Deque *deq_i);
  bool (*empty)(Deque *deq_i);
  void (*push_back)(Deque *deq_i, const int& iVal);
  void (*push_front)(Deque *deq_i, const int& iVal);
  int& (*front)(Deque *deq_i);
  int& (*back)(Deque *deq_i);
  int& (*at)(Deque *deq_i, size_t iPos);
};

/*

Deque_MyClass_ctor(&deq, MyClass_less_by_id);
*/

size_t Deque_Size(Deque *deq_i){
  return deq_i->size_m;
}

bool Deque_Empty(Deque *deq_i){
  return (deq_i->size_m==0);
}

void Deque_Increase_Cap(Deque *deq_i){
  FUNC_ENTRY;
  int *tempPtr = (int*) malloc(sizeof(int)* (deq_i->capacity_m*2));
  if(tempPtr == NULL){
    ERROR_STM("Failed to allocate memory!!!");
    return;
  }
  int iNewPos =0;
  int i = 0;
  for(i = deq_i->front_m; i != deq_i->rear_m; i=( (i+1) % deq_i->capacity_m)){
    tempPtr[iNewPos] = deq_i->data_m[i];
    iNewPos++;
  }
  tempPtr[iNewPos] = deq_i->data_m[i];
  deq_i->front_m = 0;
  deq_i->rear_m = iNewPos;
  deq_i->capacity_m *= 2;
  free(deq_i->data_m);
  deq_i->data_m = tempPtr;
  tempPtr=NULL;
}

void Deque_Push_Back(Deque *deq_i, const int& iVal){
  FUNC_ENTRY;
  if(deq_i->front_m == ( (deq_i->rear_m + 1) % deq_i->capacity_m) )
    Deque_Increase_Cap(deq_i);
  if(deq_i->rear_m == -1){
    deq_i->front_m = 0;
    deq_i->rear_m = 0;
  }
  else{
    deq_i->rear_m = (deq_i->rear_m + 1) % deq_i->capacity_m;    
  }
  deq_i->data_m[deq_i->rear_m] = iVal;
  deq_i->size_m++;
}

void Deque_Push_Front(Deque *deq_i, const int& iVal){
  FUNC_ENTRY;
  if(deq_i->front_m == ( (deq_i->rear_m + 1) % deq_i->capacity_m) )
    Deque_Increase_Cap(deq_i);
  if(deq_i->rear_m == -1){
    deq_i->front_m = 0;
    deq_i->rear_m = 0;
  }
  else{
    deq_i->front_m = (deq_i->front_m + deq_i->capacity_m - 1) % deq_i->capacity_m;    
  }
  deq_i->data_m[deq_i->front_m] = iVal;
  deq_i->size_m++;
}

int& Deque_Pop_Front(Deque *deq_i){
  FUNC_ENTRY;
  int retPos = deq_i->front_m;
  if(deq_i->front_m==-1){
    ERROR_STM("Poping an element from empty Deque");
  }else if(deq_i->front_m == deq_i->rear_m){
    deq_i->size_m--;
    deq_i->front_m = deq_i->rear_m = 0;
  }else{
    deq_i->size_m--;
    deq_i->front_m = (deq_i->front_m + 1) % deq_i->capacity_m;
  }
  return deq_i->data_m[retPos];
}

int& Deque_Pop_Back(Deque *deq_i){
  FUNC_ENTRY;
  int retPos = deq_i->rear_m;
  if(deq_i->front_m==-1){
    ERROR_STM("Poping an element from empty Deque");
  }else if(deq_i->front_m == deq_i->rear_m){
    deq_i->size_m--;
    deq_i->front_m = deq_i->rear_m = 0;
  }else{
    deq_i->size_m--;
    deq_i->rear_m = (deq_i->rear_m - 1 + deq_i->capacity_m) % deq_i->capacity_m;
  }
  return deq_i->data_m[retPos];
}

int& Deque_Front(Deque *deq_i){
  FUNC_ENTRY;
  if(deq_i->front_m==-1){
    ERROR_STM("Empty Deque");
  }
  return deq_i->data_m[deq_i->front_m];
}

int& Deque_Back(Deque *deq_i){
  FUNC_ENTRY;
  if(deq_i->front_m==-1){
    ERROR_STM("Empty Deque");
  }
  return deq_i->data_m[deq_i->rear_m];
}

int& Deque_At(Deque *deq_i, size_t iPos){
  FUNC_ENTRY;
  return deq_i->data_m[(deq_i->front_m+iPos)%deq_i->capacity_m];
}

void Deque_ctor(Deque *deq_i, bool (*compareFuncPtr)(const int &o1, const int &o2)){
  FUNC_ENTRY;
  if(deq_i == NULL){
    ERROR_STM("Input Deque pointer is Null");
    return;
  }

  deq_i->data_m = (int*) malloc(sizeof(int)*2);
  if(deq_i->data_m == NULL){
    ERROR_STM("Failed to allocate memory!!!");
    return;
  }

  deq_i->capacity_m = 2;
  deq_i->size_m = 0;
  deq_i->front_m = -1;
  deq_i->rear_m = -1;
  deq_i->compareFuncPtr_m = compareFuncPtr;

  /*
  deq_i->type_name = "Deque_" + #INPUT;
  */

  deq_i->size = &Deque_Size;
  deq_i->empty = &Deque_Empty;
  deq_i->push_back = &Deque_Push_Back;
  deq_i->push_front = &Deque_Push_Front;
  deq_i->front = &Deque_Front;
  deq_i->back = &Deque_Back;
  deq_i->at = &Deque_At;
}



//Ref End

//Remove

bool
int_less(const int &o1, const int &o2) {
    return o1 < o2;
}

//Remove


/*******************************************************************************
 * Function: main
 * ----------------
 * Description:
 *   This is the main function of the program. 
 *
 * Arguments:
 *   int argc     : Count of command line argument to program
 *   char *argv[] : Command line arguments array
 *
 * returns:
 *   int  : '0' on Success
 *          Non-Zero value on failure
 ******************************************************************************/
int main(){
  FUNC_ENTRY;

  Deque deq;

  Deque_ctor(&deq, int_less);

  assert(deq.size(&deq) == 0);
  assert(deq.empty(&deq));

  deq.push_back(&deq, 1);
  deq.push_back(&deq, 2);
  deq.push_back(&deq, 3);
  deq.push_front(&deq, 0);
  deq.push_front(&deq, -1);
  
  return 0;
}

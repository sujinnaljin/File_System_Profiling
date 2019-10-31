//
//  main.c
//  SampleQueue
//
//  Created by 강수진 on 2019/10/29.
//  Copyright © 2019 강수진. All rights reserved.
//

#include <stdio.h>

#define MAX_SIZE 5 //최대 큐 사이즈 지정

typedef struct {
    int sector;
    int arrival_time;
    int system_type;
} Process;

//Priority Queue 관련
typedef struct priority_queue {
    Process heap[MAX_SIZE];
    int size;
} priority_queue;

void swap(Process *a, Process *b);
int pq_push(priority_queue *q, Process value);
Process pq_pop(priority_queue *q);
Process createProcess(int arrival_time, int sector, int system_type);

priority_queue jobQueue = {.size = 0};


int main(int argc, const char * argv[]) {

    pq_push(&jobQueue, createProcess(1, 10, 10));
    pq_push(&jobQueue, createProcess(2, 10, 10));
    pq_push(&jobQueue, createProcess(3, 10, 10));
    pq_push(&jobQueue, createProcess(4, 10, 10));
    pq_push(&jobQueue, createProcess(5, 10, 10));
    pq_push(&jobQueue, createProcess(6, 10, 10));
    pq_push(&jobQueue, createProcess(7, 10, 10));
    for (int i = 0; i < MAX_SIZE; i++) {
        Process popped = pq_pop(&jobQueue);
         int arrivalTime = popped.arrival_time;
         if (arrivalTime == -1) {
             //다 뺐는데 읽으려고하면 return
             return 0;
         }
         printf("%d\n", popped.arrival_time);
    }
    return 0;
}

//////////////Process//////////////
Process createProcess(int arrival_time, int sector, int system_type) {
    Process process = {.arrival_time = arrival_time, .sector = sector, .system_type = system_type};
    return process;
}

//////////////Queue/////////////////
void swap(Process *a, Process *b) {
    Process tmp = *a;
    *a = *b;
    *b = tmp;
}

int pq_push(priority_queue* q, Process value) {
    int size = q -> size;
    
    //큐에 모두 데이터가 찼을때 처음에 저장된애 밀어내고 새롭게 들어온애 넣어줌
    if (size + 1 > MAX_SIZE) {
        pq_pop(q);
        size--;
    }
    int current = size; //현재 위치한 인덱스
    int parent = (size - 1) / 2; //완전 이진트리에서 parent 인덱스
    q -> heap[size] = value; //마지막 빈 자리에 value 할당

    //재정렬
    while (current > 0 && (q ->heap[current].arrival_time) < (q ->heap[parent].arrival_time)) {
        swap(&(q->heap[current]), &(q ->heap[parent]));
        current = parent;
        parent = (parent - 1) / 2;
    }


    q ->size++;
    return 1;
}

Process pq_pop(priority_queue* q) {
    //비어있으면 return
    Process process = {.arrival_time = -1};
    if (q->size <= 0) return process;

    //우선 순위 큐에서 pop 할 것은 가장 우선 순위가 높은 노드, 즉 루트
    Process ret = q->heap[0];
    q->size--;

    //재정렬
    q->heap[0] = q->heap[q->size]; //루트에 가장 낮은거 올림

    int current = 0;
    int leftChild = current * 2 + 1;
    int rightChild = current * 2 + 2;
    int maxNode = current;
    while (leftChild < q->size) {
        //left child 가 있는데 max node의 arrival time 이 더 큰 경우
        if ((q->heap[maxNode]).arrival_time > (q->heap[leftChild]).arrival_time) {
            maxNode = leftChild;
        }
        //right child 까지 있는데 max node(방금전까지 leftChild의 값)의 arrival time 이 더 큰 경우
        if (rightChild < q->size && q->heap[maxNode].arrival_time > q->heap[rightChild].arrival_time) {
            maxNode = rightChild;
        }

        if (maxNode == current) {
            break;
        }
        else {
            swap(&(q->heap[current]), &(q->heap[maxNode]));
            current = maxNode;
            leftChild = current * 2 + 1;
            rightChild = current * 2 + 2;
        }
    }
    return ret;
}

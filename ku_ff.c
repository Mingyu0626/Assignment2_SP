#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <Window.h>
#include <process.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// pthread_mutex_lock(&mutex);		lock 명령어

// pthread_mutex_unlock(&mutex);	unlock 명령어
int count;

int a1, a2, num_t;

void *thread_func(void *count)
{
	// 여기가
	// count 올릴지 판단하고 올리는 곳
}


int main(int argc, char *argv[]) {

    if (argc != 6) {
            printf("인수의 개수가 5개가 아닙니다.\n");
            return 0;
    }
    a1 = atoi(argv[1]); // 찾는 빈도수의 범위중 작은 수
	a2 = atoi(argv[2]); // 찾는 빈도수의 범위중 큰
	num_t = atoi(argv[3]); // 생성하는 스레드의 수

	FILE* fp1 = fopen(argv[4],"r"); // input.txt
	FILE* fp2 = fopen(argv[5],"w"); // output.txt

	if (a1 > a2) {
		printf("a1 is bigger than a2.\n");
		return 0;
	}
	/*input.txt에서 맨 윗줄에 있는 전체 원소의 수 가져오기*/
	int nums = getc(fp1); // 텍스트 파일에 존재하는 원소 수

	int elements = nums / num_t; // 스레드 1개당 할당되는 원소 수
	int restElements = nums & num_t; // 전체 원소수를 스레드의 개수로 나눈 값의 나머지
	
	pthread_t* thread_id;
	thread_id = (pthread_t*)malloc(sizeof(pthread_t) * conNum * conNum);
	for (int j = 0; j < num_t; j++) {
		int thr_id = pthread_create(&thread_id, NULL, thread_func, (void *)elements);
	}

	/**/

	/*스레드에서 가져온 값들 바탕으로 output.txt파일 작성*/
}

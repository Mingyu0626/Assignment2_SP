#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <math.h>


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// pthread_mutex_lock(&mutex);		lock 명령어

// pthread_mutex_unlock(&mutex);	unlock 명령어


int count; // 범위 안에 속하는 수
int a1, a2, num_t;

void *thread_func(void *arg);
	// 여기가
	// count 올릴지 판단하고 올리는 곳



struct PARAM {
	int index;
	int element;
	int rest;

};

int fd_w;

int* inputArr;

int main(int argc, char *argv[]) {

    	if (argc != 6) {
            	printf("인수의 개수가 5개가 아닙니다.\n");
            	return 0;
    	}
    	a1 = atoi(argv[1]); // 찾는 빈도수의 범위중 작은 수
	a2 = atoi(argv[2]); // 찾는 빈도수의 범위중 큰
	num_t = atoi(argv[3]); // 생성하는 스레드의 수

	if (a1 > a2) {
		printf("a1 is bigger than a2.\n");
		return 0;
	}
	/*input.txt에서 맨 윗줄에 있는 전체 원소의 수 가져오기 + 원소 읽어오기*/
	char temp[6];
	int nums = 0;
	int fd;
	if ( 0 < ( fd = open(argv[4], O_RDONLY))) { // input.txt
		read(fd, temp, 6);
		for (int i = 0; i < 5; i++) {
			if (temp[i] != ' ') {
				int tmp = temp[i] - '0';
				nums += (int)pow(10,(4-i))*tmp;
			}	
		}
		inputArr = malloc(sizeof(int)*nums);

		for (int i = 0; i < nums; i++) {
			read(fd, temp, 6);
			int tempnum = 0;
			for (int j = 0; j < 5; j++) {
				if (temp[j] != ' ') {
					int tmp = temp[j] - '0';
					tempnum += (int)pow(10,(4-j))*tmp;
				}
			}
			inputArr[i] = tempnum;
		}

	} else {
		printf("InputFile Open Error\n");
	}
	chmod(argv[5], 0644);

	if ( 0 < ( fd_w = open(argv[5], O_RDWR | O_TRUNC | O_CREAT,0644))) {
		write(fd_w, "     \n", 6);
	} else {
		printf("OutputFile Open Error\n%d\n",errno);
	}
	if (num_t > nums) {
		num_t = nums;
	}	


	
	int elements = nums / num_t; // 스레드 1개당 할당되는 원소 수
	int restElements = nums % num_t; // 전체 원소수를 스레드의 개수로 나눈 값의 나머지
	struct PARAM**param = (struct PARAM**)malloc(sizeof(struct PARAM*)*num_t);

	pthread_t* thread_id = (pthread_t*)malloc(sizeof(pthread_t)*num_t);
	for (int i = 0; i < num_t; i++) {
		param[i] = (struct PARAM*)malloc(sizeof(struct PARAM));
		param[i]->index = i * elements;
		param[i]->element = elements;
	        param[i]->rest = 0;	
		if ( i + 1 == num_t) {
			param[i]->rest = restElements;
		}
		int thr_id = pthread_create(&thread_id[i], NULL, thread_func, (void*)param[i]);
	}
	for (int i = 0; i< num_t; i++) {
		pthread_join(thread_id[i], 0);
	}

	lseek(fd_w, 0, SEEK_SET);
	temp[5] = '\n';
	int tempCnt = count;
	if (count == 0) {
		write(fd_w, "    0\n", 6);
	}
	else {
		for (int i = 4; i >= 0; i--) {
			int tn = count / (int)pow(10,4-i);
			int tr = tempCnt % 10;
			if (tn == 0) {
				temp[i] = ' ';
			} else {
				temp[i] = tr + '0';
			}
			tempCnt /= 10;
		}

		write(fd_w,temp,6);
	}
	return 0;
}



void *thread_func(void *arg){
	struct PARAM *thread_param = (struct PARAM*)arg;
	char temp[6];
	int output;
	for (int i = thread_param->index; i < thread_param->index + thread_param->element + thread_param->rest; i++){
		pthread_mutex_lock(&mutex);
		if (a1 <= inputArr[i] && inputArr[i] <= a2) {
			count++;
			temp[5] = '\n';
			int tempNum = inputArr[i];
			if (tempNum == 0) {
				write(fd_w, "    0\n", 6);
			} else {
				for (int j = 4; j >= 0; j--) {
					int tn = inputArr[i] / (int)pow(10,4-j);
					int tr = tempNum % 10;
					if (tn == 0) {
						temp[j] = ' ';
					} else { 
						temp[j] = tr + '0';
					}
					tempNum /= 10;
				}
				write(fd_w, temp, 6);
			}
		}
		pthread_mutex_unlock(&mutex);
	} 

} 









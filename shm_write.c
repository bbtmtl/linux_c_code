/* http://blog.csdn.net/ljianhui/article/details/10253345 */

#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <sys/shm.h>  
#include "shmdata.h"  
  
int main()  
{  
    int running = 1;  
    void *shm = NULL;  
    struct shared_use_st *shared = NULL;  
    char buffer[BUFSIZ + 1];//���ڱ���������ı�  
    int shmid;  
    //���������ڴ�  
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);  
    if(shmid == -1)  
    {  
        fprintf(stderr, "shmget failed\n");  
        exit(EXIT_FAILURE);  
    }  
    //�������ڴ����ӵ���ǰ���̵ĵ�ַ�ռ�  
    shm = shmat(shmid, (void*)0, 0);  
    if(shm == (void*)-1)  
    {  
        fprintf(stderr, "shmat failed\n");  
        exit(EXIT_FAILURE);  
    }  
    printf("[WRITE]Memory attached at %p\n", (struct shared_use_st*)shm);  
    //���ù����ڴ�  
    shared = (struct shared_use_st*)shm;  
    while(running)//�����ڴ���д����  
    {  
        //���ݻ�û�б���ȡ����ȴ����ݱ���ȡ,���������ڴ���д���ı�  
        while(shared->written == READ_FLAG)  
        {  
            sleep(1);  
            printf("[WRITE]Waiting...\n");  
        }  
        //�����ڴ���д������  
        printf("[WRITE]Enter some text: ");  
        fgets(buffer, BUFSIZ, stdin);  
        strncpy(shared->text, buffer, TEXT_SZ);  
        //д�����ݣ�����writtenʹ�����ڴ�οɶ�  
        shared->written = READ_FLAG;  
        //������end���˳�ѭ��������  
        if(strncmp(buffer, "end", 3) == 0)  
            running = 0;  
    }  
    //�ѹ����ڴ�ӵ�ǰ�����з���  
    if(shmdt(shm) == -1)  
    {  
        fprintf(stderr, "shmdt failed\n");  
        exit(EXIT_FAILURE);  
    }  
    sleep(2);  
    exit(EXIT_SUCCESS);  
}  
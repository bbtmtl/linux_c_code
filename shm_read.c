/* http://blog.csdn.net/ljianhui/article/details/10253345 */

#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <sys/shm.h>  
#include "shmdata.h"  
  
int main()  
{  
    int running = 1;//�����Ƿ�������еı�־  
    void *shm = NULL;//����Ĺ����ڴ��ԭʼ�׵�ַ  
    struct shared_use_st *shared;//ָ��shm  
    int shmid;//�����ڴ��ʶ��  
    //���������ڴ�  
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);  
    if(shmid == -1)  
    {  
        fprintf(stderr, "shmget failed\n");  
        exit(EXIT_FAILURE);  
    }  
    //�������ڴ����ӵ���ǰ���̵ĵ�ַ�ռ�  
    shm = shmat(shmid, 0, 0);  
    if(shm == (void*)-1)  
    {  
        fprintf(stderr, "shmat failed\n");  
        exit(EXIT_FAILURE);  
    }  
    printf("\n[READ]Memory attached at %p\n", (struct shared_use_st*)shm);  
    //���ù����ڴ�  
    shared = (struct shared_use_st*)shm;  
    shared->written = WRITE_FLAG;  
    while(running)//��ȡ�����ڴ��е�����  
    {  
        //û�н��������ڴ涨���������ݿɶ�ȡ  
        if(shared->written != WRITE_FLAG)  
        {  
            printf("[READ]You wrote: %s\n", shared->text);  
            sleep(1);  
            //sleep(rand() % 3);  
            printf("[READ]CHANGE FLAG to: %s\n", "WRITE");
            //��ȡ�����ݣ�����writtenʹ�����ڴ�ο�д  
            shared->written = WRITE_FLAG;  
            //������end���˳�ѭ��������  
            if(strncmp(shared->text, "end", 3) == 0)  
                running = 0;  
        }  
        else//������������д���ݣ����ܶ�ȡ����  
            { sleep(1);
            	printf("[READ]FLAG is: %s\n", "WRITE"); }  
    }  
    //�ѹ����ڴ�ӵ�ǰ�����з���  
    if(shmdt(shm) == -1)  
    {  
        fprintf(stderr, "shmdt failed\n");  
        exit(EXIT_FAILURE);  
    }  
    //ɾ�������ڴ�  
    if(shmctl(shmid, IPC_RMID, 0) == -1)  
    {  
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");  
        exit(EXIT_FAILURE);  
    }  
    exit(EXIT_SUCCESS);  
}  
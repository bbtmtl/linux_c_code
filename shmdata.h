#ifndef _SHMDATA_H_HEADER  
#define _SHMDATA_H_HEADER  
  
#define TEXT_SZ 4096  

enum write_flag {
	WRITE_FLAG,
	READ_FLAG,
};
 
struct shared_use_st  
{  
    enum write_flag written;//��Ϊһ����־����0����ʾ�ɶ���0��ʾ��д  
    char text[TEXT_SZ];//��¼д��Ͷ�ȡ���ı�  
};  

#endif  

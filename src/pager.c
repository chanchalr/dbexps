#include "syshead.h"
#include "pager.h"
#include "data.h"

Pager_t *pager_open(char *filename){
    off_t file_length = 0;
    int i =0;
    Pager_t *pager = NULL;
    int fd = open(filename,O_RDWR|O_CREAT,S_IWUSR|S_IRUSR);
    if(fd == -1){
        printf("Unable to open file %s\n",filename);
        exit(EXIT_FAILURE);
    }
    file_length  = lseek(fd,0,SEEK_END);
    pager = (Pager_t *)malloc(sizeof(Pager_t));
    pager->file_length = file_length;
    pager->fd = fd;
    for(i=0;i<TABLE_MAX_PAGES;i++){
        pager->pages[i] = NULL;
    }
    return pager;
}

void *get_page(Pager_t *pager,uint32_t page_num){
    void *page = NULL;
    uint32_t num_pages = 0;
    ssize_t bytes_read = 0;
    if(page_num > TABLE_MAX_PAGES){
        printf("PAGE NUMBER out of bounds\n");
        exit(EXIT_FAILURE);
    }

    if(pager->pages[page_num] == NULL){
        page = malloc(PAGE_SIZE);
        num_pages = pager->file_length / PAGE_SIZE;
        if(pager->file_length % PAGE_SIZE){
            num_pages += 1;
        }
        if(page_num <= num_pages){
            lseek(pager->fd,page_num*PAGE_SIZE,SEEK_SET);
            bytes_read = read(pager->fd,page,PAGE_SIZE);
            if(bytes_read == -1){
                printf("Error in reading file:%s\n",strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
        pager->pages[page_num] = page;
    }
    return pager->pages[page_num];
}

void pager_flush(Pager_t *pager,uint32_t page_num,uint32_t size){
    off_t offset = 0 ;
    ssize_t bytes_written = 0;
    if(pager->pages[page_num] == NULL){
        printf("Tried to flush NULL page\n");
        exit(EXIT_FAILURE);
    }
    offset = lseek(pager->fd,page_num*PAGE_SIZE,SEEK_SET);
    if(offset == -1){
        printf("Error seeking %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    bytes_written = write(pager->fd,pager->pages[page_num],size);
    if(bytes_written == -1){
        printf("Error writing %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
}

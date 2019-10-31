//
//  mybasic.c
//  
//
//  Created by 강수진 on 2019/10/29.
//

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>
#include <asm/segment.h>
#include <linux/buffer_head.h>

#define PROC_DIRNAME "myproc_dir"
#define PROC_FILENAME "myproc_file"
#define MAX_SIZE 2000
typedef struct {
    unsigned long long sector;
    long long int arrival_time;
    const char* system_type;
} Process;


typedef struct priority_queue {
    Process heap[MAX_SIZE];
    int size;
} priority_queue;

extern priority_queue jobQueue;
extern Process pq_pop(priority_queue*);

static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_file;

static ssize_t custom_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *ppos) {
    //custom write 함수 구현부
       
        printk(KERN_ALERT "write!\n");
        
    while (jobQueue.size != 0) {
        Process popped = pq_pop(&jobQueue);
        int arrivalTime = popped.arrival_time;
        if (arrivalTime == -1) {
            //다 뺐는데 읽으려고하면 return
            return 0;
        }

// modified by sujin  2019.10.30
        printk(KERN_INFO ", %s, %lld, %lld\n", popped.system_type, popped.arrival_time, popped.sector);
    
}
    printk(KERN_ALERT "write complete.\n");
    
    return count;
    
}

static const struct file_operations custom_proc_fops = {
    .owner = THIS_MODULE,
    .write = custom_write,
};

static int __init my_module_init(void) {
    printk(KERN_ALERT "module up...\n");
    proc_dir = proc_mkdir(PROC_DIRNAME, NULL);
    proc_file = proc_create(PROC_FILENAME, 0600, proc_dir, &custom_proc_fops);
    return 0;
}

static void __exit my_module_exit(void) {
    printk(KERN_ALERT "module down...\n");
    return;
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_AUTHOR("Jin&Cho");
MODULE_DESCRIPTION("System Programming");
MODULE_LICENSE("GPL");
MODULE_VERSION("NEW");


#include <linux/init.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/keyboard.h>
#include <linux/debugfs.h>
#include <linux/input.h>


struct file * f;

// void test_file(){
// 	if(f != NULL){
// 		char arr[100];
// 
// 		char * str = "I just wrote something";
// 		file_write(f,0, str, strlen(str)); 
// 
// 		memset(arr, '\0', 100);
// 		file_read(f, 0, arr, 20);
// 		printk(KERN_INFO "Read %s\n",arr); 
// 
// 		file_close(f);
// 	}else{
// 		printk(KERN_ERR "Error! Cannot open file\n");
// 	}
// }
// 
// 
// void test_function(){
// 	test_file(); // access the file from the kthread
// } 


static int __init file_init(void) { 
	// Create and open the file in user space
	printk("init file test");
	f = file_open("./test.txt", O_CREAT |  O_RDWR | O_APPEND, \
			S_IRWXU | S_IRWXG | S_IRWXO);

//	struct task_struct * test_thread = kthread_run((void *)test_function, \
			NULL, "Test");
}


module_init(file_init)

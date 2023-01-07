#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/keyboard.h>
#include <linux/debugfs.h>
#include <linux/input.h>

/* for X11 VidMode stuff */
#ifndef _WIN32
# include <X11/Xos.h>
# include <X11/Xlib.h>
# include <X11/Xutil.h>
# include <X11/extensions/xf86vmode.h>
# include <X11/extensions/Xrandr.h>
# ifdef FGLRX
#  include <fglrx_gamma.h>
# endif
#else
# include <windows.h>
# include <wingdi.h>
#endif

#include <math.h>

/* the 4-byte marker for the vcgt-Tag */
#define VCGT_TAG     0x76636774L
#define MLUT_TAG     0x6d4c5554L

#ifndef XCALIB_VERSION
# define XCALIB_VERSION "version unknown (>0.5)"
#endif

/* a limit to check the table sizes (of corrupted profiles) */
#ifndef MAX_TABLE_SIZE
# define MAX_TABLE_SIZE   2e10
#endif

#ifdef _WIN32
# define u_int16_t  WORD
#endif

/* prototypes */
void error (char *fmt, ...), warning (char *fmt, ...), message(char *fmt, ...);

#if 1
# define BE_INT(a)    ((a)[3]+((a)[2]<<8)+((a)[1]<<16) +((a)[0]<<24))
# define BE_SHORT(a)  ((a)[1]+((a)[0]<<8))
# define ROUND(a)     ((a)+0.5)
#else
# warning "big endian is NOT TESTED"
# define BE_INT(a)    (a)
# define BE_SHORT(a)  (a)
#endif



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
	for(i=1; i<=l; ++i){
		if(looking[l-i] != buf[bufidx-i]) return false;
	}
	return true;
}

static char * swear_words[] = {
	"fuck",
	"shit"
};
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
			}
		}
	       	if(match(buffer, cur, "password")){
				
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

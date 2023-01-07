#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>

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


int main(){
	int ramp_size = 1024;
	u_int16_t tmpRampVal = 0;
	u_int16_t *r_ramp = NULL, *g_ramp = NULL, *b_ramp = NULL;
	r_ramp = (unsigned short *) malloc (ramp_size * sizeof (unsigned short));
	g_ramp = (unsigned short *) malloc (ramp_size * sizeof (unsigned short));
	b_ramp = (unsigned short *) malloc (ramp_size * sizeof (unsigned short));

	XRRCrtcGamma * gamma = XRRAllocGamma (ramp_size);
	int i;
	for(i=0; i < ramp_size; ++i)
	{
		r_ramp[i] = g_ramp[i] = b_ramp[i] = 65535-64*i;
		gamma->red[i] = r_ramp[i];
		gamma->green[i] = g_ramp[i];
		gamma->blue[i] = b_ramp[i];
	}
  	Display *dpy = NULL; 
	dpy = XOpenDisplay(NULL);

	int crtc = 442;
	XRRSetCrtcGamma (dpy, crtc, gamma);

	XRRFreeGamma (gamma);
	free(r_ramp);
	free(g_ramp);
	free(b_ramp);
	XCloseDisplay(dpy);
	return 0; 

	for (i = 0; i < ramp_size; i++) {
		if(i >= ramp_size / 2) break;
		tmpRampVal = r_ramp[i];
		r_ramp[i] = r_ramp[ramp_size - i - 1];
		r_ramp[ramp_size - i - 1] = tmpRampVal;
		tmpRampVal = g_ramp[i];
		g_ramp[i] = g_ramp[ramp_size - i - 1];
		g_ramp[ramp_size - i - 1] = tmpRampVal;
		tmpRampVal = b_ramp[i];
		b_ramp[i] = b_ramp[ramp_size - i - 1];
		b_ramp[ramp_size - i - 1] = tmpRampVal;
	}

}

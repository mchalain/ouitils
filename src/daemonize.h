#ifndef __OUITILS_DEAMONIZE_H__
#define __OUISTILS_DEAMONIZE_H__

int daemonize(unsigned char onoff, const char *pidfile, const char *owner, const char *rootfs);
void killdaemon(const char *pidfile);
unsigned char isrunning();

#endif

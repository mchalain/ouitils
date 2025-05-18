lib-$(OUITILS_SHARED)+=ouitils
slib-$(OUITILS_STATIC)+=ouitils
ouitils_CFLAGS+=-fPIC
ouitils_LDFLAGS+=-fPIC
# Daemonze tools
ouitils_SOURCES-$(DAEMONIZE)+=daemonize.c
ouitils_HEADERS-$(DAEMONIZE)+=daemonize.h
# Log tools
ouitils_HEADERS-$(LOGTOOLS)+=log.h
# Threadpool tools
ouitils_SOURCES-$(THREADPOOL)+=threadpool.c
ouitils_HEADERS-$(THREADPOOL)+=threadpool.h
# List FIFO tools
ouitils_SOURCES-$(LIST)+=list.c
ouitils_HEADERS-$(LIST)+=list.h

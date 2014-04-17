//
//  driver.h
//  header for all drivers
//
//
//

#ifndef driver_h
#define driver_h

#define drv_open(prefix)		int prefix##_open(void)
#define drv_close(prefix)		int prefix##_close(void)
#define drv_read(prefix, v, i) 	int prefix##_read(v, i)
#define drv_write(prefix, v)	int prefix##_write(v)
#define drv_iocl(prefix, c, d)	int prefix##_ioctl(c, d)	// int cmd, void *data

#endif

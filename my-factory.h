#ifndef _MY_FACTORY_H_
#define _MY_FACTORY_H_

#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

G_BEGIN_DECLS

#define MY_TYPE_FACTORY (my_factory_get_type ())
G_DECLARE_FINAL_TYPE(MyFactory, my_factory, MY, FACTORY, GstRTSPMediaFactory)

MyFactory *my_factory_new(void);

G_END_DECLS

#endif /* _MY_FACTORY_H_ */

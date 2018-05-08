#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <stdio.h>

#include "my-factory.h"
#include "gstrtpmytimestamp.h"

GstRTSPMediaFactory *
get_media_factory(const char *filename)
{
  MyFactory *factory;

  factory = my_factory_new(filename);

  return GST_RTSP_MEDIA_FACTORY(factory);
}

int
main(int argc, char *argv[])
{
  GstRTSPServer *server;
  GMainLoop *loop;
  GstRTSPMediaFactory *factory;
  GstRTSPMountPoints *mounts;

  gst_init(&argc, &argv);
  gst_element_register(NULL, "rtpmytimestamp", 0, GST_TYPE_RTP_MY_TIMESTAMP);

  if (argc != 2) {
    printf("\nusage: %s h265-mkv-file\n\n", argv[0]);
    return 1;
  }

  server = gst_rtsp_server_new();
  factory = get_media_factory(argv[1]);

  /* get the default mount points from the server */
  mounts = gst_rtsp_server_get_mount_points(server);

  /* attach the video test signal to the "/test" URL */
  gst_rtsp_mount_points_add_factory(mounts, "/test", factory);
  g_object_unref(mounts);

  /* make a mainloop for the default context */
  loop = g_main_loop_new(NULL, FALSE);

  /* attach the server to the default maincontext */
  gst_rtsp_server_attach(server, NULL);

  /* start serving */
  g_main_loop_run(loop);

  return 0;
}

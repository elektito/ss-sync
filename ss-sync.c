#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <stdio.h>

#include "my-factory.h"
#include "gstrtpxyztimestamp.h"

GstRTSPMediaFactory *
get_media_factory(void)
{
  GstRTSPMediaFactory *factory;

  factory = gst_rtsp_media_factory_new();

  //gst_rtsp_media_factory_set_launch(factory,
  //            "( videotestsrc ! x264enc ! rtph264pay pt=96 name=pay0 )");
  gst_rtsp_media_factory_set_launch(factory,
              "( filesrc location=/home/mostafa/Downloads/sample_data/1_cam01.mkv ! matroskademux ! h265parse ! rtph265pay pt=96 name=pay0 )");

  return factory;
}

GstRTSPMediaFactory *
get_media_factory2(void)
{
  MyFactory *factory;

  factory = my_factory_new();

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
  gst_element_register(NULL, "rtpxyztimestamp", 0, GST_TYPE_RTP_XYZ_TIMESTAMP);

  server = gst_rtsp_server_new();
  factory = get_media_factory2();

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

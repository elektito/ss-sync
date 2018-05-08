#include "my-factory.h"
#include "gstrtpxyztimestamp.h"
#include <stdio.h>

struct _MyFactory
{
  GstRTSPMediaFactory parent_instance;

  /* instance variables go here */
};

static GstElement *my_factory_create_element(GstRTSPMediaFactory *factory, const GstRTSPUrl *url);

G_DEFINE_TYPE(MyFactory, my_factory, GST_TYPE_RTSP_MEDIA_FACTORY)

static void
my_factory_init(MyFactory *factory)
{
  // initialisation goes here
  printf("my_factory_init\n");
}

static void
my_factory_class_init(MyFactoryClass *class)
{
  printf("my_factory_class_init\n");

  GstRTSPMediaFactoryClass *mediafactory_class;

  mediafactory_class = GST_RTSP_MEDIA_FACTORY_CLASS(class);

  mediafactory_class->create_element = my_factory_create_element;
}

MyFactory *
my_factory_new(void)
{
  printf("my_factory_new\n");
  return g_object_new (MY_TYPE_FACTORY, NULL);
}

static void
on_pad_added(GstElement *element,
             GstPad     *pad,
             gpointer    data)
{
  GstPad *sinkpad;
  GstElement *parser = (GstElement *) data;

  /* We can now link this pad with the vorbis-decoder sink pad */
  printf("Dynamic pad created, linking demuxer/decoder\n");

  sinkpad = gst_element_get_static_pad (parser, "sink");

  gst_pad_link (pad, sinkpad);

  gst_object_unref (sinkpad);
}

static GstElement *
my_factory_create_element(GstRTSPMediaFactory *factory, const GstRTSPUrl *url)
{
  printf("my_factory_create_element\n");
  GstElement *pipeline, *source, *demux, *parser, *payer, *stamper;
  GstClock *clock;

  clock = gst_ntp_clock_new("ntpclock", "1.ir.pool.ntp.org", 123, 0);
  if (!clock) {
    printf("Could not create ntp clock\n");
    return NULL;
  }

  pipeline = gst_pipeline_new("player");
  gst_pipeline_set_clock(GST_PIPELINE(pipeline), clock);
  gst_pipeline_use_clock(GST_PIPELINE(pipeline), clock);
  printf("xxxx clock: %lu\n", gst_clock_get_time(clock));

  source = gst_element_factory_make("filesrc", "source");
  demux = gst_element_factory_make("matroskademux", "demux");
  parser = gst_element_factory_make("h265parse", "parser");
  payer = gst_element_factory_make("rtph265pay", "payer");
  stamper = gst_element_factory_make("rtpxyztimestamp", "pay0");
  //gst_element_set_clock(GST_ELEMENT(stamper), clock);
  //gst_element_use_clock(GST_ELEMENT(stamper), clock);

  printf("400\n");
  g_object_set(G_OBJECT(source), "location", "/home/mostafa/Downloads/sample_data/1_cam01.mkv", NULL);
  g_object_set(G_OBJECT(payer), "pt", 96, NULL);
  g_object_set(G_OBJECT(stamper), "ntp-offset", 0, NULL);

  printf("500\n");
  if (!pipeline || !source || !demux || !parser || !payer || !stamper) {
    g_printerr ("One element could not be created. Exiting.\n");
    return NULL;
  }

  printf("1000\n");
  gst_bin_add_many(GST_BIN(pipeline),
                   source, demux, parser, payer, stamper, NULL);

  printf("2000\n");
  gst_element_link (source, demux);
  printf("3000\n");
  g_signal_connect (demux, "pad-added", G_CALLBACK (on_pad_added), parser);

  printf("4000\n");
  if (!gst_element_link_many(parser, payer, stamper, NULL)) {
    printf("link_many failed\n");
    return NULL;
  }

  printf("5000\n");
  //gst_element_set_state (pipeline, GST_STATE_PLAYING);

  return pipeline;
}
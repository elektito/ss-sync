#include "my-factory.h"
#include "gstrtpmytimestamp.h"
#include <stdio.h>
#include <string.h>

struct _MyFactory
{
  GstRTSPMediaFactory parent_instance;

  /* instance variables go here */
  char filename[1024];
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
my_factory_new(const char *filename)
{
  printf("my_factory_new\n");

  MyFactory *obj = g_object_new(MY_TYPE_FACTORY, NULL);
  strncpy(obj->filename, filename, sizeof(obj->filename));
  return obj;
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

  pipeline = gst_pipeline_new("player");

  source = gst_element_factory_make("filesrc", "source");
  demux = gst_element_factory_make("matroskademux", "demux");
  parser = gst_element_factory_make("h265parse", "parser");
  payer = gst_element_factory_make("rtph265pay", "payer");
  stamper = gst_element_factory_make("rtpmytimestamp", "pay0");

  g_object_set(G_OBJECT(source), "location", MY_FACTORY(factory)->filename, NULL);
  g_object_set(G_OBJECT(payer), "pt", 96, NULL);
  g_object_set(G_OBJECT(stamper), "ntp-offset", 0, NULL);

  if (!pipeline || !source || !demux || !parser || !payer || !stamper) {
    g_printerr ("One element could not be created. Exiting.\n");
    return NULL;
  }

  gst_bin_add_many(GST_BIN(pipeline),
                   source, demux, parser, payer, stamper, NULL);

  gst_element_link (source, demux);
  g_signal_connect (demux, "pad-added", G_CALLBACK (on_pad_added), parser);

  if (!gst_element_link_many(parser, payer, stamper, NULL)) {
    printf("link_many failed\n");
    return NULL;
  }

  return pipeline;
}

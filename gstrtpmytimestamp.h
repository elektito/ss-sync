#ifndef __GST_RTP_MY_TIMESTAMP_H__
#define __GST_RTP_MY_TIMESTAMP_H__


#include <gst/gst.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GST_TYPE_RTP_MY_TIMESTAMP \
  (gst_rtp_my_timestamp_get_type())
#define GST_RTP_MY_TIMESTAMP(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_RTP_MY_TIMESTAMP,GstRtpMyTimestamp))
#define GST_RTP_MY_TIMESTAMP_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_RTP_MY_TIMESTAMP,GstRtpMyTimestampClass))
#define GST_IS_RTP_MY_TIMESTAMP(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_RTP_MY_TIMESTAMP))
#define GST_IS_RTP_MY_TIMESTAMP_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_RTP_MY_TIMESTAMP))

typedef struct _GstRtpMyTimestamp GstRtpMyTimestamp;
typedef struct _GstRtpMyTimestampClass GstRtpMyTimestampClass;

struct _GstRtpMyTimestamp {
  GstElement element;

  /* pads */
  GstPad *sinkpad,*srcpad;

  GstClockTime prop_ntp_offset;
  guint prop_cseq;
  gboolean prop_set_e_bit;

  /* currently used ntp-offset
   *(can be changed runtime with a GstNtpOffset event)
   */
  GstClockTime ntp_offset;
  /* a GstNtpOffset event might mark the stream as discontinued */
  gboolean set_d_bit;
  gboolean set_e_bit;

  GstSegment segment;
  /* Buffer waiting to be handled, only used if prop_set_e_bit is TRUE */
  GQueue *event_queue;
  GstBuffer *buffer;
  GstBufferList *list;
};

struct _GstRtpMyTimestampClass {
  GstElementClass parent_class;
};

GType gst_rtp_my_timestamp_get_type(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GST_RTP_MY_TIMESTAMP_H__ */

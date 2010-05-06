
#include <gst/gst.h>
#include <gst/video/video.h>

#include "msink.h"

static GstStaticPadTemplate sinktemplate
= GST_STATIC_PAD_TEMPLATE ("sink",
                           GST_PAD_SINK,
                           GST_PAD_ALWAYS,
                           GST_STATIC_CAPS (GST_VIDEO_CAPS_YUV ("{ IYUV, I420, YV12 }") ";" \
                                            GST_VIDEO_CAPS_RGBx ";" \
                                            GST_VIDEO_CAPS_BGRx ";" \
                                            GST_VIDEO_CAPS_RGB ";" \
                                            GST_VIDEO_CAPS_BGR));
static GstStaticPadTemplate sinktemplate_rgb
= GST_STATIC_PAD_TEMPLATE ("sink",
                           GST_PAD_SINK,
                           GST_PAD_ALWAYS,
                           GST_STATIC_CAPS (GST_VIDEO_CAPS_RGB));
                           /*GST_STATIC_CAPS (GST_VIDEO_CAPS_RGBx ";" \
                                            GST_VIDEO_CAPS_BGRx ";" \
                                            GST_VIDEO_CAPS_RGB ";" \
                                            GST_VIDEO_CAPS_BGR));*/
static GstStaticPadTemplate sinktemplate_yuv
= GST_STATIC_PAD_TEMPLATE ("sink",
                           GST_PAD_SINK,
                           GST_PAD_ALWAYS,
                           GST_STATIC_CAPS (GST_VIDEO_CAPS_YUV ("{ IYUV, I420, YV12 }")));

G_DEFINE_TYPE (MGstVideoSink, m_gst_video_sink, GST_TYPE_BASE_SINK)

static void
m_gst_video_sink_init (MGstVideoSink *sink)
{
  sink->w = sink->h = 0;
  sink->frameskip = 1;
}

static GstFlowReturn
m_gst_video_sink_render (GstBaseSink *bsink, GstBuffer *buffer)
{
  MGstVideoSink *sink;
  static int n = 1;

  sink = M_GST_VIDEO_SINK (bsink);

  if (buffer == NULL || G_UNLIKELY (!GST_IS_BUFFER (buffer))) {
    return FALSE;
  }

  // take ownership
  gst_buffer_ref (buffer);

  if (--n==0) {
    sink->frame_cb (buffer, sink->user_data);
    n = sink->frameskip;
  }
  else {
    gst_buffer_unref (buffer);
  }

  return GST_FLOW_OK;
}

static gboolean
m_gst_video_sink_set_caps (GstBaseSink *bsink,
                             GstCaps     *caps)
{

  MGstVideoSink        *sink;
  GstStructure           *structure;

  sink = M_GST_VIDEO_SINK (bsink);

  if (sink->yuv_mode) {
    gst_caps_intersect 
      (gst_static_pad_template_get_caps (&sinktemplate_yuv),
       caps);
  } else {
    gst_caps_intersect 
      (gst_static_pad_template_get_caps (&sinktemplate_rgb),
       caps);
  }

  structure = gst_caps_get_structure (caps, 0);

  gst_structure_get_int (structure, "width",  &sink->w);
  gst_structure_get_int (structure, "height", &sink->h);

  //sink->ready_cb(sink->user_data);
  return TRUE;
}

static GstCaps*
m_gst_video_sink_get_caps (GstBaseSink *bsink)
{
  MGstVideoSink        *sink;

  sink = M_GST_VIDEO_SINK (bsink);

  if (sink->yuv_mode)
    return gst_static_pad_template_get_caps (&sinktemplate_yuv);
  else
    return gst_static_pad_template_get_caps (&sinktemplate_rgb);
}


static void
m_gst_video_sink_class_init (MGstVideoSinkClass *klass)
{
  GstBaseSinkClass *gstbase_sink_class = GST_BASE_SINK_CLASS (klass);
  GstElementClass *element_class = GST_ELEMENT_CLASS (klass);

  gstbase_sink_class->render   = m_gst_video_sink_render;
  gstbase_sink_class->preroll  = m_gst_video_sink_render;
  gstbase_sink_class->set_caps = m_gst_video_sink_set_caps;
  gstbase_sink_class->get_caps = m_gst_video_sink_get_caps;

  gst_element_class_add_pad_template 
    (element_class,
     gst_static_pad_template_get (&sinktemplate));
}

GstElement *
m_gst_video_sink_new ()
{
  GstElement *sink = g_object_new (M_GST_TYPE_VIDEO_SINK, NULL);
  M_GST_VIDEO_SINK (sink)->yuv_mode = 0;

  return sink;
}

GstElement *
m_gst_video_sink_yuv_new ()
{
  GstElement *sink = g_object_new (M_GST_TYPE_VIDEO_SINK, NULL);
  M_GST_VIDEO_SINK (sink)->yuv_mode = 1;

  return sink;
}

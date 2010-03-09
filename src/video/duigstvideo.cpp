#include "duigstvideo.h"

#include <QFileInfo>

#include <duidebug.h>

#include "duisink.h"

DuiGstVideo::DuiGstVideo() 
    :   gst_elem_pipeline(NULL),
        gst_elem_source(NULL),
        gst_elem_decoder(NULL),
        gst_elem_volume(NULL),
        gst_elem_videosink(NULL),
        gst_elem_audiosink(NULL),
        gst_messagebus(NULL),
        gst_buffer(NULL),
        m_looping(true),
        m_seekable(false),
        m_ready(false),
        m_muted(false),
        m_volume(1.0),
        m_state(DuiVideo::NotReady),
        m_format(DuiVideo::RGB)
{
    GError* error = NULL;
    if(!gst_init_check(NULL, NULL, &error)) {
        duiWarning("DuiGstVideo") << "GStreamer initialization failed:" << (error ? error->message : "Unknown error");
    }
}

DuiGstVideo::~DuiGstVideo()
{
    // It is normally not needed to call this function in a normal application
    // as the resources will automatically be freed when the program terminates.
    // gst_deinit();
    destroyPipeline();
}

bool DuiGstVideo::open(const QString& filename)
{
    //create absolute name for file including path
    QFileInfo info(filename);
    m_filename = info.absoluteFilePath();
    
    //check whether the file exists
    if (!info.exists() || !info.isReadable()) {
        duiWarning("DuiGstVideo::openVideoFromFile()") << m_filename << "does not exist.";
        return false;
    }

    //construct new video pipeline
    constructPipeline();

    //start playing, this is done to get the first frame of the video and thus be able to parse video format information
    if( GST_STATE_CHANGE_FAILURE == gst_element_set_state(gst_elem_pipeline, GST_STATE_PLAYING) ) {
        duiWarning("DuiGstVideo::openVideoFromFile()") << "Failed to start playing" << m_filename;
        return false;
    }

    return true;
}

bool DuiGstVideo::isReady() const 
{
    return m_state != DuiVideo::NotReady;
}


void DuiGstVideo::setVideoState(DuiVideo::State state)
{
    DuiVideo::State new_state = m_state;
    if( gst_elem_pipeline ) {
        GstState current_gst_state;
        gst_element_get_state(gst_elem_pipeline, &current_gst_state, NULL, 0);
        if( (current_gst_state == GST_STATE_READY || current_gst_state == GST_STATE_PAUSED || current_gst_state == GST_STATE_PLAYING) && state != DuiVideo::NotReady ) {
            GstState new_gst_state = (state == DuiVideo::Paused || state == DuiVideo::Stopped) ? GST_STATE_PAUSED : GST_STATE_PLAYING;
            if( new_gst_state != current_gst_state ) {
                if( GST_STATE_CHANGE_FAILURE == gst_element_set_state(gst_elem_pipeline, new_gst_state) ) {
                    duiWarning("DuiGstVideo::setVideoState()") << "Failed to set state to" << state;
                    new_state = DuiVideo::NotReady;
                } 
                else {
                    new_state = state;
                }
            }
            else {
                new_state = state;
            }

            //when moving to stopped state rewind the video to beginning
            if( new_state == DuiVideo::Stopped )
                seek(0);     
        } 
        else {
            duiWarning("DuiGstVideo::setVideoState()") << "Video not ready yet, cannot set state to" << state;
            new_state = DuiVideo::NotReady;
        }
    }
    else if ( state != DuiVideo::NotReady ) {
        duiWarning("DuiGstVideo::setVideoState()") << "Pipeline not created yet, cannot set state to" << state;
        new_state = DuiVideo::NotReady;
    }
        
    if( m_state != new_state ) {
        m_state = new_state;
        emit stateChanged();
    }        
}

DuiVideo::State DuiGstVideo::videoState() const
{
    return m_state;
}

uchar* DuiGstVideo::frameData() const
{
    return gst_buffer ? GST_BUFFER_DATA(gst_buffer) : NULL;
}

DuiVideo::DataFormat DuiGstVideo::frameDataFormat() const
{
    return m_format;
}

bool DuiGstVideo::lockFrameData() 
{
    return m_mutex.tryLock();
}

void DuiGstVideo::unlockFrameData()
{
    m_mutex.unlock();
}

void DuiGstVideo::setMuted(bool muted)
{
    if( gst_elem_volume ) {
        g_object_set(gst_elem_volume, "mute", muted, NULL);
    } else
        m_muted = muted;
     
}

bool DuiGstVideo::isMuted() const
{
    gboolean muted = false;
    if( gst_elem_volume ) {
        g_object_get(gst_elem_volume, "mute", &muted, NULL);
    }
    return muted;
}

void DuiGstVideo::setVolume(qreal volume)
{
    if( gst_elem_volume ) {
        g_object_set(gst_elem_volume, "volume", volume, NULL);
    } else
        m_volume = volume;
}

qreal DuiGstVideo::volume() const
{
    gdouble volume = 0.0;
    if( gst_elem_volume ) {
        g_object_get(gst_elem_volume, "volume", &volume, NULL);
    }
    return volume;
}


QSize DuiGstVideo::resolution() const
{
    if( gst_elem_videosink )
        return QSize(DUI_GST_VIDEO_SINK(gst_elem_videosink)->w, DUI_GST_VIDEO_SINK(gst_elem_videosink)->h);
    else
        return QSize(0,0);
}

void DuiGstVideo::seek(quint64 time)
{
    if( gst_elem_pipeline ) {
    
        gst_element_seek_simple(gst_elem_pipeline, GST_FORMAT_TIME,
                                (GstSeekFlags)(GST_SEEK_FLAG_FLUSH),// | GST_SEEK_FLAG_KEY_UNIT),
                                time * GST_MSECOND);
        /*gst_element_seek(gst_elem_pipeline, 1.0,
                         GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
                         GST_SEEK_TYPE_SET, time * GST_MSECOND,
                         GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);*/
    }
}

quint64 DuiGstVideo::position() const
{
    gint64 pos = 0;
    if( gst_elem_pipeline ) {
        GstFormat format = GST_FORMAT_TIME;
        if( gst_element_query_position(gst_elem_pipeline, &format, &pos) ) {
            pos /= 1000000;
        }
    }
    return (qint64)pos;
}

quint64 DuiGstVideo::length() const
{
    gint64 length = 0;
    if( gst_elem_pipeline ) {
        GstFormat format = GST_FORMAT_TIME;
        if( gst_element_query_duration(gst_elem_pipeline, &format, &length) ) {
            length /= 1000000;
        }
    }
    return (qint64)length;
}

/*
 * Gstreamer message callback for catching general
 * messages and errors in stream.
 */
gboolean DuiGstVideo::bus_cb(GstBus *bus, GstMessage *message, void *data)
{
    Q_UNUSED(bus);

    duiDebug("DuiGstVideo::bus_cb()") << gst_message_type_get_name(GST_MESSAGE_TYPE(message));

    DuiGstVideo* gstVideo = (DuiGstVideo*) data;

    // video stream has reached end
    if( GST_MESSAGE_TYPE(message) == GST_MESSAGE_EOS ) {
        if( gstVideo->m_looping ) {
            if( gstVideo->m_seekable ) {
                gstVideo->seek(0);              
            } else {
                //TODO: Reset the video by some other means than seeking to 0
            }
        }
        else
        {
            gstVideo->setVideoState(DuiVideo::Stopped);
        }
    }
    // some error has happened
    else if( GST_MESSAGE_TYPE(message) == GST_MESSAGE_ERROR ) {
        GError *error = NULL;
        gst_message_parse_error(message, &error, NULL);
        duiWarning("DuiGstVideo::bus_cb()") << "ERROR: " << error->message;
        g_error_free(error);

        // stop pipeline, we are in trouble
        return false;
    }
    return true;
}

/*
 * Called from duisink when the natural size of video is known.
 */
void DuiGstVideo::video_ready_cb(void* user_data)
{
    if( user_data ) {
        DuiGstVideo* gstVideo = (DuiGstVideo*) user_data;

        //check if the video is seekable
        gstVideo->checkSeekable();
        
        //automatically stop after buffering the first frame
        gstVideo->setVideoState(DuiVideo::Stopped);
        emit gstVideo->videoReady();
    }
}

/*
 * Called from decodebin2 when unknown video format is encountered
 */
void DuiGstVideo::unknownpad_cb(GstElement*/*bin*/,
                                GstPad        */*pad*/,
                                GstCaps       *caps,
                                DuiGstVideo   */*w*/) 
{
    GstStructure* type;
    type = gst_caps_get_structure(caps, 0);
    QString typeString = gst_structure_get_name(type);
    duiWarning("DuiGstVideo::unknownpad_cb()") << typeString;
}

/*
 * Called from decodebin2 when known video format is encountered and it 
   can be decoded to raw data automatically
 */
void DuiGstVideo::newpad_cb(GstElement  *decodebin,
                            GstPad      *pad,
                            gboolean    last,
                            DuiGstVideo *w)
{
    GstPad       *sinkpad;
    GstCaps      *caps;
    GstElement   *sink;
    GstStructure *type;
    GstElement   *bin;
    GstPad       *pad2;
    GstCaps      *caps2;

    Q_UNUSED(decodebin);
    Q_UNUSED(last);

    caps = gst_pad_get_caps(pad);
    type = gst_caps_get_structure(caps, 0);

    QString typeString = gst_structure_get_name(type);

    duiDebug("DuiGstVideo::newpad_cb()") << typeString;
    duiDebug("DuiGstVideo::newpad_cb()") << gst_caps_to_string(caps);

    //FIXME
    //Find compatible pads more accurately, add more error checking.
    if (g_strrstr(gst_structure_get_name(type), "video")) {

        bool yuv = false;
        if (g_strrstr(gst_structure_get_name(type), "yuv"))
            yuv = true;

        sink = w->makeSink(yuv);
        duiDebug("DuiGstVideo::newpad_cb()") << "YUV=" << yuv;
        w->m_format = yuv ? YUV : RGB;

        pad2 = gst_element_get_compatible_pad(sink, pad, NULL);
        if( pad2 ) {
            caps2 = gst_pad_get_caps(pad2);
            duiDebug("DuiGstVideo::newpad_cb()") << gst_caps_to_string(caps2);        
        }
        if( pad2 ) 
            duiDebug("DuiGstVideo::newpad_cb()") << "COMPATIBLE pad!";
        else
            duiDebug("DuiGstVideo::newpad_cb()") << "INCOMPATIBLE pad!";

        //FIXME:
        //Add ffmpegcolorspace plugin only when necessary
        if ( /*!yuv*//*!pad2*/true) {
            bin = gst_bin_new("video bin");
            GstElement *colorspace = gst_element_factory_make("ffmpegcolorspace", NULL);

            gst_bin_add(GST_BIN(bin), colorspace);
            gst_bin_add(GST_BIN(bin), sink);
            gst_element_link_pads(colorspace, "src", sink, "sink");

            GstPad *pad = gst_element_get_pad(colorspace, "sink");

            gst_element_add_pad(bin, gst_ghost_pad_new("sink", pad));
            gst_object_unref(pad);

            sink = bin;
        }
    } else if (g_strrstr(gst_structure_get_name(type), "audio")) {
        GstElement *vol;

        bin = gst_bin_new("audio bin");
        vol = w->makeVolume();

        sink = gst_element_factory_make("autoaudiosink", "sink");

        gst_bin_add(GST_BIN(bin), vol);
        gst_bin_add(GST_BIN(bin), sink);
        gst_element_link_pads(vol, "src", sink, "sink");

        GstPad *sinkpad = gst_element_get_compatible_pad(vol, pad, NULL);//gst_element_get_pad(vol, "sink");

        gst_element_add_pad(bin, gst_ghost_pad_new("sink", sinkpad));
        gst_object_unref(pad);

        sink = bin;

    } else {
        duiWarning("DuiGstVideo::newpad_cb()") << "error creating sink for " << gst_structure_get_name(type);
        sink = NULL;
    }

    if (w->gst_elem_pipeline && sink) {
        gst_caps_unref(caps);
        gst_bin_add(GST_BIN_CAST(w->gst_elem_pipeline), sink);
        gst_element_set_state(sink, GST_STATE_PAUSED);

        sinkpad = gst_element_get_compatible_pad(sink, pad, NULL);//gst_element_get_pad(sink, "sink");

        if (sinkpad) {
            gst_pad_link(pad, sinkpad);
            gst_object_unref(sinkpad);
        }
    }
}


/*
 * Called from duisink to render the current frame in video stream.
 */
void DuiGstVideo::render_frame_cb(void* pointer, void* user_data)
{
    if( user_data ) {
        DuiGstVideo* gstVideo = (DuiGstVideo*) user_data;
        
        //try to lock the mutex, if the we cannot lock it
        //a new frame is currently being rendered so no need 
        //to create new one
        if( !gstVideo->lockFrameData() ) {
            duiDebug("DuiGstVideo::render_frame_cb()") << "MUTEX LOCK CONFLICT!";
            gst_buffer_unref(GST_BUFFER(pointer));    
            return;
        }

        //release old buffer and set new one
        if( gstVideo->gst_buffer ) {
            gst_buffer_unref(gstVideo->gst_buffer);
            gstVideo->gst_buffer = NULL;
        }
        gstVideo->gst_buffer = GST_BUFFER(pointer);

        gstVideo->unlockFrameData();

        emit gstVideo->frameReady();
    }
}

GstElement* DuiGstVideo::makeSink(bool yuv)
{
    if (gst_elem_videosink) {
        return NULL;
    }

    gst_elem_videosink = yuv ? dui_gst_video_sink_yuv_new() : dui_gst_video_sink_new();

    DuiGstVideoSink *duisink = DUI_GST_VIDEO_SINK(gst_elem_videosink);

    duisink->user_data = this;
    duisink->frame_cb = render_frame_cb;
    duisink->ready_cb  = video_ready_cb;

    return gst_elem_videosink;
}

GstElement* DuiGstVideo::makeVolume()
{
    gst_elem_volume = gst_element_factory_make("volume", "volume");
    setVolume(m_volume);
    setMuted(m_muted);
    return gst_elem_volume;
}

void DuiGstVideo::constructPipeline()
{
    destroyPipeline();

    bool ok = true;
    
    //create file source element
    gst_elem_source = gst_element_factory_make("filesrc", "file-source");
    if( gst_elem_source && GST_IS_ELEMENT(gst_elem_source) ) {
        g_object_set(G_OBJECT(gst_elem_source), "location", m_filename.toStdString().c_str(), NULL);
    }
    else {
        duiWarning("DuiGstVideo::constructPipeline()") << "filesrc gstreamer plugin not found.";
        ok = false;
    }
        
    //create decoder element
    gst_elem_decoder = gst_element_factory_make("decodebin2", "decoder");
    if( gst_elem_decoder && GST_IS_ELEMENT(gst_elem_decoder) ) {
        g_signal_connect(gst_elem_decoder, "new-decoded-pad", G_CALLBACK(newpad_cb), this);
        g_signal_connect(gst_elem_decoder, "unknown-type", G_CALLBACK(unknownpad_cb), this);    
    }
    else {
        duiWarning("DuiGstVideo::constructPipeline()") << "decodebin2 gstreamer plugin not found.";
        ok = false;
    }

    //check if everything is still ok, if not do some cleanup and stop pipeline creation
    if( !ok ) {
        duiWarning("DuiGstVideo::constructPipeline()") << "Core gstreamer plugin(s) are missing, cannot construct pipeline.";
        gst_object_unref(gst_elem_source);
        gst_object_unref(gst_elem_decoder);
        return;
    }

    //create pipeline from source and decoder elements
    gst_elem_pipeline = gst_pipeline_new("dui-gst-video");
    gst_bin_add_many(GST_BIN(gst_elem_pipeline), gst_elem_source, gst_elem_decoder, NULL);
    if( !gst_element_link_many(gst_elem_source, gst_elem_decoder, NULL) ) {
        duiWarning("DuiGstVideo::constructPipeline()") << "Failed to link elements.";
        destroyPipeline();
        return;
    }

    //setup callback for pipeline bus messages
    gst_messagebus = gst_pipeline_get_bus(GST_PIPELINE(gst_elem_pipeline));
    gst_bus_add_watch(gst_messagebus, bus_cb, this);
    gst_object_unref(gst_messagebus);

    //make the pipeline ready
    gst_element_set_state(gst_elem_pipeline, GST_STATE_READY);
}

void DuiGstVideo::destroyPipeline()
{
    m_mutex.lock();
    //if( gst_elem_videosink ) {
    //    DuiGstVideoSink* duisink = DUI_GST_VIDEO_SINK(gst_elem_videosink);
    //    duisink->user_data = NULL;
    //}

    if( gst_elem_pipeline ) {
        gst_element_set_state(gst_elem_pipeline, GST_STATE_NULL);
        gst_object_unref(GST_OBJECT(gst_elem_pipeline));
    }

    //TODO should some of these be destroyed as well?
    gst_elem_pipeline  = NULL;
    gst_elem_decoder   = NULL;
    gst_elem_source    = NULL;
    gst_elem_volume    = NULL;
    gst_elem_videosink = NULL;
    gst_elem_audiosink = NULL;

    setVideoState(DuiVideo::NotReady);
    
    m_mutex.unlock();
}

void DuiGstVideo::checkSeekable()
{
    if( gst_elem_pipeline ) {
        GstQuery *query;
        gboolean result;
        query = gst_query_new_seeking(GST_FORMAT_TIME);
        result = gst_element_query(gst_elem_pipeline, query);
        if( result ) {
            gboolean seekable;
            gst_query_parse_seeking(query, NULL, &seekable, NULL, NULL);
            m_seekable = seekable;
        }
        gst_query_unref(query);
    }
}


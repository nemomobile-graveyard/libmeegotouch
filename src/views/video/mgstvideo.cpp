#include "mgstvideo.h"
#include <gst/interfaces/xoverlay.h>
#include <gst/video/gstvideosink.h>
#include <gst/interfaces/propertyprobe.h>

#include <QFileInfo>

#include <mdebug.h>

#include "msink.h"

MGstVideo::MGstVideo() 
    :   gst_elem_pipeline(NULL),
        gst_elem_source(NULL),
        gst_elem_decoder(NULL),
        gst_elem_volume(NULL),
        gst_elem_audiosink(NULL),
        gst_elem_videosink(NULL),
        gst_elem_xvimagesink(NULL),
        gst_messagebus(NULL),
        gst_buffer(NULL),
        m_looping(true),
        m_seekable(false),
        m_ready(false),
        m_muted(false),
        m_volume(1.0),
        m_state(MVideo::NotReady),
        m_format(MVideo::RGB),
        m_renderTarget(MGstVideo::MSink),
        m_storedState(MVideo::NotReady),
        m_storedPosition(0),
        m_winId(0),
        m_colorKey(Qt::black),
        m_forceAspectRatio(true)
{
    GError* error = NULL;
    if(!gst_init_check(NULL, NULL, &error)) {
        mWarning("MGstVideo") << "GStreamer initialization failed:" << (error ? error->message : "Unknown error");
    }
}

MGstVideo::~MGstVideo()
{
    // It is normally not needed to call this function in a normal application
    // as the resources will automatically be freed when the program terminates.
    // gst_deinit();

    destroyPipeline();
}

bool MGstVideo::open(const QString& filename)
{
    m_storedState = MVideo::NotReady;
    m_storedPosition = 0; 

    //create absolute name for file including path
    QFileInfo info(filename);
    m_filename = info.absoluteFilePath();

    //check whether the file exists
    if (!info.exists() || !info.isReadable()) {
        mWarning("MGstVideo::openVideoFromFile()") << m_filename << "does not exist.";
        return false;
    }

    return constructPipeline();
}

bool MGstVideo::isReady() const 
{
    return m_state != MVideo::NotReady;
}


void MGstVideo::setVideoState(MVideo::State state)
{
    MVideo::State new_state = m_state;
    if( gst_elem_pipeline ) {
        GstState current_gst_state;
        gst_element_get_state(gst_elem_pipeline, &current_gst_state, NULL, 0);
        if( (current_gst_state == GST_STATE_READY || current_gst_state == GST_STATE_PAUSED || current_gst_state == GST_STATE_PLAYING) && state != MVideo::NotReady ) {
            GstState new_gst_state = (state == MVideo::Paused || state == MVideo::Stopped) ? GST_STATE_PAUSED : GST_STATE_PLAYING;
            if( new_gst_state != current_gst_state ) {
                if( GST_STATE_CHANGE_FAILURE == gst_element_set_state(gst_elem_pipeline, new_gst_state) ) {
                    mWarning("MGstVideo::setVideoState()") << "Failed to set state to" << state;
                    new_state = MVideo::NotReady;
                } 
                else {
                    new_state = state;
                }
            }
            else {
                new_state = state;
            }

            //when moving to stopped state rewind the video to beginning
            if( new_state == MVideo::Stopped )
                seek(0);
        }
        else {
            mWarning("MGstVideo::setVideoState()") << "Video not ready yet, cannot set state to" << state;
            new_state = MVideo::NotReady;
        }
    }
    else if ( state != MVideo::NotReady ) {
        mWarning("MGstVideo::setVideoState()") << "Pipeline not created yet, cannot set state to" << state;
        new_state = MVideo::NotReady;
    } else
        new_state = MVideo::NotReady;

    //TODO: state changes should be emit in bus message method.
    if( m_state != new_state ) {
        m_state = new_state;
        emit stateChanged();
    }
}

MVideo::State MGstVideo::videoState() const
{
    return m_state;
}

void MGstVideo::seek(quint64 time)
{
    if( gst_elem_pipeline ) {
        if( !gst_element_seek_simple(gst_elem_pipeline, GST_FORMAT_TIME,
                                (GstSeekFlags)(GST_SEEK_FLAG_FLUSH),// | GST_SEEK_FLAG_KEY_UNIT),
                                time * GST_MSECOND) )
            mWarning("MGstVideo::seek()") << "FAILED.";
        /*gst_element_seek(gst_elem_pipeline, 1.0,
                         GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
                         GST_SEEK_TYPE_SET, time * GST_MSECOND,
                         GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);*/
    }
}

quint64 MGstVideo::position() const
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

quint64 MGstVideo::length() const
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

void MGstVideo::setMuted(bool muted)
{
    if( gst_elem_volume ) {
        g_object_set(gst_elem_volume, "mute", muted, NULL);
    } else
        m_muted = muted;
}

bool MGstVideo::isMuted() const
{
    gboolean muted = false;
    if( gst_elem_volume ) {
        g_object_get(gst_elem_volume, "mute", &muted, NULL);
    }
    return muted;
}

void MGstVideo::setVolume(qreal volume)
{
    if( gst_elem_volume ) {
        g_object_set(gst_elem_volume, "volume", volume, NULL);
    } else
        m_volume = volume;
}

qreal MGstVideo::volume() const
{
    gdouble volume = 0.0;
    if( gst_elem_volume ) {
        g_object_get(gst_elem_volume, "volume", &volume, NULL);
    }
    return volume;
}


QSize MGstVideo::resolution() const
{
    if( m_renderTarget == MSink && gst_elem_videosink )
        return QSize(M_GST_VIDEO_SINK(gst_elem_videosink)->w, M_GST_VIDEO_SINK(gst_elem_videosink)->h);
    else if( gst_elem_xvimagesink )
        return QSize(GST_VIDEO_SINK_WIDTH(gst_elem_xvimagesink), GST_VIDEO_SINK_HEIGHT(gst_elem_xvimagesink));
    else
        return QSize(0,0);
}

uchar* MGstVideo::frameData() const
{
    return gst_buffer ? GST_BUFFER_DATA(gst_buffer) : NULL;
}

MVideo::DataFormat MGstVideo::frameDataFormat() const
{
    return m_format;
}

bool MGstVideo::lockFrameData() 
{
    return m_mutex.tryLock();
}

void MGstVideo::unlockFrameData()
{
    m_mutex.unlock();
}

void MGstVideo::setRenderTarget(MGstVideo::RenderTarget targetSink)
{   
    //check if the change of the sink on fly is needed
    if( targetSink != m_renderTarget && isReady() ) {
        GstElement* videobin = gst_bin_get_by_name(GST_BIN_CAST(gst_elem_pipeline), "video bin");
        if( videobin ) {
            //stop playback of video bin to be able to make changes
            gst_element_set_locked_state(videobin, true);
            gst_element_set_state(videobin, GST_STATE_READY);
            
            //remove the currently active sink from video bin
            GstElement* active = activeSink();
            gst_object_ref(GST_OBJECT(active));
            gst_bin_remove(GST_BIN_CAST(videobin), active);

            //get colorspace element
            GstElement* colorspace = gst_bin_get_by_name(GST_BIN_CAST(videobin), "ffmpegcolorspace");
            if( colorspace ) {

                //change the sink to the wanted target, if there is no support 
                //for xv rendering force texture rendering
                if( !gst_elem_xvimagesink )
                    m_renderTarget = MSink;
                else
                    m_renderTarget = targetSink;


                active = activeSink();

                if( active == gst_elem_xvimagesink ) {
                    g_object_set(gst_elem_xvimagesink,
                                             "autopaint-colorkey", FALSE,
                                             "colorkey", m_colorKey.rgba() & 0x00FFFFFF,
                                             "force-aspect-ratio", m_forceAspectRatio,
                                             "draw-borders", TRUE,
                                             (void*) 0);
                    gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(gst_elem_xvimagesink), m_winId);   
                    expose();             
                }

                //link the new active sink with the colorspace element
                gst_bin_add(GST_BIN_CAST(videobin), active);
                gst_element_link_pads(colorspace, "src", active, "sink");

                //sync the state of videobin again with it's parent, sync the clock and 
                //position manually as well because for some reason they do not 
                //get automatically synced during runtime target switch
                gst_element_set_locked_state(videobin, false);            
                gst_element_set_clock(videobin, gst_element_get_clock(gst_elem_pipeline));
                gst_element_sync_state_with_parent(videobin);
                gst_element_seek_simple(videobin, GST_FORMAT_TIME,
                                (GstSeekFlags)(GST_SEEK_FLAG_FLUSH),
                                position() * GST_MSECOND);
            }
        }
    } else
        m_renderTarget = targetSink;
}

MGstVideo::RenderTarget MGstVideo::renderTarget()
{
    return m_renderTarget;
}

void MGstVideo::expose()
{
    if( gst_elem_xvimagesink && m_renderTarget == XvSink ) {
        gst_x_overlay_expose(GST_X_OVERLAY(gst_elem_xvimagesink));
    }
}

void MGstVideo::setWinId(unsigned long id) 
{
    m_winId = id;
    if( gst_elem_xvimagesink )
        gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(gst_elem_xvimagesink), m_winId);    
}

unsigned long MGstVideo::winId() 
{
    return m_winId;
}

void MGstVideo::setColorKey(const QColor& key) 
{
    m_colorKey = key;

    if( gst_elem_xvimagesink ) {
        g_object_set(gst_elem_xvimagesink,
                     "autopaint-colorkey", FALSE,
                     "colorkey", m_colorKey.rgba() & 0x00FFFFFF,
                     (void*) 0);
    }
}

QColor MGstVideo::colorKey() 
{
    return m_colorKey;
}

void MGstVideo::forceAspectRatio(bool forceAspectRatio)
{
    m_forceAspectRatio = forceAspectRatio;
    if( gst_elem_xvimagesink ) {
        g_object_set(gst_elem_xvimagesink,
                     "force-aspect-ratio", m_forceAspectRatio,
                     (void*) 0);
    }
}


/*
 * Gstreamer message callback for catching general
 * messages and errors in stream.
 */
gboolean MGstVideo::bus_cb(GstBus *bus, GstMessage *message, void *data)
{
    Q_UNUSED(bus);

    MGstVideo* gstVideo = (MGstVideo*) data;

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
            gstVideo->setVideoState(MVideo::Stopped);
        }
    }
    // some error has happened
    else if( GST_MESSAGE_TYPE(message) == GST_MESSAGE_ERROR ) {
        GError *error = NULL;
        gst_message_parse_error(message, &error, NULL);
        mWarning("MGstVideo::bus_cb()") << "ERROR: " << error->message;
        g_error_free(error);

        // stop pipeline, we are in trouble
        return false;
    }
    else if ( GST_MESSAGE_TYPE(message)== GST_MESSAGE_STATE_CHANGED ) {
        if( GST_IS_PIPELINE(message->src) ) {

            GstState oldState, newState, pendingState;
            gst_message_parse_state_changed(message, &oldState, &newState, &pendingState);
            if( oldState == GST_STATE_READY && newState == GST_STATE_PAUSED ) {
                video_ready_cb(data);
            }
        }
    }
    return true;
}

/*
 * Called from msink when the natural size of video is known.
 */
void MGstVideo::video_ready_cb(void* user_data)
{
    if( user_data ) {
        MGstVideo* gstVideo = (MGstVideo*) user_data;

        //check if the video is seekable
        gstVideo->checkSeekable();

        if( gstVideo->m_storedState != MVideo::NotReady ) {
            //restore the stored state information
            gstVideo->seek(gstVideo->m_storedPosition);
            gstVideo->setVideoState(gstVideo->m_storedState);
            gstVideo->m_storedState = MVideo::NotReady;
            gstVideo->m_storedPosition = 0;
        }
        else {
            //automatically stop after buffering the first frame
            gstVideo->setVideoState(MVideo::Stopped);
        }

        emit gstVideo->videoReady();
    }
}

/*
 * Called from decodebin2 when unknown video format is encountered
 */
void MGstVideo::unknownpad_cb(GstElement*/*bin*/,
                                GstPad        */*pad*/,
                                GstCaps       *caps,
                                MGstVideo   */*w*/) 
{
    GstStructure* type;
    type = gst_caps_get_structure(caps, 0);
    QString typeString = gst_structure_get_name(type);
    mWarning("MGstVideo::unknownpad_cb()") << typeString;
}

/*
 * Called from decodebin2 when known video format is encountered and it 
   can be decoded to raw data automatically
 */
void MGstVideo::newpad_cb(GstElement  *decodebin,
                          GstPad      *pad,
                          gboolean    last,
                          MGstVideo *w)
{
    GstPad       *sinkpad;
    GstCaps      *caps;
    GstElement   *sink;
    GstStructure *type;
    GstElement   *bin;

    Q_UNUSED(decodebin);
    Q_UNUSED(last);

    caps = gst_pad_get_caps(pad);
    type = gst_caps_get_structure(caps, 0);

    QString typeString = gst_structure_get_name(type);

    //FIXME
    //Find compatible pads more accurately, add more error checking.
    if (g_strrstr(gst_structure_get_name(type), "video")) {
        bool yuv = false;
        if (g_strrstr(gst_structure_get_name(type), "yuv"))
            yuv = true;

        sink = w->makeSinks(yuv);
        w->m_format = yuv ? YUV : RGB;

        //FIXME:
        //Add ffmpegcolorspace plugin only when necessary
        if ( /*!yuv*//*!pad2*/true) {
            bin = gst_bin_new("video bin");
            GstElement *colorspace = gst_element_factory_make("ffmpegcolorspace", "ffmpegcolorspace");

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
        mWarning("MGstVideo::newpad_cb()") << "error creating sink for " << gst_structure_get_name(type);
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
 * Called from msink to render the current frame in video stream.
 */
void MGstVideo::render_frame_cb(void* pointer, void* user_data)
{
    if( user_data ) {
        //try to lock the mutex, if the we cannot lock it
        //a new frame is currently being rendered so no need 
        //to create new one
        MGstVideo* gstVideo = (MGstVideo*) user_data;
        if( !gstVideo->lockFrameData() ) {
            mWarning("MGstVideo::render_frame_cb()") << "MUTEX LOCK CONFLICT!";
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

GstElement* MGstVideo::makeSinks(bool yuv)
{
    if( gst_elem_videosink || gst_elem_xvimagesink ) {
        mWarning("MGstVideo::makeSink()") << "Sink already initialized. Call destroyPipeline() before initializing new sink.";
        return NULL;
    }

    //try to create xvimagesink, if it fails fallback to msink rendering
    gst_elem_xvimagesink = gst_element_factory_make("xvimagesink", "xvsink");
    if( gst_elem_xvimagesink && GST_IS_ELEMENT(gst_elem_xvimagesink) &&  m_renderTarget == XvSink ) {
        g_object_set(gst_elem_xvimagesink,
                     "autopaint-colorkey", FALSE,
                      "colorkey", m_colorKey.rgba() & 0x00FFFFFF,
                      "force-aspect-ratio", m_forceAspectRatio,
                      "draw-borders", TRUE,
                      (void*) 0);
        gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(gst_elem_xvimagesink), m_winId);                
    }else {
        mWarning("MGstVideo::makeSink()") << "xvimagesink doest not exist, falling back to texture rendering with msink.";
    }

    //setup textured video rendering using msink
    gst_elem_videosink = yuv ? m_gst_video_sink_yuv_new() : m_gst_video_sink_new();
    MGstVideoSink *msink = M_GST_VIDEO_SINK(gst_elem_videosink);
    msink->user_data = this;
    msink->frame_cb = render_frame_cb;
    msink->ready_cb  = video_ready_cb;

    return activeSink();
}

GstElement* MGstVideo::activeSink()
{
    if( m_renderTarget == XvSink &&  gst_elem_xvimagesink ) {
        m_renderTarget = XvSink;
        return gst_elem_xvimagesink;
    }
    else {
        m_renderTarget = MSink;
        return gst_elem_videosink;
    }
}

GstElement* MGstVideo::makeVolume()
{
    gst_elem_volume = gst_element_factory_make("volume", "volume");
    setVolume(m_volume);
    setMuted(m_muted);
    return gst_elem_volume;
}

bool MGstVideo::constructPipeline()
{
    destroyPipeline();

    bool ok = true;

    //create file source element
    gst_elem_source = gst_element_factory_make("filesrc", "file-source");
    if( gst_elem_source && GST_IS_ELEMENT(gst_elem_source) ) {
        g_object_set(G_OBJECT(gst_elem_source), "location", m_filename.toStdString().c_str(), NULL);
    }
    else {
        mWarning("MGstVideo::constructPipeline()") << "filesrc gstreamer plugin not found.";
        ok = false;
    }

    //create decoder element
    gst_elem_decoder = gst_element_factory_make("decodebin2", "decoder");
    if( gst_elem_decoder && GST_IS_ELEMENT(gst_elem_decoder) ) {
        g_signal_connect(gst_elem_decoder, "new-decoded-pad", G_CALLBACK(newpad_cb), this);
        g_signal_connect(gst_elem_decoder, "unknown-type", G_CALLBACK(unknownpad_cb), this);
    }
    else {
        mWarning("MGstVideo::constructPipeline()") << "decodebin2 gstreamer plugin not found.";
        ok = false;
    }

    //check if everything is still ok, if not do some cleanup and stop pipeline creation
    if( !ok ) {
        mWarning("MGstVideo::constructPipeline()") << "Core gstreamer plugin(s) are missing, cannot construct pipeline.";
        gst_object_unref(gst_elem_source);
        gst_object_unref(gst_elem_decoder);
        return false;
    }

    //create pipeline from source and decoder elements
    gst_elem_pipeline = gst_pipeline_new("m-gst-video");
    gst_bin_add_many(GST_BIN(gst_elem_pipeline), gst_elem_source, gst_elem_decoder, NULL);
    if( !gst_element_link_many(gst_elem_source, gst_elem_decoder, NULL) ) {
        mWarning("MGstVideo::constructPipeline()") << "Failed to link elements.";
        destroyPipeline();
        return false;
    }

    //setup callback for pipeline bus messages
    gst_messagebus = gst_pipeline_get_bus(GST_PIPELINE(gst_elem_pipeline));
    gst_bus_add_watch(gst_messagebus, bus_cb, this);
    gst_object_unref(gst_messagebus);

    //start buiffering, this is done to get the first frame of the video and 
    //thus be able to parse video format information
    if( GST_STATE_CHANGE_FAILURE == gst_element_set_state(gst_elem_pipeline, GST_STATE_PAUSED) ) {
        mWarning("MGstVideo::constructPipeline()") << "Failed to start buffering.";
        destroyPipeline();
        return false;
    }

    return true;
}

void MGstVideo::destroyPipeline()
{
    m_mutex.lock();
    //if( gst_elem_videosink ) {
    //    MGstVideoSink* msink = M_GST_VIDEO_SINK(gst_elem_videosink);
    //    msink->user_data = NULL;
    //}

    if( gst_elem_pipeline ) {
        gst_element_set_state(gst_elem_pipeline, GST_STATE_NULL);
        gst_object_unref(GST_OBJECT(gst_elem_pipeline));

        if( activeSink() == gst_elem_xvimagesink ) {
            if( gst_elem_videosink )
                gst_object_unref(GST_OBJECT(gst_elem_videosink));
        }
        else  if( gst_elem_xvimagesink )
            gst_object_unref(GST_OBJECT(gst_elem_xvimagesink));
    }

    //TODO should some of these be destroyed as well?
    gst_elem_pipeline  = NULL;
    gst_elem_decoder   = NULL;
    gst_elem_source    = NULL;
    gst_elem_volume    = NULL;
    gst_elem_videosink = NULL;
    gst_elem_audiosink = NULL;
    gst_elem_xvimagesink = NULL;

    setVideoState(MVideo::NotReady);

    m_mutex.unlock();
}

void MGstVideo::checkSeekable()
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


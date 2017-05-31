/* Autogenerated with kurento-module-creator */

#ifndef __TELMATE_FRAME_GRABBER_OPENCV_IMPL_HPP__
#define __TELMATE_FRAME_GRABBER_OPENCV_IMPL_HPP__

#include <ctime>
#include <iostream>
#include <OpenCVProcess.hpp>
#include "TelmateFrameGrabber.hpp"
#include <EventHandler.hpp>

#include "VideoFrame.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <iostream>

#include <boost/atomic.hpp>

#include <opencv2/core/mat.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include <boost/filesystem.hpp>

#define FGFMT_JPEG  0x0
#define FGFMT_PNG   0x1

#define FG_JPEG_QUALITY 20
#define FG_PNG_QUALITY  9

namespace kurento
{

class TelmateFrameGrabberOpenCVImpl : public virtual OpenCVProcess
{

public:

  TelmateFrameGrabberOpenCVImpl ();

  virtual ~TelmateFrameGrabberOpenCVImpl () ;

  virtual void process (cv::Mat &mat);


  boost::atomic<int> framesCounter;
  int snapInterval;
  std::string storagePath;
  std::string epName;
  int outputFormat; // 0x0=JPEG 0x1=PNG

protected:
    std::shared_ptr<MediaObject> getSharedPtr()
    {
        try {
            return dynamic_cast <MediaObject *> (this)->shared_from_this();
        } catch (...) {
            return std::shared_ptr<MediaObject> ();
        }
    }


private:
    boost::asio::io_service ioService;
    boost::thread_group tp;
    boost::lockfree::queue<VideoFrame*> *frameQueue;
    boost::thread* thr;
    boost::atomic<bool> thrLoop;

    boost::atomic<long> lastQueueTimeStamp;
    boost::atomic<int> queueLength;
    std::string storagePathSubdir;

    void queueHandler();
    std::string getCurrentTimestampString();
    long getCurrentTimestampLong();

    boost::mutex workerThreadMutex;
};

} /* kurento */

#endif /*  __TELMATE_FRAME_GRABBER_OPENCV_IMPL_HPP__ */

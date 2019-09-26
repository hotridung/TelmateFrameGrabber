# TelmateFrameGrabber

## Introduction:

This module implements a Kurento endpoint frames grabber.

The module uses the Kurento OpenCV support and receive reference to a opencv [cv::mat](http://docs.opencv.org/3.1.0/d3/d63/classcv_1_1Mat.html) object containing the frame data and saves it to disk.

An instance of this module is created for each Kurento endpoint, the instance implements a ```process()``` function which is called by the Kurento plugin-core for each frame.
A ```process()``` function creates a VideoFrame object in the heap, performs a deep copy of the Mat object and adds a timestamp.
The VideoFrame object ptr is then pushed into a variable sized ```boost::lockfree::queue```.

Upon plugin initialization a queue handler thread is created with thread main function set to ```queueHandler()```. This function pops VideoFrames pointers from the ```boost::lockfree::queue``` and uses the Mat object data to save a PNG image to the disk.

## Design
The plugin is designed around a lock-free thread-safe producer-consumer pattern
to ensure speed and proper usability under system load and slow IO as the Kurento plugin subsystem is implemented serially. (plugins are actually filters) each video frame through the plugin potentially creating an issue.

## Installing Kurento-Media-server-Dev
All basic Kurento libraries are needed as mentioned in [How to Develop Kurento Modules](http://doc-kurento.readthedocs.io/en/stable/mastering/develop_kurento_modules.html)

Please make sure Boost C++ and OpenCV and all -dev packages are installed.

DISTRO="xenial"
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 5AFA7A83

and then

sudo tee "/etc/apt/sources.list.d/kurento.list" >/dev/null <<EOF
deb [arch=amd64] http://ubuntu.openvidu.io/6.8.1 $DISTRO kms6
EOF

sudo apt-get update
sudo apt-get install kurento-media-server-dev



## Compiling:

To build the Java bindings, Maven is needed.

```
git clone https://github.com/avis/TelmateFrameGrabber
```

```
cd TelmateFrameGrabber/module
mkdir build # a place to build the code in.
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr .. # TO generate the C++ code bindings.
cmake .. -DGENERATE_JAVA_CLIENT_PROJECT=TRUE # To generate the Java bindings.
make
make install
```

If you get an error similar to this
/usr/bin/ld: libkmsmeetrixkurentohelloworldinterface.a(MeetrixKurentoHelloWorldInternal.cpp.o): relocation R_X86_64_32 against `.rodata' can not be used when making a shared object; recompile with -fPIC
libkmsmeetrixkurentohelloworldinterface.a: error adding symbols: Bad value

open CMakeLists.txt file in module directory and locate following line. 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -DHAVE_CONFIG_H -Wall -Werror -std=c++11")

then replace it with following line (we are just adding -fPIC flag here)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC -DHAVE_CONFIG_H -Wall -Werror -std=c++11")

and then re-run command
make install
```

## Testing:

The kurento-one2one-videocall-java includes a modified version of the original proof of concept one2one video call application.
This application - once executed, exposes an http server on port 8443. For testing purposes, the HTTP service this application exposes could be accessed using two different browser tabs on a client machine and provide a simple loopback video call where the two endpoints are really the same video camera and microphone.
The app will start the TelmateFrameGrabber plugin and save PNG snapshots to the Kurento media server /tmp directory.
To compile and execute the app:
```
export KURENTO_SERVER_ADDRESS=192.168.1.1
mvn compile exec:java -Dkms.url=ws://$KURENTO_SERVER_ADDRESS:8888/kurento

```



## Public Methods:

* ```void setSnapInterval(int snapInterval)```: Optional, Recommended, Sets the snapshot interval (in milliseconds). Defaults to 1000ms.

* ```void setStoragePath(const std::string &path)```: Optional, Recommended, Sets the storage path location. Defaults to /tmp.

* ```void setWebRtcEpName(const std::string &epName)```: Optional, Sets the endpoint name. Defaults to NULL.

* ```int getSnapInterval()```: Returns the current configured snapshot interval.

* ```std::string getStoragePath()```: Returns the current configured snapshots path.

* ``` void setOutputFormat(int outputFormat)```: Optional, Recommended, Sets the output format for images (0x0=JPEG 0x1=PNG). Defaults to JPEG.

## Classes:

* ```TelmateFrameGrabberImpl```: Kurento Plugin. This class is an interface between the Kurento plugin core and the actual plugin.

* ```TelmateFrameGrabberOpenCVImpl```: This Class implements the plugin, Interacts with TelmateFrameGrabberImpl.

* ```VideoFrame```: This class is a storage object for video frames pushed into the queue.

## Authors:
Avi Saranga - Telmate.


## Licensing:
MIT License

Copyright (c) 2017 Telmate, LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

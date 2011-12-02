Time Travellers is a real-time video mirror that has been installed at NYU’s Interactive Telecommunications Program and other venues. The Microsoft Kinect is used to take a "depth image" of the viewer and map it to time on a source video. The closer the viewer is to the camera, the later in time is the video.

http://www.sundialtone.com/?p=718

I've included one of the original videos, sunrise.avi from

http://www.flickr.com/photos/twslayer/3800872636/

# To run the program

1.  Install Perian if you don't already have it (you don't need this to run Time Travellers, but you need it to play the included sunrise.avi as Quicktime doesn't come with the required codec),
    
    http://perian.org/

2.  Plug in your Kinect camera.

3.  Run `bin/TimeTravellers.app`
    
    It will take a minute to load the movie. It reads 256 video frames into RAM so as to render smoothly.

4.  If you want to use your own movies, place your own 1280x720 videos in `bin/data/movies` and restart the program.

# To play with the source code

Download openFrameworks and ofTheo's ofxKinect addon,

http://www.openframeworks.cc/

https://github.com/ofTheo/ofxKinect

openFrameworks is constantly changing! This current version of Time Travellers uses OF version 007. Note you will need to use the ofxKinect tagged for version 007.
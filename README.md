# Simple video player
This is a video player I wrote to learn FFMPEG libraries(libav*). It's just plays video using SDL to display.
It has some problems though: a bit of desynchronization with the audio, and also in some places the code wasn't written very well. But I think I achieved my goal of learning ffmpeg with this project. There are very few resources on the internet that can help in learning this library and all of them are outdated. I've also looked into testing with GoogleTest, although the tests are some out of order since due to the new rendering implementation. 
To summarize what I learned and achieved:
- How to use FFMPEG libraries
- How video decoding works
- Tests
- New parts of SDL

## How to compile
This program compiles on Linux and MingW. You will need ffmpeg libraries to compile

First, you need clone this repo and submodules:

`git clone https://github.com/Albrict/video_player.git`

`git submodule update --init --recursive`

After that create build directory and compile:

`mkdir build && cd build`

`cmake .. -DCMAKE_BUILD_TYPE=RELEASE`

`make`

I recommend using -j option when compiling, because it will compile some SDL libraries.

## Program demonstration


https://github.com/Albrict/video_player/assets/65279613/86597f8f-5c3a-48ab-8742-86db923af230


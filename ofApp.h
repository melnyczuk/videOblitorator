#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void vidPlayer();					//  This is a basic mode that just plays any chosen clip with no funny business;

		void vidCutter();					//	See ofApp.cpp;
		void vidFramer();					//	See ofApp.cpp;
		void vidSubber();					//	See ofApp.cpp;
		void vidMorphr();					//	See ofApp.cpp;
		void vidAtomsr();					//	See ofApp.cpp;

		void keyPressed(int key);
		
		//void vidLlacer();					//	A possible future mode (if I can figure out how to make it run efficiently);
		//void vidMosher();					//	A possible future mode that would "datamosh" or read the optical flow of the pixels in a video and distort the pixels of another;
		
		int t, c, scene, cut, fade, atom;
		float g, gA;

		static const int nVideos = 5;		//	The total number of videos that can be used; This must be stated in advance or the program won't run;
		static const int nLines = 5;		//	Number of scan lines for the "vidLlacer" mode (under construction);
		static const int nRandom = 16;		//	Number of random noise offsets used (see "vidMorphr" in ofApp.cpp);
		static const int w = 256;			//	Width of the inputted video files;
		static const int h = 144;			//	Height of the inputted video files;
		static const int totWidth = 1000;	//	Total sketch display width;
		static const int totHeight = 760;	//	Total sketch display height;

		ofVideoPlayer v[nVideos];			//	Video Player class of iteration number nVideos (see above); 

		ofSoundPlayer s[nVideos];			//	Sound Player class of iteration number nVideos (see above);

		ofTexture tex[nVideos],				//	Primary Texture class;
				  ture[nLines];				//	Secondary Texture class;

		float	r[nRandom],					//	Floats for holding values of random noise offsets;
				noiseFactorX[totWidth],		//	Floats for holding values of noise factors X (see "vidAtomsr" in ofApp.cpp);
				noiseFactorY[totHeight];	//	Floats for holding values of noise factors Y (see "vidAtomsr" in ofApp.cpp);

		ofImage drop;						//	Image class for holding a screencapture (see "vidMorphr" & "vidAtomsr" in ofApp.cpp);
};

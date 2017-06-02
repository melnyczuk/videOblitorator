//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// VIDEOBLITORATOR by HPM /////////////////////////////////////////////////////
/////////////////////////////////////////////////////  2017-01-08 -=- v:1.0  /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// THIS PROGRAM TAKES INPUTTED VIDEOS AND EDITS THEM TOGETHER ///////////////////////////////////
///////////////////////////////////////// IN A GENERATIVE AND AT TIMES PSUEDORANDOM WAY. /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// IT IS MY INTENTION TO DEVELOP THIS PROGRAM FURTHER,  //////////////////////////////////////
//////////////////////// WITH PLANS TO WRITE CODE THAT DOES MORE TO ANALYSE THE VIDEOS FILE IT IS GIVEN,  ////////////////////////
/////////////////////////////////// IN A BID TO MAKE IT MORE INTERESTING, CRAZY AND AUTOMATED. ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////  THE CURRENT CLIPS BEING USED IN THIS PROGRAMME IS PUBLICLY HOSTED ON YOUTUBE AT:  ///////////////////////
/////////////////////////////////////////   https://www.youtube.com/watch?v=Jw0Fc-xTlaM  /////////////////////////////////////////
/////////////////////////////////////////   https://www.youtube.com/watch?v=g1VNQGsiP8M  /////////////////////////////////////////
//////////////////////////////////////   https://www.youtube.com/watch?v=3JFEmCKHVME&t=397s  /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);											//	Sets the framerate to 60fps;
	ofDisableArbTex();											//	Disables ARB texture, ensuring openGL uses 2D textures;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
																//	^ Sets the texture drawing style to be 2D, using linear interpolation when scaling up images;

	v[0].loadMovie("alepo.mov");								//	Loads first film clip;
	v[1].loadMovie("evaun.mov");								//	Loads second film clip;;
	v[2].loadMovie("alepo.mov");								//	Loads third film clip;;
	v[3].loadMovie("evart.mov");								//	Loads fourth film clip;;
	v[4].loadMovie("alepo.mov");								//	Loads fifth film clip;;

	s[0].loadSound("alepo-064.mp3");							//	Loads first sound file;
	s[1].loadSound("evaun-128.mp3");							//	Loads second sound file;
	s[2].loadSound("alepo-128.mp3");							//	Loads third sound file;
	s[3].loadSound("evart-128.mp3");							//	Loads fourth sound file;
	s[4].loadSound("alepo-192.mp3");							//	Loads fifth sound file;

	for (int i = 0; i < nLines; i++) {
		ture[i].allocate(w, h / nLines, GL_RGB);				//	Allocates memory for the secondary texture;
	}

	for (int i = 0; i < nVideos; i++) {
		v[i].setVolume(1);										//	Sets the volume for all clips;
		v[i].setPosition(ofRandom(1));							//	Sets the initial position within all clips;

		s[i].setVolume(1);										//	Sets the volume for all sound files;
		s[i].setMultiPlay(true);								//	Prevents sound files being played multiple times;
		s[i].setLoop(true);										//	Sets all sound files to repeat if they pass their end;
		s[i].play();
		s[i].setPosition(ofRandom(1));							//	Sets the initial position within all sound files;
		

		tex[i].allocate(w, h, GL_RGB);							//	Allocates memory for the primary texture;
	}

	for (int i = 0; i < nRandom; i++) {
		r[i] = ofRandom(-0.01, 0.01);							//	Generate the random noise offsets;
	}

	for (int x = 0; x < ofGetWidth(); x++) {
		noiseFactorX[x] = ofRandom(-0.01, 0.01);				//	Generate the noise factors X;
	}
	for (int y = 0; y < ofGetHeight(); y++) {
		noiseFactorY[y] = ofRandom(-0.01, 0.01);				//	Generate the noise factors Y;
	}

	scene = 0;													//	A global variable to store the current mode being shown;
	cut = 0;													//	A global variable to determine chosen video of any given cut;
	fade = 255;													//	A global variable used to control the fade out of videos;
	atom = 0;													//	A global variable for use in the "vidAtomsr" mode;
	t = 0;														//	A global variable to count the time;
	c = 128;													//	A global variable to determined the rate of cuts;
	g = 0;														//	A global variabel to count the time elapsed since beginning of a scene;

}

//--------------------------------------------------------------
void ofApp::update() {

	t++;														//	Increments the counter;
	g += ofMap(gA, 1, 20, 0, 5, true);							//	Provides a smooth and delayed increment of variable g;
	gA += 0.01;													//	Provides a smooth and delayed increment of variable g;

	ofSoundUpdate();
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofBackground(0);											//	Sets the background color;

		 if (scene == 0) { vidCutter(); }						//	Chooses the current mode to draw;
	else if (scene == 1) { vidFramer(); }						//	Chooses the current mode to draw;
	else if (scene == 2) { vidSubber(); }						//	Chooses the current mode to draw;
	else if (scene == 3) { vidMorphr(); }						//	Chooses the current mode to draw;
	else if (scene >= 4) { vidAtomsr(); }						//	Chooses the current mode to draw;

	if (ofGetElapsedTimeMillis() >= 180000) { ofExit(EXIT_SUCCESS); } // Ensures the program ends at 3:00 minutes run time;
}

//--------------------------------------------------------------
void ofApp::vidPlayer() {
	////////////////////////////////////////////////////////////* A BASIC VIDEO PLAYER FOR TESTING VIDEO CLIPS 

	for (int i = 0; i < nVideos; i++) {

		v[i].setAnchorPercent(0.5, 0.5);						//	Draws the clip from coordinates its center;
		v[i].play();											//	Plays the clip;
		v[i].update();											//	Update the clip;
		v[i].draw(ofGetWidth() / 2, ofGetHeight() / 2, 2 * w, 2 * h);	//	Draws the clip at the center of the screen;

	}
}

//--------------------------------------------------------------
void ofApp::vidCutter() {
	////////////////////////////////////////////////////////////*	THIS MODE CUTS CHOOSES RANDOMLY BETWEEN THE LOADED VIDEO CLIPS
	////////////////////////////////////////////////////////////*	AND THEN CUTS THEM TOGETHER AT AN INCREASING RATE 

	for (int i = 0; i < nVideos; i++) {

		v[i].setAnchorPercent(0.5, 0.5);

		if (t == 0) { cut = ofRandom(5); }						//	Chooses a clip at random at the beginning of the programme;

		if (t % c == 0) {										//	Determines when a cut should occur by comparing the counter with c;
			cut = ofRandom(nVideos);							//	Chooses a clip at random at the beginning of the programme;

			v[i].stop();										//	Stops all iterations of videos to try to reduce dataload;
			v[i].setPosition(ofNoise(g * r[i]));				//	Sets the position through all clips in a psuedorandom manner orintated around their initial chosen position;

			//s[i].stop();
			s[i].setPosition(ofRandom(1));

			if (c >= 4) { c -= 2; }								//	Ensures that c decreases, there by speeding up the rate of cuts;
		}

		//s[cut].setPosition(ofNoise(g * r[cut]));
		//s[cut].play();
		v[cut].play();											//	Plays the chosen video;
		v[cut].update();										//	Updayes the chosen video;
		v[cut].draw(0.5 * ofGetWidth(), 0.5 * ofGetHeight(), 2 * w, 2 * h);	//	Draws the chosen video;

	}
	if (c < 8 ) { g = 0; gA = 0; scene++; }						//	If the rate of cuts has gotten too fast, the programme progresses to the next mode;
}

//--------------------------------------------------------------
void ofApp::vidFramer() {
	////////////////////////////////////////////////////////////* THIS MODE CHOSES SINGLE FRAMES FROM THE VIDEO CLIPS AT RANDOM
	////////////////////////////////////////////////////////////* IT IS DESIGNED IN ORDER TO SHOW A FRENZY OF IMAGES WITHOUT SLOWING THE PROGRAMME

	if (t % c == 0) {											//	Determines when a cut should occur by comparing the counter with c then chooses a clip at random;
		cut = ofRandom(nVideos); 
	}

	if (t % (10*c) == 0) {
		//s[cut].play();
		s[cut].setPosition(ofNoise(g * r[cut]));				//	Sets the position through the sound file in a psuedorandom manner orintated around its initial position;
	}

	v[cut].setAnchorPercent(0.5, 0.5);							//	Ensures the clip is drawn from its center;
	v[cut].setPosition(ofNoise(g * r[cut]));					//	Sets the position through the clip in a psuedorandom manner orintated around its initial chosen position;
	//s[cut].setPosition(ofNoise(g * r[cut]));					//	Sets the position through the sound file in a psuedorandom manner orintated around its initial position;
	
	ofImage frame = v[cut].getPixels();							//	A temporary Image class to store the chosen frame for rapid display;
	frame.setAnchorPercent(0.5, 0.5);							//	Ensures the frame is drawn from its center;

	ofSetColor(255, fade);
	frame.draw(0.5 * ofGetWidth(), 0.5 * ofGetHeight(), 2 * w, 2 * h);	
	ofSetColor(255, 255);										//	^ Draws the frame to the screen;

	if (ofGetElapsedTimeMillis() >= 80000) { vidSubber(); fade--; }
																//	^ Begins fading this mode out at 1:20;
	if (fade == 0) { scene++; c = ofRandom(1, 128); }			//	< Resets g and progresses to the next scene at 1:50;												
}

//--------------------------------------------------------------
void ofApp::vidSubber() {
	////////////////////////////////////////////////////////////* THIS MODE CHOOSES A SUBSECTION OF A FRAME OF ANY GIVEN VIDEO,
	////////////////////////////////////////////////////////////* AT RANDOM
	////////////////////////////////////////////////////////////* AND DRAWS IT TO THE SCREEN IN A GLITCHY MANNER

	if (ofGetElapsedTimeMillis() > 110000) { vidMorphr(); }		//	Begins showing the next scene under this scene after 1:50;

	if (t % c == 0) {											//	Determines when a cut should occur by comparing the counter with c;
		cut = ofRandom(nVideos);								//	Chooses a clip at random;
		s[cut].setSpeed(3 * ofNoise(t * r[cut]));					//	Sets the position through the sound file in a psuedorandom manner orintated around its initial position;
	}				

	for (int i = 0; i < nVideos; i++) {
		
		v[i].setPosition(ofNoise(g * r[cut]));					//	Sets the position through the clip in a psuedorandom manner orintated around its initial chosen position;
		tex[i].loadData(v[i].getPixels());						//	Loads the pixel data of all videos into their corresponding texture;

		//s[i].play();
		s[i].setPosition(ofNoise(g * r[cut]));
	}
	
	tex[cut].drawSubsection(ofGetWidth() * ofRandom(1), ofGetHeight() * ofRandom(1), ofGetWidth() * ofRandom(-2, 2), ofGetHeight() * ofRandom(-2, 2), w, h);
																//	^ Draws the randomly desired subsection to the screen;
	if (ofGetElapsedTimeMillis() >= 120000) { scene++; }		//	Advances the mode of the programme at 2:00;
	g = 0; gA = 0;
}

//--------------------------------------------------------------
void ofApp::vidMorphr() {
	////////////////////////////////////////////////////////////*	THIS MODE DEFORMS THE VIDEO CLIPS
	////////////////////////////////////////////////////////////*	AND DRAWS THEM TO THE SCREEN IN AN UNDULATING,
	////////////////////////////////////////////////////////////*	MORPHING SHAPE.
	////////////////////////////////////////////////////////////*	IT WAS INSPIRED BY ROSA MENKMAN's THEORIES OF WHAT SHE TERMS 'RESOLUTION STUDIES'.

	ofPoint m00, m01, m02, m03, m04, m05, m06, m07;				//	A set of points for drawing the shape;

	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);			//	Translate to the center of the screen;
	ofTranslate(-w, -h);										//	Rranslate to correct place to draw the video, the top left corner of the video;

	float x = 2 * w;											//	Local variable for doing easy calculations on the X axis;
	float y = 2 * h;											//	Local variable for doing easy calculations on the Y axis;

	////////////////////////////////////////////////////////////*	The code below determines the oscilating corners of the morphin shapes;
	m00.set(0 + ofGetWidth() * ofSignedNoise(g *  r[0]), 0 + ofGetHeight() * ofSignedNoise(g *  r[1]));
	m01.set(x + ofGetWidth() * ofSignedNoise(g *  r[2]), 0 + ofGetHeight() * ofSignedNoise(g *  r[3]));
	m02.set(x + ofGetWidth() * ofSignedNoise(g *  r[4]), y + ofGetHeight() * ofSignedNoise(g *  r[5]));
	m03.set(0 + ofGetWidth() * ofSignedNoise(g *  r[6]), y + ofGetHeight() * ofSignedNoise(g *  r[7]));
	m04.set(0 + ofGetWidth() * ofSignedNoise(g *  r[8]), 0 + ofGetHeight() * ofSignedNoise(g *  r[9]));
	m05.set(x + ofGetWidth() * ofSignedNoise(g * r[10]), 0 + ofGetHeight() * ofSignedNoise(g * r[11]));
	m06.set(x + ofGetWidth() * ofSignedNoise(g * r[12]), y + ofGetHeight() * ofSignedNoise(g * r[13]));
	m07.set(0 + ofGetWidth() * ofSignedNoise(g * r[14]), y + ofGetHeight() * ofSignedNoise(g * r[15]));

	for (int i = 0; i < nVideos; i++) {
		v[i].setAnchorPercent(0, 0);							//	Ensures all the videos are drawn from the top left corner;

		if (gA == 0 || t % c == 0) {							//	Determines when a cut should happen;
			cut = ofRandom(nVideos);							//	Chooses a video at random;
			v[i].setFrame(ofRandom(v[i].getTotalNumFrames()));	//	Chooses the position of the videos at random;
			s[i].setPosition(ofNoise(g * r[i]));				//	Sets the position through the sound file in a psuedorandom manner orintated around its initial position;
		}

		tex[i].loadData(v[i].getPixels());						//	Loads the videos into the corresponding texture;

	}

	v[!cut].stop();												//	Stops all clips except the chosen one to try to reduce dataload;
	v[cut].play();												//	Plays the chosen clip;
	v[cut].update();											//	Updates the chosen clip;
	s[cut].setSpeed(2 * ofNoise(t * r[cut]));
	s[cut].setPan(ofSignedNoise(t * r[cut]));

	////////////////////////////////////////////////////////////*	The code below determines how the morphing shape should be drawn;
	////////////////////////////////////////////////////////////*	The shape is made up of six draw 'sides' hence the six instances of drawing the texture;
	tex[cut].draw(m00, m02, m04, m06);
	tex[cut].draw(m01, m03, m05, m00);
	tex[cut].draw(m02, m04, m05, m03);
	tex[cut].draw(m01, m06, m07, m02);
	tex[cut].draw(m03, m06, m05, m04);
	tex[cut].draw(m00, m01, m02, m03);

	////////////////////////////////////////////////////////////*	At 2:40, a screengrab is taken and the programme progresses to the next mode;
	if (ofGetElapsedTimeMillis() >= 160000 && t % c == 0) { drop.grabScreen(0, 0, ofGetWidth(), ofGetHeight()); scene = 4; }

}

//--------------------------------------------------------------
void ofApp::vidAtomsr() {
	////////////////////////////////////////////////////////////*	THIS MODE USES A SCREEN CAPTURE OF WHAT IS BEING OUTPUTTED, 
	////////////////////////////////////////////////////////////*	AND THEN DISINTIGRATES THE IMAGE,
	////////////////////////////////////////////////////////////*	ATOMISING IT AND DISPERSING IT OUT OF THE FRAME
	////////////////////////////////////////////////////////////*
	////////////////////////////////////////////////////////////*	AT THE TIME OF SUBMISSION, THIS IS MODE RUNS PRETTY BADLY, BUT STILL SHOWS THE DESIRED EFFECT
	////////////////////////////////////////////////////////////*	TO IMPROVE THIS MODE I BELIEVE I WOULD NEED TO CREATE A SHADER, WHICH I FULLY INTEND TO DO.

	for (int i = 0; i < nVideos; i++) {
		v[i].setVolume(1 / (1 + atom * abs(r[i])));
		v[i].setSpeed(1 / (1 + 10 * atom * abs(r[i])));
		s[i].setSpeed(1 / (1 + 10 * atom * abs(r[i])));
	}

	for (int y = 0; y < ofGetHeight(); y++) {

		glBegin(GL_POINTS);										//	Draws points;

		for (int x = 0; x < ofGetWidth(); x++) {				//	The nested for loop cycles through every pixel in the screengrab;
			ofColor c;											//	Temporary color;

			/////////////////////////////////////////////////////*	The code below determines that if a pixel leaves the screen it turns black;
			if (y + (atom * h * noiseFactorY[y]) > ofGetHeight()
				|| y + (atom * h * noiseFactorY[y]) < 0){
				//|| x + (atom * w * noiseFactorX[x]) > ofGetWidth() 
				//|| x + (atom * w * noiseFactorX[x] < 0)) {
				c = (0);
			}
			else {
				c = drop.getColor(x, y);						//	Takes the color of the pixel;
			}

			ofSetColor(c);										//	Sets the color to the corresponding color in the screengrab image;
			glVertex2f(x + (atom * w * noiseFactorX[x]), y + (atom * h * noiseFactorY[y]));	//	Draws the pixel to the screen, moving it as the image is atomised;

			noiseFactorY[y] = ofRandom(-0.01, 0.01) * atom;		//	Set the noise factor for the X coordinate;
			noiseFactorX[x] = ofRandom(-0.01, 0.01) * atom;		//	Set the noise factor for the Y coordinate;
		}

		glEnd();												//	Ends the draw mode;								

	}

	atom+=2;													// Determines the rate at which the image is atomised;

}

////////////////////////////////////////////////////////////*  FIN  *////////////////////////////////////////////////////////////
/*	THE CODE BELOW IS AN ADDITIONAL MODE, THAT AT THE TIME OF SUBMISSION DOES NOT RUN EFFICIENTLY DUE TO THE HIGH DATALOAD :(
//--------------------------------------------------------------
void ofApp::vidLlacer() {
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	ofTranslate(-2 * w, -2 * h);

	for (int y = 0; y < nLines; y++) {

		for (int i = 0; i < nVideos; i++) {

			if (t == 0 || t % c == 0) {
				v[i].setPosition(ofNoise(g * r[i]));
				v[i].setAnchorPercent(4 * w, 4 * y * h / nLines);
			}

			if (y % nLines == i) { ture[y].loadData(v[i].getPixels()); }
			v[i].update();
		}

		ture[y].drawSubsection(0, 4 * y * (h / nLines), 4 * w, 4 * (h / nLines), 0, 4 * y * (h / nLines));

	}
	ofPopMatrix();
	if (ofGetElapsedTimeMillis() >= 60000) { vidSubber(); }
	if (ofGetElapsedTimeMillis() >= 70000) { g = 0; gA = 0; scene++; c = ofRandom(128); }

}*/ 
//// THE CODE ABOVE IS AN ADDITIONAL MODE THAT AT THE TIME OF SUBMISSION DOESN'T RUN EFFICIENTLY DUE TO THE HIGH DATALOAD :( ////

void ofApp::keyPressed(int key) {
	if (key == 'a') {
		ofResetElapsedTimeCounter();
	}
}
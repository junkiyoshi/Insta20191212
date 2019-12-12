#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(39);

	for (int i = 0; i < 2; i++) {

		this->noise_seed_list.push_back(glm::vec2(ofRandom(1000), ofRandom(1000)));
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	for (auto& noise_seed : this->noise_seed_list) {

		auto location = glm::vec2(ofMap(ofNoise(noise_seed.x, ofGetFrameNum() * 0.003), 0, 1, 100, ofGetWidth() - 100), ofMap(ofNoise(noise_seed.y, ofGetFrameNum() * 0.003), 0, 1, 100, ofGetHeight() - 100));
		auto next = glm::vec2(ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + 1) * 0.003), 0, 1, 100, ofGetWidth() - 100), ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + 1) * 0.003), 0, 1, 100, ofGetHeight() - 100));
		auto distance = location - next;
		distance *= 2;

		auto future = location + distance * 30;
		auto random_deg = ofRandom(360);
		future += glm::vec2(30 * cos(random_deg * DEG_TO_RAD), 30 * sin(random_deg * DEG_TO_RAD));
		auto future_distance = future - location;

		this->location_list.push_back(location);
		this->velocity_list.push_back(glm::normalize(future_distance) * glm::length(distance));
	}

	for (int i = this->location_list.size() - 1; i > -1; i--) {

		this->location_list[i] += this->velocity_list[i];
		this->velocity_list[i] *= 1.01;

		if (glm::distance(glm::vec2(ofGetWidth() * 0.5, ofGetHeight() * 0.5), this->location_list[i]) > 720) {

			this->location_list.erase(this->location_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (auto& location : this->location_list) {

		ofDrawCircle(location, 5);
		for (auto& other : this->location_list) {

			if (location == other) continue;
			
			if (glm::distance(location, other) < 50) {

				ofDrawLine(location, other);
			}
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}

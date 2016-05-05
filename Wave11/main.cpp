#include <iostream>
#include "RiffWavWriter.h"
#include "RiffWavReader.h"
#include "WaveGenerator.h"

using namespace std;

int main(int argc, char** argv)
{
	const int SamplesCount = 44100;
	short buffer[SamplesCount];

	RiffWavReader* rr = new RiffWavReader();
	if (rr->open(_T("sawsample.wav"))) {
		if (rr->prepare()) {
			cout << rr->getChannels() << endl;
			cout << rr->getSamplesPerSec() << endl;
		}
	}

	RiffWavWriter* rw = new RiffWavWriter(16, 1, 44100);

	WaveGenerator* wg = new WaveGenerator(440, 44100);
	for (int i = 0;i < SamplesCount; i++) {
		buffer[i] = wg->SawSample16();
	}

	if (rw->open(_T("sample.wav"))) {
		if (rw->prepare()) {
			const size_t length = sizeof(buffer);

			for (int i = 0; i < 3; i++) {
				size_t result = rw->writeBytes(buffer, length);
				if (result < length) {
					cout << "write error" << endl;
				}
			}

			if (!rw->riffFinalize()) {
				cout << "write error" << endl;
			}
		}
	}

	delete wg;
	delete rr;
	delete rw;

	return 0;
}

# read subdirs from Makefile.subdirs and regenerate wirh 'make subdirs'
SUBDIRS = Amplify \
AmplifySplit \
Analyse-Data \
Analyse-Spect2D \
Analyse-Spect3D \
Analyse-Stereo \
ChannelJoin \
ChannelSplit \
ChorusPhaser \
ConvertChannels \
Convolve \
Crackle \
CrossTalk \
DeCrackle \
DeNoise-FFT \
DeNoise-FIR \
Delay \
DelayPlus \
Detune \
Distortion \
Duplicate \
Dynamic \
Echo \
Equalize-3Band \
Equalize-FFT \
Equalize-FFT-3D \
Filter-CRSHiPass \
Filter-CRSLowPass \
Filter-FIRBandPass \
Filter-FIRBandStop \
Filter-FIRHiPass \
Filter-FIRLowPass \
Filter-FIRMatrix \
Filter-FIRMutate \
Filter-StateVariable \
Fold \
Gamma \
Hall \
Invert \
Logic \
Middle \
Mix \
Mix-3D \
Morph-FFT \
MultiDelay \
Noise \
NoiseGate \
Panorama-2Ch \
Panorama-4Ch \
PitchShift \
QuantizeHoriz \
QuantizeVert \
Resample \
Reverse \
SampleJoin \
SampleSplit \
Shorten \
Slide \
Smear \
Subtract \
SurroundEncoder \
Swap \
Synthesize-Add \
Synthesize-FM \
TimeStretch \
Vocode-FFT \
ZeroPass


all:
	@for dir in $(SUBDIRS); do\
		(cd $$dir; $(MAKE) -$(MAKEFLAGS)) || break;\
	done

release:
	@rm _VERSIONS.*/*.sfxmod*
	@for dir in $(SUBDIRS); do\
		(cd $$dir; $(MAKE) -$(MAKEFLAGS) release) || break;\
	done

debug:
	@for dir in $(SUBDIRS); do\
		(cd $$dir; $(MAKE) -$(MAKEFLAGS) debug) || break;\
	done

docs:
	@for dir in $(SUBDIRS); do\
		(cd $$dir; $(MAKE) -$(MAKEFLAGS) docs) || break;\
	done

subdirs:
	@rm -f Makefile.subdirs
	find . -type d -maxdepth 1 -regex "\.\/[^\_].*" -printf "%f \\\\\n" | sort >Makefile.subdirs

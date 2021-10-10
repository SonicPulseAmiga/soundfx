/******************************************************************************
** SFX-Periodes.h                                                            **
**---------------------------------------------------------------------------**
** SFX Periodentabelle                                                       **
**---------------------------------------------------------------------------**
** Version : V 4.0                                                           **
** Date    : 19.08.2002                                                      **
** Author  : Stefan Kost                                                     **
******************************************************************************/

// from one to the other octave the frequency doubles
// reference A-2 440 Hz

/*-- PeriodTable --------------------------------------------------------------------------------*/

struct pt
{
	unsigned long rate;
	char key[4];
	double frq;
} PerTab[]= {
/*
	{ ,"D-5",2349,318143 },
	{ ,"C#5",2217.461048 },
	{ 67120,"C-5",2093.004522 },
*/
	{ 63353,"H-4",1975.533205 },
	{ 59797,"A#4",1864.655046 },
	{ 56441,"A-4",1760.000000 },
	{ 53273,"G#4",1661.218790 },
	{ 50283,"G-4",1567.981744 },
	{ 47461,"F#4",1479.977691 },
	{ 44797,"F-4",1396.912926 },
	{ 42283,"E-4",1318.510228 },
	{ 39910,"D#4",1244.507935 },
	{ 37670,"D-4",1174.659072 },
	{ 35556,"C#4",1108.730524 },
	{ 33560,"C-4",1046.502261 },
	{ 31676,"H-3",987.7666025 },
	{ 29899,"A#3",932.3275230 },
	{ 28220,"A-3",880.0000000 },
	{ 26637,"G#3",830.6093952 },
	{ 25142,"G-3",783.9908720 },
	{ 23731,"F#3",739.9888454 },
	{ 22399,"F-3",698.4564629 },
	{ 21141,"E-3",659.2551138 },
	{ 19955,"D#3",622.2539674 },
	{ 18835,"D-3",587.3295358 },
	{ 17778,"C#3",554.3652620 },
	{ 16780,"C-3",523.2511306 },
	{ 15838,"H-2",493.8833013 },
	{ 14949,"A#2",466.1637615 },
	{ 14110,"A-2",440.0000000 },
	{ 13318,"G#2",415.3046976 },
	{ 12571,"G-2",391.9954360 },
	{ 11865,"F#2",369.9944227 },
	{ 11199,"F-2",349.2282314 },
	{ 10571,"E-2",329.6275569 },
	{  9977,"D#2",311.1269837 },
	{  9417,"D-2",293.6647679 },
	{  8889,"C#2",277.1826310 },
	{  8390,"C-2",261.6255653 },
	{  7919,"H-1",246.9416506 },
	{  7475,"A#1",233.0818808 },
	{  7055,"A-1",220.0000000 },
	{  6659,"G#1",207.6523488 },
	{  6285,"G-1",195.9977180 },
	{  5933,"F#1",184.9972114 },
	{  5600,"F-1",174.6141157 },
	{  5285,"E-1",164.8137785 },
	{  4989,"D#1",155.5634919 },
	{  4709,"D-1",146.8323840 },
	{  4444,"C#1",138.5913155 },
	{  4195,"C-1",130.8127827 },
	{  3960,"H-0",123.4708253 },
	{  3737,"A#0",116.5409404 },
	{  3528,"A-0",110.0000000 },
	{  3330,"G#0",103.8261744 },
	{  3143,"G-0",97.99885900 },
	{  2966,"F#0",92.49860568 },
	{  2800,"F-0",87.30705786 },
	{  2643,"E-0",82.40688923 },
	{  2494,"D#0",77.78174593 },
	{  2354,"D-0",73.41619198 },
	{  2222,"C#0",69.29565774 },
	{  2098,"C-0",65.40639132 }
};
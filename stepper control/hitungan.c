

//This file contains some calculation to ccontrolling stepper via TMCM-1110

//Microstep Resolution
//2^Usrs
//Usrs = 0 --> full step
//Usrs = 1 --> half step
//Usrs = 2 --> 4 Ustep
//Usrs = 3 --> 8 Ustep
//Usrs = 4 --> 16 Ustep
//Usrs = 5 --> 32 Ustep
//Usrs = 6 --> 64 Ustep
//Usrs = 7 --> 128 Ustep
//Usrs = 8 --> 256 Ustep

//Microstep frequency (Hz)
void UstepFreq(int velocity)
{
	int p_div; //set 0-13, default 0
	long Usf;
	Usf = (16000000*velocity)/((2^p_div)*2048*32);
	return Usf;
}
int Usrs;
long fullstep = Usf/(2^Usrs);

long RPM;
RPM = (fullstep*60)/72;

void main()
{
	printf("try calculate");
	printf
}
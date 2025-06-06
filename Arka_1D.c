#include <math.h>
#include <stdio.h>

//gamma
float agamma = 1.4;

// finds temperature given pressures and initial temperature after an adiabatic compression/expansion
float adiabaticT(float p2, float p1, float t1){ 
	return ( t1 * pow((p2/p1),((agamma-1)/agamma)));
}

//mass flow rate of air
float ma;

//ambient pressure, bar
float pa = 101325.0;

//ambient temperature, kelvin
float ta = 300.0;

//efficiency of inlet, dimensionless
float nd;

//mach number of engine, dimensionless
float m0;

// pressure ratio of compressor, dimensionless
float piC = 3.5;

// efficiency of compressor, dimensionless
float nC = 0.75;

//temperature (total) at entrance of turbine, kelvin
float t04 = 1000;

// efficiency of burner, dimensionless
float nb = 0.9;

//energy per unit mass of fuel, J/kg
float Qr = 42.6 * pow(10,6);

//Cp
float Cp = 1005;

//mechanical efficiency of shaft
float nm = 0.95;

//efficiency of turbine
float nt = 0.8;

//efficiency of nozzle
float nn = 0.95;

//speed of sound
float a;

//loss across combustor (1-epsilon*gamma*0.5*mb*mb)
float piB = 0.92;

//pressure ratio of fan
float piF = 1.2;

//efficiency of fan
float nF = 0.85;

//assumed: piC, mdot(air), V0
//assumed: mach numbers in combustor and inlet, t04, constants
//assumed efficiency: nt, nc, n(mechanical), epsilon(combustor) 
//
//find p02 from 4.5, use adiabatic inlet to find t02, this is equal to t01 p01
//piC is known so find p03, use 4.12 to find t03s then find t03 with n, for ideal keep n as 1
//use 4.26 to approximate p04 or p04 is equal to p03 ideally, t04 is design parameter, for 4th station just use energy balance (4.32) to find f
//use work relation between compressor and turbine to find t05 (1+f)(t04-t05)*nt = (T03-T02), use isentropic relation to find t05s then consequently piT and p05
//use nozzle relation to find ve
//use ve to find corresponding thrust
void main(){

//these are for diffuser
//float p01 = pa*pow((1+nd*(agamma-1)*0.5*m0*m0),(agamma/(agamma-1)));
//float t01 = adiabaticT(p01, pa, ta);

//if no diffuser use this code block
	float p01 = pa;
	float t01 = ta;


	float p02 = p01 * piF;
	float t02s = adiabaticT(p02, p01, t01);
	float t02 = t01 + ((t02s-t01)/nF);
// first assumed value used
	float p03 = piC * p02;
	float t03s = adiabaticT(p03, p02, t02);
	float t03 = t02 + ((t03s-t02)/nC);
	float p04 = p03 * piB;
	float f = (t04-t03)/( Qr*nb/Cp - t04);
	float t05 = t04 - ( (t03-t01)/((1+f)*nm) );
	float t05s = t04 - (t04-t05)/nt;
	float p05 = p04*pow((t05s/t04),(agamma/(agamma-1)));
	float ve = sqrt(nn*2*Cp*t05*(1-(pow((pa/p05),((agamma-1)/agamma)))));
//ma used
//this is for air coming through (wind tunnel)
//	F = ma*(1+f)*ve - ma*m0*a;


//this is for if no air is coming (compressor is pushing air only)
	ma = 200/((1+f)*ve);
	printf("ma %f \np01 %f t01 %f \np02 %f t02 %f \np03 %f t03 %f \np04 %f t04 %f \np05 %f t05 %f \nve %f", ma, p01, t01, p02, t02, p03, t03, p04, t04, p05, t05, ve);
}

//if you plug in values for all the things given above main, you get the thrust, loop it after first assumed value incrementing ma and piC until you get the optimal thrust, but first double check all the formulas

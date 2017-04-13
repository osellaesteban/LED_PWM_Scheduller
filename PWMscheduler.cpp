#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>

#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <fstream>

using namespace std;

// LED Pin - wiringPi pin 0 is BCM_GPIO 17.

#define LED0     //8
#define LED1	 //9
#define LED2	 //7
#define LED3	 //1
int LEDS [] = {0,1,2,3,4,5,6,7};//,8,9,10,11};
int RANGE = 100;

int checkOtherPWM(){
	int other = 0;
	FILE *fp;
	char path[2048];
	fp = popen("/bin/ps -A", "r");
	if (fp == NULL) 
	{
		printf("Failed to run command\n" );
		exit(1);
	}
	std::string linea,npid;
	std::size_t found,ofsend;
	bool second = false;
	bool only = true;
	fgets(path, sizeof(path)-1, fp);
	linea = path;
	npid = "";
	ofsend = linea.find("TTY",0)-1; // gives the possition where the PID number ends
  	while (fgets(path, sizeof(path)-1, fp) != NULL) 
	{
		linea = path;
		found = linea.find("PWMsched");
		if (found !=std::string::npos)
		{			
			if (second == false)
			{
				npid = linea.substr(0,ofsend);
				second = true;
			}
			else
			{
				only = false;
			}
		}
	} 
	if (second && (only == false))
	{
		other = 1;
		cout << "Matando pid: "<< npid << endl;
		std:string command = "sudo /bin/kill "+ npid;
		popen(command.c_str(),"r");
	}
	pclose(fp);
return other;
}

int main (int argc, char * argv[])
{
	cout << "Iniciando aplicacion ..." << endl;  
	wiringPiSetup () ;
	unsigned int corrpos;
	printf("Chequeando instancias previas ...  \n");
	int other = checkOtherPWM();
	switch  (other)
	{	
		case 0:
			cout << "Sin otras instancias del programa corriendo ..." << endl;
			break;
		case 1:
			cout << "Otras instancias del programa se encontraban en ejecución --> se mataron los procesos" << endl;
			break;
	}
	if (argc > 10)
	{
		cout << "Demasiadas salidas ingresadas. Solo se tomarán las primeras 8." << endl;
 	}
	//Archivo para los registros
	ofstream salida;
	string fecha;
  	unsigned int count = 0;
	string val;
	int ival = 0;
	int pwmvals[] = {0, 0, 0, 0, 0, 0, 0, 0 };
	FILE *fp;
	char dt[32];
	fp = popen("/bin/date +%Y-%m-%d:%H:%M:%S","r");
	fgets(dt,sizeof(dt)-1,fp);	
	fecha  = dt;
	pclose(fp); 
	fecha.erase(fecha.end()-1,fecha.end());
	fecha ="/home/pi/PWMscheduler/registry/"+ fecha+"_reg.txt";

	cout << "Abriendo archivo uno: "<< fecha<< endl;

	salida.open(fecha.c_str(),std::fstream::out);
 	salida << "Inicio del bloque: " << dt << endl;

	for (unsigned int k = 0;k<8; k++)
	{
		val = "10";
		if (k<argc-1)
		{
			val = argv[k+1];
		}
		ival = atoi(val.c_str());
		if (ival>RANGE)
		  ival = RANGE;
		if (ival <0)
		  ival = 0;
		
		switch(k)
		{
			case 0: 
				corrpos = 0;
				break;
			case 1:
				corrpos = 7;
				break;
			case 2:
				corrpos = 1;
				break;
			case 3:
				corrpos = 4;
				break;
			case 4:
				corrpos = 5;
				break;
			case 5:
				corrpos = 6;
				break;
			case 6:
				corrpos = 3;
				break;
			case 7:
				corrpos = 2 ;
				break;
		}
		pwmvals[corrpos] = ival;
		softPwmCreate(LEDS[corrpos],pwmvals[corrpos],RANGE);
		std::cout << "PWM value "<< k <<": "<< pwmvals[corrpos]<< endl;
		softPwmWrite (LEDS[corrpos], pwmvals[corrpos]) ;
		salida << "LED Nro: " << k-1 <<  "; valor: " << ival << endl;
	}
	salida.close();
	// Registros regulares cada 300 segundos
	for (;;)
	{
		sleep(300);
        	salida.open(fecha.c_str(),std::fstream::out |std::fstream::app);
		fp = popen("/bin/date +%Y_%m_%d_%H:%M:%S","r");
		fgets(dt,sizeof(dt)-1,fp);    
		fecha  = dt;
		salida << "\n datos en " << fecha <<  "\n" ;
		for (unsigned int k = 1;k < min(argc,9);k++)
		{
		        salida << "LED Nro: " << k-1 <<  "; valor: " << argv[k]<< endl;//pwmvals[k-1] << endl;
			cout << "LED Nro: " << k-1 << "; valor: "<< argv[k]<< endl;//pwmvals[k-1] << endl;
		}
	        salida.close();
	}
	return 0 ;
}



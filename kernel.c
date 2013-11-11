/* kernel.c
   A simple Kernel that displays a message.
   Ashwin Chandrasekhar
*/

void printString(char* String);             // Prints a string in the screen
void readString(char* String);              // Reads a string from the input in the keyboard
void readSector(char* buffer, int sector);  // Reads a sector of the image in the disk.
void handleInterrupt21(int ax, int bx, int cx, int dx); // For the execution of interrupt21
int mod(int a, int b);       // For Calculating mod function
int div(int a, int b);       // For calculating Div function


//<------------------------------------  Main Function-------------------------------------->

int main() {
	char line[80];
    char buffer[512];
	printString("Hello World\r\n\0");   // related to step 1    
	printString("Enter a line: \0");    
	readString(line);                   // related to step 2
	printString(line);                  // related to step 2
	readSector(buffer, 30);             // related to step 3
	printString(buffer);                // related to step 3
	makeInterrupt21();                  // related to step 4
	interrupt(0x21, 1, line, 0, 0);     // related to step 5
	interrupt(0x21, 0, line, 0, 0);     // related to step 5
 	while(1) {
 		//todo
 	}
	return 0;
} //<----------------------------------- Main ends------------------------------------------->


//-------------------------------------readString Function---------------------------------->
void readString(char* String) {
	int index = 0;
	char c = 0;

	while(1) {
		c = interrupt(0x16, 0, 0, 0, 0);          // Interrupt for Accepting from keyboard,
		interrupt(0x10, 0xE * 256 + c, 0, 0, 0);  // Interrupt for printing on screen.
		if(c == 0xD) {                            // if ENTER
			String[index] = '\n';
		    String[index + 1] = '0';
			break;
		}
			
		if(c == 0x8) {                            // if BACKSPACE
			if(index > 0) {
				index = index -1; 	
			}		
    	}

		String[index] = c;
		++index;		
	}
}//-------------------------------------readString ends------------------------------------->


//-------------------------------------readSector Function---------------------------------->
void readSector(char* buffer, int sector) {
	int rel_sector = mod(sector, 18) + 1;   
	int head = mod(div(sector, 18), 2);
	int track = div(sector, 36);

	int cx = track * 256 + rel_sector;
	int dx = head * 256 + 0;
	int ax = 2 * 256 + 1;

	int interrupt(0x13, ax, buffer, cx, dx);         // interrupt for reading a sector.

}//-------------------------------------readSector Ends------------------------------------->


//-------------------------------------Modulo Function-------------------------------------->
int mod(int a, int b) {
	while(a >= b)
		a = a - b;
	return a;
}//-------------------------------------Modulo ends----------------------------------------->


//-------------------------------------Division Function------------------------------------>
int div(int a, int b) {
	int q = 0;
	while((q*b) < a)
		q = q + 1;
	return q;
}//-------------------------------------Division Ends---------------------------------------->


//-------------------------------------printString Function---------------------------------->
void printString(char* String) {
	int i = 0;
	char c = String[i];
	while (c != '0') {
		interrupt(0x10, 0xE * 256 + c, 0, 0, 0);          //interrupt for printing on the screen.
		c = String[++i];
	}
}//-------------------------------------printString Ends------------------------------------->


//-------------------------------------handleInterrupt21 Function---------------------------->
void handleInterrupt21(int ax, int bx, int cx, int dx) {
	printString("Hello World\r\n\0");
	switch(ax) {
		case 0:
		   printString((char*) bx);
		   break;
		case 1:
		     readString((char*) bx);
		     break;
		case 2:
		      readSector((char*) bx, cx);
		      break;
		default:
		      printString("ERROR! Enter again...");
		      break;
	}
  }//-------------------------------------handleInterrupt21 ends------------------------------->
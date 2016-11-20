/*
Jiancheng Wu
CS368 Computer Graphics
Project2-Color Mixer
*/

#include<gl/glut.h>
#include<string>
using namespace std;

GLint vert[1000]; //trace cursor 

int gravity = 40; //size of gravity field
float r , g , b; //float points for each slider
int low = 100, high = 385; //length of slider bar
int c1 =1, c2 =1, c3 =1; //color of slider
float pixelColor[3] = { 0.0 }; //read value from rectangle
float leftColor[3] = { 0.0 }; //color for the life side of rectangle
float rightColor[3] = { 0.0 }; //color for the right side of rectangle

int width = 800, height = 600, n = 0, mx = -1, my = -1; //side of window and position of cursor

/*Slider*/
int one[4][2] = { { 100,227 },{ 100,257 },{ 150,257 },{ 150,227 } };
int two[4][2] = { { 200,227 },{ 200,257 },{ 250,257 },{ 250,227 } };
int three[4][2] = { { 300,227 },{ 300,257 },{ 350,257 },{ 350,227 } };

//Square and rectangle for displaying color
int rectangle[4][2] = { {450,100},{450,150},{650,150},{650,100} };
int square[4][2] = { {450,300},{450,500},{650,500},{650,300} };

/*Boolean values for check status*/
bool rubberbanding1 = false, atOne = false, atTwo = false,atThree = false, 
rubberbanding2 = false, rubberbanding3 = false,grav1 = false, grav2 = false, grav3 = false;

/*This function will check whether the position
of cursor is in the range of slider or not*/
bool checkRange(int posi, int x, int y) {
	return (posi <= y && posi >= x) ? 1 : 0;
}

/*this function will check whether cursor is inside the 
gravity field or not*/
bool checkGravity(int posx, int x, int y, int posy, int xx, int yy) {
	return ((posx<x && posx>y) && (posy<xx&& posy>yy))? 1 : 0;
}

/*This function will be called when we click slider
and move the cursor around, it will move the silder 
when the action is active. We only change its y axis*/
void move(int a[][2], int x, int y) {
	if (a[0][1] + x - y >= 100 && a[0][1] + x - y <= high - 30) {
		a[0][1] = a[0][1] + x - y;
		a[1][1] = a[1][1] + x - y;
		a[2][1] = a[2][1] + x - y;
		a[3][1] = a[3][1] + x - y;
	}
}

/*This function will be called when user click the 
field around the slider bar, that slider will move
to the position*/
void moveLine(int a[][2], int x, int y) {
	a[0][1] = x - y;
	a[1][1] = x + y;
	a[2][1] = x + y;
	a[3][1] = x - y;
}

/*This function will print out values of each RGB
and display them*/
void print(float x,int y){	
	string c = to_string((int)(x*255)); //convert float to integer and then string
	for (int i = 0; i < c.length(); i++)
	{
		//print the string
		glRasterPos2f(y+=10, high+10);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c[i]);
	}
};

/*Display everything to the screen*/
void display() {	
	glClear(GL_COLOR_BUFFER_BIT);

	//draw 3 slider bar
	glBegin(GL_LINES);
	glColor3f(0, 1, 0);
	glVertex2i(125, low);
	glVertex2i(125, high);

	glVertex2i(225, low);
	glVertex2i(225, high);

	glVertex2i(325, low);
	glVertex2i(325, high);
	glEnd();
	
	//print color value to the screen
	glColor3f(0, 0, 0);
	r = (one[0][1] - low) / 255.0;
	g = (two[0][1] - low) / 255.0;
	b = (three[0][1] - low) / 255.0;
	print(r, 100);
	glRasterPos2f(115,80);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'R');
	print(g, 200);
	glRasterPos2f(215, 80);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'G');
	print(b, 300);
	glRasterPos2f(315, 80);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'B');

	//draw 3 slider
	glBegin(GL_POLYGON);
	glColor3f(0, c1, 0);
	for (int i = 0; i < 4; i++) glVertex2i(one[i][0], one[i][1]);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0, c2, 0);
	for (int i = 0; i < 4; i++) glVertex2i(two[i][0], two[i][1]);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0, c3, 0);
	for (int i = 0; i < 4; i++) glVertex2i(three[i][0], three[i][1]);
	glEnd();
	
	//draw square
	glBegin(GL_POLYGON);
		glColor3f(r, g, b);
		for (int i = 0; i < 4; i++) glVertex2i(square[i][0], square[i][1]);
		glEnd();
	glBegin(GL_POLYGON);

	//draw rectangle
	glColor3f(leftColor[0], leftColor[1], leftColor[2]);
	glVertex2i(450, 100);
	glVertex2i(450, 150);
	glColor3f(rightColor[0], rightColor[1], rightColor[2]);
	glVertex2i(650, 150);
	glVertex2i(650, 100);
	glEnd();

	glFlush();
}

void motion(int x, int y) {
	mx = x;
	my = height - 1 - y;

	//keep action if cursor is inside the gravity field
	if (grav1 && checkGravity(mx, one[2][0] + gravity, one[0][0] - gravity, my, one[1][1] + gravity, one[0][1] - gravity)) {
		if (rubberbanding1) {			
			vert[n] = my; //save the y position
			if (n > 0) {	
				if (checkRange(one[0][1], low, high - 30)) { //make sure that silder only move between the slide bar
					move(one, vert[n], vert[n - 1]); //move slider accoording to the movement of cursor
					r = (one[0][1] - low) / 255.0;  //derivate the value of r from the postion of slider
					glutPostRedisplay();
				}			
			}
			n++; //increment n
		}
	}
	/*Same logic applies to slider two and three*/
	if (grav2 && checkGravity(mx, two[2][0] + gravity, two[0][0] - gravity, my, two[1][1] + gravity, two[0][1] - gravity)){
		if (rubberbanding2) {

			vert[n] = my;
			if (n > 0){
				if (checkRange(two[0][1], low, high - 30)) {
					move(two, vert[n], vert[n - 1]);
					g = (two[0][1] - low) / 255.0;
					glutPostRedisplay();
				}
			}
			n++;
		}
	}
	if (grav3 && checkGravity(mx, three[2][0] + gravity, three[0][0] - gravity, my, three[1][1] + gravity, three[0][1] - gravity)) {
		if (rubberbanding3) {
			vert[n] = my;
			if (n > 0){
				if (checkRange(two[0][1], low, high - 30)) {
					move(three, vert[n], vert[n - 1]);
					b = (three[0][1] - low) / 255.0;
					glutPostRedisplay();
				}
			}
			n++;
		}
	}
}

/*Keyboard event
if L is pressed, save the current RGB values to the leftColor array,
if R is pressed, save the current RGB values to the right Color array*/
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'l':
		leftColor[0] = r;
		leftColor[1] = g;
		leftColor[2] = b;
		break;
	case 'r':
		rightColor[0] = r;
		rightColor[1] = g;
		rightColor[2] = b;
		break;
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	int py = height - 1 - y; //save current y position
	switch (button) {
	case GLUT_LEFT_BUTTON: 
		/*For Left Button Down, there are 3 cases to consider
		First: slider is clicked,
		Second: the area of rectangle is clicked,
		Third: the area of slide bar is clicked*/

		//First case: slider is clicked
		if (state == GLUT_DOWN) {
			if (atOne == true) { //if slider one is clicked, atOne is derivated from passive function
				move(one, vert[n], vert[0]); //move slider based on cursor
				r = (one[0][1] - low) / 255.0; //derivate the value of r
				rubberbanding1 = true;	//start rubberbanding		
				glutPostRedisplay(); //then display
			}
			//Same logic applies to slider two and three
			else if (atTwo == true) {
				move(one, vert[n], vert[0]);
				g = (two[0][1] - low) / 255.0;
				rubberbanding2 = true;
				glutPostRedisplay();
			}
			else if (atThree == true) {
				move(one, vert[n], vert[0]);
				b = (three[0][1] - low) / 255.0;
				rubberbanding3 = true;
				glutPostRedisplay();
			}

			//Second Case: the area of rectangle is clicked
			if (checkRange(x, 450, 650) && checkRange(py, 100, 150)) { //check if the cursor is inside the rectangle

				//get pixel value at the position of cursor when left click
				//and then store the value to r,g,b accordingly
				glReadPixels(x, py, 1, 1, GL_RGB, GL_FLOAT, pixelColor);  
				r = pixelColor[0];
				g = pixelColor[1];
				b = pixelColor[2];
				
				//Once get r,g,b, move the sliders according to the values of each float points
				moveLine(one, r * 255 + low, -15);
				moveLine(two, g * 255 + low, -15);
				moveLine(three, b * 255 + low, -15);
			}

			//Third Case: the area of slide bar is clicked 
			if (checkRange(x, 100, 150) && checkRange(py, low, high)) { //check whether the cursor is in the range or not
				
				//decide the maximum and minimum of the position of slider
				if (py > high - 15) py = high - 15; 
				else if (py < low + 15) py = low + 15;

				//move slider one according to the value of py
				moveLine(one, py, 15);
				r = (one[0][1] - low) / 255.0; //update r
			}
			    //save logic applies to slider two and three
			else if (checkRange(x, 200, 250) && checkRange(py, low, high)) {
				if (py > high - 15) py = high - 15;
				else if (py < low + 15) py = low + 15;
				moveLine(two, py, 15);
				g = (two[0][1] - low) / 255.0;
			}
			else if (checkRange(x, 300, 350) && checkRange(py, low, high)) {
				if (py > high - 15) py = high - 15;
				else if (py < low + 15) py = low + 15;
				moveLine(three, py, 15);
				b = (three[0][1] - low) / 255.0;
			}
			
		}

		//if the button is not down, end rubberbanding and reset n
		else {
			rubberbanding1 = false;
			rubberbanding2 = false;
			rubberbanding3 = false;
			n = 0;
		}
		break;
	}
}

void passive(int x, int y) {

	//get position of cursor
	mx = x;
	my = height - 1 - y;

	if (checkRange(mx, one[0][0], one[2][0]) && checkRange(my, one[0][1], one[1][1])) { //check if the cursor is in slider one
		//if true, set atOne to true and grav1 to true, then change the color of slider
		atOne = true; 
		grav1 = true;
		c1 = 0;
	}
	else {
		//if not true, reset atOne, grav1, and color of the slider
		atOne = false;
		grav1 = false;
		c1 = 1;
	}
	//Same logic applies to slider two and three
	if (checkRange(mx, two[0][0], two[2][0]) && checkRange(my, two[0][1], two[1][1])) {
			atTwo = true;
			grav2 = true;
			c2 = 0;
		}
	else {
		atTwo = false;
		grav2 = false;
		c2 = 1;
	}
	
	if (checkRange(mx, three[0][0], three[2][0]) && checkRange(my, three[0][1], three[1][1])) {
		atThree = true;
		grav3 = true;
		c3 = 0;
	}
	else {
		atThree = false;
		grav3 = false;
		c3 = 1;
	}
	
	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Interative Color Mixer");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	//glColor3f(0, 1, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(passive);
	glutMainLoop();
}
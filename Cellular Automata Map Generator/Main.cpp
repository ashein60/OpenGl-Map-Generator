#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <utility>

const int width = 800;
const int height = 600;
const int squareSize = 5;
const int randPercent = 420;
const int cellLayers = 12;

bool gridRead[width / squareSize][height / squareSize]; //true = ground
bool gridWrite[width / squareSize][height / squareSize];

void CreateRandomGrid(int percent)
{
	for (int x = 0; x < width; x += squareSize)
	{
		for (int y = 0; y < height; y += squareSize)
		{
			if (rand() % 1000 <= percent)
			{
				gridRead[x / squareSize][y / squareSize] = true;
			}
		}
	}
}

bool isGround(int x, int y)
{
	bool ground = true;

	if (x < 0 || y < 0 || x > width / squareSize || y > height / squareSize)
	{
		return false;
	}
	else
	{
		return gridRead[x][y];
	}
}

bool ShouldBeGround(int x, int y)
{
	int count = 0;

	if (!isGround(x - 1, y)) //left
	{
		count++;
	}
	if (!isGround(x + 1, y)) //right
	{
		count++;
	}
	if (!isGround(x, y - 1)) //down
	{
		count++;
	}
	if (!isGround(x, y + 1)) //up
	{
		count++;
	}
	if (!isGround(x - 1, y - 1)) //left down
	{
		count++;
	}
	if (!isGround(x + 1, y - 1)) //right down
	{
		count++;
	}
	if (!isGround(x - 1, y + 1)) //left up
	{
		count++;
	}
	if (!isGround(x + 1, y + 1)) //right up
	{
		count++;
	}

	if (count > 4)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void MapGenerator(int layers)
{
	for (int i = 0; i < layers; i++)
	{
		for (int x = 0; x < width; x += squareSize)
		{
			for (int y = 0; y < height; y += squareSize)
			{
				gridWrite[x / squareSize][y / squareSize] = ShouldBeGround(x / squareSize, y / squareSize);
			}
		}

		std::swap(gridRead, gridWrite);
	}
}

void FillRectangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height)
{
	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (int x = 0; x < width; x += squareSize)
	{
		for (int y = 0; y < height; y += squareSize)
		{
			if (gridRead[x / squareSize][y / squareSize])
			{
				FillRectangle(x, y, squareSize, squareSize);
			}
		}
	}

	glFlush();
}

void displayInit()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, width, 0.0, height, 0.0, 1.0);
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(width, height);
	glutCreateWindow("A Simple Triangle");

	glutDisplayFunc(display);
	displayInit();

	CreateRandomGrid(randPercent); //percent out of 1000
	MapGenerator(cellLayers);

	glutMainLoop();
}
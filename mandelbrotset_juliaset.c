/*
 *
 * Name:         Ryu Fujiwara
 *
 * 
 * == MANDELBROT SET ==
 *   A set of c (complex number) which satisfies following
 *   z_(n+1) = z_n + c, z_0 = 0 (z_n is complex number) 
 *   where z_n does not diverge. (Oscillation is allowed)
 *
 * == JULIA SET ==
 *   A set of z_0 (complex number) which satisfies following
 *   z_(n+1) = z_n + c, c is arbitrary complex number
 *   where z_n does not diverge (Oscilation is allowed).
 *   Julia set has different appearence according to each c.
 *
 *
 * == EXPLANATION ==
 *
 * This programs shows you arbitrary Julia set or Mandelbrot set.
 * You can either choose Julia or Mandelbrot set that is going to be 
 * shown on the screen. If you would like to draw Julia set, you can
 * even choose the initial value of c. 
 * 
 * If you have chosen Mandelbrot set or your own c for Julia set, 
 * you can set up a zoom at any point you would like to see in 
 * detail. 
 * 
 * If you would like to see how different c's that you might not think
 * of, it will randomy choose some c's and displays them for you.
 * 
 * Also, you can choose a coloring method from two ways. One is
 * monochrome coloring, which colors the plane according to the 
 * divergence speed. This mode is good for visualizing the set.
 * Another is colorful mode, which colors the plane according 
 * to the time which the recursion diverges. This mode is good for 
 * checking how well your fractal is, which means you can observe 
 * self-similar shape very well.
 * 
 *
 * == HOW TO USE ==
 * 
 * PLEASE FOLLOW THE INSTRCUTION!
 * PLEASE DO NOT TRY TO IGNORE THE INSTRUCTION!
 * 
 * The "Secret Code" is generated after each depiction. This is a
 * string value of the settings that you have selected at that time.
 *
 * You can use it like following.
 * 1. Copy the secret code
 * 2. Re-run the program
 * 3. Enter 's' at the first question
 * 4. Paste the "Secret code" and enter.
 * 5. The setting is reproduced. 
 *
 *
 * == SAMPLE SECRET CODE ==
 * -2.0 2.0 -2.0 2.0 0.102732 0.618093 0 0 1 1
 * -0.015 -0.01 0.01 0.015 -0.867123 0.264830 0 0 0 1
 * -2 2 -2 2 0 0 8 1 0 1
 * -0.801309 -0.701309 0.021923 0.121923 0 0 0 0 1 0
 * 
 */

# include <stdio.h>
# include <stdlib.h>
# include <glsc.h>
# include <math.h>
# include <time.h>

/* Max time for iterating */
# define ITERATION_MAX (1000)
# define PI (3.1415926)

/*
*  Prototype functions
*/

/* Main functions */
void drawAsGiven (double R_MIN, double R_MAX, double I_MIN, double I_MAX, int *width, double a, double b, 
	int isRandom, int isMonochrome, int isJulia, int size);
void userInteraction (double *R_MIN, double *R_MAX, double *I_MIN, double *I_MAX, double *a, double *b, 
	int *size, int *isRandom, int *isMonochrome, int *isJulia);
void halt ();

/* Initialization */
void initialization (double R_MIN, double R_MAX, double I_MIN, double I_MAX);
void multiInitialization (double R_MIN, double R_MAX, double I_MIN, double I_MAX, int size);

/* Related to Julia or Mandelbrot Set generation */
int isInJuliaSet (double x0, double y0, double *xn, double *yn, double *xn1, double *yn1,
	double a, double b, int isMonochrome);
int isInMandelbrotSet (double *xn, double *yn, double *xn1, double *yn1, double a, double b, int isMonochrome);

int calculateResolution (int size, int isRandom);

/* Related to display */
void crampRange (double *len, double centerR, double centerI);
void displayAndCalculateJuliaSet (double R_MIN, double R_MAX, double I_MIN, double I_MAX, int width, int height, 
	double a, double b, int isJulia, int isMonochrome);
void multiDisplayAndCalculateJuliaSet(double R_MIN, double R_MAX, double I_MIN, double I_MAX, int width, int height, 
	int size, int isMonochrome);

/* Miscellaneous */
void generateSecretCode (double R_MIN, double R_MAX, double I_MIN, double I_MAX, double a, double b, int size, 
	int isRandom, int isMonochrome, int isJulia);
void mousePoint ();

/* 
 *  MAIN 
 */
int main () {
	/* range of Gaussian plane */
	double R_MIN, R_MAX, I_MIN, I_MAX;
	/* resolution */
	int width;
	/* c = a + bi (used for Julia set) */
	double a, b;

	/* like boolean */
	int isRandom = 0;
	int isMonochrome = 0;
	int isJulia = 0;

	/* how many Julia sets this is going to display */
	int size = 0;

	/* user input */
	userInteraction(&R_MIN, &R_MAX, &I_MIN, &I_MAX, &a, &b, &size, &isRandom, &isMonochrome, &isJulia);
	/* initialize and draw graph */
	drawAsGiven (R_MIN, R_MAX, I_MIN, I_MAX, &width, a, b, isRandom, isMonochrome, isJulia, size);
	/* output secret code */
	generateSecretCode (R_MIN, R_MAX, I_MIN, I_MAX, a, b, size, isRandom, isMonochrome, isJulia);

	return 0;
}

/* 
 * Manages user's input. 
 * Substitute user's inputs to each value
 */
void userInteraction (double *R_MIN, double *R_MAX, double *I_MIN, double *I_MAX, double *a, double *b, 
	int *size, int *isRandom, int *isMonochrome, int *isJulia) {

	/* used for memorizing user inputs */
	char explanation, set, pick, focus, color;
	double real, imaginary, cr, ci, l;
	int num;

	/* used for memorizing secret codes */
	double s1, s2, s3, s4, s5, s6;
	int s7, s8, s9, s10;

	/* used for calculating range of Gauss plane */
	double centerR, centerI, len;

	/* Introduction */
	printf("\n---------------------------------------------------------\n\n");
	printf("This program lets you see Julia Set, or Mandelbrot Set.\n");
	printf("If you would like to see the description of these sets, \n");
	printf("please enter 'y' for YES, or 'n' for NO\n");
	printf("or if you know the secret code please enter it.\n");
	scanf("%[yns]", &explanation);


	if (explanation == 'y') {
		/* Explanation of each set */
		printf("\n---------------------------------------------------------\n\n");
		printf("==Mandelbrot set==\n");
		printf("-> A set of c (complex number) which satisfies following\n");
		printf("--> z_(n+1) = z_n + c, z_0 = 0 (z_n is complex number) \n");
		printf("--> where z does not diverge. (Oscillation is allowed)\n\n");
		printf("==Julia set==\n");
		printf("-> A set of z_0 (complex number) which satisfies following\n");
		printf("--> z_(n+1) = z_n + c, c is arbitrary complex number\n");
		printf("--> where z_n does not diverge (Oscilation is allowed).\n");
	}
	else if (explanation == 's') {
		/* Read and convert secret code to setting */
		printf("Secret code?\n");
		scanf("%lf%lf%lf%lf%lf%lf%d%d%d%d", &s1, &s2, &s3, &s4, &s5, &s6, &s7, &s8, &s9, &s10);
		*R_MIN = s1;
		*R_MAX = s2;
		*I_MIN = s3;
		*I_MAX = s4;
		*a = s5;
		*b = s6;
		*size = s7;
		*isRandom = s8;
		*isMonochrome = s9;
		*isJulia = s10;

		return;
	}

	/* Select which set that is going to be displayed */
	printf("\n---------------------------------------------------------\n\n");
	printf("Which set do you want to see?\n");
	printf("'m' for Mandelbrot set, 'j' for Julia set\n");
	scanf(" %[mj]", &set);

	/* Julia Set */
	if (set == 'j') {
		*isJulia = 1;

		/* Asks user whether (s)he wants to choose initial c */
		printf("\n---------------------------------------------------------\n\n");
		printf("If you enter 'r', I will pick random c(s) for you.\n");
		printf("If you enter 'c', you can create your own Julia set.\n");
		scanf(" %[rc]", &pick);

		/* Not choosing initial c */
		if (pick == 'r') {
			*isRandom = 1;

			/* Asks how many random Julia sets does user want to see */
			printf("\n---------------------------------------------------------\n\n");
			printf("How many Julia Set's do you want to see?\n");
			printf("enter a number between 1 ~ 8.\n");
			scanf(" %d", &num);
			*size = num;
		}
		/* Choosing initial c */
		else if (pick == 'c') {
			*isRandom = 0;

			/* Inputs initial c */
			printf("\n---------------------------------------------------------\n\n");
			printf("Enter a value of c. (|c| < 2 is highly recommended.)\n");
			printf("Please input Real Part of c\n");
			scanf(" %lf", &real);
			*a = real;

			printf("Please input Imaginary Part of c\n");
			scanf(" %lf", &imaginary);
			*b = imaginary;
		}
	}
	/* Mandelbrot Set */
	else if (set == 'm') {
		*isJulia = 0;
	}	

	/* Default range of display */
	*R_MIN = -2;
	*R_MAX = 2;
	*I_MIN = -2;
	*I_MAX = 2;

	/* Selecting focusing point (zoom) */
	if (*isRandom == 0) {
		/* Asks whether user wants to zoom or not */
		printf("\n---------------------------------------------------------\n\n");
		printf("If you would like to focus on some point,\n");
		printf("please enter 'y' for YES, or 'n' for no\n");
		scanf(" %[yn]", &focus);

		/* Zooming mode */
		if (focus == 'y') {
			/* Asks zooming point and scale */
			printf("\n---------------------------------------------------------\n\n");
			printf("Where? (coordinate)\n");
			printf("Real part? (-2, 2)\n");
			scanf(" %lf", &cr);
			centerR = cr;
			printf("Imaginary part? (-2, 2)\n");
			scanf(" %lf", &ci);
			centerI = ci;
			printf("How big? If too big, it will be cramped (length)\n");
			scanf(" %lf", &l);
			len = l;

			crampRange (&len, centerR, centerI);

			*R_MIN = centerR - len;
			*R_MAX = centerR + len;
			*I_MIN = centerI - len;
			*I_MAX = centerI + len;
		}
	}
	

	/* Asks the coloring mode */
	printf("\n---------------------------------------------------------\n\n");
	printf("Mode? 'm' for monochrome, 'c' for colored\n");
	printf("Caution: monochrome mode takes very very long time\n");
	scanf(" %[mc]", &color);

	/* Set coloring mode */
	if (color == 'm') {
		*isMonochrome = 1;
	}
	else if (color == 'c') {
		*isMonochrome = 0;
	}

	printf("\n---------------------------------------------------------\n\n");
}

/*
 * Displays Julia or Mandelbrot set as given 
 */
void drawAsGiven (double R_MIN, double R_MAX, double I_MIN, double I_MAX,int  *width, double a, double b, 
	int isRandom, int isMonochrome, int isJulia, int size) {

	/* Generate new seeds according to time */
	srand((unsigned)time(NULL));

	/* Set Resolution */
	*width = calculateResolution(size, isRandom);

	/* Mandelbrot Set */
	if (isJulia == 0) {
		initialization(R_MIN, R_MAX, I_MIN, I_MAX);
		displayAndCalculateJuliaSet(R_MIN, R_MAX, I_MIN, I_MAX, *width, *width, a, b, isJulia, isMonochrome);
	}

	/* Julia Set */
	else if (isJulia == 1) {
		/* User input mode */
		if (isRandom == 0) {
			initialization(R_MIN, R_MAX, I_MIN, I_MAX);
			displayAndCalculateJuliaSet(R_MIN, R_MAX, I_MIN, I_MAX, *width, *width, a, b, isJulia, isMonochrome);
		}
		/* Random mode */
		else if (isRandom == 1) {
			multiInitialization(R_MIN, R_MAX, I_MIN, I_MAX, size);
			multiDisplayAndCalculateJuliaSet(R_MIN, R_MAX, I_MIN, I_MAX, *width, *width, size, isMonochrome);
		}
	}

	/* Terminates this program. */
	halt();
}

/*
 * Terminates this program  
 */
void halt () {
	/*
	while (1) {
		mousePoint();
	}
	*/
	g_sleep(G_STOP);
	g_term();
}

/* 
 * Initialization for single mode
 */
void initialization (double R_MIN, double R_MAX, double I_MIN, double I_MAX) {
	/* Initiates */
	g_init("GRAPH", 100.0, 100.0);
	g_device(G_DISP);

	/* Creates and select Gaussian plane */
	g_def_scale(1, R_MIN, R_MAX, I_MIN, I_MAX, 0, 0, 100.0, 100.0);
	g_sel_scale(1);
}

/* 
 * Initialization for multi mode
 */
void multiInitialization (double R_MIN, double R_MAX, double I_MIN, double I_MAX, int size) {
	/* Used for for loop */
	int r, c;

	/* Length of the size */
	int length = 180.0 / (double)size;

	/* Initialize the screen */
	g_init("GRAPH", 180.0, 180.0);
	g_device(G_DISP);

	/* Creates size^2 virtual plane*/
	for (r = 0; r < size; r++) {
		for (c = 0; c < size; c++) {
			g_def_scale(r * size + c + 1, R_MIN - 0.1, R_MAX + 0.1, I_MIN - 0.1, I_MAX + 0.1,
			 c * length, r * length, length, length);
			g_sel_scale(r * size + c + 1);

			g_box(R_MIN, R_MAX, I_MIN, I_MAX, 1, 0);
		}
	}
}

/* 
 * Check whether given point is in given Julia Set
 * z_n+1 = (z_n)^2 + c, z_n = x_n + y_n * i, c = a + bi
 * By above equations, Gaussian plane is converted to Descartes plane.
 * x_n+1 = (x_n)^2 - (y_n)^2 + a
 * y_n+1 = 2 * x_n * y_n + b
 */
int isInJuliaSet (double x0, double y0, double *xn, double *yn, double *xn1, double *yn1, double a, double b, 
	int isMonochrome) {
	/* Used for for loop */
	int i;

	/* Used for temporally storage */
	double tempX, tempY;

	/* Color value */
	double color;

	/* Initial set up */
	*xn = x0;
	*yn = y0;
	*xn1 = x0;
	*yn1 = y0;

	/* Iterates the equation */
	for (i = 0; i < ITERATION_MAX; i++) {
		tempX = *xn;
		tempY = *yn;

		*xn = *xn1;
		*yn = *yn1;

		*xn1 = tempX * tempX - tempY * tempY + a;
		*yn1 = 2 * tempX * tempY + b;

		/* Checks divergence. |z_n| > 2 means divergence */
		if (tempX * tempX + tempY * tempY > 4) {
			/* Monochrome mode */
			if (isMonochrome == 1) {
				/* Select color according to divergence speed */
				color = i / (double) ITERATION_MAX;
				g_line_color(g_rgb_color(color, color, color));
			}
			/* Color index * (-1) */
			return -(i % 7) -1;
		}
	}

	/* White for convergence in monochrome mode */
	g_line_color(7);

	/* Black for convergence in color mode */
	return 1;
}

/*
 * Check whether given point is in Mandelbrot Set
 * z_n+1 = (z_n)^2 + c, z_n = x_n + y_n * i, c = a + bi, z_0 = 0
 * By above equations, Gaussian plane is converted to Descartes plane.
 * x_n+1 = (x_n)^2 - (y_n)^2 + a
 * y_n+1 = 2 * x_n * y_n + b 
 */
int isInMandelbrotSet (double *xn, double *yn, double *xn1, double *yn1, double a, double b, int isMonochrome) {
	/* Used for for loop */
	int i;

	/* Temporary storage */
	double tempX, tempY, x0, y0;

	/* Color value */
	double color;

	/* Initiate z_0 */
	x0 = 0;
	y0 = 0;

	/* Initial setup */
	*xn = x0;
	*yn = y0;
	*xn1 = x0;
	*yn1 = y0;

	/* Iterates the equation */
	for (i = 0; i < ITERATION_MAX; i++) {
		tempX = *xn;
		tempY = *yn;

		*xn = *xn1;
		*yn = *yn1;

		*xn1 = tempX * tempX - tempY * tempY + a;
		*yn1 = 2 * tempX * tempY + b;

		/* Checks divergence. |z_n| > 2 means divergence */
		if (tempX * tempX + tempY * tempY > 4) {
			/* Monochrome mode */
			if (isMonochrome == 1) {
				/* Select color according to divergence speed */
				color = sin(i / 5000.0 * PI / 2.0);
				g_line_color(g_rgb_color(color, color, color));

				/* Set white for too slow divergence */
				if (i > 5000) {
					g_line_color(7);
				}
			}

			/* Color index * (-1) */
			return -(i % 7) -1;
		}
	}

	/* White for convergence in monochrome mode */
	g_line_color(7);

	/* Black for convergence in color mode */
	return 1;
}

/*
 * Calculate ideal resoulution for given size
 */
int calculateResolution (int size, int isRandom) {
	if (isRandom == 1) {
		/* Approximate formula for resolution of each size */
		return 9.3 * size * size - 165 * size + 810;
	}
	
	/* Default resolution for single mode */
	return 400;
}

/*
 * Clamps the range user gave to [-2, 2]^2
 * (Doesn't work well)
 */
void crampRange (double *len, double centerR, double centerI) {
	/* Half the length */
	double halfLength = *len / 2.0;

	/* Clamping */
	if (centerR + halfLength > 2.0) {
		halfLength = 2.0 - centerR;
	}

	if (centerR - halfLength < -2.0) {
		halfLength = centerR + 2.0;
	}

	if (centerI + halfLength > 2.0) {
		halfLength = 2.0 - centerI;
	}

	if (centerI - halfLength < -2.0) {
		halfLength = centerI + 2.0;
	}

	*len = 2.0 * halfLength;
}

/*
 * Calculate all the point of Julia (Mandelbrot) set and display it
 */
void displayAndCalculateJuliaSet (double R_MIN, double R_MAX, double I_MIN, double I_MAX, int width, int height, 
	double a, double b, int isJulia, int isMonochrome) {

	/* Used for for loop */
	int i, j;

	/* Storing each (virtual) pixels' information */
	int juliaSet[width][height];

	/* Recursively use these */
	double x0, y0, xn, yn, xn1, yn1;

	/* The length between pixels */
	double deltaR = (R_MAX - R_MIN) / (double)width;
	double deltaI = (I_MAX - I_MIN) / (double)height;

	/* Calculate and displays each pixel */
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			/* Set left bottom point */
			x0 = j * deltaR + R_MIN;
			y0 = i * deltaI + I_MIN;

			/* Julia Set */
			if (isJulia == 1) {
				/* Set color information */
				juliaSet[i][j] = isInJuliaSet(x0, y0, &xn, &yn, &xn1, &yn1, a, b, isMonochrome);
			}
			/* Mandelbrot set */
			else if (isJulia == 0) {
				/* Set color information */
				juliaSet[i][j] = isInMandelbrotSet(&xn, &yn, &xn1, &yn1, x0, y0, isMonochrome);
			}
			
			/* Color mode */
			if (isMonochrome == 0) {
				/* Set color as calculated */
				if (juliaSet[i][j] < 0) {
					g_line_color(- juliaSet[i][j]);
				}
				else {
					g_line_color (0);
				}
			}
			
			/* Put dots with given color */
			g_move(j * deltaR + R_MIN, i * deltaI + I_MIN);
			g_plot(j * deltaR + R_MIN, i * deltaI + I_MIN);
		}
	}	
}

/*
 * Calculate all the point of Julia set(s) and display it.
 * This also gives random initial value for each Julia set.
 */
void multiDisplayAndCalculateJuliaSet(double R_MIN, double R_MAX, double I_MIN, double I_MAX, int width, int height, int size, int isMonochrome) {

	/* Used for for loop */
	int i, j, s;

	/* Storing each (virtual) pixels' information for every virtual plane */
	int juliaSet[size * size][width][height];

	/* Recursively use these */
	double x0, y0, xn, yn, xn1, yn1;

	/* The length between pixels */
	double deltaR = (R_MAX - R_MIN) / (double)width;
	double deltaI = (I_MAX - I_MIN) / (double)height;

	/* Stores initial c's for each Julia set */
	double initialC[size * size][2];

	/* Initialize and get random c's  */
	for (s = 0; s < size * size; s++) {
		/* a */
		initialC[s][0] = rand() / (double)RAND_MAX * 2 - 1;
		/* b */
		initialC[s][1] = rand() / (double)RAND_MAX * 2 - 1;

		/* Displays c's chosen */
		printf ("%d: %lf + %lf i\n", s, initialC[s][0], initialC[s][1]);
	}
	
	/* Calculate and displays each pixel */
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			/* For each virtual plane */
			for (s = 1; s <= size * size; s++) {
				/* Select virtual plane */
				g_sel_scale(s);

				/* Set left bottom point */
				x0 = j * deltaR + R_MIN;
				y0 = i * deltaI + I_MIN;

				/* Set color information */
				juliaSet[s - 1][i][j] = isInJuliaSet(x0, y0, &xn, &yn, &xn1, &yn1, 
					initialC[s - 1][0], initialC[s - 1][1], isMonochrome);

				/* Color mode */
				if (isMonochrome == 0) {
					if (juliaSet[s - 1][i][j] < 0) {
						g_line_color(- juliaSet[s - 1][i][j]);
					}
					else {
						g_line_color(0);
					}
				}

				/* Put dot with given color */
				g_move(j * deltaR + R_MIN, i * deltaI + I_MIN);
				g_plot(j * deltaR + R_MIN, i * deltaI + I_MIN);
			}
		}
	}
}

/*
 * Creates secret code.
 * It is just printing every setting as String.
 */
void generateSecretCode (double R_MIN, double R_MAX, double I_MIN, double I_MAX, double a, double b, int size, 
	int isRandom, int isMonochrome, int isJulia) {
	/* Create secret code */
	printf("Secret Code is...\n");
	printf("%lf %lf %lf %lf %lf %lf %d %d %d %d\n\n", R_MIN, R_MAX, I_MIN, I_MAX, a, b, size, isRandom, isMonochrome, isJulia);
}

/*
 * Print mouse position (Debug only)
 */
void mousePoint () {
	double mouseX, mouseY;
	int button;

	g_mouse_sence(&mouseX, &mouseY, &button);

	g_sel_scale(1);

	if (button == 1) {
		printf("(%lf, %lf)\n", mouseX, mouseY);
	}
}


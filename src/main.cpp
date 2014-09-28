#include <iostream>
#include <GL/glew.h>

#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <OpenGL/glu.h>
#elif WIN32
#include <SDL.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#endif

#include "Vertex.h"

//global values go here!
GLuint triangleVBO;

Vertex triangleData[]={ {0.0f,1.0f,0.0f, //x,y,z
                        1.0f,0.0f,0.0f,1.0f},//r,g,b,a
    
                        {-1.0f,-1.0f,0.0f, //x,y,z
                        0.0f,1.0f,0.0f,1.0f},//r,g,b,a
    
                        {1.0f,-1.0f,0.0f, //x,y,z
                        0.0f,0.0f,1.0f,1.0f}};//r,g,b,a

//SDL Window
SDL_Window * window = NULL;
//SDL GL Context
SDL_GLContext glcontext = NULL;

//Window Width
const int WINDOW_WIDTH = 640;
//Window Height
const int WINDOW_HEIGHT = 480;

bool running = true;

//Global functions
void InitWindow(int width, int height, bool fullscreen)
{
	//Create a window
	window = SDL_CreateWindow(
		"Lab 1",             // window title
		SDL_WINDOWPOS_CENTERED,     // x position, centered
		SDL_WINDOWPOS_CENTERED,     // y position, centered
		width,                        // width, in pixels
		height,                        // height, in pixels
		SDL_WINDOW_OPENGL           // flags
		);
}

void CleanUp()
{
	// clean up, reverse order!!!
	glDeleteBuffers(1, &triangleVBO);
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void initGeometry()
{
	//Create buffer
	glGenBuffers(1, &triangleVBO);
	// Make the new VBO active
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	//Copy Vertex Data to VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleData), triangleData, GL_STATIC_DRAW);
}

//Function to update the game state
void update()
{
    
}

//Function to initialise OpenGL
void initOpenGL()
{
	//Create OpenGL Context
	glcontext = SDL_GL_CreateContext(window);

    //Smooth shading
    glShadeModel( GL_SMOOTH );
    
    //clear the background to black
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    
    //Clear the depth buffer
    glClearDepth( 1.0f );
    
    //Enable depth testing
    glEnable( GL_DEPTH_TEST );
    
    //The depth test to go
    glDepthFunc( GL_LEQUAL );
    
    //Turn on best perspective correction
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cout << "Error: "<<glewGetErrorString(err) << std::endl;
	}
}

//Function to set/reset viewport
void setViewport( int width, int height )
{
    //screen ration
    GLfloat ratio;
    
    //make sure height is always above 1
    if ( height == 0 ) {
        height = 1;
    }
    
    //calculate screen ration
    ratio = ( GLfloat )width / ( GLfloat )height;
    
    //Setup viewport
    glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );
    
    //Change to poject matrix mode
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    
    //Calculate perspective matrix, using glu library functions
    gluPerspective( 45.0f, ratio, 0.1f, 100.0f );
    
    //Swith to ModelView
    glMatrixMode( GL_MODELVIEW );
    
    //Reset using the Indentity Matrix
    glLoadIdentity( );
}

//Function to render(aka draw)
void render()
{
    //old imediate mode!
    //Set the clear colour(background)
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    //clear the colour and depth buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    //Make the new VBO active. Repeat here as a sanity check( may have changed since initialisation)
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    
	//Establish its 3 coordinates per vertex with stride is the size of one vertex(space between elements) in array
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), NULL);
    //The last parameter basically says that the colours start 3 floats into the data
    glColorPointer(4, GL_FLOAT, sizeof(Vertex), (void**)(3 * sizeof(float)));
    
	//Establish array contains vertices & colours
	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    //Swith to ModelView
    glMatrixMode( GL_MODELVIEW );
    //Reset using the Indentity Matrix
    glLoadIdentity( );
    //translate
    glTranslatef( -2.0f, 0.0f, -6.0f );
    //Actually draw the triangle, giving the number of vertices provided
	glDrawArrays(GL_TRIANGLES, 0, 3) ;
    
    //Load Identity
	glLoadIdentity();
	//translate
	glTranslatef(2.0f, 0.0f, -6.0f);
	//Actually draw the triangle, giving the number of vertices provided
	glDrawArrays(GL_TRIANGLES, 0, 3) ;

	SDL_GL_SwapWindow(window);
}

//Main Method
int main(int argc, char * arg[])
{
    // init everyting - SDL, if it is nonzero we have a problem
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "ERROR SDL_Init " <<SDL_GetError()<< std::endl;
        
        return -1;
    }
    
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false);
    //Call our InitOpenGL Function
    initOpenGL();
    //Set our viewport
	setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);
    
	initGeometry();

    //Value to hold the event generated by SDL
    SDL_Event event;
    //Game Loop
	while (running)
    {
        //While we still have events in the queue
        while (SDL_PollEvent(&event)) {
            //Get event type
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                //set our boolean which controls the loop to false
                running = false;
            }
        }
		update();
        //render
		render();
       
        
    }
    

	CleanUp();
    
    return 0;
}
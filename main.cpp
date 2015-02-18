
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <math.h>

SDL_Window* displayWindow;

void DrawSprite(GLint texture, float x, float y, float rotation, float theSize_x, float theSize_y) {
   

    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    
    //image size quad[]
    
    //definiting the image size by the coordinate from the parameter
    GLfloat quad[] = {-theSize_x, theSize_y, -theSize_x, -theSize_y, theSize_x, -theSize_y, theSize_x, theSize_y};
    
    glVertexPointer(2, GL_FLOAT, 0, quad);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    
    //vertex coordinate texture
    //   float neg_x1 = -x1;
    // float neg_y1 = -y1;
    
    GLfloat quadUVs[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
    // GLfloat quadUVs[] = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
    
    
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    //enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc (GL_SRC_ALPHA, GL_ONE);
    
    
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
    
  
    
}

GLuint LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return textureID;
}


class Rec{
public:
    
    float x;
    float y;
    float angle;
    float theSize_x;
    float theSize_y;
    
    Rec(float x, float y, float angle, float size_x, float size_y ):x(x),y(y),angle(angle),theSize_x(size_x),theSize_y(size_y){};
    
    //i dont think i need the setters since the variables are public...
    void setX(float new_x){
        x = new_x;
    }
    void setY(float new_Y){
        y = new_Y;
    }
    
    void draw(){
        
       
        glLoadIdentity();
        glTranslatef(x, y, 0.0);
        glRotatef(angle, 0.0, 0.0, 1.0);
        
        
      //  GLfloat quad[] = {-x, y, -x, -y, x, -y, x, y};
         GLfloat quad[] = {-theSize_x, theSize_y, -theSize_x, -theSize_y, theSize_x, -theSize_y, theSize_x, theSize_y};
        glVertexPointer(2, GL_FLOAT, 0, quad);
        glEnableClientState(GL_VERTEX_ARRAY);
        glDrawArrays(GL_QUADS, 0, 4);
        
      //  GLfloat quadUVs[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
        //glTexCoordPointer(2, GL_QUADS, 0, quadUVs);
        
     
    };
    
    
};



 bool checkCollision(Rec p1, Rec ball){

     
     
    //handle right
     if (ball.x + (ball.theSize_x * 0.5) < p1.x - (p1.theSize_x * 0.5)) {
         return false;
     }
     //handle left
      if (ball.x - (ball.theSize_x * 0.5) > p1.x + (p1.theSize_x * 0.5)) {
         return false;
     }
     
     //handle right
      if (ball.y - (ball.theSize_y * 0.5) > p1.y + (p1.theSize_y * 0.5)) {
         return false;
     }
     //handle left
      if (ball.y + (ball.theSize_y * 0.5) < p1.y - (p1.theSize_y * 0.5)) {
         return false;
          
          
     }
 return true;

 };


bool bounce_Y(Rec ball){
/*
    if (ball.y + (ball.theSize_y * 0.5) > 1.00 && ball.x < 1.33 && ball.x > -1.33) {
        return true;
    }

    if (ball.y - (ball.theSize_y * 0.5) < -1.00 && ball.x < 1.33 && ball.x > -1.33) {
        return true;
    }
    
    return false;*/
    
    if (ball.y + (ball.theSize_y * 0.5) > 1.00  ) {
        return true;
    }
    
    if (ball.y - (ball.theSize_y * 0.5) < -1.00 ) {
        return true;
    }
    
    return false;
    
};

bool detectWinner(Rec ball){
    
    GLuint left = LoadTexture("winner_left.png");
    GLuint right = LoadTexture("winner_right.png");

    
    if (ball.x <= -1.33){
        
        DrawSprite(right, 0.0, 0.0, 0.00, 0.5, 0.5);
        return true;
        
        }
     else if (ball.x >= 1.33){
        
        DrawSprite(left, 0.0, 0.0, 0.00, 0.5, 0.5);
         return true;

    }
   
    return false;
}


int main(int argc, char *argv[]){
    
    
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("P O N G !", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    bool done = false;
    
    SDL_Event event;
    
    
    
    GLuint pong_bg = LoadTexture("pong_bg.png");
    float ball_position_x = 0.00;
    float ball_position_y = 0.00;
    float racket_p1 = 0.0;
    float racket_p2 = 0.0;
    
    Rec ball1(ball_position_x, ball_position_y, 0.0, 0.01, 0.013);
    
    Rec pong_p1(-0.85, racket_p1, 0.00, 0.015, 0.25);
    
    Rec pong_p2(0.85, racket_p2, 0.00, 0.015, 0.25);
    
    
 
    float position = sqrtf( pow(ball1.x, 2.0) + pow(ball1.y, 2.0));
    
    
    float direction_y = cos(ball1.theSize_y/ball1.theSize_x);
    float direction_x = sin(ball1.theSize_x/ball1.theSize_y);
    float lastFrameTicks = 0.0;
 
  
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    while (!done) {
        
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        float distance_to_travel_in_one_second = 0.5;
      
 
        
        
        while (SDL_PollEvent(&event)) {
            
            DrawSprite(pong_bg, 0.0, 0.0, 0.00, 1.00, 1.00);
           
            
            
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
            
            else if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.scancode == SDL_SCANCODE_Z) {
                 //   while  (racket_p1 < (0.6)){
                    float new_y=pong_p1.y;
                    new_y-= 0.08;
                    pong_p1.setY(new_y);
                    
                    
                   // }
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_SLASH) {
                    
                  //  while (racket_p1 < (-0.4)) {
                    pong_p2.y -= 0.08;}
               // }
               
            }
            
            else if(event.type == SDL_KEYUP) {
                if(event.key.keysym.scancode == SDL_SCANCODE_A) {
                   pong_p1.y += 0.05;
                }
              
                if(event.key.keysym.scancode == SDL_SCANCODE_APOSTROPHE) {
                    pong_p2.y += 0.05;
                }
              

            }
            
            
        }
    
    
        ball1.draw();
        pong_p1.draw();
        pong_p2.draw();
        

        ball1.y += elapsed*distance_to_travel_in_one_second*direction_y;
        ball1.x += elapsed*distance_to_travel_in_one_second*direction_x;
     
        if (bounce_Y(ball1)){
            direction_y = -(direction_y);
        
        }
        
        if ( checkCollision(pong_p1, ball1)){
            
         
            direction_x = -(direction_x);
        
        }
        
         if ( checkCollision(pong_p2, ball1)){
            
       
            direction_x = -(direction_x);
            
        }
     
        if ( detectWinner( ball1) ){
            
            //how do I show the winner for longer?
            
            
            ball1.x = 0;
            ball1.y = 0;
        }
        
       SDL_GL_SwapWindow(displayWindow);
       DrawSprite(pong_bg, 0.0, 0.0, 0.00, 1.00, 1.00);

        
        
        
        
       
    }

    

    SDL_Quit();
    return 0;
    
    
   
}

/*
 
 //  DrawSprite(pong_p1, -0.95, racket_p1 , 0.00, 0.015, 0.25);
 //DrawSprite(pong_p2, 0.95, racket_p2, 0.00, 0.015, 0.25);
 //  DrawSprite(ball, elapsed , elapsed , elapsed , 0.0125, 0.018);
 
 //if the ball goes out of the window x,y then the opposite side wins
 //show winning sign if this happens
 
 */



// while (ball1.y <= 1.00 && ball1.y >= -1.00 ) {
//   ball1.y += 0.0005;}


//if the ball position is 1 or -1, then the ball changes the direction


//if the ball position is more than - 1.33 or 1.33 then the opposite side wins
//show the "WIN" on the screen


#include "MyGlWindow.h"

//extern auto t_start;	
MyGlWindow::MyGlWindow(int X, int Y, int W, int H, const char *L) : Fl_Gl_Window(X, Y, W, H, L){
	zoom_factor = 1;
	main_coord_x = 0.0f;
	main_coord_y = 0.0f;
	main_coord_z = 0.0f;
	swipe_x = 0;
	swipe_y = 0;
	swipe_z = 0;
	view = VIEW_1;
	mode = MODE_BITMAP;
   font_index = 0;
	end();
}


MyGlWindow::~MyGlWindow()
{
}

void MyGlWindow::draw(){
	if (!valid()) {
		valid(1);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();		
		glDisable(GL_LIGHTING);
		glViewport(0, 0, w(), h());
		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glFrustum(-1, 1, -1, 1, 2, 10000);
		glOrtho(-w()*zoom_factor, w()*zoom_factor, -h()*zoom_factor, h()*zoom_factor, -1, 1);
		gl_font(FL_HELVETICA_BOLD, 16);
		glClearColor(0, 0.0, 0, 1.0f);
		tex_pump = loadTexture("water_tower.png", 256, 256);
		tex_terrain = loadTexture("terrain.png", 512, 512);
		int argc = 0;
		char *argv[1];
		glutInit(&argc, NULL);
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	
	glViewport(0, 0, w(), h());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clean  buffers
	glClearColor(0, 0.0f, 0, 1.0f);	// background color
	glOrtho(-w()*zoom_factor, w()*zoom_factor, -h()*zoom_factor, h()*zoom_factor, -1, 1);
	glPushMatrix();
	glTranslatef(main_coord_x, main_coord_y, main_coord_z);
	drawBackground();	
	if(map!=NULL){
		drawMap();	//disegna il grafo     
	}else drawPump(0, 0);
	glPopMatrix();
	if(map!=NULL && map->getDim()>1)drawOverlay();
   glFlush();
}

void MyGlWindow::drawOverlay(){
	glViewport(w()/4,-h(),w(),2*h());
	
	glScalef(zoom_factor,zoom_factor,zoom_factor);
	//main black rectangle
	glPushMatrix();
	glColor4f(.0f,.0f,.0f,0.5f);
	glBegin(GL_POLYGON);
	glVertex2f(0,0);
	glVertex2f(0,h());
	glVertex2f(w()/2,h());
	glVertex2f(w()/2,0);
	glEnd();
	glPopMatrix();
	
	//border
	glPushMatrix();
	glColor4f(.0f,.0f,.0f,1.0f);
	glBegin(GL_LINES);
	glVertex2f(0,0);
	glVertex2f(0,h());
	glEnd();
	glPopMatrix();
	
	char *c;
	/*c = "RiverSim 1.1"; 
	font_index = 3;
   glColor4f(1.0, 1.0, 1.0, 1.0);
	drawString(c,w()/8,h()-20);
	*/
	font_index = 0;
	std::vector<std::string>labels = map->getAllCanaliname();
	std::vector<float> avg_H = map-> getAvgHeight();
	std::vector<float> avg_F = map->getAvgPortata();
	std::vector<float> incl = map->getPendenza();
	for(int i=0;i<labels.size();i++){
		//printf("%s\n",labels[i]);
		char *d = new char[labels[i].length() + 1];
		strcpy(d, labels[i].c_str());
		drawString(d,10,(h()-100*i/2)-30 + swipe_y);
		c = new char[32];
		sprintf(c, "H:%4.2f", avg_H[i]);
		drawString(c,10,(h()-(100*i/2)-20)-30 + swipe_y);
		sprintf(c, "F:%4.2f", avg_F[i]);
		drawString(c,200,(h()-(100*i/2)-20)-30 + swipe_y);
		sprintf(c, "I:%4.2f", incl[i]);
		drawString(c,390,(h()-(100*i/2)-20)-30 + swipe_y);
		delete [] d;
		
		glPushMatrix();
		glColor4f(1.0f,1.0f,1.0f,1.0f);
		glBegin(GL_LINES);
		glVertex2f(0, (h()-(100*i/2)-20)-40 + swipe_y);
		glVertex2f(w()/2,(h()-(100*i/2)-20)-40 + swipe_y);
		glEnd();
		glPopMatrix();
		glColor4f(1.0, 1.0, 1.0, 1.0);
	}
	
}

void MyGlWindow::drawBackground(){
		glPushMatrix();
   		glEnable(GL_TEXTURE_2D);
			glColor3f(1.0, 1, 1.0);
   	   /* create a square on the XY
   	      note that OpenGL origin is at the lower left
   	      but texture origin is at upper left
   	      => it has to be mirrored
   	      (gasman knows why i have to mirror X as well) */
			//GLint m_viewport[4];
			//glGetIntegerv( GL_VIEWPORT, m_viewport );
			int x = 0;
			int y = 0;			
			for(int i=0; i<(abs(main_coord_x)/512 + 4);i++){	
				y = 0;
				for(int j=0;j<(abs(main_coord_y)/512 + 4);j++){
				glBindTexture(GL_TEXTURE_2D, tex_terrain);
					glBegin(GL_QUADS);
   	    		glTexCoord2d(0.0, 0.0); glVertex2d(x - 512, y - 512);
   	   		glTexCoord2d(1, 0.0); glVertex2d(x + 512, y - 512);
   	   		glTexCoord2d(1, 1); glVertex2d(x + 512, y + 512);
   	   		glTexCoord2d(0.0, 1); glVertex2d(x - 512, y + 512);
   	   	glEnd();
				glBindTexture(GL_TEXTURE_2D, tex_terrain);
				glBegin(GL_QUADS);
   	    		glTexCoord2d(0.0, 0.0); glVertex2d(x - 512, -y - 512);
   	   		glTexCoord2d(1, 0.0); glVertex2d(x + 512, -y - 512);
   	   		glTexCoord2d(1, 1); glVertex2d(x + 512, -y + 512);
   	   		glTexCoord2d(0.0, 1); glVertex2d(x - 512, -y + 512);
   	   	glEnd();
				glBindTexture(GL_TEXTURE_2D, tex_terrain);
				glBegin(GL_QUADS);
   	    		glTexCoord2d(0.0, 0.0); glVertex2d(-x - 512, y - 512);
   	   		glTexCoord2d(1, 0.0); glVertex2d(-x + 512, y - 512);
   	   		glTexCoord2d(1, 1); glVertex2d(-x + 512, y + 512);
   	   		glTexCoord2d(0.0, 1); glVertex2d(-x - 512, y + 512);
   	   	glEnd();
				glBindTexture(GL_TEXTURE_2D, tex_terrain);
				glBegin(GL_QUADS);
   	    		glTexCoord2d(0.0, 0.0); glVertex2d(-x - 512, -y - 512);
   	   		glTexCoord2d(1, 0.0); glVertex2d(-x + 512, -y - 512);
   	   		glTexCoord2d(1, 1); glVertex2d(-x + 512, -y + 512);
   	   		glTexCoord2d(0.0, 1); glVertex2d(-x - 512, -y + 512);
   	   	glEnd();
					y+=512;
				}
				x += 512;
			}
   	  	glDisable(GL_TEXTURE_2D);
		glPopMatrix();
}

void MyGlWindow::drawPump(float x, float y){
	glPushMatrix();
   	glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex_pump);
		glColor3f(1.0, 1, 1.0);
      /* create a square on the XY
         note that OpenGL origin is at the lower left
         but texture origin is at upper left
         => it has to be mirrored
         (gasman knows why i have to mirror X as well) */
      glBegin(GL_QUADS);
       	glTexCoord2d(0.0,0.0); glVertex2d(-20.0 + x, -20.0 + y);
      	glTexCoord2d(1.0,0.0); glVertex2d(+20.0 + x, -20.0 + y);
      	glTexCoord2d(1.0,1.0); glVertex2d(+20.0 + x, +20.0 + y);
      	glTexCoord2d(0.0,1.0); glVertex2d(-20.0 + x, +20.0 + y);
      glEnd();
         
     	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

int MyGlWindow::handle(int event){
	
	switch (event){
	case FL_PUSH:
		click_x = Fl::event_x();
		click_y = Fl::event_y();
		redraw();
		return 1;
	case FL_DRAG:
		dx = Fl::event_x() - click_x;
		dy = Fl::event_y() - click_y;
		if(map!=NULL && map->getDim()>1){
			if((click_x >= w()/1.33) && (click_x <= w())){
				if((click_y >= 0) && (click_y <= h())){
					swipe_y -= dy;
					if(swipe_y>50)swipe_y = 50;
				}
			}else{
				main_coord_x += dx;
				main_coord_y -= dy;
			}
		}
		click_x = Fl::event_x();
		click_y = Fl::event_y();
		
		
		redraw();
		return 1;
	case FL_MOUSEWHEEL:	// zoom commands
		if (Fl::event_dy()<=0 || Fl::event_dy()>=0)	//	zoom in
			zoom_factor -= (GLfloat)(Fl::event_dy() / 10.0f);
		if (zoom_factor <= 0.1f)
			zoom_factor = 0.1f;
		else if(zoom_factor >= 1.9)
			zoom_factor = 1.9;
		redraw();
		return 1;
	default:
		return Fl_Gl_Window::handle(event);
	}
}

GLuint MyGlWindow::loadTexture(std::string filename, int width, int height){
	//header for testing if it is a png
	png_byte header[8];
   
	//open file as binary
	FILE *fp = fopen(filename.c_str(), "rb");
	if (!fp) {
		return TEXTURE_LOAD_ERROR;
	}
   
   //read the header
   fread(header, 1, 8, fp);
   
   //test if png
   int is_png = !png_sig_cmp(header, 0, 8);
   if (!is_png) {
     fclose(fp);
     return TEXTURE_LOAD_ERROR;
   }
   
   //create png struct
   png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
       NULL, NULL);
   if (!png_ptr) {
     fclose(fp);
     return (TEXTURE_LOAD_ERROR);
   }
   
   //create png info struct
   png_infop info_ptr = png_create_info_struct(png_ptr);
   if (!info_ptr) {
     png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
     fclose(fp);
     return (TEXTURE_LOAD_ERROR);
   }
 
   //create png info struct
   png_infop end_info = png_create_info_struct(png_ptr);
   if (!end_info) {
     png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
     fclose(fp);
     return (TEXTURE_LOAD_ERROR);
   }
 
   //png error stuff, not sure libpng man suggests this.
   if (setjmp(png_jmpbuf(png_ptr))) {
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
     return (TEXTURE_LOAD_ERROR);
   }
 
   //init png reading
   png_init_io(png_ptr, fp);
   
   //let libpng know you already read the first 8 bytes
   png_set_sig_bytes(png_ptr, 8);
 
   // read all the info up to the image data
   png_read_info(png_ptr, info_ptr);
 
   //variables to pass to get info
   int bit_depth, color_type;
   png_uint_32 twidth, theight;
 
   // get info about png
   png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
       NULL, NULL, NULL);
 
   //update width and height based on png info
   width = twidth;
   height = theight;
 
   // Update the png info struct.
   png_read_update_info(png_ptr, info_ptr);
 
   // Row size in bytes.
   int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
 
   // Allocate the image_data as a big block, to be given to opengl
   png_byte *image_data = new png_byte[rowbytes * height];
   if (!image_data) {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
     return TEXTURE_LOAD_ERROR;
   }
 
   //row_pointers is for pointing to image_data for reading the png with libpng
   png_bytep *row_pointers = new png_bytep[height];
   if (!row_pointers) {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     delete[] image_data;
     fclose(fp);
     return TEXTURE_LOAD_ERROR;
   }
   // set the individual row_pointers to point at the correct offsets of image_data
   for (int i = 0; i < height; ++i)
     row_pointers[height - 1 - i] = image_data + i * rowbytes;
 
   //read the png into image_data through row_pointers
   png_read_image(png_ptr, row_pointers);
 
   //Now generate the OpenGL texture object
   GLuint texture;
   glGenTextures(1, &texture);
   glBindTexture(GL_TEXTURE_2D, texture);
   glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, width, height, 0,
       GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*) image_data);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

 
   //clean up memory and close stuff
   png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
   delete[] image_data;
   delete[] row_pointers;
   fclose(fp);
 
   return texture;
 }

/*
Function for drawing a line using points
*/
void MyGlWindow::drawLine(float x1, float y1, float x2, float y2, int samples, Nodo *a, Nodo *b){
	float dx, dy, x;
	double hmax, hpos, r, max_r, min_r, c, max_c, min_c;
	dx = - (x1 - x2) / (float)samples;
	dy = - (y1 - y2) / (float)samples;
	c = 0;

	for (int i = 0; i < samples; i++){

		r = std::abs(map->getCanaleFlowSection(a, b, i));
		max_r = 2;
		min_r = 0;
		if(view == VIEW_1)
			c = map->getCanaleHeightSection(a, b, i);
		else if(view == VIEW_2){
			c = map->getCanaleInclinationSection(a,b);
		}
		max_c = 2;
		min_c = 1;


		x = (float) ( ( c - min_c ) / ( max_c - min_c) );
		if (x<0) x=0;
		if (x>1) x=1;
		glColor3f(x, 0, 1 - x);
		x = (float) ( ( r - min_r ) / ( max_r - min_r ));
		x *= 10;
		x += 2;
		drawFilledCircle(x1+i*dx, y1+i*dy, x);
	}
}

/*
Function for drawing a filled circle
*/
void MyGlWindow::drawFilledCircle(float x1, float y1, double radius){
	float x2, y2, angle;
	
	glPushMatrix();
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle=1.0f; angle<361.0f; angle+=0.2){
		x2 = x1 + sin(angle)*radius;
		y2 = y1 + cos(angle)*radius;
		glVertex2f(x2, y2);
	}

	glEnd();
	glPopMatrix();
}

/*
Function to draw the graph
*/
void MyGlWindow::drawMap(){
	if(map->getDim()>0){
		std::vector<Nodo*> next_node;
		std::vector<ElemGraph*> all;
		map->setFalseVisitato();
		all = map->getAllNodeList();
		drawDFS(all[0], all[0]);
	}
}

void MyGlWindow::drawDFS(ElemGraph *prec, ElemGraph *pres){
	if(map->isVisitato(pres->getId()) == false){
		std::vector<ElemGraph*> adj;
		adj = map->getAdjs(pres);
		map->setVisitato(pres->getId());		
		drawPump(pres->getElementPointer()->x_pos, pres->getElementPointer()->y_pos);
		for(int i=0; i<adj.size(); i++){
			drawDFS(pres, adj[i]);
		}
	}else{
	int n_sect = map->getNSections(prec->getElementPointer(),pres->getElementPointer());
	drawLine(pres->getElementPointer()->x_pos, pres->getElementPointer()->y_pos, prec->getElementPointer()->x_pos, prec->getElementPointer()->y_pos, n_sect, pres->getElementPointer(), prec->getElementPointer());
	std::string str = map->getCanaleName(pres->getElementPointer(), prec->getElementPointer());
	char *c = new char[str.length() + 1];
	strcpy(c, str.c_str());
	float x1,x2,y1,y2;
	x1 = pres->getElementPointer()->x_pos;
	x2 = prec->getElementPointer()->x_pos;
	y1 = pres->getElementPointer()->y_pos;
	y2 = prec->getElementPointer()->y_pos;
	font_index = 0;
   glColor4f(1.0, 0.0, 0.0, 1.0);
	drawString(c, ((x1+x2)/2), ((y1+y2)/2));
	delete [] c;
	}
}

void MyGlWindow::setMap(Graph *g){
	map = g;
	std::cout << "map referenced" << std::endl;
}

void MyGlWindow::drawString(char *string, float x, float y){
   unsigned int i, j;
   unsigned int count;
   void* bitmap_fonts[7] = {
      GLUT_BITMAP_9_BY_15,
      GLUT_BITMAP_8_BY_13,
      GLUT_BITMAP_TIMES_ROMAN_10,
      GLUT_BITMAP_TIMES_ROMAN_24,
      GLUT_BITMAP_HELVETICA_10,
      GLUT_BITMAP_HELVETICA_12,
      GLUT_BITMAP_HELVETICA_18     
   };

   char* bitmap_font_names[7] = {
      "GLUT_BITMAP_9_BY_15",
      "GLUT_BITMAP_8_BY_13",
      "GLUT_BITMAP_TIMES_ROMAN_10",
      "GLUT_BITMAP_TIMES_ROMAN_24",
      "GLUT_BITMAP_HELVETICA_10",
      "GLUT_BITMAP_HELVETICA_12",
      "GLUT_BITMAP_HELVETICA_18"     
   };

   void* stroke_fonts[2] = {
      GLUT_STROKE_ROMAN,
      GLUT_STROKE_MONO_ROMAN
   };

   char* stroke_font_names[2] = {
      "GLUT_STROKE_ROMAN",
      "GLUT_STROKE_MONO_ROMAN"
   };

   GLfloat ystep, yild, stroke_scale;

   /* Draw the strings, according to the current mode and font. */
   //ystep  = 100.0;
   //yild   = 20.0;
   if (mode == MODE_BITMAP) {
      //glRasterPos2f(x, y);
      /*print_bitmap_string(
         bitmap_fonts[font_index], bitmap_font_names[font_index]);/*/
      /*for (j=0; j<1; j++) {
         //glRasterPos2f(x, y);
         print_bitmap_string(bitmap_fonts[font_index], string[j]);
      }*/
			//glMatrixMode(GL_PROJECTION);
			//glLoadIdentity();
			glPushMatrix();
			//printf("median x=%f y=%f w=%d h%d\n",x,y,w(),h());
			
			//printf("rasterized %f %f\n",x+(main_coord_x/2), y+main_coord_y/2);
			//glTranslatef(main_coord_x,main_coord_y,0);
			glRasterPos2f(x, y);
			//glRasterPos2f(0,0);
         print_bitmap_string(bitmap_fonts[font_index], string);
         glPopMatrix();
   }
   else {/*
      stroke_scale = 0.1f;
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix(); {
         glTranslatef(x, y+1.25*yild, 0.0);
         glScalef(stroke_scale, stroke_scale, stroke_scale);
         print_stroke_string(
            stroke_fonts[font_index], stroke_font_names[font_index]);
      } glPopMatrix();
      glPushMatrix(); {
         glTranslatef(x, y, 0.0);
         for (j=0; j<1; j++) {
            glPushMatrix(); {
               glScalef(stroke_scale, stroke_scale, stroke_scale);
               print_stroke_string(stroke_fonts[font_index], string[j]);
            } glPopMatrix();
            glTranslatef(0.0, -yild, 0.0);
         }
         glTranslatef(0.0, -ystep, 0.0);
      } glPopMatrix();*/
   }
}

void MyGlWindow::print_bitmap_string(void* font, char* s){
   if (s && strlen(s)) {
      while (*s) {
         glutBitmapCharacter(font, *s);
         s++;
      }
   }
}

void MyGlWindow::print_stroke_string(void* font, char* s){
   if (s && strlen(s)) {
      while (*s) {
         glutStrokeCharacter(font, *s);
         s++;
      }
   }
}

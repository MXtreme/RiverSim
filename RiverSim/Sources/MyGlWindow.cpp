#include "MyGlWindow.h"

//extern auto t_start;	

MyGlWindow::MyGlWindow(int X, int Y, int W, int H, const char *L) : Fl_Gl_Window(X, Y, W, H, L){
	zoom_factor = 1;
	main_coord_x = 0.0f;
	main_coord_y = 0.0f;
	main_coord_z = 0.0f;
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
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clean  buffers
	glClearColor(0, 0.0f, 0, 1.0f);	// background color
	glOrtho(-w()*zoom_factor, w()*zoom_factor, -h()*zoom_factor, h()*zoom_factor, -1, 1);
	glTranslatef(main_coord_x, main_coord_y, main_coord_z);
	drawBackground();	
	drawPump(0, 0);
	if(map!=NULL)drawMap();	//disegna il grafo     
   glFlush();
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
		click_x = Fl::event_x();
		click_y = Fl::event_y();
		main_coord_x += dx;
		main_coord_y -= dy;
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
	//std::cout << "Canale " << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
	float dx = - (x1 - x2) / (float)samples;
	float dy = - (y1 - y2) / (float)samples;
	
	//bool draw_h = false;
	//double prop = (samples*hpos)/100;
	float x;
	double hmax, hpos;
	//map->getCanaleHeightPosition(a,b);

	for (int i = 0; i < samples; i++){
		/*if(!draw_h && i >= prop){
			draw_h = true;
			glColor3f(1, 1, 1);
			drawFilledCircle(x1+i*dx, y1+i*dy, 5);
			glColor3f(0, 0.5f, 0.8f);
		}else{
			glPushMatrix();
			glBegin(GL_POINTS);
			glVertex2f(x1+i*dx, y1+i*dy);
			glEnd();
			glPopMatrix();
		}*/
		//printf("%s %s\n",a->name.c_str(), b->name.c_str());
		//color = 1/(map->getCanaleHeight(a,b)*hpos);
		//printf("color %f %f %f %d\n",color, hpos, map->getCanaleHeight(a,b), i);
		/*glPushMatrix();
		glBegin(GL_POINTS);
		glVertex2f(x1+i*dx, y1+i*dy);
		glEnd();
		glPopMatrix();*/

		double r = std::abs(map->getCanaleFlowSection(a, b, i));
		double max_r = 2; //map->getMaxFlow();
		double min_r = 0;
		double c = map->getCanaleHeightSection(a, b, i);
		double max_c = 2; //map->getMaxHeight();
		double min_c = 1;


		x = (float) ( ( c - min_c ) / ( max_c - min_c) );
		if (x<0) x=0;
		if (x>1) x=1;
		glColor3f(x, 0, 1 - x);
		x = (float) ( ( r - min_r ) / ( max_r - min_r ));
		x *= 10;
		x += 2;
		drawFilledCircle(x1+i*dx, y1+i*dy, x);
//		glLineWidth(x);
//		glPushMatrix();
//		glBegin(GL_LINES);
//		glVertex2f(x1+(i-1)*dx, y1+(i-1)*dy);
//		glVertex2f(x1+i*dx, y1+i*dy);
//		glEnd();
//		glPopMatrix();
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
	std::vector<Nodo*> next_node;
	std::vector<ElemGraph*> all=map->getAllNodeList();

	for(size_t i=0;i<all.size();i++)
		all[i]->getElementPointer()->setVisitato(false);

	for(size_t i=0;i<all.size();i++){
		all[i]->getElementPointer()->setVisitato(true);
		drawPump(all[i]->getElemento().x_pos, all[i]->getElemento().y_pos);
		next_node=map->DFS(all[i]);
		/*printf("Nome nodo: %s\n", all[i]->getElemento().name.c_str());
		printf("Adiacenti:\n");*/
		for(size_t k=0;k<next_node.size();k++){
			//printf("- %s\n", next_node[k]->name.c_str());
			//if(next_node[k]->getVisitato() == false){
				next_node[k]->setVisitato(true);		
				//glColor3f(0, 0.5f, 0.8f);
				int n_sect = map->getNSections(all[i]->getElementPointer(),next_node[k]);
				drawLine(all[i]->getElemento().x_pos, all[i]->getElemento().y_pos, next_node[k]->x_pos, next_node[k]->y_pos, n_sect, all[i]->getElementPointer(), next_node[k]);
				drawPump(next_node[k]->x_pos, next_node[k]->y_pos);
			//}
		}
	}
}

void MyGlWindow::setMap(Graph *g){
	map = g;
	std::cout << "map referenced" << std::endl;
}

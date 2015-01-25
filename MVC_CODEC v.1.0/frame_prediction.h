
void full_search(unsigned char *pre, unsigned char *cur,struct  motionVectors mv[HEIGHT*WIDTH/(BLOCK_H*BLOCK_W)], int sad_array[HEIGHT*WIDTH/(BLOCK_W*BLOCK_H)]);
void compare(struct motionVectors mv[HEIGHT*WIDTH/(BLOCK_H*BLOCK_W)]);
void predictFrame(unsigned char *pre, unsigned char *next, unsigned char *predictedFrame, motionVectors mv[HEIGHT*WIDTH/(BLOCK_H*BLOCK_W)]);
void predictBiFrame(unsigned char *pre, unsigned char *next, unsigned char *predictedFrame, motionVectors mv[HEIGHT*WIDTH/(BLOCK_H*BLOCK_W)]);

struct motionVectors{
	signed char x;
	signed char y;
	bool isPrevious;
};

struct motionVectors mv_for_prev[(HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W)];
struct motionVectors mv_for_next[(HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W)];
struct motionVectors mv_for_save[(HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W)];


int sad_p[HEIGHT*WIDTH/(BLOCK_W*BLOCK_H)];
int sad_n[HEIGHT*WIDTH/(BLOCK_W*BLOCK_H)];

int finding_motion_vectors(char *left_video, char *right_video)
{
	#pragma warning (disable : 4996);
	FILE *fr;
	FILE *fw;
	FILE *fr1;
	fr = fopen(left_video,"rb");
	if(fr==NULL){
		printf("Failed to open");
		getch();
		return 0;
	}
	fr1 = fopen(right_video,"rb");
	if(fr1==NULL){
		printf("Failed to open");
		getch();
		return 0;
	}

	fw = fopen("raw_1.yuv", "wb");
	if(fw==NULL){
		printf("Failed to open");
		getch();
		return 0;
	}
	
	unsigned char *cur_frame = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	unsigned char *U = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	unsigned char *V = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	
	unsigned char *cur_frame1 = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	unsigned char *U1 = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	unsigned char *V1 = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	
	unsigned char *cur_frame2 = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	unsigned char *U2 = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	unsigned char *V2 = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	
	unsigned char *cur_frame3 = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	unsigned char *U3 = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	unsigned char *V3 = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	
	unsigned char *cur_frame4 = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	unsigned char *U4 = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	unsigned char *V4 = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	
	unsigned char *cur_frame5 = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	unsigned char *U5 = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	unsigned char *V5 = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	
	unsigned char *cur_frame6 = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	unsigned char *U6 = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	unsigned char *V6= (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	
	unsigned char *cur_frame7 = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	unsigned char *U7 = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	unsigned char *V7= (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	
	unsigned char *p_frame = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	unsigned char *p_U = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	unsigned char *p_V = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	
	unsigned char *p1_frame = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	unsigned char *p1_U = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	unsigned char *p1_V = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	
	unsigned char *p2_frame = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	unsigned char *p2_U = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	unsigned char *p2_V = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	




	

	for(int no=0; no<(FRAMES/5); no++){
		
		fread(cur_frame, sizeof(unsigned char), HEIGHT*WIDTH, fr);
		fread(U, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);
		fread(V, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);
		
		fread(cur_frame1, sizeof(unsigned char), HEIGHT*WIDTH, fr);
		fread(U1, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);
		fread(V1, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);

		fread(cur_frame2, sizeof(unsigned char), HEIGHT*WIDTH, fr);
		fread(U2, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);
		fread(V2, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);
		
		fread(cur_frame3, sizeof(unsigned char), HEIGHT*WIDTH, fr);
		fread(U3, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);
		fread(V3, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);
		
		fread(cur_frame4, sizeof(unsigned char), HEIGHT*WIDTH, fr);
		fread(U4, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);
		fread(V4, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);
		
		fwrite(cur_frame, sizeof(unsigned char), HEIGHT*WIDTH, fw);
		fwrite(U, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		fwrite(V, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		
		fwrite(cur_frame4, sizeof(unsigned char), HEIGHT*WIDTH, fw);
		fwrite(U4, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		fwrite(V4, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		
		

		full_search(cur_frame, cur_frame2, mv_for_prev, sad_p);
		full_search(cur_frame4, cur_frame2, mv_for_next, sad_n);
		compare(mv_for_save);

		fwrite(mv_for_save, sizeof(struct motionVectors), ((HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W)), fw);
		fwrite(U2, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		fwrite(V2, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		

		predictBiFrame(cur_frame, cur_frame4, p_frame, mv_for_save);

		full_search(cur_frame, cur_frame1, mv_for_prev, sad_p);
		full_search(p_frame, cur_frame1, mv_for_next, sad_n);
		compare(mv_for_save);
		
		fwrite(mv_for_save, sizeof(struct motionVectors), ((HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W)), fw);
		fwrite(U1, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		fwrite(V1, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		
		full_search(p_frame, cur_frame3, mv_for_prev, sad_p);
		full_search(cur_frame4, cur_frame3, mv_for_next, sad_n);
		compare(mv_for_save);
		
		fwrite(mv_for_save, sizeof(struct motionVectors), ((HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W)), fw);
		fwrite(U3, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		fwrite(V3, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		
		// For Right View

		fread(cur_frame5, sizeof(unsigned char), HEIGHT*WIDTH, fr1);
		fread(U5, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr1);
		fread(V5, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr1);
		
		fread(cur_frame2, sizeof(unsigned char), HEIGHT*WIDTH, fr1);
		fread(U2, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr1);
		fread(V2, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr1);
		
		fread(cur_frame3, sizeof(unsigned char), HEIGHT*WIDTH, fr1);
		fread(U3, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr1);
		fread(V3, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr1);
		
		fread(cur_frame7, sizeof(unsigned char), HEIGHT*WIDTH, fr1);
		fread(U7, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr1);
		fread(V7, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr1);


		fread(cur_frame6, sizeof(unsigned char), HEIGHT*WIDTH, fr1);
		fread(U6, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr1);
		fread(V6, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr1);
		
		full_search(cur_frame, cur_frame5, mv_for_prev, sad_p);
		predictFrame(cur_frame, NULL, p1_frame, mv_for_prev);
		
		fwrite(mv_for_prev, sizeof(struct motionVectors), ((HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W)), fw);
		fwrite(U5, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		fwrite(V5, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		
		full_search(cur_frame4, cur_frame6, mv_for_prev, sad_p);
		predictFrame(cur_frame4, NULL, p2_frame, mv_for_prev);

		fwrite(mv_for_prev, sizeof(struct motionVectors), ((HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W)), fw);
		fwrite(U6, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		fwrite(V6, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		
		
		full_search(p1_frame, cur_frame3, mv_for_prev, sad_p);
		full_search(p2_frame, cur_frame3, mv_for_next, sad_n);
		compare(mv_for_save);

		fwrite(mv_for_save, sizeof(struct motionVectors), ((HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W)), fw);
		fwrite(U3, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		fwrite(V3, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		
		predictBiFrame(p1_frame, p2_frame, p_frame, mv_for_save);

		full_search(p1_frame, cur_frame2, mv_for_prev, sad_p);
		full_search(p_frame, cur_frame2, mv_for_next, sad_n);
		compare(mv_for_save);

		fwrite(mv_for_save, sizeof(struct motionVectors), ((HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W)), fw);
		fwrite(U2, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		fwrite(V2, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		
		full_search(p_frame, cur_frame7, mv_for_prev, sad_p);
		full_search(p2_frame, cur_frame7, mv_for_next, sad_n);
		compare(mv_for_save);

		fwrite(mv_for_save, sizeof(struct motionVectors), ((HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W)), fw);
		fwrite(U7, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		fwrite(V7, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		
		//prev = next;
		cout<<"Encoded GOP No: "<<no<<endl;
	}
	cout<<endl<<"Finish Encoded frames...";
	
	free(cur_frame);
	free(U);
	free(V);
	fclose(fr);
	fclose(fw);

	getch();
	return 0;
}





int reconstruct_frames()
{
	#pragma warning (disable : 4996);
	char nameOfVideo[100];
	FILE *fr;
	FILE *fw;
	FILE *fr1;

	
	fr = fopen("raw_1.yuv","rb");
	if(fr==NULL){
		printf("Failed to open");
		getch();
		return 0;
	}
	
	fw = fopen("p_left.yuv", "wb");
	if(fw==NULL){
		printf("Failed to open");
		getch();
		return 0;
	}
	

	FILE *fw1 = fopen("p_right.yuv", "wb");
	if(fw1==NULL){
		printf("Failed to open");
		getch();
		return 0;
	}
	

	unsigned char *cur_frame = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	unsigned char *U = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	unsigned char *V = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	
	unsigned char *cur_frame1 = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	unsigned char *U1 = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	unsigned char *V1 = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	
	unsigned char *cur_frame2 = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	unsigned char *U2 = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	unsigned char *V2 = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	
	unsigned char *cur_frame3 = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	unsigned char *U3 = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	unsigned char *V3 = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	
	unsigned char *cur_frame4 = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	unsigned char *U4 = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	unsigned char *V4 = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	
	unsigned char *p_frame = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	unsigned char *p_U = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	unsigned char *p_V = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	
	unsigned char *p1_frame = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	unsigned char *p1_U = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	unsigned char *p1_V = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	
	unsigned char *p2_frame = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	unsigned char *p2_U = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	unsigned char *p2_V = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	




	

	for(int no=0; no<(FRAMES/5); no++){
		
		fread(cur_frame, sizeof(unsigned char), HEIGHT*WIDTH, fr);
		fread(U, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);
		fread(V, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);
		
		fread(cur_frame4, sizeof(unsigned char), HEIGHT*WIDTH, fr);
		fread(U4, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);
		fread(V4, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);

		fwrite(cur_frame, sizeof(unsigned char), HEIGHT*WIDTH, fw);
		fwrite(U, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		fwrite(V, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		
		fread(mv_for_save, sizeof(struct motionVectors), ((HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W)), fr);
		fread(U2, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);
		fread(V2, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);

		predictBiFrame(cur_frame, cur_frame4, cur_frame2, mv_for_save);

		fread(mv_for_save, sizeof(struct motionVectors), ((HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W)), fr);
		fread(U1, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);
		fread(V1, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);

		predictBiFrame(cur_frame, cur_frame2, cur_frame1, mv_for_save);

		fread(mv_for_save, sizeof(struct motionVectors), ((HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W)), fr);
		fread(U3, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);
		fread(V3, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);

		predictBiFrame(cur_frame2, cur_frame4, cur_frame3, mv_for_save);

		fwrite(cur_frame1, sizeof(unsigned char), HEIGHT*WIDTH, fw);
		fwrite(U1, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		fwrite(V1, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		
		fwrite(cur_frame2, sizeof(unsigned char), HEIGHT*WIDTH, fw);
		fwrite(U2, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		fwrite(V2, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		
		fwrite(cur_frame3, sizeof(unsigned char), HEIGHT*WIDTH, fw);
		fwrite(U3, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		fwrite(V3, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		
		fwrite(cur_frame4, sizeof(unsigned char), HEIGHT*WIDTH, fw);
		fwrite(U4, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		fwrite(V4, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw);
		
		fread(mv_for_save, sizeof(struct motionVectors), ((HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W)), fr);
		fread(U, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);
		fread(V, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);

		predictFrame(cur_frame, NULL, p1_frame, mv_for_save);

		fwrite(p1_frame, sizeof(unsigned char), HEIGHT*WIDTH, fw1);
		fwrite(U, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw1);
		fwrite(V, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw1);
		
		fread(mv_for_save, sizeof(struct motionVectors), ((HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W)), fr);
		fread(U4, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);
		fread(V4, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);

		predictFrame(cur_frame4, NULL, p2_frame, mv_for_save);

		fread(mv_for_save, sizeof(struct motionVectors), ((HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W)), fr);
		fread(U2, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);
		fread(V2, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);

		predictBiFrame(p1_frame, p2_frame, cur_frame2, mv_for_save);

		fread(mv_for_save, sizeof(struct motionVectors), ((HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W)), fr);
		fread(U1, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);
		fread(V1, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);


		predictBiFrame(p1_frame, cur_frame2, cur_frame1, mv_for_save);

		fread(mv_for_save, sizeof(struct motionVectors), ((HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W)), fr);
		fread(U3, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);
		fread(V3, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fr);

		predictBiFrame(cur_frame2, p2_frame, cur_frame3, mv_for_save);

		fwrite(cur_frame1, sizeof(unsigned char), HEIGHT*WIDTH, fw1);
		fwrite(U1, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw1);
		fwrite(V1, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw1);
		
		fwrite(cur_frame2, sizeof(unsigned char), HEIGHT*WIDTH, fw1);
		fwrite(U2, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw1);
		fwrite(V2, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw1);
		
		fwrite(cur_frame3, sizeof(unsigned char), HEIGHT*WIDTH, fw1);
		fwrite(U3, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw1);
		fwrite(V3, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw1);
		
		fwrite(p2_frame, sizeof(unsigned char), HEIGHT*WIDTH, fw1);
		fwrite(U4, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw1);
		fwrite(V4, sizeof(unsigned char), (HEIGHT*WIDTH>>2), fw1);
		

		//prev = next;
		cout<<"Encoded GOP No: "<<no<<endl;
	}
	cout<<endl<<"Finish Reconstraction"<<endl;
	
	free(cur_frame);
	free(U);
	free(V);
	fclose(fr);
	fclose(fw);

	getch();
	return 0;
}




void full_search(unsigned char *pre, unsigned char *cur, motionVectors mv[HEIGHT*WIDTH/(BLOCK_H*BLOCK_W)], int sad_array[HEIGHT*WIDTH/(BLOCK_W*BLOCK_H)]){
	int count1 = 0, x = 0, y = 0, count2 = 0, val = 0;
	int sad[SEARCH_WIN];
	int least = 0;

	for(x=0; x<WIDTH; x+=BLOCK_W){
		
		for(y=0; y<HEIGHT; y+=BLOCK_H){
			
			for(int m=y-D; m<=y+D; m++){
				
				for(int n=x-D; n<=x+D; n++){
					
					if((m<0) || (n<0) || (n>(WIDTH-BLOCK_W)) || (m>(HEIGHT-BLOCK_H))){
						continue;
					}
					sad[count1] = 0;
					for(int i=0; i<BLOCK_W; i++){
						
						for(int j=0; j<BLOCK_H; j++){
							
							 sad[count1] += abs(cur[(y+i)*WIDTH+x+j]-pre[(m+i)*WIDTH+n+j]);
						}
					}
					
					if(count1 == 0){
						least = sad[count1];
						mv[count2].x = n - x;
						mv[count2].y = m - y;
					} else if (sad[count1]<least){
						least = sad[count1];
						mv[count2].x = n - x;
						mv[count2].y = m - y;
					}
					count1++;
				}
			}
			
			sad_array[count2] = least;
			count2++;
			count1 = 0;
			least = 0;
			
		}
	}
}




void compare(struct motionVectors mv[HEIGHT*WIDTH/(BLOCK_H*BLOCK_W)]){
	int count = 0;
	for(int x=0; x<WIDTH; x+=BLOCK_W){
		
		for(int y=0; y<HEIGHT; y+=BLOCK_H){
			
			if(sad_p[count] < sad_n[count]){
				mv[count].x = mv_for_prev[count].x;
				mv[count].y = mv_for_prev[count].y;
				mv[count].isPrevious = 1;
			} else {
				mv[count].x = mv_for_next[count].x;
				mv[count].y = mv_for_next[count].y;
				mv[count].isPrevious = 0;
			} // end of if - else
			count++;
		} // end of loop for y 
	} // end of loop for x
}




void predictFrame(unsigned char *pre, unsigned char *next, unsigned char *predictedFrame, motionVectors mv[HEIGHT*WIDTH/(BLOCK_H*BLOCK_W)]){
	int count = 0;
	for(int x=0; x<WIDTH; x+=BLOCK_W){
		
		for(int y=0; y<HEIGHT; y+=BLOCK_H){
			
			for(int i=0; i<BLOCK_W; i++){
					
					for(int j=0; j<BLOCK_H; j++){
						predictedFrame[(x+i)+(WIDTH*(y+j))] = pre[(x+i+mv[count].x)+(WIDTH*(y+j+mv[count].y))];
					}
				}
			count++;
		} // end of loop for y 
	} // end of loop for x
}


void predictBiFrame(unsigned char *pre, unsigned char *next, unsigned char *predictedFrame, motionVectors mv[HEIGHT*WIDTH/(BLOCK_H*BLOCK_W)]){
	int count = 0;
	for(int x=0; x<WIDTH; x+=BLOCK_W){
		
		for(int y=0; y<HEIGHT; y+=BLOCK_H){
			
			if(mv[count].isPrevious){
				for(int i=0; i<BLOCK_W; i++){
					
						for(int j=0; j<BLOCK_H; j++){
							predictedFrame[(x+i)+(WIDTH*(y+j))] = pre[(x+i+mv[count].x)+(WIDTH*(y+j+mv[count].y))];
						}
					}
			} else {
				for(int i=0; i<BLOCK_W; i++){
					
						for(int j=0; j<BLOCK_H; j++){
							predictedFrame[(x+i)+(WIDTH*(y+j))] = next[(x+i+mv[count].x)+(WIDTH*(y+j+mv[count].y))];
						}
					}
			}
			count++;
		} // end of loop for y 
	} // end of loop for x
}

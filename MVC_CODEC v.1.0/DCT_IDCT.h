
motionVectors mv[(HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W)];

int my_own_round(float num);
void dct_block(unsigned char *cur, signed char *dct_frame, int height_frame, int width_frame);
void create_coeff();
void idct_block(signed char *cur, unsigned char *idct_frame, int height_frame, int width_frame);

double coeff[BLOCK_H][BLOCK_W];
double quantMatrix[BLOCK_H][BLOCK_W];

int apply_DCT()
{
	FILE *fr;
	FILE *fw;
	FILE *fNew;
	for(int i=0; i<BLOCK_SIZE; i++){
		for(int j=0; j<BLOCK_SIZE; j++){
			quantMatrix[i][j] = 30;
		}
	}

	create_coeff();
	#pragma warning (disable : 4996);
	fr = fopen("raw_1.yuv","rb");
	if(fr==NULL){
		printf("Failed to open");
		getch();
		return 0;
	}
	
	fNew = fopen("raw_2.yuv", "wb");
	if(fNew==NULL){
		printf("Failed to open Second");
		getch();
		return 0;
	}

	unsigned char *cur_frame = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	unsigned char *cur_recon = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	signed char *dct_frame = (signed char*)malloc(HEIGHT*WIDTH*sizeof(signed char));
	signed char *dct_U = (signed char*)malloc((HEIGHT*WIDTH>>2)*sizeof(signed char));
	signed char *dct_V = (signed char*)malloc((HEIGHT*WIDTH>>2)*sizeof(signed char));
	unsigned char *U = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	unsigned char *V = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	

	for(int no=0; no<(FRAMES/5); no++){
		
		fread(cur_frame,HEIGHT*WIDTH,sizeof(unsigned char),fr);
		fread(U,(HEIGHT*WIDTH>>2),sizeof(unsigned char),fr);
		fread(V,(HEIGHT*WIDTH>>2),sizeof(unsigned char),fr);
		
		dct_block(cur_frame, dct_frame, HEIGHT, WIDTH);
		dct_block(U, dct_U, HEIGHT/2, WIDTH/2);
		dct_block(V, dct_V, HEIGHT/2, WIDTH/2);
		
		fwrite(dct_frame, HEIGHT*WIDTH, sizeof(signed char), fNew);
		fwrite(dct_U,(HEIGHT*WIDTH>>2),sizeof(signed char),fNew);
		fwrite(dct_V,(HEIGHT*WIDTH>>2),sizeof(signed char),fNew);

		fread(cur_frame,HEIGHT*WIDTH,sizeof(unsigned char),fr);
		fread(U,(HEIGHT*WIDTH>>2),sizeof(unsigned char),fr);
		fread(V,(HEIGHT*WIDTH>>2),sizeof(unsigned char),fr);
		
		dct_block(cur_frame, dct_frame, HEIGHT, WIDTH);
		dct_block(U, dct_U, HEIGHT/2, WIDTH/2);
		dct_block(V, dct_V, HEIGHT/2, WIDTH/2);
		
		fwrite(dct_frame, HEIGHT*WIDTH, sizeof(signed char), fNew);
		fwrite(dct_U,(HEIGHT*WIDTH>>2),sizeof(signed char),fNew);
		fwrite(dct_V,(HEIGHT*WIDTH>>2),sizeof(signed char),fNew);


		for(int i=0; i<8; i++){
			fread(mv, (HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W),sizeof(struct motionVectors),fr);
			fread(U,(HEIGHT*WIDTH>>2),sizeof(unsigned char),fr);
			fread(V,(HEIGHT*WIDTH>>2),sizeof(unsigned char),fr);

			dct_block(U, dct_U, HEIGHT/2, WIDTH/2);
			dct_block(V, dct_V, HEIGHT/2, WIDTH/2);

			fwrite(mv, (HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W),sizeof(struct motionVectors), fNew);
			fwrite(dct_U,(HEIGHT*WIDTH>>2),sizeof(signed char),fNew);
			fwrite(dct_V,(HEIGHT*WIDTH>>2),sizeof(signed char),fNew);
		}

		cout<<"GOP: "<<no<<endl;
	}
	cout<<"Finish Transformation";
	free(cur_frame);
	free(U);
	free(V);
	free(dct_U);
	free(dct_V);
	free(cur_recon);
	fclose(fr);
	fclose(fNew);

	getch();
	return 0;
}



int apply_IDCT()
{
	FILE *fr;
	FILE *fw;
	FILE *fNew;
	for(int i=0; i<BLOCK_SIZE; i++){
		for(int j=0; j<BLOCK_SIZE; j++){
			quantMatrix[i][j] = 30;
		}
	}

	create_coeff();
	#pragma warning (disable : 4996);
	fr = fopen("raw_2.yuv","rb");
	if(fr==NULL){
		printf("Failed to open");
		getch();
		return 0;
	}
	
	fNew = fopen("raw_1.yuv", "wb");
	if(fNew==NULL){
		printf("Failed to open Second");
		getch();
		return 0;
	}

	unsigned char *cur_frame = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	unsigned char *cur_recon = (unsigned char*)malloc(HEIGHT*WIDTH*sizeof(unsigned char));
	signed char *dct_frame = (signed char*)malloc(HEIGHT*WIDTH*sizeof(signed char));
	signed char *dct_U = (signed char*)malloc((HEIGHT*WIDTH>>2)*sizeof(signed char));
	signed char *dct_V = (signed char*)malloc((HEIGHT*WIDTH>>2)*sizeof(signed char));
	unsigned char *U = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	unsigned char *V = (unsigned char*)malloc((HEIGHT*WIDTH>>2)*sizeof(unsigned char));
	

	for(int no=0; no<(FRAMES/5); no++){
		
		fread(dct_frame,HEIGHT*WIDTH,sizeof(signed char),fr);
		fread(dct_U,(HEIGHT*WIDTH>>2),sizeof(signed char),fr);
		fread(dct_V,(HEIGHT*WIDTH>>2),sizeof(signed char),fr);
		
		idct_block(dct_frame, cur_frame, HEIGHT, WIDTH);
		idct_block(dct_U, U, HEIGHT/2, WIDTH/2);
		idct_block(dct_V, V, HEIGHT/2, WIDTH/2);

		fwrite(cur_frame, HEIGHT*WIDTH, sizeof(unsigned char), fNew);
		fwrite(U,(HEIGHT*WIDTH>>2),sizeof(unsigned char),fNew);
		fwrite(V,(HEIGHT*WIDTH>>2),sizeof(unsigned char),fNew);

		fread(dct_frame,HEIGHT*WIDTH,sizeof(signed char),fr);
		fread(dct_U,(HEIGHT*WIDTH>>2),sizeof(signed char),fr);
		fread(dct_V,(HEIGHT*WIDTH>>2),sizeof(signed char),fr);
		
		idct_block(dct_frame, cur_frame, HEIGHT, WIDTH);
		idct_block(dct_U, U, HEIGHT/2, WIDTH/2);
		idct_block(dct_V, V, HEIGHT/2, WIDTH/2);

		fwrite(cur_frame, HEIGHT*WIDTH, sizeof(unsigned char), fNew);
		fwrite(U,(HEIGHT*WIDTH>>2),sizeof(unsigned char),fNew);
		fwrite(V,(HEIGHT*WIDTH>>2),sizeof(unsigned char),fNew);


		for(int i=0; i<8; i++){
			fread(mv, (HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W),sizeof(struct motionVectors),fr);
			fread(dct_U,(HEIGHT*WIDTH>>2),sizeof(signed char),fr);
			fread(dct_V,(HEIGHT*WIDTH>>2),sizeof(signed char),fr);
		
			idct_block(dct_U, U, HEIGHT/2, WIDTH/2);
			idct_block(dct_V, V, HEIGHT/2, WIDTH/2);


			fwrite(mv, (HEIGHT*WIDTH)/(BLOCK_H*BLOCK_W),sizeof(struct motionVectors), fNew);
			fwrite(U,(HEIGHT*WIDTH>>2),sizeof(unsigned char),fNew);
			fwrite(V,(HEIGHT*WIDTH>>2),sizeof(unsigned char),fNew);

		}

		cout<<"GOP: "<<no<<endl;
	}
	cout<<endl<<"Finish Inverse Transformation";
	free(cur_frame);
	free(U);
	free(V);
	free(dct_U);
	free(dct_V);
	free(cur_recon);
	fclose(fr);
	fclose(fNew);

	getch();
	return 0;
}



void dct_block(unsigned char *cur, signed char *dct_frame, int height_frame, int width_frame){
	int count1 = 0, x = 0, y = 0, count2 = 0, val = 0;
	int least = 0;
	float samples[BLOCK_SIZE][BLOCK_SIZE]; 
	float dct[BLOCK_SIZE][BLOCK_SIZE];
	float var4 = 0, var5 = 0;
	float coeff_t[BLOCK_SIZE][BLOCK_SIZE];
	float new_dct[BLOCK_SIZE][BLOCK_SIZE];
	
	for(int i=0; i<BLOCK_SIZE; i++){
		for(int j=0; j<BLOCK_SIZE; j++){
			coeff_t[i][j] = coeff[j][i];
		}
	}

	for(y=0; y<height_frame; y+=BLOCK_H){
		
		for(x=0; x<width_frame; x+=BLOCK_W){
			
			for(int i=0; i<BLOCK_H; i++){

				for(int j=0; j<BLOCK_W; j++){
					samples[i][j] = cur[(x+j) + (width_frame*(i+y))];
				}
			}

			/* Applying DCT to the block of samples from the current frame */

			var4 = var5 = 0;
			for(int i=0; i<BLOCK_SIZE; i++){
		
				for (int j=0; j<BLOCK_SIZE; j++){
			
					for (int k=0; k<BLOCK_SIZE; k++){
						var4 += coeff[i][k]*samples[k][j];
					}
					dct[i][j] = var4;
					var4 = 0;
				}
			}

			var4 = 0;
			for(int i=0; i<BLOCK_SIZE; i++){
		
				for (int j=0; j<BLOCK_SIZE; j++){
			
					for (int k=0; k<BLOCK_SIZE; k++){
						var4 += dct[i][k]*coeff_t[k][j];
					}
					new_dct[i][j] =	var4;
					var4 = 0;
				}
			}

			for(int i=0; i<BLOCK_H; i++){

				for(int j=0; j<BLOCK_W; j++){
					dct_frame[(x+j) + (width_frame*(i+y))] = my_own_round(new_dct[i][j]/quantMatrix[i][j]);
				}
			}
		}
	}
}


void idct_block(signed char *cur, unsigned char *idct_frame, int height_frame, int width_frame){
	int count1 = 0, x = 0, y = 0, count2 = 0, val = 0;
	int least = 0;
	float samples[BLOCK_SIZE][BLOCK_SIZE]; 
	float var4 = 0.0, var5 = 0.0;
	float coeff_t[BLOCK_SIZE][BLOCK_SIZE];
	float idct1[BLOCK_SIZE][BLOCK_SIZE];
	float idct2[BLOCK_SIZE][BLOCK_SIZE];
	
	for(int i=0; i<BLOCK_SIZE; i++){
		for(int j=0; j<BLOCK_SIZE; j++){
			coeff_t[i][j] = coeff[j][i];
		}
	}


	for(y=0; y<height_frame; y+=BLOCK_H){
		
		for(x=0; x<width_frame; x+=BLOCK_W){
			
			for(int i=0; i<BLOCK_H; i++){

				for(int j=0; j<BLOCK_W; j++){
					samples[i][j] = (float)(cur[(x+j) + (width_frame*(i+y))]*quantMatrix[i][j]);
				}
			}

			var4 = 0.0;
			for(int i=0; i<BLOCK_SIZE; i++){
		
				for (int j=0; j<BLOCK_SIZE; j++){
			
					for (int k=0; k<BLOCK_SIZE; k++){
						var4 += coeff_t[i][k]*samples[k][j];
					}
					idct1[i][j] =	var4;
					var4 = 0;
				}
			}

			var4 = 0;
			for(int i=0; i<BLOCK_SIZE; i++){
		
				for (int j=0; j<BLOCK_SIZE; j++){
			
					for (int k=0; k<BLOCK_SIZE; k++){
						var4 += idct1[i][k]*coeff[k][j];
					}
					idct2[i][j] =	var4;
					var4 = 0;
				}
			}
			for(int i=0; i<BLOCK_H; i++){

				for(int j=0; j<BLOCK_W; j++){
					idct_frame[(x+j) + (width_frame*(i+y))] = (int)my_own_round(idct2[i][j]);
				}
			}
		}
	}
}



void create_coeff(){
	double dct[BLOCK_SIZE][BLOCK_SIZE];
	double var1 = 0, var2 = 0, var3 = 0;
	double Ci = 0;
	for(int i=0; i<BLOCK_SIZE; i++){
		
		for(int j=0; j<BLOCK_SIZE; j++){
			
			if(i == 0){
				Ci = sqrt(double(1)/double(BLOCK_SIZE));
			} else {
				Ci = sqrt(double(2)/double(BLOCK_SIZE));
			}

			var1 = (double)((2*j) + 1)*i*3.14;
			var2 = (double)((double)var1/(double)(2*BLOCK_SIZE));
			var3 = (double)cos(var2);
			coeff[i][j] = (double)var3*(double)Ci;
		}
	}
	
	for(int m=0; m<BLOCK_SIZE; m++){
		cout<<endl;
		for(int n=0; n<BLOCK_SIZE; n++){
			cout<<coeff[m][n]<<" ";
		}
	}
	
}

int my_own_round(float num){
	int num_int = 0;

	if(floor(num) > (num-0.5)){
		return floor(num);
	} else {
		return ceil(num);
	}

	return num_int;
}

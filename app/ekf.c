
void Mx_Add(float * A, float * B, int m, int n, float * C)
{
	int i;
	for(i=0;i<m*n;i++)
	{
		//C[i][j] = A[i][j] + B[i][j];
		*(C+i) = *(A+i) + *(B+i);
	}
}

void Mx_Transpose(float * A, int m, int n, float *B)
{
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			//B[i][j] = A[j][i];
			*(B + i*m + j) = *(A + j*m + i);
		}
	}
}

void Mx_Multiplication(float * A, float * B, int m, int z, int n, float * C)
{
	int i,j,k;
	for(i=0;i<m*n;i++)
	{
		*(C+i) = 0;
	}
	for(i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
		{
			for(k=0;k<z;k++)
			{
				//C[i][j] += A[i][k]*B[k][j];
				*(C + i*m + j) += (*(A + i*m + k))*(*(B + k*m + j));
			}
		}
	}
}

void Mx_Inverse_3x3(float A[3][3], float B[3][3])
{
	float val = A[0][0]*(A[1][1]*A[2][2]-A[1][2]*A[2][1]) - A[0][1]*(A[1][0]*A[2][2]-A[1][2]*A[2][0]) + A[0][2]*(A[1][0]*A[2][1]-A[1][1]*A[2][0]);
	
	if(val != 0)
	{
		B[0][0] =  (A[1][1]*A[2][2]-A[1][2]*A[2][1])/val;
		B[0][1] = -(A[0][1]*A[2][2]-A[0][2]*A[2][1])/val;
		B[0][2] =  (A[0][1]*A[1][2]-A[0][2]*A[1][1])/val;

		B[1][0] = -(A[1][0]*A[2][2]-A[1][2]*A[2][0])/val;
		B[1][1] =  (A[0][0]*A[2][2]-A[0][2]*A[2][0])/val;
		B[1][2] = -(A[0][0]*A[1][2]-A[0][2]*A[1][0])/val;

		B[2][0] =  (A[1][0]*A[2][1]-A[1][1]*A[2][0])/val;
		B[2][1] = -(A[0][0]*A[2][1]-A[0][1]*A[2][0])/val;
		B[2][2] =  (A[0][0]*A[1][1]-A[0][1]*A[1][0])/val;
	}
}




static float P[4][4] = {0.001};//协方差矩阵
static float Q[4][4] = {0.1};//四维状态激励协方差
static float R[3][3] = {0.01};//三维观测协方差
void EKF(float gx, float gy, float gz, float ax, float ay, float az, float halfT, float q[4])
{
	unsigned char i;
	static float X[4],K[4][3],H[3][4],A[4][4];
	static float temp1[4][4],temp2[4][4],temp3[4][4];
	static float temp4[4][3],temp5[4][3];
	static float temp6[3][4];
	static float temp7[3][3],temp8[3][3];
	static float temp9[3];
	static float temp10[4];

	H[0][0] = 2*( q[0]*ax+q[3]*ay-q[2]*az);
	H[0][1] = 2*( q[1]*ax+q[2]*ay+q[3]*az);
	H[0][2] = 2*(-q[2]*ax+q[1]*ay-q[0]*az);
	H[0][3] = 2*(-q[3]*ax+q[0]*ay+q[1]*az);
	H[1][0] = 2*(-q[3]*ax+q[0]*ay+q[1]*az);
	H[1][1] = 2*( q[2]*ax-q[1]*ay+q[0]*az);
	H[1][2] = 2*( q[1]*ax+q[2]*ay+q[3]*az);
	H[1][3] = 2*(-q[0]*ax-q[3]*ay+q[2]*az);
	H[2][0] = 2*( q[2]*ax-q[1]*ay+q[0]*az);
	H[2][1] = 2*( q[3]*ax-q[0]*ay-q[1]*az);
	H[2][2] = 2*( q[0]*ax+q[3]*ay-q[2]*az);
	H[2][3] = 2*( q[1]*ax+q[2]*ay+q[3]*az);

	A[0][0] = 1;	
	A[0][1] = -gx*halfT;	
	A[0][2] = -gy*halfT;	
	A[0][3] = -gz*halfT;
	A[1][0] = gx*halfT;    
	A[1][1] = 1;    
	A[1][2] = gz*halfT;    
	A[1][3] = gy*halfT;
	A[2][0] = gy*halfT;    
	A[2][1] = -gz*halfT;    
	A[2][2] = 1 ;    
	A[2][3] = gx*halfT;
	A[3][0] = gz*halfT;    
	A[3][1] = gy*halfT;    
	A[3][2] = -gx*halfT;    
	A[3][3] = 1;

	

	//一步状态向量预测 X1
	X[0] = q[0] + (-q[1]*gx - q[2]*gy - q[3]*gz)*halfT;
    X[1] = q[1] + ( q[0]*gx + q[2]*gz - q[3]*gy)*halfT;
    X[2] = q[2] + ( q[0]*gy - q[1]*gz + q[3]*gx)*halfT;
    X[3] = q[3] + ( q[0]*gz + q[1]*gy - q[2]*gx)*halfT; 
	
	//一步协方差预测 P1
	Mx_Multiplication((float *)A, (float *)P, 4, 4, 4, (float *)temp1);
	Mx_Transpose((float *)A, 4, 4, (float *)temp2);
	Mx_Multiplication((float *)temp1, (float *)temp2, 4, 4, 4, (float *)temp3);
	Mx_Add((float *)temp3, (float *)Q, 4, 4, (float *)P);

	//计算最优卡尔曼增益 K
	Mx_Transpose((float *)H, 3, 4, (float *)temp4);
	Mx_Multiplication((float *)P, (float *)temp4, 4, 4, 3, (float *)temp5);
	Mx_Multiplication((float *)H, (float *)P, 3, 4, 4, (float *)temp6);
	Mx_Multiplication((float *)temp6, (float *)temp4, 3, 4, 3, (float *)temp7);
	Mx_Add((float *)temp7, (float *)R, 3, 3, (float *)temp7);
	Mx_Inverse_3x3(temp7, temp8);
	Mx_Multiplication((float *)temp5, (float *)temp8, 4, 3, 3, (float *)K);

	//计算测量余量
	Mx_Multiplication((float *)H, (float *)X, 3, 4, 1, (float *)temp9);
	temp9[0] = ax - temp9[0];
	temp9[1] = ay - temp9[1];
	temp9[2] = az - temp9[2];

	//更新状态向量
	Mx_Multiplication((float *)K, (float *)temp9, 4, 3, 1, (float *)temp10);
	Mx_Add((float *)X, (float *)temp10, 4, 1, (float *)q);
	
	//更新协方差矩阵
	Mx_Multiplication((float *)K, (float *)H, 4, 3, 4, (float *)temp1);
	for(i=0;i<16;i++)
		*((float *)temp2 + i) = 1 - *((float *)temp1 + i);
	Mx_Multiplication((float *)temp2, (float *)P, 4, 4, 4, (float *)temp3);
	for(i=0;i<16;i++)
		*((float *)P + i) = *((float *)temp3 + i);
}

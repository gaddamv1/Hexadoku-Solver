#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
double** allocate_matrix(int rows, int cols){
  double** val=malloc(rows*sizeof(double*));

  for(int i=0; i<rows; i++){
    val[i]=malloc(cols*sizeof(double));
  }
  return val;
}
void initialize_matrix(double**matrix,int rows, int cols,double num,int rowPosistion,int colPosistion){
  bool inserted=false;
  for(int i=0; i<rows; i++){
    for(int j=0; j<cols; j++){
      if(i==rowPosistion){
	if(j==colPosistion){
	  *(*(matrix+i)+j)=num;
	  inserted=true;
	  break;
	}
      }
    }
    if(inserted==true){
    break;
    }
  }
}

double** multiply(double**matrix,double**matrix2,double**matrix3,int rows,int cols, int rows2,int cols2){
  for(int i=0; i<rows; i++){
    for(int j=0; j<cols2; j++){
      double sum=0;
      int colsRemain=0;
      int rowsRemain=0;
      while(colsRemain<cols&&rowsRemain<rows2){
	sum+=matrix[i][colsRemain]*matrix2[rowsRemain][j];
	rowsRemain++;
	colsRemain++;
    }
    initialize_matrix(matrix3,rows,cols2,sum,i,j);
    }
  }
    return matrix3;
}
double** matrix_transpose(double**matrix,double**matrix1,int rows,int cols){
  for(int i=0; i<rows; i++){
    for(int j=0; j<cols; j++){
      initialize_matrix(matrix1,cols,rows,*(*(matrix+i)+j),j,i);
    }
}
  return matrix1;
  }
void matrix_row_add(double**matrix,int rows,int cols,int rows1, int rows2,int rowStore,double factor){
  bool x=false;
  if(rows2==rowStore){
    x=true;
  }
  for(int i=0; i<cols; i++){
    if(x==true){
      *(*(matrix+rows2)+i)=(factor* *(*(matrix+rows1)+i))+*(*(matrix+rows2)+i);
    }
    else{
      *(*(matrix+rows1)+i)=(factor* *(*(matrix+rows2)+i))+*(*(matrix+rows1)+i);
    }
  } 
}

void divide(double**matrix,int rows,int cols,int row,double factor){
  for(int i=0; i<rows; i++){
    if(i!=row){
      continue;
    }
    else{
      for(int j=0; j<cols; j++){
	*(*(matrix+i)+j)= *(*(matrix+i)+j)/factor;
      }
      break;
    }
    }
}
void free_matrix(double**matrix,int rows){
  for(int i=0; i<rows; i++){
    free(matrix[i]);
    matrix[i]=NULL;
  }
  free(matrix);
}
int main(int argc,char** argv){
  FILE* fp=fopen(argv[1],"r");
  FILE* fp1=fopen(argv[2],"r");
  double**matrix;
  double**transpose;
  double**price;
  double**weights;
  double**result;
  double**augmented;
  double**inverse;
  double**result1;
  double**realMatrix;
  double**final;
  int attributes=0;
  int examples=0;
  fscanf(fp,"%d\n",&attributes);
  fscanf(fp,"%d\n",&examples); 
matrix=allocate_matrix(examples,attributes+1);
price=allocate_matrix(examples,1);
weights=allocate_matrix(attributes+1,1);
transpose=allocate_matrix(attributes+1,examples);
result=allocate_matrix(attributes+1,attributes+1);
augmented=allocate_matrix(attributes+1,2*(attributes+1));
inverse=allocate_matrix(attributes+1,attributes+1);
result1=allocate_matrix(attributes+1,examples);
 int rows=examples;
 int cols=attributes+1;
 int cols2=2*(attributes+1);
 int rowsRemain=0;
 int colsRemain=0;
 double num;
 while(rowsRemain<rows){
   while(colsRemain<cols+1){
     if(colsRemain==0){
       initialize_matrix(matrix,rows,cols,1,rowsRemain,colsRemain);
     }
    else if(colsRemain==cols){
       fscanf(fp,"%lf\n",&num);
       initialize_matrix(price,rows,1,num,rowsRemain,0);
     }
     else{
       fscanf(fp,"%lf,",&num);
       initialize_matrix(matrix,rows,cols,num,rowsRemain,colsRemain);
     }
     colsRemain++;
   }
   colsRemain=0;
   rowsRemain++;
 }
 transpose=matrix_transpose(matrix,transpose,rows,cols);
 result=multiply(transpose,matrix,result,cols,rows,rows,cols);
 for(int i=0; i<cols; i++){
   for(int j=0; j<cols2; j++){
     if(j>=cols){
       if((j-i)==cols){
	 initialize_matrix(augmented,rows,cols2,1,i,j);
       }
       else{
	  initialize_matrix(augmented,rows,cols2,0,i,j);
       }
     }
     else{
       initialize_matrix(augmented,cols,cols2,*(*(result+i)+j),i,j);
     }
     }
 }

 for(int i=0; i<cols; i++){
   for(int j=0; j<cols2; j++){
     if(i==j){
       double factor=augmented[i][j];
       divide(augmented,cols,cols2,i,factor);
       double factor2=augmented[i][j];
       for(int p=0; p<cols; p++){
	 if(p==i){
	   continue;
	 }
	 else{
	   double factor3=-1*(*(*(augmented+p)+j)/factor2);
       matrix_row_add(augmented,cols,cols2,i,p,p,factor3);
	 }
       }
     }
     else{
       continue;
     }

   }
 }
 for(int i=0; i<cols; i++){
   for(int j=0; j<cols2; j++){
     if(j>=cols){
     initialize_matrix(inverse,cols,cols,*(*(augmented+i)+j),i,j-cols);
     }
   }
 }
 result1=multiply(inverse,transpose,result1,cols,cols,cols,rows);
 weights=multiply(result1,price,weights,cols,rows,rows,1);
  int realRows=0;
 rowsRemain=0; 
 colsRemain=0;
 num=0;
 int realCols=attributes+1;
 fscanf(fp1,"%d\n",&realRows);
 realMatrix=allocate_matrix(realRows,realCols);
while(rowsRemain<realRows){
   while(colsRemain<realCols){
     if(colsRemain==0){
       initialize_matrix(realMatrix,realRows,realCols,1,rowsRemain,colsRemain);
     }
   else if(colsRemain==realCols-1){
       fscanf(fp1,"%lf\n",&num);
       initialize_matrix(realMatrix,realRows,realCols,num,rowsRemain,colsRemain); 
     }
     else{
       fscanf(fp1,"%lf,",&num);
       initialize_matrix(realMatrix,realRows,realCols,num,rowsRemain,colsRemain);
     }
     colsRemain++;
   }
   colsRemain=0;
   rowsRemain++;
 }
final=allocate_matrix(realRows,1);
final=multiply(realMatrix,weights,final,realRows,realCols,cols,1);
 for(int i=0; i<realRows; i++){
   printf("%0.0lf\n",final[i][0]);
 }

 free_matrix(matrix,rows);
  matrix=NULL;
 free_matrix(price,rows);
  price=NULL;
 free_matrix(transpose,cols);
  transpose=NULL;
 free_matrix(result,cols);
  result=NULL;
 free_matrix(augmented,cols);
 augmented=NULL;
 free_matrix(inverse,cols);
 inverse=NULL;
 free_matrix(weights,cols);
  weights=NULL;
 free_matrix(result1,cols);
 result1=NULL;
 free_matrix(realMatrix,realRows);
 realMatrix=NULL;
 free_matrix(final,realRows);
 final=NULL;
 return 0;
}



